#include "party_utils.h"
#include "../inventories/inventory_utils.h"
#include "../impl.h"

std::string getCharacterName (u8 characterID) {
    switch (characterID) {
        case 0:
            return std::string("CLOUD");
        case 1:
            return std::string("BARRET");
        case 2:
            return std::string("TIFA");
        case 3:
            return std::string("AERIS");
        case 4:
            return std::string("REDXIII");
        case 5:
            return std::string("YUFFIE");
        case 6:
            return std::string("CAITSITH");
        case 7:
            return std::string("VINCENT");
        case 8:
            return std::string("CID");
        case 9:
            return std::string("YCLOUD");
        case 10:
            return std::string("SEPHIROTH");
        default: {
            return std::string("NULL");
        }
    }
}

PartyMember* getGamePartyMember(u8 actorIdx) {
    if (G_SAVE_MAP->activeParty[actorIdx] == 0xFF) {
        return nullptr;
    }
    return &(PARTY_STRUCT_ARRAY[actorIdx]);
}

CharacterRecord* getPartyActorCharacterRecord(u8 partyIdx) {
    CharacterRecord* characterRecordArray = CHARACTER_RECORD_ARRAY;
    auto characterIdx = getCharacterRecordIndex(partyIdx);
    return gContext.characters.getResource(characterIdx).gameCharacter;
}

u16 getEquippedGear(u8 characterID, u8 gearType) {
    auto& characterRecord = gContext.characters.getResource(characterID);
    u16 kernelObjectID;

    switch (gearType) { //probably refactor this into a utility
    case 1: {
        kernelObjectID = characterRecord.equippedWeapon;
        break;
    }
    case 2: {
        kernelObjectID = characterRecord.equippedArmor;
        break;
    }
    case 3: {
        kernelObjectID = characterRecord.equippedAccessory;
        break;
    }
    default: {
        return 0;
    }
    }
    return kernelObjectID;
}

bool characterCanEquipItem(u8 characterID, u16 itemID){
    auto characterMask = getCharacterRestrictionMask(itemID);
    bool characterCanUse = (bool)(characterMask & (1 << characterID));

    return characterCanUse;
}

u8 getCharacterRecordIndex(u8 partyIndex) {
    return (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[partyIndex])];
}

u16 getMateriaID(u8 characterID, u8 slot, u8 gearType) {
    u16 materiaID;
    auto name = getCharacterName(characterID);
    switch (gearType) {
        case 0:
            materiaID = gContext.characters.getElement(name).wpnMaterias[slot].item_id;
            break;
        case 1:
            materiaID = gContext.characters.getElement(name).wpnMaterias[slot].item_id;
            break;
        default: {
            break;
        }
    }
    return materiaID;
}

bool isPartyActor(u8 actorIdx) {
    return (actorIdx < 3);
}

bool actorHasStatus(u8 actorID, u32 maskOfStatus) {
    auto& actorAIStates = AI_BATTLE_CONTEXT->actorAIStates;
    return actorAIStates[actorID].statusMask & maskOfStatus;
}

