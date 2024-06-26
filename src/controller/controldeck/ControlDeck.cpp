#include "ControlDeck.h"

#include "Context.h"
#include "controller/controldevice/controller/Controller.h"
#include <Utils/StringHelper.h>
#include "public/bridge/consolevariablebridge.h"
#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include <imgui.h>
#include "controller/deviceindex/ShipDeviceIndexMappingManager.h"

namespace Ship {

ControlDeck::ControlDeck(std::vector<CONTROLLERBUTTONS_T> additionalBitmasks)
    : mPads(nullptr), mSinglePlayerMappingMode(false) {
    for (int32_t i = 0; i < MAXCONTROLLERS; i++) {
        mPorts.push_back(std::make_shared<ControlPort>(i, std::make_shared<Controller>(i, additionalBitmasks)));
    }

    mDeviceIndexMappingManager = std::make_shared<ShipDeviceIndexMappingManager>();
}

ControlDeck::ControlDeck() : ControlDeck(std::vector<CONTROLLERBUTTONS_T>()) {
}

ControlDeck::~ControlDeck() {
    SPDLOG_TRACE("destruct control deck");
}

void ControlDeck::Init(uint8_t* controllerBits) {
    mControllerBits = controllerBits;
    *mControllerBits |= 1 << 0;

    for (auto port : mPorts) {
        if (port->GetConnectedController()->HasConfig()) {
            port->GetConnectedController()->ReloadAllMappingsFromConfig();
        }
    }

    // if we don't have a config for controller 1, set default keyboard bindings
    if (!mPorts[0]->GetConnectedController()->HasConfig()) {
        mPorts[0]->GetConnectedController()->AddDefaultMappings(ShipDeviceIndex::Keyboard);
    }

    Context::GetInstance()->GetWindow()->GetGui()->GetGuiWindow("Controller Reordering")->Show();
}

bool ControlDeck::ProcessKeyboardEvent(KbEventType eventType, KbScancode scancode) {
    bool result = false;
    for (auto port : mPorts) {
        auto controller = port->GetConnectedController();

        if (controller != nullptr) {
            result = controller->ProcessKeyboardEvent(eventType, scancode) || result;
        }
    }

    return result;
}

bool ControlDeck::AllGameInputBlocked() {
    return !mGameInputBlockers.empty();
}

bool ControlDeck::GamepadGameInputBlocked() {
    // block controller input when using the controller to navigate imgui menus
    return AllGameInputBlocked() ||
           (CVarGetInteger(CVAR_MENU_BAR_OPEN, 0) && CVarGetInteger(CVAR_IMGUI_CONTROLLER_NAV, 0));
}

bool ControlDeck::KeyboardGameInputBlocked() {
    // block keyboard input when typing in imgui
    return AllGameInputBlocked() || ImGui::GetIO().WantCaptureKeyboard;
}

void ControlDeck::WriteToPad(OSContPad* pad) {
    SDL_PumpEvents();

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

std::shared_ptr<ShipDeviceIndexMappingManager> ControlDeck::GetDeviceIndexMappingManager() {
    return mDeviceIndexMappingManager;
}

void ControlDeck::SetSinglePlayerMappingMode(bool singlePlayer) {
    mSinglePlayerMappingMode = singlePlayer;
}

bool ControlDeck::IsSinglePlayerMappingMode() {
    return mSinglePlayerMappingMode;
}
} // namespace Ship
