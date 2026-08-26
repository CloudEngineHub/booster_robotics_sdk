#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <string>
#include <vector>

namespace booster {
namespace robot {
namespace audio {

/** @brief Error returned when an audio capability is unavailable on the target. */
inline constexpr int32_t kAudioErrNotSupported = 7001;

/**
 * @brief Audio input source or file encoding.
 * @note Supported model: K1 | T1 | T2
 * @note Requires the audio service to provide the codec.
 */
enum class AudioSourceType : int32_t {
    kPcmFile = 0,   ///< Raw PCM data read from a file.
    kWavFile = 1,   ///< WAV audio read from a file.
    kPcmStream = 2, ///< Raw PCM data supplied as a stream.
    kMp3File = 3,   ///< MP3 audio read from a file.
};

/** @brief Playback lifecycle state. */
enum class PlayerState : int32_t {
    kIdle = 0,      ///< The player has not been initialized.
    kReady = 1,     ///< The player is initialized and ready to start.
    kPlaying = 2,   ///< Audio playback is in progress.
    kPaused = 3,    ///< Playback is paused and can be resumed.
    kStopped = 4,   ///< Playback was stopped before completion.
    kCompleted = 5, ///< Playback reached the end of the source.
    kError = 6,     ///< Playback stopped because of an error.
};

/** @brief Arbitration priority used by an audio player. */
enum class PlayerPriority : int32_t {
    kLow = 0,    ///< Low-priority playback that may yield to other players.
    kMedium = 1, ///< Normal playback priority.
    kHigh = 2,   ///< High-priority playback that may preempt lower priorities.
};

/** @brief Recorder lifecycle state. */
enum class RecorderState : int32_t {
    kIdle = 0,      ///< The recorder has not been initialized.
    kReady = 1,     ///< The recorder is initialized and ready to start.
    kRecording = 2, ///< Audio recording is in progress.
    kPaused = 3,    ///< Recording is paused and can be resumed.
    kStopped = 4,   ///< Recording has been stopped.
    kError = 5,     ///< Recording stopped because of an error.
};

/** @brief Raw/NAEC capture stream lifecycle state. */
enum class AudioCaptureStreamState : int32_t {
    kIdle = 0,      ///< The capture stream has not been initialized.
    kReady = 1,     ///< The capture stream is initialized and ready to start.
    kStreaming = 2, ///< Captured audio frames are being delivered.
    kPaused = 3,    ///< Frame delivery is paused and can be resumed.
    kStopped = 4,   ///< The capture stream has been stopped.
    kError = 5,     ///< Capture stopped because of an error.
};

/** @brief Selects input or output devices for enumeration. */
enum class AudioDeviceQueryType : int32_t {
    GET_DEVICES_INPUTS = 0,  ///< Query audio input devices.
    GET_DEVICES_OUTPUTS = 1, ///< Query audio output devices.
};

/** @brief Direction supported by an audio endpoint. */
enum class AudioDeviceDirection : int32_t {
    INPUT = 0,  ///< Audio input endpoint.
    OUTPUT = 1, ///< Audio output endpoint.
};

/** @brief Physical or virtual transport of an audio endpoint. */
enum class AudioDeviceTransport : int32_t {
    BUILTIN = 0,   ///< Device integrated into the robot.
    USB = 1,       ///< Device connected over USB.
    BLUETOOTH = 2, ///< Device connected over Bluetooth.
    VIRTUAL = 3,   ///< Software-defined audio endpoint.
    UNKNOWN = 4,   ///< Transport could not be identified.
};

/** @brief Audio backend associated with an endpoint. */
enum class AudioDeviceBackendAffinity : int32_t {
    PULSE = 0,            ///< PulseAudio-compatible backend.
    BOOSTERAEC_ARRAY = 1, ///< Booster microphone-array AEC backend.
    ALSA_DIAGNOSTIC = 2,  ///< ALSA endpoint intended for diagnostics.
};

/** @brief PCM sample format. Rates and widths are validated by the active audio backend. */
struct PcmFormat {
    int32_t sample_rate_hz{16000};
    int32_t channels{1};
    int32_t bits_per_sample{16};
};

/** @brief Parameters used to initialize an AudioPlayer. */
struct PlayerInitOptions {
    AudioSourceType source_type{AudioSourceType::kPcmFile};
    std::string source_uri;
    int32_t sample_rate_hz{16000};
    int32_t channels{1};
    int32_t bits_per_sample{16};
    PlayerPriority priority{PlayerPriority::kMedium};
};

/** @brief Parameters used to initialize an AudioRecorder. */
struct RecorderInitOptions {
    std::string output_path;
    int32_t sample_rate_hz{16000};
    int32_t channels{1};
    int32_t bits_per_sample{16};
};

/** @brief Requested streams and format for AudioCaptureStream. */
struct AudioCaptureStreamOptions {
    bool enable_raw_pcm{true};
    bool enable_naec_pcm{false};
    PcmFormat requested_raw_format{16000, 1, 16};
};

/** @brief Snapshot of playback progress and state. */
struct PlayerInfo {
    PlayerState state{PlayerState::kIdle};
    int64_t played_bytes{0};
    int64_t total_bytes{0};
    float volume{1.0F};
};

/** @brief Snapshot of recording progress and state. */
struct RecorderInfo {
    RecorderState state{RecorderState::kIdle};
    int64_t captured_bytes{0};
};

/** @brief One captured audio frame and its optional raw/NAEC PCM payloads. */
struct AudioCaptureFrame {
    int64_t frame_seq{0};
    int64_t timestamp_ms{0};

    bool raw_valid{false};
    PcmFormat raw_format;
    int32_t raw_frame_samples_per_channel{0};
    std::vector<int16_t> raw_pcm;

    bool naec_valid{false};
    PcmFormat naec_format;
    int32_t naec_frame_samples_per_channel{0};
    std::vector<int16_t> naec_pcm;
};

/** @brief Snapshot of capture stream counters and negotiated formats. */
struct AudioCaptureStreamInfo {
    AudioCaptureStreamState state{AudioCaptureStreamState::kIdle};
    bool raw_enabled{false};
    bool naec_enabled{false};
    PcmFormat actual_raw_format;
    PcmFormat actual_naec_format;
    int64_t published_frames{0};
    int64_t dropped_frames{0};
};

/** @brief Metadata reported for one input or output endpoint. */
struct AudioDeviceInfo {
    std::string device_id;
    std::string display_name;
    AudioDeviceDirection direction{AudioDeviceDirection::INPUT};
    AudioDeviceTransport transport{AudioDeviceTransport::UNKNOWN};
    AudioDeviceBackendAffinity backend_affinity{AudioDeviceBackendAffinity::PULSE};
    bool is_available{false};
    bool is_system_default{false};
    bool supports_input{false};
    bool supports_output{false};
    std::string provider_name;
    std::string native_id;
    std::string metadata_json;
};

/** @brief Detailed error delivered through audio callbacks. */
struct AudioError {
    int32_t ret_code{0};
    std::string ret_msg;
    int32_t error_category{0};
    int32_t error_detail{0};
};

/** @brief Bluetooth discovery state. */
enum class BluetoothScanState : int32_t {
    kIdle = 0,     ///< No Bluetooth discovery is running.
    kScanning = 1, ///< Bluetooth discovery is in progress.
};

/** @brief Bluetooth connection state. */
enum class BluetoothDeviceState : int32_t {
    kUnknown = 0,       ///< Connection state is unavailable.
    kPaired = 1,        ///< Device is paired but not connected.
    kConnecting = 2,    ///< A connection attempt is in progress.
    kConnected = 3,     ///< Device is connected.
    kDisconnecting = 4, ///< Device disconnection is in progress.
    kFailed = 5,        ///< The most recent connection operation failed.
};

/** @brief Bluetooth device major class. */
enum class BluetoothMajorClass : int32_t {
    kAudio = 0,      ///< Audio or video device.
    kPeripheral = 1, ///< Peripheral device.
    kPhone = 2,       ///< Phone-class device.
    kComputer = 3,    ///< Computer-class device.
    kOther = 4,       ///< Device from another or unknown class.
};

/** @brief Bluetooth audio profile. */
enum class BluetoothAudioProfile : int32_t {
    kNone = 0,        ///< No preferred or active audio profile.
    kA2dpSink = 1,    ///< Advanced Audio Distribution Profile sink.
    kA2dpSource = 2,  ///< Advanced Audio Distribution Profile source.
    kHfpHeadset = 3,  ///< Hands-Free Profile headset role.
    kHfpAg = 4,       ///< Hands-Free Profile audio-gateway role.
};

/** @brief Discovered or paired Bluetooth endpoint and its audio capabilities. */
struct BluetoothDeviceInfo {
    std::string address;
    std::string name;
    int16_t rssi{0};
    BluetoothDeviceState state{BluetoothDeviceState::kUnknown};
    BluetoothMajorClass major_class{BluetoothMajorClass::kOther};
    bool paired{false};
    bool trusted{false};
    bool connected{false};
    bool is_audio_sink{false};
    bool is_audio_source{false};
    bool is_hfp_capable{false};
    std::vector<BluetoothAudioProfile> connected_profiles;
    std::string linked_pulse_sink_id;
    std::string linked_pulse_source_id;
    int64_t last_seen_ms{0};
};

/** @brief Options controlling a Bluetooth scan. */
struct BluetoothScanOptions {
    int32_t timeout_ms{30000};
    bool audio_only{true};
};

/** @brief Options controlling Bluetooth pairing/connection and routing. */
struct BluetoothConnectOptions {
    std::string address;
    bool auto_pair{true};
    bool make_default{true};
    BluetoothAudioProfile preferred_profile{BluetoothAudioProfile::kNone};
    int32_t timeout_ms{15000};
};

/** @brief Result of a Bluetooth connection and optional default-route operation. */
struct BluetoothConnectResult {
    BluetoothDeviceInfo device;
    std::string pulse_sink_id;
    std::string pulse_source_id;
    BluetoothAudioProfile active_profile{BluetoothAudioProfile::kNone};
    bool pulse_endpoint_ready{false};
    bool default_sink_applied{false};
    bool default_source_applied{false};
    int32_t default_route_error_code{0};
    std::string default_route_error_msg;
};

/** @name Audio callback types */
///@{
using PlayerStateCallback = std::function<void(PlayerState)>;
using PlayerProgressCallback = std::function<void(const PlayerInfo&)>;
using PlayerCompletionCallback = std::function<void(const PlayerInfo&)>;
using PlayerErrorCallback = std::function<void(const AudioError&)>;

using RecorderStateCallback = std::function<void(RecorderState)>;
using RecorderProgressCallback = std::function<void(const RecorderInfo&)>;
using RecorderErrorCallback = std::function<void(const AudioError&)>;

using AudioCaptureFrameCallback = std::function<void(const AudioCaptureFrame&)>;
using AudioCaptureStreamStateCallback = std::function<void(AudioCaptureStreamState)>;
using AudioCaptureStreamErrorCallback = std::function<void(const AudioError&)>;
///@}

} // namespace audio
} // namespace robot
} // namespace booster
