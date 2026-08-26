#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <string>

#include <booster/robot/audio/audio_types.h>

namespace booster {
namespace robot {
namespace audio {

class AudioManager;

/**
 * @brief Controls one playback session through AudioManager.
 * @note Supported model: K1 | T1 | T2
 */
class AudioPlayer {
public:
    /** @brief Creates a player associated with @p audio_manager. */
    explicit AudioPlayer(AudioManager &audio_manager);
    ~AudioPlayer();

    AudioPlayer(const AudioPlayer &) = delete;
    AudioPlayer &operator=(const AudioPlayer &) = delete;

    /** @brief Initializes the source and format. */
    int32_t Init(const PlayerInitOptions &options);
    /** @brief Starts or resumes playback. */
    int32_t Start();
    /** @brief Pauses playback. */
    int32_t Pause();
    /** @brief Stops playback and resets the play cursor. */
    int32_t Stop();
    /** @brief Resets a stopped/completed player for reuse. */
    int32_t Reset();
    /** @brief Destroys the service-side session. */
    int32_t Destroy();
    /** @brief Alias for Destroy(), retained for lifecycle compatibility. */
    int32_t Release();

    /** @brief Sets the session volume. */
    int32_t SetVolume(float volume);
    /** @brief Selects an output endpoint by AudioDeviceInfo::device_id. */
    int32_t SetPreferredDevice(const std::string &device_id);
    /** @brief Reads the cached playback information. */
    int32_t GetInfo(PlayerInfo *out) const;
    /** @brief Appends PCM bytes when source_type is kPcmStream. */
    int32_t PushPcmStream(const uint8_t *data, size_t size);

    /** @brief Returns the service session identifier. */
    int64_t GetSessionId() const;
    /** @brief Returns the most recently observed state without an RPC. */
    PlayerState GetCachedState() const;

    /** @brief Registers a state-change callback. */
    void SetStateCallback(PlayerStateCallback callback);
    /** @brief Registers a progress callback. */
    void SetProgressCallback(PlayerProgressCallback callback);
    /** @brief Registers a completion callback. */
    void SetCompletionCallback(PlayerCompletionCallback callback);
    /** @brief Registers an error callback. */
    void SetErrorCallback(PlayerErrorCallback callback);

private:
    struct StateData;

    void EnsureTopicListenersRegistered();

    static PlayerState ParsePlayerState(int32_t raw_state);

    AudioManager *audio_manager_{nullptr};
    int64_t progress_listener_id_{0};
    int64_t error_listener_id_{0};
    std::shared_ptr<StateData> state_;
};

} // namespace audio
} // namespace robot
} // namespace booster
