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
    void UpdateControllerNamesFromConfig();

    void HandlePhysicalDeviceConnect(int32_t sdlDeviceIndex);
    void HandlePhysicalDeviceDisconnect(int32_t sdlJoystickInstanceId);


    void InitializeMappingsSinglePlayer();

    void SaveMappingIdsToConfig();

  private:
    bool mIsInitialized;
};
} // namespace Ship
