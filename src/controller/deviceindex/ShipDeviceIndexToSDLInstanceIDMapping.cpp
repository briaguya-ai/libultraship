#include "ShipDeviceIndexToSDLInstanceIDMapping.h"
#include "utils/StringHelper.h"
#include "public/bridge/consolevariablebridge.h"

namespace Ship {
ShipDeviceIndexToSDLInstanceIDMapping::ShipDeviceIndexToSDLInstanceIDMapping(
    ShipDeviceIndex shipDeviceIndex, int32_t sdlInstanceID, std::string sdlJoystickGuid, std::string sdlControllerName,
    int32_t stickAxisThresholdPercentage, int32_t triggerAxisThresholdPercentage)
    : ShipDeviceIndexToPhysicalDeviceIndexMapping(shipDeviceIndex), mSDLInstanceID(sdlInstanceID),
      mSDLJoystickGUID(sdlJoystickGuid), mSDLControllerName(sdlControllerName),
      mStickAxisThresholdPercentage(stickAxisThresholdPercentage),
      mTriggerAxisThresholdPercentage(triggerAxisThresholdPercentage) {
}

ShipDeviceIndexToSDLInstanceIDMapping::~ShipDeviceIndexToSDLInstanceIDMapping() {
}

int32_t ShipDeviceIndexToSDLInstanceIDMapping::GetSDLInstanceID() {
    return mSDLInstanceID;
}

void ShipDeviceIndexToSDLInstanceIDMapping::SetSDLInstanceID(int32_t instanceID) {
    mSDLInstanceID = instanceID;
}

int32_t ShipDeviceIndexToSDLInstanceIDMapping::GetStickAxisThresholdPercentage() {
    return mStickAxisThresholdPercentage;
}

void ShipDeviceIndexToSDLInstanceIDMapping::SetStickAxisThresholdPercentage(int32_t stickAxisThresholdPercentage) {
    mStickAxisThresholdPercentage = stickAxisThresholdPercentage;
}

int32_t ShipDeviceIndexToSDLInstanceIDMapping::GetTriggerAxisThresholdPercentage() {
    return mTriggerAxisThresholdPercentage;
}

void ShipDeviceIndexToSDLInstanceIDMapping::SetTriggerAxisThresholdPercentage(int32_t triggerAxisThresholdPercentage) {
    mTriggerAxisThresholdPercentage = triggerAxisThresholdPercentage;
}

std::string ShipDeviceIndexToSDLInstanceIDMapping::GetJoystickGUID() {
    return mSDLJoystickGUID;
}

std::string ShipDeviceIndexToSDLInstanceIDMapping::GetSDLControllerName() {
    return mSDLControllerName;
}

void ShipDeviceIndexToSDLInstanceIDMapping::SaveToConfig() {
    const std::string mappingCvarKey = CVAR_PREFIX_CONTROLLERS ".DeviceMappings." + GetMappingId();
    CVarSetString(StringHelper::Sprintf("%s.DeviceMappingClass", mappingCvarKey.c_str()).c_str(),
                  "ShipDeviceIndexToSDLInstanceIDMapping");
    CVarSetInteger(StringHelper::Sprintf("%s.ShipDeviceIndex", mappingCvarKey.c_str()).c_str(), mShipDeviceIndex);
    CVarSetInteger(StringHelper::Sprintf("%s.SDLInstanceID", mappingCvarKey.c_str()).c_str(), mSDLInstanceID);
    CVarSetString(StringHelper::Sprintf("%s.SDLJoystickGUID", mappingCvarKey.c_str()).c_str(),
                  mSDLJoystickGUID.c_str());
    CVarSetString(StringHelper::Sprintf("%s.SDLControllerName", mappingCvarKey.c_str()).c_str(),
                  mSDLControllerName.c_str());
    CVarSetInteger(StringHelper::Sprintf("%s.StickAxisThresholdPercentage", mappingCvarKey.c_str()).c_str(),
                   mStickAxisThresholdPercentage);
    CVarSetInteger(StringHelper::Sprintf("%s.TriggerAxisThresholdPercentage", mappingCvarKey.c_str()).c_str(),
                   mTriggerAxisThresholdPercentage);
    CVarSave();
}

void ShipDeviceIndexToSDLInstanceIDMapping::EraseFromConfig() {
    const std::string mappingCvarKey = CVAR_PREFIX_CONTROLLERS ".DeviceMappings." + GetMappingId();

    CVarClear(StringHelper::Sprintf("%s.DeviceMappingClass", mappingCvarKey.c_str()).c_str());
    CVarClear(StringHelper::Sprintf("%s.ShipDeviceIndex", mappingCvarKey.c_str()).c_str());
    CVarClear(StringHelper::Sprintf("%s.SDLInstanceID", mappingCvarKey.c_str()).c_str());
    CVarClear(StringHelper::Sprintf("%s.SDLJoystickGUID", mappingCvarKey.c_str()).c_str());
    CVarClear(StringHelper::Sprintf("%s.SDLControllerName", mappingCvarKey.c_str()).c_str());
    CVarClear(StringHelper::Sprintf("%s.StickAxisThresholdPercentage", mappingCvarKey.c_str()).c_str());
    CVarClear(StringHelper::Sprintf("%s.TriggerAxisThresholdPercentage", mappingCvarKey.c_str()).c_str());

    CVarSave();
}
} // namespace Ship
