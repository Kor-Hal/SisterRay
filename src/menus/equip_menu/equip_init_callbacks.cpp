#include "equip_init_callbacks.h"
#include "../../impl.h"
#include "../../party/party_utils.h"
#include "../../widgets/updaters.h"
#include "../../party/stat_names.h"

/* This Module contains code which initializes the Widget that represents the equip menu internally
   You can mutate this widget prior to draw by registered your listeners to the "OnEquipMenuDraw" event
   The Widget is also mutated by listeners registered to the Equip-Menu's input Handlers.
*/

using namespace EquipWidgetNames;

void initCharDataWidget(const MenuInitEvent* event) {
    const char * menuText;
    auto characterID = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    TextWidget* textWidget;
    DrawTextParams textParams;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    PortraitWidget* portraitWidget;

    auto currentEquipWidget = createWidget(CHAR_DATA_WIDGET_NAME);

    boxParams = {
       equipMenuWindowConfig[0].drawDistance1,
       equipMenuWindowConfig[0].drawDistance2,
       equipMenuWindowConfig[0].drawDistance3,
       equipMenuWindowConfig[0].drawDistance4,
       0.3f
    };
    boxWidget = createBoxWidget(boxParams, CHAR_DATA_BOX_NAME);
    addChildWidget(currentEquipWidget, (Widget*)boxWidget, CHAR_DATA_BOX_NAME);

    drawPortraitParams portraitParams = { 17, 9, 0, 0.2f };
    portraitWidget = createPortraitWidget(portraitParams, PORTRAIT_WIDGET_NAME);
    addChildWidget(currentEquipWidget, (Widget*)portraitWidget, PORTRAIT_WIDGET_NAME);

    drawHPBarParams hpBarParams = { 110, 17, 0, 0.2f };
    auto HPBarWidget = createHPBarWidget(hpBarParams, HPBAR_WIDGET_NAME);
    addChildWidget(currentEquipWidget, (Widget*)HPBarWidget, HPBAR_WIDGET_NAME);

    std::vector<std::string> gearNames = { GEAR_SLOT_1_NAME, GEAR_SLOT_2_NAME, GEAR_SLOT_3_NAME };
    std::vector<std::string> equippedGearNames = { EQUIPPED_WEAPON, EQUIPPED_ARMOR, EQUIPPED_ACC };
    for (int row = 0; row < gearNames.size(); row++) {
        menuText = gContext.gameStrings.equipMenuTexts.get_string(row);
        setTextParams(&textParams, 250, 13 + (34 * row), menuText, COLOR_TEAL, 0.2f);
        textWidget = createTextWidget(textParams, gearNames[row]);
        addChildWidget(currentEquipWidget, (Widget*)textWidget, gearNames[row]);

        setTextParams(&textParams, 303, 13 + (34 * row), menuText, COLOR_WHITE, 0.2f);
        textWidget = createTextWidget(textParams, equippedGearNames[row]);
        addChildWidget(currentEquipWidget, (Widget*)textWidget, equippedGearNames[row]);
    }

    addChildWidget(mainWidget, currentEquipWidget, CHAR_DATA_WIDGET_NAME);
}

void initGearDescWidget(const MenuInitEvent* event) {
    const char* fetchedName;
    u16 kernelObjectID;
    auto characterID = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];

    TextWidget* textWidget;
    DrawTextParams textParams;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto GearDescWidget = createWidget(GEAR_DESC_WIDGET_NAME);

    boxParams = {
        equipMenuWindowConfig[1].drawDistance1,
        equipMenuWindowConfig[1].drawDistance2,
        equipMenuWindowConfig[1].drawDistance3,
        equipMenuWindowConfig[1].drawDistance4,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, GEAR_DESC_BOX);
    addChildWidget(mainWidget, (Widget*)boxWidget, GEAR_DESC_BOX);

    kernelObjectID = getEquippedGear(characterID, 1);
    fetchedName = getNameFromRelativeID(kernelObjectID, 1);
    setTextParams(&textParams, 4, equipMenuWindowConfig[1].drawDistance2 + 13, fetchedName, COLOR_WHITE, 0.2f);
    textWidget = createTextWidget(textParams, GEAR_DESCRIPTION);
    addChildWidget(GearDescWidget, (Widget*)textWidget, GEAR_DESCRIPTION);

    addChildWidget(mainWidget, GearDescWidget, GEAR_DESC_WIDGET_NAME);
}

/* Initialize the Widget for the characters Materia Slots. This will be updated when Handling in Handlers*/
void initGearMateriaSlotWidget(const MenuInitEvent* event) {
    const char * menuText;
    u8 materiaGrowth;
    auto characterID = (RECYCLE_SLOT_OFFSET_TABLE)[(((u8*)CURRENT_PARTY_MEMBER_ARRAY)[*EQUIP_MENU_PARTY_INDEX])];

    TextWidget* textWidget;
    DrawTextParams textParams;
    SlotsWidget* slotsWidget;
    drawSlotsParams slotsParams;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto equipMateraSlotWidget = createWidget(GEAR_SLOTS_WIDGET_NAME);

    boxParams = {
        equipMenuWindowConfig[2].drawDistance1,
        equipMenuWindowConfig[2].drawDistance2,
        equipMenuWindowConfig[2].drawDistance3,
        equipMenuWindowConfig[2].drawDistance4,
        0.3f
    };
    boxWidget = createBoxWidget(boxParams, GEAR_SLOTS_BOX);
    addChildWidget(equipMateraSlotWidget, (Widget*)boxWidget, GEAR_SLOTS_BOX);

    std::vector<std::string> equipSlotDataNames = { SLOTS_NAME, GROWTH_NAME };
    for (int i = 0; i < 2; i++) {
        menuText = gContext.gameStrings.equipMenuTexts.get_string(14 + i);
        setTextParams(&textParams, 27, 42 * i + equipMenuWindowConfig[2].drawDistance2 + 21, menuText, COLOR_TEAL, 0.1f);
        textWidget = createTextWidget(textParams, equipSlotDataNames[i]);
        addChildWidget(equipMateraSlotWidget, (Widget*)textWidget, equipSlotDataNames[i]);
    }

    //create a default static variable later to a valid bytearray representing materia slot data
    slotsParams = { 153, equipMenuWindowConfig[2].drawDistance2 + 21, nullptr };
    slotsWidget = createSlotsWidget(slotsParams, GEAR_SLOTS);
    addChildWidget(equipMateraSlotWidget, (Widget*)slotsWidget, GEAR_SLOTS);

    materiaGrowth = 1;
    if (materiaGrowth < 0 || materiaGrowth > 3) //display any invalid materia growth as "None"
        materiaGrowth = 4;

    menuText = gContext.gameStrings.equipMenuTexts.get_string(materiaGrowth + 4);
    i32 growthTypeY = equipMenuWindowConfig[2].drawDistance2 + 64;
    i32 growthTypeX = 35;
    setTextParams(&textParams, 243 - growthTypeX / 2, growthTypeY, menuText, COLOR_WHITE, 0.2f);
    textWidget = createTextWidget(textParams, GEAR_GROWTH);
    addChildWidget(equipMateraSlotWidget, (Widget*)textWidget, GEAR_GROWTH);

    addChildWidget(mainWidget, equipMateraSlotWidget, GEAR_SLOTS_WIDGET_NAME);
}

/*Initialize the Widget That displays stats*/
void initStatDiffWidget(const MenuInitEvent* event) {
    u16 windowTop = equipMenuWindowConfig[3].drawDistance2 + 26;
    const char* menuText;

    TextWidget* textWidget;
    DrawTextParams textParams;
    NumberWidget* numberWidget;
    DrawNumberParams numberParams;
    SimpleAssetWidget* simpleAssetWidget;
    DrawSimpleAssetParams simpleAssetParams;
    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto statDiffWidget = createWidget(STAT_DIFF_WIDGET_NAME);

    boxParams = {
        equipMenuWindowConfig[3].drawDistance1,
        equipMenuWindowConfig[3].drawDistance2,
        equipMenuWindowConfig[3].drawDistance3,
        equipMenuWindowConfig[3].drawDistance4,
        0.5f
    };
    boxWidget = createBoxWidget(boxParams, STAT_DIFF_BOX);
    addChildWidget(statDiffWidget, (Widget*)boxWidget, STAT_DIFF_BOX);

    std::vector<std::string> statNames = { STAT_NAME_1, STAT_NAME_2, STAT_NAME_3, STAT_NAME_4, STAT_NAME_5, STAT_NAME_6, STAT_NAME_7 };
    std::vector<std::string> numberNames = { STAT_VALUE_1, STAT_VALUE_2, STAT_VALUE_3, STAT_VALUE_4, STAT_VALUE_5, STAT_VALUE_6, STAT_VALUE_7 };
    std::vector<std::string> candidateNumberNames = { NEW_STAT_VALUE_1, NEW_STAT_VALUE_2, NEW_STAT_VALUE_3, NEW_STAT_VALUE_4, NEW_STAT_VALUE_5, NEW_STAT_VALUE_6, NEW_STAT_VALUE_7 };
    std::vector<std::string> arrowNames = { ARROW_1, ARROW_2, ARROW_3, ARROW_4, ARROW_5, ARROW_6, ARROW_7 };
    std::vector<std::string> displayStatNames = {
        StatNames::WEAPON_ATTACK, StatNames::WEAPON_ACCURACY,
        StatNames::ARMOR_DEFENSE, StatNames::EVADE,
        StatNames::WEAPON_MAGIC, StatNames::ARMOR_MDEFENSE,
        StatNames::MEVADE_NAME
    };
    for (u8 displayStatIdx = 0; displayStatIdx < 7; ++displayStatIdx) {
        menuText = gContext.stats.getElement(displayStatNames[displayStatIdx]).displayName.str();
        srLogWrite("CREATING EQUIP MENU WIDGET WITH STAT NAME: %s", gContext.stats.getElement(displayStatNames[displayStatIdx]).displayName.unicode());
        setTextParams(&textParams, 53, windowTop + 26 * displayStatIdx - 6, menuText, COLOR_TEAL, 0.2f);
        textWidget = createTextWidget(textParams, statNames[displayStatIdx]);
        addChildWidget(statDiffWidget, (Widget*)textWidget, statNames[displayStatIdx]);

        simpleAssetParams = { 244, 26 * displayStatIdx + windowTop, 0xDAu, COLOR_TEAL, 0.2f };
        simpleAssetWidget = createSimpleGameAssetWidget(simpleAssetParams, arrowNames[displayStatIdx]);
        addChildWidget(statDiffWidget, (Widget*)simpleAssetWidget, arrowNames[displayStatIdx]);

        setNumberParams(&numberParams, 200, windowTop + 26 * displayStatIdx, 0, 3, COLOR_WHITE, 0.2f);
        numberWidget = createNumberWidget(numberParams, numberNames[displayStatIdx]);
        addChildWidget(statDiffWidget, (Widget*)numberWidget, numberNames[displayStatIdx]);

        setNumberParams(&numberParams, 270, windowTop + 26 * displayStatIdx, 0, 3, COLOR_WHITE, 0.2f);
        numberWidget = createNumberWidget(numberParams, candidateNumberNames[displayStatIdx]);
        addChildWidget(statDiffWidget, (Widget*)numberWidget, candidateNumberNames[displayStatIdx]);
    }
    addChildWidget(mainWidget, statDiffWidget, STAT_DIFF_WIDGET_NAME);
}

//Initialize the gear list with just a box and a series of disabled widgets.
void initGearListWidget(const MenuInitEvent* event) {
    auto gearChoiceCursor = getStateCursor(event->menu, 1);

    BoxWidget* boxWidget;
    DrawBoxParams boxParams;
    auto menuObject = event->menu;
    auto mainWidget = menuObject->menuWidget;

    auto gearListWidget = createWidget(GEAR_LIST_WIDGET_NAME);

    boxParams = {
        equipMenuWindowConfig[4].drawDistance1,
        equipMenuWindowConfig[4].drawDistance2,
        equipMenuWindowConfig[4].drawDistance3,
        equipMenuWindowConfig[4].drawDistance4,
        0.5f
    };
    boxWidget = createBoxWidget(boxParams, GEAR_LIST_BOX);
    addChildWidget(gearListWidget, (Widget*)boxWidget, GEAR_LIST_BOX);

    drawGridParams gridParams = { EQUIP_MENU_NAME.c_str(), 1, &gearViewNameUpdater, 427, 193, nullptr, 0 };
    auto cursorListWidget = createGridWidget(gridParams, EQUIP_LIST, TextWidgetKlass());
    addChildWidget(gearListWidget, (Widget*)cursorListWidget, EQUIP_LIST);

    addChildWidget(mainWidget, gearListWidget, GEAR_LIST_WIDGET_NAME);
}


void gearViewNameUpdater(CollectionWidget* self, Widget* widget, u16 flatIndex) {
    if (self->collectionType != GridWidgetClass()) {
        return;
    }

    auto typedPtr = (CursorGridWidget*)self;
    auto gearType = gContext.gearViewData.getItemType();
    auto relativeItemID = gContext.gearViewData.getResource(flatIndex).relative_item_id;
    srLogWrite("updating gear view widget with item ID: %i at relative index %i", relativeItemID, flatIndex);
    const char* name = getNameFromRelativeID(relativeItemID, gearType);
    srLogWrite("updating gear view widget with name: %s", name);
    updateText(widget, name);
}
