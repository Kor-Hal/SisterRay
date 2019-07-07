#include "command_sr_callbacks.h"
#include "../impl.h"

void srSetupAction(CommandSetupEvent setupEvent) {
    i32 sceneAbilityIndex;
    i32 enemyActionIndex;
    u16 elementMask;
    u8 characterID;
    i32 deathSentenceFlag;
    u16 cameraOverrideData;
    AttackData abilityData;
    BattleQueueEntry executingAction = (*(BattleQueueEntry*)0x9A9884);
    AttackData* currentSceneAbilities = (AttackData*)(0x9A90C4);
    char* attackNamesPtr = (char*)(0x9A9484);
    u16* currentSceneAbilityIDs = (u16*)(0x9A9444);
    AttackData* kernelAbilities = (AttackData*)(0xDB9690);
    EnemyData* enemyData = (EnemyData*)(0x9A8E9C);
    auto damageContext = setupEvent.damageContext;

    cameraOverrideData = 0xFFFF;
    deathSentenceFlag = 0;
    damageContext->MPCost = -1;
    auto abilityKey = assemblekey(damageContext->commandIndexCopy, damageContext->relAttackIndex);
    switch (damageContext->commandIndexCopy) {
        case 20: {
            if (damageContext->attackerID < 3)
                abilityData = initializeLimitContext(damageContext, abilityKey);
            break;
        }
        case 32: {
            auto enemyAttack = gContext.attacks.get_element(abilityKey);
            abilityData = enemyAttack.attackData;
            currentSceneAbilities[0] = enemyAttack.attackData;
            memcpy((void*)attackNamesPtr, (void*)(enemyAttack.attackName.str()), enemyAttack.attackName.size());
            *(attackNamesPtr + enemyAttack.attackName.size()) = char(255);
            *currentSceneAbilityIDs = enemyAttack.attackID;
            damageContext->sceneAbilityIndex = 0;
            break;
        }
        default: {
            abilityData = gContext.attacks.get_element(abilityKey).attackData;
            break;
        }
    }
    if (damageContext->commandIndex == CMD_MAGIC && damageContext->relAttackIndex == 54) //death sentence case hardcoded
        deathSentenceFlag = 1;

    if (!((damageContext->miscActionFlags) & 0x400000)) {
        EnabledSpell* spellData = getSpellSlot(damageContext->attackerID, damageContext->commandIndexCopy, damageContext->relAttackIndex);
        if (spellData)
            updatePlayerSpellData(damageContext, spellData, abilityData);
    }

    if (deathSentenceFlag) {
        damageContext->animationScriptID = 52;
    }
    else if (damageContext->attackerID >= 4) {
        auto actingEnemyRecord = enemyData[damageContext->enemySceneIndex]; //Finds the "formation data" for the current actor
        for (enemyActionIndex = 0; enemyActionIndex < 16; ++enemyActionIndex) {
            if (damageContext->absAttackIndex == actingEnemyRecord.attackSceneIDs[enemyActionIndex]) { //offset into the actual actor IDs
                damageContext->animationScriptID = actingEnemyRecord.attackAnimScripts[enemyActionIndex]; //offset into the animation script indexes for this model
                cameraOverrideData = actingEnemyRecord.attackCameraIDs[enemyActionIndex];
                break;
            }
        }
    }

    if (damageContext->MPCost = -1)
        damageContext->MPCost = abilityData.MPCost;

    if (executingAction.entryPriority == 3 || damageContext->miscActionFlags & 0x400000) //This is triggered by mime
        damageContext->MPCost = 0;

    damageContext->abilityHitRate = abilityData.abilityHitRate;
    damageContext->damageFormulaID = abilityData.damageFormula;

    if (abilityData.elementMask == 0xFFFF)
        elementMask = 0;
    else
        elementMask = abilityData.elementMask;

    damageContext->attackElementsMask = elementMask;
    damageContext->abilityPower = abilityData.attackPower;
    damageContext->targetReactionAnimation = abilityData.targetReactionID;

    damageContext->abilityTargetingFlags = abilityData.targetingFlags;

    if (damageContext->commandIndexCopy == CMD_LIMIT)
        cameraOverrideData = (((*(u16*)0x9AB0CC) & 8) != 0) ? abilityData.cameraMovementMultiple : abilityData.cameraMovementSingle;

    if (cameraOverrideData == 0xFFFF) {
        damageContext->cameraDataSingle = abilityData.cameraMovementSingle;
        damageContext->cameraDataMultiple = abilityData.cameraMovementMultiple;
    }
    else {
        damageContext->cameraDataSingle = cameraOverrideData;
        damageContext->cameraDataMultiple = cameraOverrideData;
    }

    if (abilityData.impactEffectID != 255)
        damageContext->impactEffectID = abilityData.impactEffectID;
    damageContext->animationEffectID = abilityData.animationEffectID;
    damageContext->specialAbilityFlags = abilityData.specialAttackFlags;
    damageContext->impactSound = abilityData.impactSoundID;
    damageContext->critAtkSound = abilityData.impactSoundID;
    damageContext->missAtkSound = abilityData.impactSoundID;

    if (!((damageContext->specialAbilityFlags) & 4) && gAiActorVariables[damageContext->attackerID].statusMask & 0x4000000)
        damageContext->abilityHitRate >>= 1; //(half hit chance)
    setStatusInflictionData(damageContext, abilityData.statusInflictType, abilityData.statusMask);

    if (deathSentenceFlag)
        gAiActorVariables[damageContext->attackerID].statusMask &= 0xFFDFFFFF;
    copyAdditionalEffects(abilityData.additionalEffect, abilityData.additionalEffectModifier);
}

AttackData initializeLimitContext(DamageCalcStruct* damageContext, const std::string& abilityKey) {
    u8 characterID;
    u8* activeLimitIDs;
    u8* kernelLimitScriptIndexes = (u8*)(0x7B76A0);
    u8* unkPartyStructPtr = (u8*)(0x9A87F4);
    AttackData abilityData;

    characterID = unkPartyStructPtr[16 * damageContext->attackerID];
    activeLimitIDs = (u8*)PARTY_STRUCT_ARRAY[damageContext->attackerID].enabledLimitBytes;
    if (damageContext->absAttackIndex >= 96 && damageContext->absAttackIndex <= 128) {
        abilityData = gContext.attacks.get_element(abilityKey).attackData;
        auto scriptIndex = damageContext->absAttackIndex - 96;
        if (kernelLimitScriptIndexes[scriptIndex] != 255)
            damageContext->animationScriptID = kernelLimitScriptIndexes[scriptIndex];
        damageContext->enabledMagicsIndex = -1;
    }
    else {
        for (i32 limitIndex = 0; limitIndex < 3; ++limitIndex) {
            auto relativeLimitIndex = (i8)activeLimitIDs[limitIndex];
            if (relativeLimitIndex + 128 == damageContext->absAttackIndex) {
                auto limitAnimationScriptIndex = 0;
                for (auto charLimitArrayIndex = 0; charLimitArrayIndex < 12; ++charLimitArrayIndex) {
                    auto fetchedRelativeIndex = getLimitRelativeIndex(characterID, charLimitArrayIndex);
                    if (fetchedRelativeIndex != 127) {
                        if (fetchedRelativeIndex == relativeLimitIndex)
                            break;
                        ++limitAnimationScriptIndex;
                    }
                }
                damageContext->animationScriptID = limitAnimationScriptIndex + 60;
                abilityData = (PARTY_STRUCT_ARRAY[damageContext->attackerID].enabledLimitData[limitIndex]);
                break;
            }
        }
    }
    return abilityData;
}

void updatePlayerSpellData(DamageCalcStruct* damageContext, EnabledSpell* spellData, const AttackData& abilityData) {
    BattleQueueEntry executingAction = (*(BattleQueueEntry*)0x9A9884);

    damageContext->MPCost = spellData->mpCost;
    srLogWrite("Setting MP Cost for player spell to: %i", damageContext->MPCost);
    if (spellData->quadCount && spellData->quadEnabled) {
        spellData->quadCount = spellData->quadCount - 1;
        damageContext->quadEnabled = spellData->quadEnabled;// quadEnabled?
        damageContext->quadCount = damageContext->quadEnabled + 3;  // numberOfCasts?
        if (damageContext->quadCount > 8) {
            damageContext->quadCount = 8;
        }
        srCreateEvent(2, damageContext->attackerID, 21, 6);
    }
    else if (damageContext->commandIndexCopy == CMD_SUMMON) {
        if (spellData->allCount) { //if yo ucan still use the summon
            if (spellData->allCount != 255) {
                spellData->allCount = spellData->allCount - 1;
                srCreateEvent(2, damageContext->attackerID, 21, 4);
            }
        }
        else {
            damageContext->displayString = 121; //Probably displays "Summon Power is all used up"
        }
    }
    else if (damageContext->miscActionFlags & 0x200) { //Handle the all case
        if (spellData->allCount) {
            if (spellData->allCount != 255) {
                spellData->allCount = spellData->allCount - 1;
                srCreateEvent(2, damageContext->attackerID, 21, 2);
            }
        }
        else if (abilityData.targetingFlags & 8) {
            damageContext->miscActionFlags |= 0x100000u;
        }
    }
    if ((executingAction.entryPriority >= 5) && !(damageContext->miscActionFlags & 0x400000)) //priority 5 and 6 actions? what are those
        damageContext->supportMatFlags = spellData->supportEffectsMask;
}

void setStatusInflictionData(DamageCalcStruct* damageContext, i32 statusInflictionByte, i32 inflictedStatusMask) {
    u16* word_9AAD1E = (u16*)(0x9AAD1E);
    damageContext->addStatusMask = 0;
    damageContext->rmStatusMask = 0;
    damageContext->toggleStatusMask = 0;
    auto statusType = statusInflictionByte >> 6;
    if (statusType < 3) {
        if (inflictedStatusMask >= 0) {
            damageContext->inflictStatusChance = 4 * (statusInflictionByte & 0x3F);// statusInflictionChance
            switch (statusType) {
                case 0:
                    damageContext->addStatusMask = inflictedStatusMask;
                    break;
                case 1:
                    damageContext->rmStatusMask = inflictedStatusMask;
                    break;
                case 2:
                    damageContext->toggleStatusMask = inflictedStatusMask;
                    break;
                default: {
                    break;
                }
            }
        }
        else {
            damageContext->addStatusMask = 2147483648; //Figure out why this gets set
            *word_9AAD1E = (i8)inflictedStatusMask & 3;
        }
    }
}