#include "equip_menu.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../menu.h"
#include <memory>

using namespace EquipWidgetNames;

SISTERRAY_API void equipMenuUpdateHandler(i32 updateStateMask) {
    Menu* menuObject = gContext.menuWidgets.get_element("EQUIP_MENU");
    auto equipMenuState = menuObject->currentState;
    characterRecord* characterRecordArray = CHARACTER_RECORD_ARRAY;

    sub_6C98A6();
    auto menuWidget = menuObject->menuWidget;
    EquipDrawEvent event = { menuObject, menuObject->currentState };
    gContext.eventBus.dispatch(DRAW_EQUIP_MENU, &event);
    drawWidget(menuWidget);
    displayMenuCursors(menuObject, equipMenuState, updateStateMask);

    if (!is_input_handling_enabled()) {
        handleEquipMenuInput(updateStateMask, menuObject);
    }
}

/*Need a self-updating bar*/
void displayMenuObjects(CursorContext* cursorContextArray, u32 menuState, i32 stateControlMask) {
    u16 equippableGearCount;

    switch (menuState) {
        case 1: {
            //sub_6FA12F(316, 171, 324, 303); //I can't figure out what this does...
            equippableGearCount = gContext.gearViewData.slots_in_use;
            drawScrollerParams* arguments = (drawScrollerParams*)word_DCA490;
            setSideScrollerArguments(arguments, 8, equippableGearCount, cursorContextArray[1].baseRowIndex, 618, 171, 17, 303);
            renderMenuSideScroller(arguments, 0.2);
            //sub_6FA347();
            break;
        }
    }
}

/*Create a Focus object making this configurable*/
void displayMenuCursors(Menu* menu, u16 menuState, u32 stateControlMask) {
    auto gearTypeChoiceCursor = getStateCursor(menu, 0);
    switch (menuState) {
        case 0: {
            drawCursor(gearTypeChoiceCursor, 0.1f);
            break;
        }
        case 1: {
            auto gearChoiceCursor = getStateCursor(menu, 1);
            drawFlashingCursor(gearTypeChoiceCursor, stateControlMask, 0.1f);
            drawCursor(gearChoiceCursor, 0.1f);
            break;
        }
        default: {
            break;
        }
    }
}

void handleEquipMenuInput(i32 updateStateMask, Menu* menuObject) {
    characterRecord* characterRecordArray = CHARACTER_RECORD_ARRAY;
    auto equipMenuState = menuObject->currentState;
    auto cursorArray = getStateCursor(menuObject, equipMenuState);
    auto menuWidget = menuObject->menuWidget;

    EquipInputEvent event = { menuObject, equipMenuState };
    handleCursorPositionUpdate((u32*)(&(cursorArray->context)));
    auto dispatchContext = std::vector<SrEventContext>({ EQUIP_MENU_CONTEXT });
    if (checkInputReceived(32)) {
        gContext.eventBus.dispatch(MENU_INPUT_OK, &event, dispatchContext);
    }
    else if (checkInputReceived(64)) {
        gContext.eventBus.dispatch(MENU_INPUT_CANCEL, &event, dispatchContext);
    }
    else if (checkInputReceived(4)) {
        gContext.eventBus.dispatch(MENU_INPUT_L1, &event, dispatchContext);
    }
    else if (checkInputReceived(8)) {
        gContext.eventBus.dispatch(MENU_INPUT_R1, &event, dispatchContext);
    }
    else if (checkInputReceived(128)) { //When switching to the materia view, square
        gContext.eventBus.dispatch(MENU_INPUT_SQUARE, &event, dispatchContext);
    }
    else if (checkInputReceived(16)) { //unequip accessory
        gContext.eventBus.dispatch(MENU_INPUT_TRIANGLE, &event, dispatchContext);
    }
}
