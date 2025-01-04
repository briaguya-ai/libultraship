#include "ShipDeviceIndexToSDLJoystickInstanceIDMapping.h"
#include "utils/StringHelper.h"
#include "public/bridge/consolevariablebridge.h"

namespace Ship {
ShipDeviceIndexToSDLJoystickInstanceIDMapping::ShipDeviceIndexToSDLJoystickInstanceIDMapping(
    ShipDeviceIndex shipDeviceIndex, int32_t sdlJoystickInstanceID, std::string sdlJoystickGuid,
    std::string sdlControllerName, int32_t stickAxisThresholdPercentage, int32_t triggerAxisThresholdPercentage)
    : ShipDeviceIndexToPhysicalDeviceIndexMapping(shipDeviceIndex), mSDLJoystickInstanceID(sdlJoystickInstanceID),
      mSDLJoystickGUID(sdlJoystickGuid), mSDLControllerName(sdlControllerName),
      mStickAxisThresholdPercentage(stickAxisThresholdPercentage),
      mTriggerAxisThresholdPercentage(triggerAxisThresholdPercentage) {
}

ShipDeviceIndexToSDLJoystickInstanceIDMapping::~ShipDeviceIndexToSDLJoystickInstanceIDMapping() {
}

int32_t ShipDeviceIndexToSDLJoystickInstanceIDMapping::GetSDLJoystickInstanceID() {
    return mSDLJoystickInstanceID;
}

void ShipDeviceIndexToSDLJoystickInstanceIDMapping::SetSDLJoystickInstanceID(int32_t instanceId) {
    mSDLJoystickInstanceID = instanceId;
}

int32_t ShipDeviceIndexToSDLJoystickInstanceIDMapping::GetStickAxisThresholdPercentage() {
    return mStickAxisThresholdPercentage;
}

void ShipDeviceIndexToSDLJoystickInstanceIDMapping::SetStickAxisThresholdPercentage(
    int32_t stickAxisThresholdPercentage) {
    mStickAxisThresholdPercentage = stickAxisThresholdPercentage;
}

int32_t ShipDeviceIndexToSDLJoystickInstanceIDMapping::GetTriggerAxisThresholdPercentage() {
    return mTriggerAxisThresholdPercentage;
}

void ShipDeviceIndexToSDLJoystickInstanceIDMapping::SetTriggerAxisThresholdPercentage(
    int32_t triggerAxisThresholdPercentage) {
    mTriggerAxisThresholdPercentage = triggerAxisThresholdPercentage;
}

std::string ShipDeviceIndexToSDLJoystickInstanceIDMapping::GetJoystickGUID() {
    return mSDLJoystickGUID;
}

std::string ShipDeviceIndexToSDLJoystickInstanceIDMapping::GetSDLControllerName() {
    return mSDLControllerName;
}

void ShipDeviceIndexToSDLJoystickInstanceIDMapping::SaveToConfig() {
    const std::string mappingCvarKey = CVAR_PREFIX_CONTROLLERS ".DeviceMappings." + GetMappingId();
    CVarSetString(StringHelper::Sprintf("%s.DeviceMappingClass", mappingCvarKey.c_str()).c_str(),
                  "ShipDeviceIndexToSDLJoystickInstanceIDMapping");
    CVarSetInteger(StringHelper::Sprintf("%s.ShipDeviceIndex", mappingCvarKey.c_str()).c_str(), mShipDeviceIndex);
    CVarSetInteger(StringHelper::Sprintf("%s.SDLJoystickInstanceID", mappingCvarKey.c_str()).c_str(),
                   mSDLJoystickInstanceID);
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

void ShipDeviceIndexToSDLJoystickInstanceIDMapping::EraseFromConfig() {
    const std::string mappingCvarKey = CVAR_PREFIX_CONTROLLERS ".DeviceMappings." + GetMappingId();

    CVarClear(StringHelper::Sprintf("%s.DeviceMappingClass", mappingCvarKey.c_str()).c_str());
    CVarClear(StringHelper::Sprintf("%s.ShipDeviceIndex", mappingCvarKey.c_str()).c_str());
    CVarClear(StringHelper::Sprintf("%s.SDLJoystickInstanceID", mappingCvarKey.c_str()).c_str());
    CVarClear(StringHelper::Sprintf("%s.SDLJoystickGUID", mappingCvarKey.c_str()).c_str());
    CVarClear(StringHelper::Sprintf("%s.SDLControllerName", mappingCvarKey.c_str()).c_str());
    CVarClear(StringHelper::Sprintf("%s.StickAxisThresholdPercentage", mappingCvarKey.c_str()).c_str());
    CVarClear(StringHelper::Sprintf("%s.TriggerAxisThresholdPercentage", mappingCvarKey.c_str()).c_str());

    CVarSave();
}
} // namespace Ship
