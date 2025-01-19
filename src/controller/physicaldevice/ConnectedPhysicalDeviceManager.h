#pragma once

#include <unordered_map>
#include <SDL2/SDL.h>

namespace Ship {

class ConnectedPhysicalDeviceManager {
  public:
    ConnectedPhysicalDeviceManager();
    ~ConnectedPhysicalDeviceManager();

    std::unordered_map<int32_t, SDL_GameController*> GetConnectedSDLGamepadsForPort(uint8_t portIndex);

    void HandlePhysicalDeviceConnect(int32_t sdlDeviceIndex);
    void HandlePhysicalDeviceDisconnect(int32_t sdlJoystickInstanceId);
    void RefreshConnectedSDLGamepads();

  private:
    std::unordered_map<int32_t, SDL_GameController*> mConnectedSDLGamepads;
};
} // namespace Ship
