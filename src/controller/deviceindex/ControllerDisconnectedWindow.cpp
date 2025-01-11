#include "ControllerDisconnectedWindow.h"
#include "utils/StringHelper.h"
#include <SDL3/SDL.h>
#include <algorithm>
#include "Context.h"

namespace Ship {

ControllerDisconnectedWindow::~ControllerDisconnectedWindow() {
}

void ControllerDisconnectedWindow::InitElement() {
    mPortIndexOfDisconnectedController = UINT8_MAX;
}

void ControllerDisconnectedWindow::UpdateElement() {
    SDL_PumpEvents();
    SDL_Event event;
    while (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_EVENT_GAMEPAD_ADDED, SDL_EVENT_GAMEPAD_ADDED) > 0) {
        // from https://wiki.libsdl.org/SDL2/SDL_GamepadDeviceEvent: which - the joystick device index for
        // the SDL_EVENT_GAMEPAD_ADDED event
        Context::GetInstance()->GetControlDeck()->GetDeviceIndexMappingManager()->HandlePhysicalDeviceConnect(
            event.gdevice.which);
    }

    while (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_EVENT_GAMEPAD_REMOVED, SDL_EVENT_GAMEPAD_REMOVED) > 0) {
        // from https://wiki.libsdl.org/SDL2/SDL_GamepadDeviceEvent: which - the [...] instance id for the
        // SDL_EVENT_GAMEPAD_REMOVED [...] event
        Context::GetInstance()->GetControlDeck()->GetDeviceIndexMappingManager()->HandlePhysicalDeviceDisconnect(
            event.gdevice.which);
    }
}

int32_t ControllerDisconnectedWindow::GetSDLIndexFromSDLInput() {
    int32_t sdlInstanceID = -1;

    std::unordered_map<int32_t, SDL_Gamepad*> sdlGamepads;
    int i, numJoysticks;
    SDL_JoystickID *joysticks = SDL_GetJoysticks(&numJoysticks);
    if (joysticks) {
        for (i = 0; i < numJoysticks; ++i) {
            SDL_JoystickID instanceId = joysticks[i];
            if (SDL_IsGamepad(instanceId)) {
                sdlGamepads[instanceId] = SDL_OpenGamepad(instanceId);
            }
        }
        SDL_free(joysticks);
    }

    for (auto [gamepadInstanceID, gamepad] : sdlGamepads) {
        for (int32_t button = SDL_GAMEPAD_BUTTON_SOUTH; button < SDL_GAMEPAD_BUTTON_COUNT; button++) {
            if (SDL_GetGamepadButton(gamepad, static_cast<SDL_GamepadButton>(button))) {
                sdlInstanceID = gamepadInstanceID;
                break;
            }
        }

        if (sdlInstanceID != -1) {
            break;
        }

        for (int32_t i = SDL_GAMEPAD_AXIS_LEFTX; i < SDL_GAMEPAD_AXIS_COUNT; i++) {
            const auto axis = static_cast<SDL_GamepadAxis>(i);
            const auto axisValue = SDL_GetGamepadAxis(gamepad, axis) / 32767.0f;
            if (axisValue < -0.7f || axisValue > 0.7f) {
                sdlInstanceID = gamepadInstanceID;
                break;
            }
        }
    }

    for (auto [i, gamepad] : sdlGamepads) {
        SDL_CloseGamepad(gamepad);
    }

    return sdlInstanceID;
}

void ControllerDisconnectedWindow::DrawKnownControllerDisconnected() {
    ImGui::Text("Controller for port %d disconnected.\nPress any button or move any axis\non an unused controller "
                "for port %d.",
                mPortIndexOfDisconnectedController + 1, mPortIndexOfDisconnectedController + 1);

    auto index = GetSDLIndexFromSDLInput();
    if (index != -1 &&
        Context::GetInstance()->GetControlDeck()->GetDeviceIndexMappingManager()->GetShipDeviceIndexFromSDLInstanceID(
            index) == ShipDeviceIndex::Max) {
        Context::GetInstance()->GetControlDeck()->GetDeviceIndexMappingManager()->InitializeSDLMappingsForPort(
            mPortIndexOfDisconnectedController, index);
        mPortIndexOfDisconnectedController = UINT8_MAX;
        ImGui::CloseCurrentPopup();
        Hide();
    }

    if (ImGui::Button(StringHelper::Sprintf("Play without controller connected to port %d",
                                            mPortIndexOfDisconnectedController + 1)
                          .c_str())) {
        mPortIndexOfDisconnectedController = UINT8_MAX;
        ImGui::CloseCurrentPopup();
        Hide();
    }

    int i, numJoysticks;
    uint8_t connectedSdlControllerCount = 0;
    SDL_JoystickID *joysticks = SDL_GetJoysticks(&numJoysticks);
    if (joysticks) {
        for (i = 0; i < numJoysticks; ++i) {
            SDL_JoystickID instanceId = joysticks[i];
            if (SDL_IsGamepad(instanceId)) {
                connectedSdlControllerCount++;
            }
        }
        SDL_free(joysticks);
    }

    if (connectedSdlControllerCount != 0 &&
        ImGui::Button(connectedSdlControllerCount > 1
                          ? "Reorder all controllers###reorderControllersButton"
                          : "Use connected controller for port 1###reorderControllersButton")) {
        mPortIndexOfDisconnectedController = UINT8_MAX;
        ImGui::CloseCurrentPopup();
        Context::GetInstance()->GetWindow()->GetGui()->GetGuiWindow("Controller Reordering")->Show();
        Hide();
    }
}

void ControllerDisconnectedWindow::DrawUnknownOrMultipleControllersDisconnected() {
    ImGui::Text("Controller(s) disconnected.");

    int i, numJoysticks;
    uint8_t connectedSdlControllerCount = 0;
    SDL_JoystickID *joysticks = SDL_GetJoysticks(&numJoysticks);
    if (joysticks) {
        for (i = 0; i < numJoysticks; ++i) {
            SDL_JoystickID instanceId = joysticks[i];
            if (SDL_IsGamepad(instanceId)) {
                connectedSdlControllerCount++;
            }
        }
        SDL_free(joysticks);
    }

    if (connectedSdlControllerCount != 0 &&
        ImGui::Button(connectedSdlControllerCount > 1
                          ? "Reorder all controllers###reorderControllersButton"
                          : "Use connected controller for port 1###reorderControllersButton")) {
        mPortIndexOfDisconnectedController = UINT8_MAX;
        ImGui::CloseCurrentPopup();
        Context::GetInstance()->GetWindow()->GetGui()->GetGuiWindow("Controller Reordering")->Show();
        Hide();
    }
}

void ControllerDisconnectedWindow::DrawElement() {
    // todo: don't use UINT8_MAX to mean we don't have a disconnected controller
    if (mPortIndexOfDisconnectedController == UINT8_MAX) {
        return;
    }

    if (mPortIndexOfDisconnectedController == UINT8_MAX - 1) {
        DrawUnknownOrMultipleControllersDisconnected();
    } else {
        DrawKnownControllerDisconnected();
    }
}

void ControllerDisconnectedWindow::SetPortIndexOfDisconnectedController(uint8_t portIndex) {
    // todo: don't use UINT8_MAX to mean we don't have a disconnected controller
    if (mPortIndexOfDisconnectedController == UINT8_MAX) {
        mPortIndexOfDisconnectedController = portIndex;
        return;
    }

    // todo: don't use UINT8_MAX-1 to mean multiple controllers disconnected
    mPortIndexOfDisconnectedController = UINT8_MAX - 1;
}
} // namespace Ship