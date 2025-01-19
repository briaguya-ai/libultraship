#pragma once

#include "stdint.h"
#include "window/gui/GuiWindow.h"
#include <imgui.h>
#include <unordered_map>
#include <string>
#include <vector>
#include "controller/controldevice/controller/Controller.h"
#include "ShipDeviceType.h"

namespace Ship {

class ControllerDisconnectedWindow : public GuiWindow {
  public:
    using GuiWindow::GuiWindow;
    ~ControllerDisconnectedWindow();

  protected:
    void InitElement() override;
    void DrawElement() override;
    void UpdateElement() override;
};
} // namespace Ship
