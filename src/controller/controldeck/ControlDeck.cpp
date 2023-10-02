#include "ControlDeck.h"

#include "Context.h"
#include "controller/controldevice/controller/Controller.h"
#include <Utils/StringHelper.h>
#include "public/bridge/consolevariablebridge.h"
#include <imgui.h>

// #ifdef __WIIU__
// #include "port/wiiu/WiiUGamepad.h"
// #include "port/wiiu/WiiUController.h"
// #endif

namespace LUS {

ControlDeck::ControlDeck() : mPads(nullptr) {
    for (int32_t i = 0; i < 4; i++) {
        mPorts.push_back(std::make_shared<ControlPort>(i, std::make_shared<Controller>(i)));
    }
}

ControlDeck::~ControlDeck() {
    SPDLOG_TRACE("destruct control deck");
}

void ControlDeck::Init(uint8_t* bits) {
    mControllerBits = bits;
    *mControllerBits |= 1 << 0;

    for (auto port : mPorts) {
        if (port->GetConnectedController()->HasConfig()) {
            port->GetConnectedController()->ReloadAllMappingsFromConfig();
        } else {
            port->GetConnectedController()->ResetToDefaultMappings(port->GetConnectedController()->GetPortIndex() == 0,
                                                                   true,
                                                                   port->GetConnectedController()->GetPortIndex());
        }
    }
}

bool ControlDeck::ProcessKeyboardEvent(KbEventType eventType, KbScancode scancode) {
    bool result = false;
    for (auto port : mPorts) {
        auto controller = port->GetConnectedController();

        if (controller != nullptr) {
            result = result || controller->ProcessKeyboardEvent(eventType, scancode);
        }
    }

    return result;
}

bool ControlDeck::AllGameInputBlocked() {
    return !mGameInputBlockers.empty();
}

bool ControlDeck::GamepadGameInputBlocked() {
    // block controller input when using the controller to navigate imgui menus
    return AllGameInputBlocked() || (CVarGetInteger("gOpenMenuBar", 0) && CVarGetInteger("gControlNav", 0));
}

bool ControlDeck::KeyboardGameInputBlocked() {
    // block keyboard input when typing in imgui
    return AllGameInputBlocked() || ImGui::GetIO().WantCaptureKeyboard;
}

void ControlDeck::WriteToPad(OSContPad* pad) {
    if (AllGameInputBlocked()) {
        return;
    }

    mPads = pad;

    for (size_t i = 0; i < mPorts.size(); i++) {
        const std::shared_ptr<Controller> controller = mPorts[i]->GetConnectedController();

        if (controller != nullptr) {
            controller->ReadToPad(&pad[i]);
        }
    }
}

OSContPad* ControlDeck::GetPads() {
    return mPads;
}

std::shared_ptr<Controller> ControlDeck::GetControllerByPort(uint8_t port) {
    return mPorts[port]->GetConnectedController();
}

void ControlDeck::BlockGameInput(int32_t blockId) {
    mGameInputBlockers[blockId] = true;
}

void ControlDeck::UnblockGameInput(int32_t blockId) {
    mGameInputBlockers.erase(blockId);
}
} // namespace LUS
