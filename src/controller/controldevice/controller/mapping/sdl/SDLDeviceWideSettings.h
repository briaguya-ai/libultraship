#pragma once

#include <cstdint>

namespace Ship {
class SDLDeviceWideSettings {
  public:
    SDLDeviceWideSettings(int32_t sdlDeviceIndex, int32_t stickAxisThresholdPercentage, int32_t triggerAxisThresholdPercentage);
    ~SDLDeviceWideSettings();

    int32_t GetStickAxisThresholdPercentage();
    void SetStickAxisThresholdPercentage(int32_t stickAxisThresholdPercentage);

    int32_t GetTriggerAxisThresholdPercentage();
    void SetTriggerAxisThresholdPercentage(int32_t triggerAxisThresholdPercentage);

  private:
    int32_t mDeviceIndex;
    int32_t mStickAxisThresholdPercentage;
    int32_t mTriggerAxisThresholdPercentage;
};
} // namespace Ship