#include "LEDMappingFactory.h"
#include "controller/controldevice/controller/mapping/sdl/SDLLEDMapping.h"
#include "public/bridge/consolevariablebridge.h"
#include "utils/StringHelper.h"
#include "libultraship/libultra/controller.h"
#include "Context.h"
#include "controller/deviceindex/ShipDeviceIndexToSDLInstanceIDMapping.h"

namespace Ship {
std::shared_ptr<ControllerLEDMapping> LEDMappingFactory::CreateLEDMappingFromConfig(uint8_t portIndex, std::string id) {
    const std::string mappingCvarKey = CVAR_PREFIX_CONTROLLERS ".LEDMappings." + id;
    const std::string mappingClass =
        CVarGetString(StringHelper::Sprintf("%s.LEDMappingClass", mappingCvarKey.c_str()).c_str(), "");

    int32_t colorSource = CVarGetInteger(StringHelper::Sprintf("%s.ColorSource", mappingCvarKey.c_str()).c_str(), -1);
    Color_RGB8 savedColor =
        CVarGetColor24(StringHelper::Sprintf("%s.SavedColor", mappingCvarKey.c_str()).c_str(), { 0, 0, 0 });

    if (colorSource != LED_COLOR_SOURCE_OFF && colorSource != LED_COLOR_SOURCE_SET &&
        colorSource != LED_COLOR_SOURCE_GAME) {
        // something about this mapping is invalid
        CVarClear(mappingCvarKey.c_str());
        CVarSave();
        return nullptr;
    }

    if (mappingClass == "SDLLEDMapping") {
        int32_t shipDeviceIndex =
            CVarGetInteger(StringHelper::Sprintf("%s.ShipDeviceIndex", mappingCvarKey.c_str()).c_str(), -1);

        if (shipDeviceIndex < 0) {
            // something about this mapping is invalid
            CVarClear(mappingCvarKey.c_str());
            CVarSave();
            return nullptr;
        }

        return std::make_shared<SDLLEDMapping>(static_cast<ShipDeviceIndex>(shipDeviceIndex), portIndex, colorSource,
                                               savedColor);
    }

    return nullptr;
}

std::shared_ptr<ControllerLEDMapping> LEDMappingFactory::CreateLEDMappingFromSDLInput(uint8_t portIndex) {
    std::unordered_map<ShipDeviceIndex, SDL_Gamepad*> sdlControllersWithLEDs;
    std::shared_ptr<ControllerLEDMapping> mapping = nullptr;
    for (auto [lusIndex, indexMapping] :
         Context::GetInstance()->GetControlDeck()->GetDeviceIndexMappingManager()->GetAllDeviceIndexMappings()) {
        auto sdlInstanceIDMapping = std::dynamic_pointer_cast<ShipDeviceIndexToSDLInstanceIDMapping>(indexMapping);

        if (sdlInstanceIDMapping == nullptr) {
            // this LUS index isn't mapped to an SDL instance id
            continue;
        }

        auto sdlInstanceID = sdlInstanceIDMapping->GetSDLInstanceID();

        if (!SDL_IsGamepad(sdlInstanceID)) {
            // this SDL device isn't a game controller
            continue;
        }

        auto controller = SDL_OpenGamepad(sdlInstanceID);
        // todo: SDL_GameControllerHasLED() - replaced with SDL_PROP_GAMEPAD_CAP_RGB_LED_BOOLEAN
        // if (SDL_GameControllerHasLED(controller)) {
        auto props = SDL_GetGamepadProperties(controller);
        if (false) {
            sdlControllersWithLEDs[lusIndex] = SDL_OpenGamepad(sdlInstanceID);
        } else {
            SDL_CloseGamepad(controller);
        }
    }

    for (auto [lusIndex, controller] : sdlControllersWithLEDs) {
        for (int32_t button = SDL_GAMEPAD_BUTTON_SOUTH; button < SDL_GAMEPAD_BUTTON_COUNT; button++) {
            if (SDL_GetGamepadButton(controller, static_cast<SDL_GamepadButton>(button))) {
                mapping = std::make_shared<SDLLEDMapping>(lusIndex, portIndex, 0, Color_RGB8({ 0, 0, 0 }));
                break;
            }
        }

        if (mapping != nullptr) {
            break;
        }

        for (int32_t i = SDL_GAMEPAD_AXIS_LEFTX; i < SDL_GAMEPAD_AXIS_COUNT; i++) {
            const auto axis = static_cast<SDL_GamepadAxis>(i);
            const auto axisValue = SDL_GetGamepadAxis(controller, axis) / 32767.0f;
            int32_t axisDirection = 0;
            if (axisValue < -0.7f) {
                axisDirection = NEGATIVE;
            } else if (axisValue > 0.7f) {
                axisDirection = POSITIVE;
            }

            if (axisDirection == 0) {
                continue;
            }

            mapping = std::make_shared<SDLLEDMapping>(lusIndex, portIndex, 0, Color_RGB8({ 0, 0, 0 }));
            break;
        }
    }

    for (auto [i, controller] : sdlControllersWithLEDs) {
        SDL_CloseGamepad(controller);
    }

    return mapping;
}
} // namespace Ship
