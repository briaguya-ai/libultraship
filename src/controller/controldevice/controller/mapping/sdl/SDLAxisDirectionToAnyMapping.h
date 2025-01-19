#pragma once

#include "controller/controldevice/controller/mapping/ControllerInputMapping.h"

namespace Ship {

enum Axis { X = 0, Y = 1 };
enum AxisDirection { NEGATIVE = -1, POSITIVE = 1 };

class SDLAxisDirectionToAnyMapping : virtual public ControllerInputMapping {
  public:
    SDLAxisDirectionToAnyMapping(int32_t sdlControllerAxis, int32_t axisDirection);
    ~SDLAxisDirectionToAnyMapping();
    std::string GetPhysicalInputName() override;
    std::string GetPhysicalDeviceName() override;
    bool PhysicalDeviceIsConnected() override;
    bool AxisIsTrigger();
    bool AxisIsStick();

  protected:
    SDL_GameControllerAxis mControllerAxis;
    AxisDirection mAxisDirection;
};
} // namespace Ship
