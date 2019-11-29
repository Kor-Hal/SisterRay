#ifndef ANIMATION_SCRIPT_OPCODE_H
#define ANIMATION_SCRIPT_OPCODE_H

#include <SisterRay/SisterRay.h>
#include "../../events/battle_events.h"
#include "../../sr_named_registry.h"

enum OpCodeControlSequence { RUN_NEXT, PLAY_ANIM, BREAK };
typedef OpCodeControlSequence(*SRPFNANMSCRIPTOPCODEHANDLER)(AnimScriptEvent*);

u8 readOpCodeArg8(u8* scriptPtr, GameAnimationScriptContext* ctx, BattleModelState* battleModelState);
u16 readOpCodeArg16(u8* scriptPtr, GameAnimationScriptContext* ctx, BattleModelState* battleModelState);
u32 readOpCodeArg32(u8* scriptPtr, GameAnimationScriptContext* ctx, BattleModelState* battleModelState);
std::string assembleOpCodeKey(u16 opCode);

class SrAnimOpCodeRegistry : public SrNamedResourceRegistry<SRPFNANMSCRIPTOPCODEHANDLER, std::string> {
public:
    SrAnimOpCodeRegistry() : SrNamedResourceRegistry<SRPFNANMSCRIPTOPCODEHANDLER, std::string>() {}
};

#endif // !ANIMATION_OPCODE_H
