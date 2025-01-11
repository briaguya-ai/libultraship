#pragma once

#include <cstdint>
#include <string>
#include "ShipDeviceIndexToPhysicalDeviceIndexMapping.h"

namespace Ship {

class ShipDeviceIndexToSDLInstanceIDMapping : public ShipDeviceIndexToPhysicalDeviceIndexMapping {
  public:
    ShipDeviceIndexToSDLInstanceIDMapping(ShipDeviceIndex shipDeviceIndex, int32_t sdlInstanceID,
                                           std::string sdlJoystickGuid, std::string sdlControllerName,
                                           int32_t stickAxisThresholdPercentage,
                                           int32_t triggerAxisThresholdPercentage);
    ~ShipDeviceIndexToSDLInstanceIDMapping();
    std::string GetJoystickGUID();
    std::string GetSDLControllerName();

    void SaveToConfig() override;
    void EraseFromConfig() override;

    int32_t GetSDLInstanceID();
    void SetSDLInstanceID(int32_t instanceID);

    int32_t GetStickAxisThresholdPercentage();
    void SetStickAxisThresholdPercentage(int32_t stickAxisThresholdPercentage);

    int32_t GetTriggerAxisThresholdPercentage();
    void SetTriggerAxisThresholdPercentage(int32_t triggerAxisThresholdPercentage);

  private:
    int32_t mSDLInstanceID;
    std::string mSDLJoystickGUID;
    std::string mSDLControllerName;
    int32_t mStickAxisThresholdPercentage;
    int32_t mTriggerAxisThresholdPercentage;
};
} // namespace Ship
