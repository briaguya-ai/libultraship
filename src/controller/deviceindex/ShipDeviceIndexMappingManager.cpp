#include "ShipDeviceIndexMappingManager.h"
#include <SDL2/SDL.h>
#include "utils/StringHelper.h"
#include "public/bridge/consolevariablebridge.h"
#include <vector>
#include "Context.h"
#include "ControllerDisconnectedWindow.h"
#include "ControllerReorderingWindow.h"

#include <sstream>

namespace Ship {
ShipDeviceIndexMappingManager::ShipDeviceIndexMappingManager() : mIsInitialized(false) {
#if _WIN32
    SDL_SetHintWithPriority(SDL_HINT_JOYSTICK_WGI, "0", SDL_HINT_DEFAULT);
#endif
    UpdateControllerNamesFromConfig();
}

ShipDeviceIndexMappingManager::~ShipDeviceIndexMappingManager() {
}

void ShipDeviceIndexMappingManager::HandlePhysicalDeviceConnect(int32_t sdlDeviceIndex) {
    if (!mIsInitialized) {
        return;
    }

    if (!SDL_IsGameController(sdlDeviceIndex)) {
        return;
    }

    auto controllerDisconnectedWindow = std::dynamic_pointer_cast<ControllerDisconnectedWindow>(
        Context::GetInstance()->GetWindow()->GetGui()->GetGuiWindow("Controller Disconnected"));
    if (controllerDisconnectedWindow != nullptr && controllerDisconnectedWindow->IsVisible()) {
        // don't try to automap if we are looking at the controller disconnected modal
        return;
    }

    std::set<ShipDeviceIndex> alreadyConnectedDevices;
    for (auto mapping : Context::GetInstance()->GetControlDeck()->GetControllerByPort(0)->GetAllMappings()) {
        auto sdlMapping = std::dynamic_pointer_cast<SDLMapping>(mapping);
        if (sdlMapping == nullptr) {
            continue;
        }

        if (sdlMapping->ControllerLoaded()) {
            alreadyConnectedDevices.insert(sdlMapping->GetShipDeviceIndex());
        }
    }

    for (auto [lusIndex, mapping] : mShipDeviceIndexToPhysicalDeviceIndexMappings) {
        auto sdlMapping = dynamic_pointer_cast<ShipDeviceIndexToSDLDeviceIndexMapping>(mapping);
        if (sdlMapping == nullptr) {
            continue;
        }

        if (alreadyConnectedDevices.contains(lusIndex)) {
            sdlMapping->SetSDLDeviceIndex(GetNewSDLDeviceIndexFromShipDeviceIndex(lusIndex));
            sdlMapping->SaveToConfig();
        }
    }

    InitializeSDLMappingsForPort(0, sdlDeviceIndex);
    return;
}

void ShipDeviceIndexMappingManager::HandlePhysicalDeviceDisconnect(int32_t sdlJoystickInstanceId) {
    Context::GetInstance()->GetWindow()->GetGui()->GetGameOverlay()->TextDrawNotification(
        5, true, "Gamepad disconnected");
}

} // namespace Ship
