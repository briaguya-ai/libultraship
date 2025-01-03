#include "RumbleMappingFactory.h"
#include "controller/controldevice/controller/mapping/sdl/SDLRumbleMapping.h"
#include "public/bridge/consolevariablebridge.h"
#include "utils/StringHelper.h"
#include "libultraship/libultra/controller.h"
#include "Context.h"

namespace Ship {
std::shared_ptr<ControllerRumbleMapping> RumbleMappingFactory::CreateRumbleMappingFromConfig(uint8_t portIndex,
                                                                                             std::string id) {
    const std::string mappingCvarKey = CVAR_PREFIX_CONTROLLERS ".RumbleMappings." + id;
    const std::string mappingClass =
        CVarGetString(StringHelper::Sprintf("%s.RumbleMappingClass", mappingCvarKey.c_str()).c_str(), "");

    int32_t lowFrequencyIntensityPercentage =
        CVarGetInteger(StringHelper::Sprintf("%s.LowFrequencyIntensity", mappingCvarKey.c_str()).c_str(), -1);
    int32_t highFrequencyIntensityPercentage =
        CVarGetInteger(StringHelper::Sprintf("%s.HighFrequencyIntensity", mappingCvarKey.c_str()).c_str(), -1);

    if (lowFrequencyIntensityPercentage < 0 || lowFrequencyIntensityPercentage > 100 ||
        highFrequencyIntensityPercentage < 0 || highFrequencyIntensityPercentage > 100) {
        // something about this mapping is invalid
        CVarClear(mappingCvarKey.c_str());
        CVarSave();
        return nullptr;
    }

    if (mappingClass == "SDLRumbleMapping") {
        int32_t shipDeviceIndex =
            CVarGetInteger(StringHelper::Sprintf("%s.ShipDeviceIndex", mappingCvarKey.c_str()).c_str(), -1);

        if (shipDeviceIndex < 0) {
            // something about this mapping is invalid
            CVarClear(mappingCvarKey.c_str());
            CVarSave();
            return nullptr;
        }

        return std::make_shared<SDLRumbleMapping>(static_cast<ShipDeviceIndex>(shipDeviceIndex), portIndex,
                                                  lowFrequencyIntensityPercentage, highFrequencyIntensityPercentage);
    }

    return nullptr;
}

std::vector<std::shared_ptr<ControllerRumbleMapping>>
RumbleMappingFactory::CreateDefaultSDLRumbleMappings(ShipDeviceIndex shipDeviceIndex, uint8_t portIndex) {
    if (!SHIP_DEVICE_INDEX_IS_SDL(shipDeviceIndex)) {
        return std::vector<std::shared_ptr<ControllerRumbleMapping>>();
    }

    std::vector<std::shared_ptr<ControllerRumbleMapping>> mappings = { std::make_shared<SDLRumbleMapping>(
        shipDeviceIndex, portIndex, DEFAULT_LOW_FREQUENCY_RUMBLE_PERCENTAGE,
        DEFAULT_HIGH_FREQUENCY_RUMBLE_PERCENTAGE) };

    return mappings;
}

std::shared_ptr<ControllerRumbleMapping> RumbleMappingFactory::CreateRumbleMappingFromSDLInput(uint8_t portIndex) {
    std::unordered_map<ShipDeviceIndex, SDL_GameController*> sdlControllersWithRumble;
    std::shared_ptr<ControllerRumbleMapping> mapping = nullptr;

    for (auto i = 0; i < SDL_NumJoysticks(); i++) {
        if (SDL_IsGameController(i)) {
            auto controller = SDL_GameControllerOpen(i);
            bool hasRumble = SDL_GameControllerHasRumble(controller);
    
            if (hasRumble) {
                sdlControllersWithRumble[static_cast<ShipDeviceIndex>(i)] = SDL_GameControllerOpen(i);
            } else {
                SDL_GameControllerClose(controller);
            }
        }
    }

    for (auto [deviceIndex, controller] : sdlControllersWithRumble) {
        for (int32_t button = SDL_CONTROLLER_BUTTON_A; button < SDL_CONTROLLER_BUTTON_MAX; button++) {
            if (SDL_GameControllerGetButton(controller, static_cast<SDL_GameControllerButton>(button))) {
                mapping =
                    std::make_shared<SDLRumbleMapping>(deviceIndex, portIndex, DEFAULT_LOW_FREQUENCY_RUMBLE_PERCENTAGE,
                                                       DEFAULT_HIGH_FREQUENCY_RUMBLE_PERCENTAGE);
                break;
            }
        }

        if (mapping != nullptr) {
            break;
        }

        for (int32_t i = SDL_CONTROLLER_AXIS_LEFTX; i < SDL_CONTROLLER_AXIS_MAX; i++) {
            const auto axis = static_cast<SDL_GameControllerAxis>(i);
            const auto axisValue = SDL_GameControllerGetAxis(controller, axis) / 32767.0f;
            int32_t axisDirection = 0;
            if (axisValue < -0.7f) {
                axisDirection = NEGATIVE;
            } else if (axisValue > 0.7f) {
                axisDirection = POSITIVE;
            }

            if (axisDirection == 0) {
                continue;
            }

            mapping = std::make_shared<SDLRumbleMapping>(deviceIndex, portIndex, DEFAULT_LOW_FREQUENCY_RUMBLE_PERCENTAGE,
                                                         DEFAULT_HIGH_FREQUENCY_RUMBLE_PERCENTAGE);
            break;
        }
    }

    for (auto [i, controller] : sdlControllersWithRumble) {
        SDL_GameControllerClose(controller);
    }

    return mapping;
}
} // namespace Ship
