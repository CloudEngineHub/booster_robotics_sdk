#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include <booster/robot/audio/audio_types.h>

namespace booster {
namespace robot {
namespace audio {

class AudioManager;

/**
 * @brief Controls one recording session through AudioManager.
 * @note Supported model: K1 | T1 | T2
 */
class AudioRecorder {
public:
    /** @brief Creates a recorder associated with @p audio_manager. */
    explicit AudioRecorder(AudioManager &audio_manager);
    ~AudioRecorder();

    AudioRecorder(const AudioRecorder &) = delete;
    AudioRecorder &operator=(const AudioRecorder &) = delete;

    /** @brief Initializes output path and PCM format. */
    int32_t Init(const RecorderInitOptions &options);
    /** @brief Starts or resumes recording. */
    int32_t Start();
    /** @brief Pauses recording. */
    int32_t Pause();
    /** @brief Stops recording and finalizes the output. */
    int32_t Stop();
    /** @brief Destroys the service-side recording session. */
    int32_t Destroy();
    /** @brief Alias for Destroy(), retained for lifecycle compatibility. */
    int32_t Release();

    int32_t GetInfo(RecorderInfo *out) const;
    /** @brief Selects an input endpoint by AudioDeviceInfo::device_id. */
    int32_t SetPreferredDevice(const std::string &device_id);
    /** @brief Reads cached recording information. */

    /** @brief Returns the service session identifier. */
    int64_t GetSessionId() const;
    /** @brief Returns the most recently observed state without an RPC. */
    RecorderState GetCachedState() const;

    /** @brief Registers a state-change callback. */
    void SetStateCallback(RecorderStateCallback callback);
    /** @brief Registers a progress callback. */
    void SetProgressCallback(RecorderProgressCallback callback);
    /** @brief Registers an error callback. */
    void SetErrorCallback(RecorderErrorCallback callback);

private:
    struct StateData;

    void EnsureTopicListenersRegistered();
    int32_t WaitForCachedState(
        RecorderState expected_state,
        int attempts = 10,
        int64_t poll_interval_ms = 200) const;
    static RecorderState ParseRecorderState(int32_t raw_state);

    AudioManager *audio_manager_{nullptr};
    int64_t progress_listener_id_{0};
    int64_t error_listener_id_{0};
    std::shared_ptr<StateData> state_;
};

} // namespace audio
} // namespace robot
} // namespace booster
