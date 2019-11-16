#ifndef BATTLE_MENU_WIDGET_NAMES
#define BATTLE_MENU_WIDGET_NAMES

#define BATTLE_INACTIVE 0xFFFFFFFF
#define BATTLE_TARGETING_STATE 0
#define BATTLE_CMD_STATE 1
#define BATTLE_SUMMON_STATE  7
#define BATTLE_MAGIC_STATE 6
#define BATTLE_ITEM_STATE 5
#define BATTLE_MANIP_STATE 19

namespace BattleMenuWidgetNames {
    const std::string BATTLE_MENU_NAME = std::string("BATTLE_MENU");
    const std::string BATTLE_MENU_WIDGET_NAME = std::string("BATTLE_MENU_WIDGET");

    //Base Menu Names
    const std::string BATTLE_BASE_WIDGET_NAME = std::string("BATTLE_BASE");
    const std::string BASE_BOX_NAME = std::string("BASE_BOX");
    const std::string TIME_ASSET_NAME = std::string("TIME_ASSET");
    const std::string WAIT_ASSET_NAME = std::string("WAIT_ASSET");
    const std::string CHAR_ASSET_NAME = std::string("CHAR_ASSET");
    const std::string HP_ASSET_NAME = std::string("HP_ASSET");
    const std::string MP_ASSET_NAME = std::string("MP_ASSET");
    const std::string LIMIT_ASSET_NAME = std::string("LIMIT_ASSET");
    const std::string BARRIER_ASSET_NAME = std::string("BARRIER_ASSET");
    const std::string PARTY_1_STATE_NAME = std::string("1_PARTY");
    const std::string PARTY_2_STATE_NAME = std::string("2_PARTY");
    const std::string PARTY_3_STATE_NAME = std::string("3_PARTY");

    //BASE PARTY DATA NAMES
    const std::string PARTY_DATA_NAME = std::string("NAME");
    const std::string PARTY_DATA_ATB = std::string("ATB");
    const std::string PARTY_DATA_LIMIT = std::string("LIMIT");
    const std::string PARTY_DATA_BARRIERS = std::string("BARRIERS");
    const std::string PARTY_DATA_HP = std::string("HP");
    const std::string PARTY_DATA_MP = std::string("MP");


    //Command Names
    const std::string BATTLE_COMMAND_WIDGET_NAME = std::string("BATTLE_COMMAND");
    const std::string PARTY_1_CMD_GRID = std::string("1_CMD_GRID");
    const std::string PARTY_2_CMD_GRID = std::string("2_CMD_GRID");
    const std::string PARTY_3_CMD_GRID = std::string("3_CMD_GRID");
    const std::string CMD_GRID_BOX = std::string("CMD_BOX");

    //Magic Names
    const std::string BATTLE_SPELL_WIDGET_NAME = std::string("BATTLE_SPELL");
    const std::string PARTY_1_SPELL_GRID = std::string("1_SPELL_GRID");
    const std::string PARTY_2_SPELL_GRID = std::string("2_SPELL_GRID");
    const std::string PARTY_3_SPELL_GRID = std::string("3_SPELL_GRID");

    //Summon Names
    const std::string BATTLE_SUMMON_WIDGET_NAME = std::string("BATTLE_SUMMON");
    const std::string PARTY_1_SUMMON_GRID = std::string("1_SUMM_GRID");
    const std::string PARTY_2_SUMMON_GRID = std::string("2_SUMM_GRID");
    const std::string PARTY_3_SUMMON_GRID = std::string("3_SUMM_GRID");

    //Item Names
    const std::string BATTLE_ITEM_WIDGET_NAME = "BATTLE_ITEM";
    const std::string PARTY_1_ITEM_GRID = "1_ITEM_GRID";
    const std::string PARTY_2_ITEM_GRID = "2_ITEM_GRID";
    const std::string PARTY_3_ITEM_GRID = "3_ITEM_GRID";


}
#endif
