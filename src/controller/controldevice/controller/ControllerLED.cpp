#include "ControllerLED.h"

#include "public/bridge/consolevariablebridge.h"
#include <Utils/StringHelper.h>
#include <sstream>
#include <algorithm>

#include "controller/controldevice/controller/mapping/factories/LEDMappingFactory.h"

namespace LUS {
ControllerLED::ControllerLED(uint8_t portIndex) : mPortIndex(portIndex) {
}

ControllerLED::~ControllerLED() {
}

void ControllerLED::SetLEDColor(Color_RGB8 color) {
    for (auto [id, mapping] : mLEDMappings) {
        mapping->SetLEDColor(color);
    }
}

void ControllerLED::SaveLEDMappingIdsToConfig() {
    // todo: this efficently (when we build out cvar array support?)
    std::string ledMappingIdListString = "";
    for (auto [id, mapping] : mLEDMappings) {
        ledMappingIdListString += id;
        ledMappingIdListString += ",";
    }

    const std::string ledMappingIdsCvarKey = StringHelper::Sprintf("gControllers.Port%d.LEDMappingIds", mPortIndex + 1);
    if (ledMappingIdsCvarKey == "") {
        CVarClear(ledMappingIdsCvarKey.c_str());
    } else {
        CVarSetString(ledMappingIdsCvarKey.c_str(), ledMappingIdListString.c_str());
    }

    CVarSave();
}

void ControllerLED::AddLEDMapping(std::shared_ptr<ControllerLEDMapping> mapping) {
    mLEDMappings[mapping->GetLEDMappingId()] = mapping;
}

void ControllerLED::ClearLEDMapping(std::string id) {
    mLEDMappings[id]->EraseFromConfig();
    mLEDMappings.erase(id);
    SaveLEDMappingIdsToConfig();
}

void ControllerLED::ClearAllMappings() {
    for (auto [id, mapping] : mLEDMappings) {
        mapping->EraseFromConfig();
    }
    mLEDMappings.clear();
    SaveLEDMappingIdsToConfig();
}

void ControllerLED::LoadLEDMappingFromConfig(std::string id) {
    auto mapping = LEDMappingFactory::CreateLEDMappingFromConfig(mPortIndex, id);

    if (mapping == nullptr) {
        return;
    }

    AddLEDMapping(mapping);
}

void ControllerLED::ReloadAllMappingsFromConfig() {
    mLEDMappings.clear();

    // todo: this efficently (when we build out cvar array support?)
    // i don't expect it to really be a problem with the small number of mappings we have
    // for each controller (especially compared to include/exclude locations in rando), and
    // the audio editor pattern doesn't work for this because that looks for ids that are either
    // hardcoded or provided by an otr file
    const std::string ledMappingIdsCvarKey = StringHelper::Sprintf("gControllers.Port%d.LEDMappingIds", mPortIndex + 1);
    std::stringstream ledMappingIdsStringStream(CVarGetString(ledMappingIdsCvarKey.c_str(), ""));
    std::string ledMappingIdString;
    while (getline(ledMappingIdsStringStream, ledMappingIdString, ',')) {
        LoadLEDMappingFromConfig(ledMappingIdString);
    }
}

std::unordered_map<std::string, std::shared_ptr<ControllerLEDMapping>> ControllerLED::GetAllLEDMappings() {
    return mLEDMappings;
}

bool ControllerLED::AddLEDMappingFromRawPress() {
    std::shared_ptr<ControllerLEDMapping> mapping = nullptr;

    mapping = LEDMappingFactory::CreateLEDMappingFromSDLInput(mPortIndex);

    if (mapping == nullptr) {
        return false;
    }

    AddLEDMapping(mapping);
    mapping->SaveToConfig();
    SaveLEDMappingIdsToConfig();
    return true;
}

bool ControllerLED::HasMappingsForLUSDeviceIndex(LUSDeviceIndex lusIndex) {
    return std::any_of(mLEDMappings.begin(), mLEDMappings.end(), 
        [lusIndex](const auto& mapping) {
            return mapping.second->GetLUSDeviceIndex() == lusIndex;
        }
    );
}
} // namespace LUS
