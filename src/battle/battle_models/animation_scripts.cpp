#include "animation_scripts.h"
#include "../../impl.h"
#include "../../system/ff7memory.h"

std::string assembleAnimScriptKey(u16 idx) {
    return std::string(BASE_PREFIX) + std::to_string(idx);
}

SrModelScripts createSrModelScripts(SrModelType modelType, const std::string archiveName, void* battleLGPBuffer) {
    LGPContext lgpContext = { 0, 1, 2, (PFNFF7MANGLER)0x5E2908 };
    LGPArchiveFile archiveFile = srOpenABFile(&lgpContext, archiveName.c_str(), (void*)battleLGPBuffer);
    u8* abFilePtr = (u8*)(archiveFile.buffer);
    ModelABHeader* header = (ModelABHeader*)abFilePtr;
    u16 animScriptLength;
    u16 animScriptCount;

    SrModelScripts srModelScripts = SrModelScripts();
    srModelScripts.header = *header;

    u8* headerEnd = abFilePtr + AB_PTR_TABLE_OFFSET;
    u32* scriptPtrTable = (u32*)(headerEnd);
    if (modelType == MODEL_TYPE_PLAYER) {
        animScriptCount = BASE_PARTY_SCRIPT_MAX;
    }
    else {
        animScriptCount = BASE_ENEMY_SCRIPT_MAX;
    }
    for (auto scriptIdx = 0; scriptIdx < animScriptCount; scriptIdx++) {
        u8* animScriptStart = ((u8*)abFilePtr) + scriptPtrTable[scriptIdx];
        if ((modelType == MODEL_TYPE_ENEMY) && scriptIdx == 31) {
            animScriptLength = 32;
        }
        else if ((modelType == MODEL_TYPE_PLAYER) && scriptIdx == 73) {
            animScriptLength = 32;
        }

        int difference = (scriptPtrTable[scriptIdx + 1] > scriptPtrTable[scriptIdx]);
        if (difference > 0) {
            animScriptLength = scriptPtrTable[scriptIdx + 1] - scriptPtrTable[scriptIdx];
        }
        else {
            animScriptLength = 32;
        }
        if (animScriptLength) {
            if (animScriptStart[0] == 0 && animScriptStart[1] == 0) {
                u16 trueAnimScriptLength = 4;
                auto animationScript = animScriptFromBuffer((((u8*)abFilePtr) + scriptPtrTable[0]), animScriptLength, &trueAnimScriptLength);
                SrAnimationScript srAnimScript = { trueAnimScriptLength, animationScript };
                srModelScripts.modelAnimScripts.addElement(assembleAnimScriptKey(scriptIdx), srAnimScript);
                continue;
            }
            u16 trueAnimScriptLength = animScriptLength;
            auto animationScript = animScriptFromBuffer(animScriptStart, animScriptLength, &trueAnimScriptLength);
            SrAnimationScript srAnimScript = { trueAnimScriptLength, animationScript };
            srModelScripts.modelAnimScripts.addElement(assembleAnimScriptKey(scriptIdx), srAnimScript);
        }
    }
    srModelScripts.scriptCount = animScriptCount;
    return srModelScripts;
}

// Initialize, enemy script related data so that we may reassemble in a way the game understands
SrBattleAnimScriptRegistry::SrBattleAnimScriptRegistry(std::unordered_set<u16> enemyModelIDs, void* battleLGPBuffer) : SrNamedResourceRegistry<SrModelScripts, std::string>() {
    LGPContext lgpContext = { 0, 1, 2, (PFNFF7MANGLER)0x5E2198 }; //This is used by the game, the name mangler is at 0x5E2198 and called when opening an lgp file
    srLogWrite("Loading animations for %i enemies", enemyModelIDs.size());
    for (auto modelID : enemyModelIDs) {
        auto name = assembleEnemyModelKey(modelID);
        srLogWrite("Loading model animation scripts from SR registry for model %s constructed from modelID: %i", name.c_str(), modelID);
        SrModelScripts srModelScripts = createSrModelScripts(MODEL_TYPE_ENEMY, name, battleLGPBuffer);
        addElement(name, srModelScripts);
    }
    for (auto name : characterModelNames) {
        srLogWrite("Loading model animation scripts from SR registry for model %s", name.c_str());
        SrModelScripts srModelScripts = createSrModelScripts(MODEL_TYPE_PLAYER, name, battleLGPBuffer);
        addElement(name, srModelScripts);
    }
    for (auto name : specialModelNames) {
        srLogWrite("Loading model animation scripts from SR registry for model %s", name.c_str());
        SrModelScripts srModelScripts = createSrModelScripts(MODEL_TYPE_PLAYER, name, battleLGPBuffer);
        addElement(name, srModelScripts);
    }
}

u32 SrBattleAnimScriptRegistry::getMemoryBufferSize(const std::string& name){
    auto scriptData = getElement(name);
    auto scriptCount = 0;
    auto scriptLength = 0;
    for (auto script : scriptData.modelAnimScripts) {
        scriptLength += script.scriptLength;
        scriptCount++;
    }
    return AB_PTR_TABLE_OFFSET + 4 * scriptCount + scriptLength;
}

AnimationScript animScriptFromBuffer(u8* animScriptStart, u16 animScriptLength, u16* trueScriptLength) {
    auto animScript = AnimationScript();
    auto scriptPosition = 0;
    while (scriptPosition < animScriptLength) {
        animScript.push_back(animScriptStart[scriptPosition]);
        if (animScriptStart[scriptPosition] == 0xEE || animScriptStart[scriptPosition] == 0xFF || ((animScriptStart[scriptPosition - 1] == 0xFE) && (animScriptStart[scriptPosition] == 0xC0))) {
            *trueScriptLength = scriptPosition + 1;
            break;
        }
        scriptPosition++;
    }
    return animScript;
}

void initAnimationScripts(void* battleLGPBuffer) {
    auto enemyModelIDs = gContext.enemies.getUniqueModelIDs();
    gContext.battleAnimationScripts = SrBattleAnimScriptRegistry(enemyModelIDs, battleLGPBuffer);
}


void* srInitializeAnimScriptsData(const char* filename, ModelAAHeader* aaHeader) {
    auto animScriptsData = gContext.battleAnimationScripts.getElement(std::string(filename));
    auto bufferSize = gContext.battleAnimationScripts.getMemoryBufferSize(std::string(filename));
    u8* animScriptBuffer = (u8*)ff7allocateMemory(1, bufferSize, nullptr, 0);
    u8** tableBufferPtr = (u8**)(animScriptBuffer + (AB_PTR_TABLE_OFFSET)); //advance this ptr to the start of the animation data table
    u8* scriptBufferPtr = (u8*)(animScriptBuffer + (AB_PTR_TABLE_OFFSET + (animScriptsData.scriptCount * 4))); //advance this ptr to the start of the raw animation script data
    /*Copy the header data from the ba file*/
    memcpy(animScriptBuffer, (void*)&(animScriptsData.header), AB_PTR_TABLE_OFFSET);

    /*Initialize the actual form of the animation scripts data in memory from the ba file data*/
    auto scriptIdx = 0;
    for (auto animScript : animScriptsData.modelAnimScripts) {
        tableBufferPtr[scriptIdx] = scriptBufferPtr; // use the relative index like in the file for the time being
        memcpy((void*)scriptBufferPtr, (void*)animScript.animScript.data(), animScript.scriptLength);
        scriptBufferPtr += animScript.scriptLength;
        scriptIdx++;
    }
    return animScriptBuffer;
}

/*Add an animation script from a provided buffer, with a given Length*/
SISTERRAY_API void addAnimationScript(const char* modName, u16 modIdx, const char* modelName, u8* script, u16 scriptLength) {
    u16 trueAnimScriptLength = 0;
    auto name = std::string(modName) + std::to_string(modIdx);
    auto animationScript = animScriptFromBuffer(script, scriptLength, &trueAnimScriptLength);
    SrAnimationScript srAnimScript = { trueAnimScriptLength, animationScript };
    auto& modelScripts = gContext.battleAnimationScripts.getElement(modelName);
    modelScripts.modelAnimScripts.addElement(name, srAnimScript);
    modelScripts.scriptCount++;
}
