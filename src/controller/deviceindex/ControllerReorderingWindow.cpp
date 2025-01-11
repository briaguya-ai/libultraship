#include "ControllerReorderingWindow.h"
#include "utils/StringHelper.h"
#include <SDL3/SDL.h>
#include <algorithm>
#include "Context.h"

namespace Ship {

ControllerReorderingWindow::~ControllerReorderingWindow() {
}

void ControllerReorderingWindow::InitElement() {
    mGamepadInstanceIDs.clear();
    mCurrentPortNumber = 1;
}

void ControllerReorderingWindow::UpdateElement() {
}

int32_t ControllerReorderingWindow::GetSDLInstanceIDFromSDLInput() {
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

void ControllerReorderingWindow::DrawElement() {
    if (Context::GetInstance()->GetControlDeck()->IsSinglePlayerMappingMode()) {
        Context::GetInstance()->GetControlDeck()->GetDeviceIndexMappingManager()->InitializeMappingsSinglePlayer();
        Hide();
        return;
    }

    // if we don't have more than one controller, just close the window
    int i, numJoysticks;
    std::vector<int32_t> connectedSdlGamepadInstanceIDs;
    SDL_JoystickID *joysticks = SDL_GetJoysticks(&numJoysticks);
    if (joysticks) {
        for (i = 0; i < numJoysticks; ++i) {
            SDL_JoystickID instanceId = joysticks[i];
            if (SDL_IsGamepad(instanceId)) {
                connectedSdlGamepadInstanceIDs.push_back(instanceId);
            }
        }
        SDL_free(joysticks);
    }
    if (connectedSdlGamepadInstanceIDs.size() <= 1) {
        Context::GetInstance()->GetControlDeck()->GetDeviceIndexMappingManager()->InitializeMappingsMultiplayer(
            connectedSdlGamepadInstanceIDs);
        Hide();
        return;
    }

    // we have more than one controller, prompt the user for order
    if (mCurrentPortNumber <= std::min(connectedSdlGamepadInstanceIDs.size(), static_cast<size_t>(MAXCONTROLLERS))) {
        ImGui::OpenPopup("Set Controller");
        if (ImGui::BeginPopupModal("Set Controller", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Press any button or move any axis\non the controller for port %d", mCurrentPortNumber);

            auto instanceID = GetSDLInstanceIDFromSDLInput();
            if (instanceID != -1 && std::find(mGamepadInstanceIDs.begin(), mGamepadInstanceIDs.end(), instanceID) == mGamepadInstanceIDs.end()) {
                mGamepadInstanceIDs.push_back(instanceID);
                mCurrentPortNumber++;
                ImGui::CloseCurrentPopup();
            }

            if (mCurrentPortNumber > 1 &&
                ImGui::Button(StringHelper::Sprintf("Play with %d controller%s", mCurrentPortNumber - 1,
                                                    mCurrentPortNumber > 2 ? "s" : "")
                                  .c_str())) {
                mCurrentPortNumber =
                    MAXCONTROLLERS + 1; // stop showing modals, it will be reset to 1 after the mapping init stuff
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        return;
    }

    Context::GetInstance()->GetControlDeck()->GetDeviceIndexMappingManager()->InitializeMappingsMultiplayer(
        mGamepadInstanceIDs);
        mGamepadInstanceIDs.clear();
    mCurrentPortNumber = 1;
    Hide();
}
} // namespace Ship