#pragma once

#include <cstdint>
#include <SDL3/SDL.h>
#include <memory>
#include <string>
#include "controller/controldevice/controller/mapping/ControllerMapping.h"

namespace Ship {
enum Axis { X = 0, Y = 1 };
enum AxisDirection { NEGATIVE = -1, POSITIVE = 1 };

class SDLMapping : public ControllerMapping {
  public:
    SDLMapping(ShipDeviceIndex shipDeviceIndex);
    ~SDLMapping();
    int32_t GetJoystickInstanceId();
    int32_t GetCurrentSDLInstanceID();
    bool CloseController();
    bool ControllerLoaded(bool closeIfDisconnected = false);

  protected:
    SDL_GamepadType GetSDLControllerType();
    uint16_t GetSDLControllerVendorId();
    uint16_t GetSDLControllerProductId();
    bool UsesPlaystationLayout();
    bool UsesSwitchLayout();
    bool UsesXboxLayout();
    bool UsesGameCubeLayout();
    std::string GetSDLDeviceName();
    int32_t GetSDLInstanceID();

    SDL_Gamepad* mController;

  private:
    bool OpenController();
    std::string GetSDLControllerName();
};
} // namespace Ship
