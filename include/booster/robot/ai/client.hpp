#pragma once

#include <memory>

#include <booster/robot/rpc/rpc_client.hpp>

#include "api.hpp"

namespace booster {
namespace robot {

/**
 * @file client.hpp
 * @brief Synchronous RPC clients for AI chat and standalone voice services.
 *
 * AiClient controls conversational AI through `rt/AiApiTopic`. LuiClient
 * controls standalone ASR and TTS through `rt/LuiApiTopic`. Initialize a client
 * before sending requests. Both clients wait up to ten seconds for each RPC
 * response.
 *
 * @par Model support
 * @note Supported model: K1 | T1 | T2
 * @note AI chat, direct speech, standalone ASR, and standalone TTS require
 * their services and audio hardware.
 * @note Face tracking supported model: K1 | T1
 * @note Face tracking requires an NVIDIA perception board. The current RTC
 * service acknowledges standalone face-tracking requests but does not apply
 * them.
 * @warning The face-tracking behavior described here reflects the RTC service
 * implementation shipped with this SDK revision. Re-verify the behavior after
 * upgrading the SDK, RTC service, or robot software.
 *
 * @par Service dependencies
 * AI chat requires the RTC client, audio service, robot credentials, and
 * network access to the configured cloud service. LUI ASR/TTS requires the LUI
 * and audio services plus network access and valid service credentials.
 *
 * @see AiApiId
 * @see LuiApiId
 */

/**
 * @brief Synchronous client for conversational AI operations.
 *
 * The client serializes typed request objects to JSON and sends them to the AI
 * RPC service. A successful start or stop response means that the service
 * accepted the asynchronous operation; it does not guarantee that cloud session
 * establishment or shutdown has already completed.
 */
class AiClient {
public:
    /** @brief Constructs an uninitialized AI client. */
    AiClient() = default;

    /** @brief Destroys the client and releases its RPC transport. */
    ~AiClient() = default;

    /**
     * @brief Initializes communication with the default robot.
     *
     * Requests are sent on `rt/AiApiTopic`. Calling Init() again replaces the
     * existing RPC transport.
     *
     * @note Supported model: K1 | T1 | T2
     */
    void Init();

    /**
     * @brief Initializes communication with a named robot.
     * @note Supported model: K1 | T1 | T2
     * @param robot_name Robot-name suffix appended to `rt/AiApiTopic/`. An empty
     * string selects the default topic.
     *
     */
    void Init(const std::string &robot_name);

    /**
     * @brief Sends an AI RPC request and returns only its status code.
     * @note Availability depends on the selected AiApiId. See api.hpp.
     *
     * This method waits synchronously for a response for up to ten seconds and
     * discards the response body.
     *
     * @param api_id AI operation identifier.
     * @param param Serialized JSON request body, or an empty string for
     * operations that take no payload.
     * @return `booster::robot::kRpcStatusCodeSuccess` (0) on success; otherwise
     * an RPC transport or AI service status code.
     *
     * @pre Init() or Init(const std::string &) has been called.
     */
    int32_t SendApiRequest(AiApiId api_id, const std::string &param);

    /**
     * @brief Sends an AI RPC request and returns the complete response.
     * @note Availability depends on the selected AiApiId. See api.hpp.
     *
     * This method waits synchronously for up to ten seconds. The response is
     * populated for successful and failed RPC responses.
     *
     * @param api_id AI operation identifier.
     * @param param Serialized JSON request body, or an empty string for
     * operations that take no payload.
     * @param[out] resp Complete response, including status header and body.
     * @return The status code stored in `resp`; zero indicates success.
     *
     * @pre Init() or Init(const std::string &) has been called.
     * @see booster::robot::Response
     */
    int32_t SendApiRequestWithResponse(AiApiId api_id, const std::string &param, Response &resp);

    /**
     * @brief Starts an AI chat session.
     * @note Supported model: K1 | T1 | T2
     * @note Face-tracking intent in `param` is hardware-dependent and is not
     * activated by the current RTC service implementation.
     * @warning This face-tracking behavior is version-sensitive; re-verify it
     * after upgrading the SDK, RTC service, or robot software.
     *
     * The service accepts the request and establishes the audio and cloud
     * session asynchronously. Starting while a session or start operation is
     * already active may return `kRpcStatusCodeConflict`.
     *
     * @param param Complete TTS, LLM, ASR, interruption, and face-tracking
     * configuration.
     * @return Zero when the start request is accepted; otherwise an RPC or AI
     * service status code.
     *
     * @pre The client is initialized and every uninitialized scalar in `param`
     * has been assigned.
     */
    int32_t StartAiChat(const StartAiChatParameter &param) {
        std::string body = param.ToJson().dump();
        return SendApiRequest(AiApiId::kStartAiChat, body);
    }

    /**
     * @brief Stops the active AI chat session.
     * @note Supported model: K1 | T1 | T2
     *
     * If a start is still in progress, the service queues the stop operation.
     * Stopping an already idle session is treated as success.
     *
     * @return Zero when the stop is accepted or the service is already idle;
     * otherwise an RPC or AI service status code.
     */
    int32_t StopAiChat() {
        return SendApiRequest(AiApiId::kStopAiChat, "");
    }

    /**
     * @brief Speaks text through the active AI chat session.
     * @note Supported model: K1 | T1 | T2
     *
     * @param param Non-empty text payload.
     * @return Zero on success. An empty message may return
     * `kRpcStatusCodeBadRequest`; calling without an active AI chat may return
     * `kRpcStatusCodeConflict`.
     *
     * @pre An AI chat session is active.
     */
    int32_t Speak(const SpeakParameter &param) {
        std::string body = param.ToJson().dump();
        return SendApiRequest(AiApiId::kSpeak, body);
    }

    /**
     * @brief Requests standalone face tracking to start.
     * @note Supported model: K1 | T1
     * @note Effective face tracking requires an NVIDIA perception board.
     *
     * @return The RPC status code. The current RTC service returns success but
     * does not activate tracking.
     *
     * @warning A zero return value from the current service is only an
     * acknowledgement and does not indicate that tracking started.
     * @warning This behavior is version-sensitive; re-verify it after upgrading
     * the SDK, RTC service, or robot software.
     */
    int32_t StartFaceTracking() {
        return SendApiRequest(AiApiId::kStartFaceTracking, "");
    }

    /**
     * @brief Requests standalone face tracking to stop.
     * @note Supported model: K1 | T1
     * @note Effective face tracking requires an NVIDIA perception board.
     *
     * @return The RPC status code. The current RTC service returns success but
     * does not change tracking state.
     *
     * @warning A zero return value from the current service is only an
     * acknowledgement and does not indicate that tracking stopped.
     * @warning This behavior is version-sensitive; re-verify it after upgrading
     * the SDK, RTC service, or robot software.
     */
    int32_t StopFaceTracking() {
        return SendApiRequest(AiApiId::kStopFaceTracking, "");
    }

private:
    /** @brief Underlying synchronous DDS RPC transport, created by Init(). */
    std::shared_ptr<RpcClient> rpc_client_;
};

/**
 * @brief Synchronous client for standalone local-user-interface ASR and TTS.
 *
 * ASR captures robot microphone audio and publishes recognized text on
 * kTopicLuiAsrChunk. TTS must be started with a voice configuration before text
 * can be submitted for synthesis and playback.
 *
 * @note Supported model: K1 | T1 | T2
 * @note Requires deployed LUI and audio services plus valid network
 * credentials.
 */
class LuiClient {
public:
    /** @brief Constructs an uninitialized LUI client. */
    LuiClient() = default;

    /** @brief Destroys the client and releases its RPC transport. */
    ~LuiClient() = default;

    /**
     * @brief Initializes communication with the default robot.
     *
     * Requests are sent on `rt/LuiApiTopic`. Calling Init() again replaces the
     * existing RPC transport.
     *
     * @note Supported model: K1 | T1 | T2
     */
    void Init();

    /**
     * @brief Initializes communication with a named robot.
     * @note Supported model: K1 | T1 | T2
     * @param robot_name Robot-name suffix appended to `rt/LuiApiTopic/`. An
     * empty string selects the default topic.
     *
     */
    void Init(const std::string &robot_name);

    /**
     * @brief Sends a LUI RPC request and returns only its status code.
     * @note Supported model: K1 | T1 | T2
     * @note This applies to all current LuiApiId values.
     *
     * This method waits synchronously for a response for up to ten seconds and
     * discards the response body.
     *
     * @param api_id LUI operation identifier.
     * @param param Serialized JSON request body, or an empty string for
     * operations that take no payload.
     * @return Zero on success; otherwise an RPC transport or LUI service status
     * code.
     *
     * @pre Init() or Init(const std::string &) has been called.
     */
    int32_t SendApiRequest(LuiApiId api_id, const std::string &param);

    /**
     * @brief Sends a LUI RPC request and returns the complete response.
     * @note Supported model: K1 | T1 | T2
     * @note This applies to all current LuiApiId values.
     *
     * This method waits synchronously for up to ten seconds. The response is
     * populated for successful and failed RPC responses.
     *
     * @param api_id LUI operation identifier.
     * @param param Serialized JSON request body, or an empty string for
     * operations that take no payload.
     * @param[out] resp Complete response, including status header and body.
     * @return The status code stored in `resp`; zero indicates success.
     *
     * @pre Init() or Init(const std::string &) has been called.
     * @see booster::robot::Response
     */
    int32_t SendApiRequestWithResponse(LuiApiId api_id, const std::string &param, Response &resp);

    /**
     * @brief Starts standalone automatic speech recognition.
     * @note Supported model: K1 | T1 | T2
     * @note Requires a working microphone, audio service, network connection,
     * and valid cloud-service credentials.
     *
     * The service captures 16 kHz, mono, 16-bit PCM audio from the robot audio
     * service and publishes recognized text as booster_interface::msg::AsrChunk
     * messages on kTopicLuiAsrChunk.
     *
     * @return Zero when ASR starts successfully; otherwise an RPC or LUI service
     * status code.
     */
    int32_t StartAsr() {
        return SendApiRequest(LuiApiId::kStartAsr, "");
    }

    /**
     * @brief Stops standalone automatic speech recognition.
     * @note Supported model: K1 | T1 | T2
     *
     * @return Zero when ASR stops successfully; otherwise an RPC or LUI service
     * status code.
     */
    int32_t StopAsr() {
        return SendApiRequest(LuiApiId::kStopAsr, "");
    }

    /**
     * @brief Starts a standalone text-to-speech session.
     * @note Supported model: K1 | T1 | T2
     * @note Requires a working speaker, audio service, network connection, and
     * valid cloud-service credentials.
     *
     * The service initializes cloud synthesis and robot audio playback for the
     * selected voice. Call this method before SendTtsText().
     *
     * @param config Voice configuration for the new session.
     * @return Zero when TTS starts successfully; otherwise an RPC or LUI service
     * status code.
     */
    int32_t StartTts(const LuiTtsConfig &config) {
        std::string body = config.ToJson().dump();
        return SendApiRequest(LuiApiId::kStartTts, body);
    }

    /**
     * @brief Stops the active standalone TTS session.
     * @note Supported model: K1 | T1 | T2
     *
     * @return Zero when TTS stops successfully; otherwise an RPC or LUI service
     * status code.
     */
    int32_t StopTts() {
        return SendApiRequest(LuiApiId::kStopTts, "");
    }

    /**
     * @brief Sends text to the active standalone TTS session.
     * @note Supported model: K1 | T1 | T2
     *
     * The LUI service requires non-empty text and enforces at least one second
     * between accepted text requests. Requests sent too frequently may return
     * `kRpcStatusCodeRequestTooFrequent`.
     *
     * @param param Non-empty text to synthesize and play.
     * @return Zero when the text is accepted; otherwise an RPC or LUI service
     * status code.
     *
     * @pre StartTts() has completed successfully.
     */
    int32_t SendTtsText(const LuiTtsParameter &param) {
        std::string body = param.ToJson().dump();
        return SendApiRequest(LuiApiId::kSendTtsText, body);
    }

private:
    /** @brief Underlying synchronous DDS RPC transport, created by Init(). */
    std::shared_ptr<RpcClient> rpc_client_;
};

}
} // namespace booster::robot
