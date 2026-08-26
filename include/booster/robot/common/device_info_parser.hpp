#pragma once

/**
 * Parse `DeviceInfo` (RPC JSON in json_ + kind_) into concrete IDL messages:
 * ImuInfoList, HandList, RobotModel, CameraList, Camera.
 */

#include <booster/idl/b1/Hand.h>
#include <booster/idl/b1/ImuInfo.h>
#include <booster/idl/b1/RobotModel.h>
#include <booster/idl/camera/Camera.h>
#include <booster/robot/common/device_info.hpp>

namespace booster {
namespace robot {

/**
 * Fill ImuInfoList from DeviceInfo when kind_ is kSensors (GetSensors RPC body in json_).
 * Otherwise clears imu_list.
 */
/** @brief Converts sensor DeviceInfo to ImuInfoList, clearing on kind mismatch. */
void ImuInfoListFromDeviceInfo(booster_interface::msg::ImuInfoList &imu_list,
                               const b1::DeviceInfo &info);

/**
 * Fill HandList from DeviceInfo when kind_ is kHands (GetHands RPC body in json_).
 * Otherwise clears hand_list.
 */
/** @brief Converts hand DeviceInfo to HandList, clearing on kind mismatch. */
void HandListFromDeviceInfo(booster_interface::msg::HandList &hand_list,
                            const b1::DeviceInfo &info);

/**
 * Fill RobotModel from DeviceInfo when kind_ is kRobotModel (GetRobotModel RPC body in json_).
 * Otherwise clears robot_model.
 */
/** @brief Converts robot-model DeviceInfo to RobotModel, clearing on mismatch. */
void RobotModelFromDeviceInfo(booster_interface::msg::RobotModel &robot_model,
                              const b1::DeviceInfo &info);

/**
 * Fill CameraList from DeviceInfo when kind_ is kCamera (GetCameras RPC body in json_).
 * Otherwise clears camera_list.
 */
/** @brief Converts camera DeviceInfo to CameraList, clearing on kind mismatch. */
void CameraListFromDeviceInfo(booster_interface::msg::CameraList &camera_list,
                            const b1::DeviceInfo &info);

} // namespace robot
} // namespace booster
