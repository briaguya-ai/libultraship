#pragma once

#include <cstdint>
#include <string>
#include "ShipDeviceIndexToPhysicalDeviceIndexMapping.h"

namespace Ship {

class ShipDeviceIndexToSDLJoystickInstanceIDMapping : public ShipDeviceIndexToPhysicalDeviceIndexMapping {
  public:
    ShipDeviceIndexToSDLJoystickInstanceIDMapping(ShipDeviceIndex shipDeviceIndex, int32_t sdlJoystickInstanceID,
                                                  std::string sdlJoystickGuid, std::string sdlControllerName,
                                                  int32_t stickAxisThresholdPercentage,
                                                  int32_t triggerAxisThresholdPercentage);
    ~ShipDeviceIndexToSDLJoystickInstanceIDMapping();
    std::string GetJoystickGUID();
    std::string GetSDLControllerName();

    void SaveToConfig() override;
    void EraseFromConfig() override;

    int32_t GetSDLJoystickInstanceID();
    void SetSDLJoystickInstanceID(int32_t instanceId);

    int32_t GetStickAxisThresholdPercentage();
    void SetStickAxisThresholdPercentage(int32_t stickAxisThresholdPercentage);

    int32_t GetTriggerAxisThresholdPercentage();
    void SetTriggerAxisThresholdPercentage(int32_t triggerAxisThresholdPercentage);

  private:
    int32_t mSDLJoystickInstanceID;
    std::string mSDLJoystickGUID;
    std::string mSDLControllerName;
    int32_t mStickAxisThresholdPercentage;
    int32_t mTriggerAxisThresholdPercentage;
};
} // namespace Ship
