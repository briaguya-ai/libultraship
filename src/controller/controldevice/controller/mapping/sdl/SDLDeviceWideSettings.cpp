#include "SDLDeviceWideSettings.h"

namespace Ship {
SDLDeviceWideSettings::SDLDeviceWideSettings(int32_t sdlDeviceIndex, int32_t stickAxisThresholdPercentage, int32_t triggerAxisThresholdPercentage) : mDeviceIndex(sdlDeviceIndex) {
} 

SDLDeviceWideSettings::~SDLDeviceWideSettings() {
}

int32_t SDLDeviceWideSettings::GetStickAxisThresholdPercentage() {
    return mStickAxisThresholdPercentage;
}

void SDLDeviceWideSettings::SetStickAxisThresholdPercentage(int32_t stickAxisThresholdPercentage) {
    mStickAxisThresholdPercentage = stickAxisThresholdPercentage;
}

int32_t SDLDeviceWideSettings::GetTriggerAxisThresholdPercentage() {
    return mTriggerAxisThresholdPercentage;
}

void SDLDeviceWideSettings::SetTriggerAxisThresholdPercentage(int32_t triggerAxisThresholdPercentage) {
    mTriggerAxisThresholdPercentage = triggerAxisThresholdPercentage;
}
} // namespace Ship
