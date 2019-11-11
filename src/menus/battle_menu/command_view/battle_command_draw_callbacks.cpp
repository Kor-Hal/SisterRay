#include "battle_summon_draw_callbacks.h"

using namespace BattleMenuWidgetNames;

void drawBattleCommandViewWidget(const BattleSpellDrawEvent* event) {
    auto menuWidget = event->menu->menuWidget;

    if (event->menuState != BATTLE_CMD_STATE) {
        disableWidget(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME));
        return;
    }

    enableWidget(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME));
    std::vector<std::string> names = { PARTY_1_CMD_GRID, PARTY_2_CMD_GRID, PARTY_3_CMD_GRID };
    for (i32 partyIdx = 0; partyIdx < names.size(); partyIdx++) {
        if (partyIdx == *BATTLE_ACTIVE_ACTOR_ID) {
            enableWidget(getChild(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME), names[partyIdx]));
            setActiveCursorIndex(event->menu, BATTLE_CMD_STATE, BATTLE_ACTIVE_ACTOR_ID)
            continue;
        }
        disableWidget(getChild(getChild(menuWidget, BATTLE_COMMAND_WIDGET_NAME), names[partyIdx]));
    }

    // Resize the box based onn the number of commands
    auto commandChoiceCursor = getStateCursor(menuObject, BATTLE_CMD_STATE, *BATTLE_ACTIVE_ACTOR_ID);
    commandChoiceCursor->context.maxColumnBound = PARTY_STRUCT_ARRAY[*BATTLE_ACTIVE_ACTOR_ID].commandColumns;
    commandChoiceCursor->context.viewColumnBound = PARTY_STRUCT_ARRAY[*BATTLE_ACTIVE_ACTOR_ID].commandColumns;
    resizeBox(getChild(menuWidget, CMD_GRID_BOX), 135, 350, 88 * commandChoiceCursor->context.maxColumnBound, 130);
}
