#include "SDLMapping.h"
#include <spdlog/spdlog.h>
#include "Context.h"

#include "utils/StringHelper.h"

namespace Ship {
SDLMapping::SDLMapping() : ControllerMapping(ShipDeviceType::SDLGamepad) {
}

SDLMapping::~SDLMapping() {
}
} // namespace Ship
