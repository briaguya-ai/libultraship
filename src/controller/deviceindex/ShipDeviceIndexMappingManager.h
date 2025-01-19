#pragma once

#include <unordered_map>
#include <memory>
#include <vector>

namespace Ship {
class ShipDeviceIndexMappingManager {
  public:
    ShipDeviceIndexMappingManager();
    ~ShipDeviceIndexMappingManager();

    void InitializeSDLMappingsForPort(uint8_t n64port);
    void HandlePhysicalDeviceConnect(int32_t sdlDeviceIndex);
    void HandlePhysicalDeviceDisconnect(int32_t sdlJoystickInstanceId);

    void InitializeMappingsSinglePlayer();

    void SaveMappingIdsToConfig();

  private:
    bool mIsInitialized;
    uint8_t GetPortIndexOfDisconnectedPhysicalDevice(int32_t sdlJoystickInstanceId);
    void HandlePhysicalDeviceDisconnectSinglePlayer(int32_t sdlJoystickInstanceId);
    void HandlePhysicalDeviceDisconnectMultiplayer(int32_t sdlJoystickInstanceId);
};
} // namespace Ship
