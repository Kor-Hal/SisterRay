#include "formations.h"
#include "../impl.h"


SISTERRAY_API FormationData srGetFormation(u16 modFormationID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modFormationID);
    return gContext.formations.getElement(name);
}

SISTERRAY_API void srAddFormation(FormationData formationData, u16 modFormationID, const char* modName) {
    auto name = std::string(modName) + std::to_string(modFormationID);
    return gContext.formations.addElement(name, formationData);
}

// For internal use, we reference by index
FormationData& getFormation(u16 modFormationID) {
    return gContext.formations.getResource(modFormationID);
}

FormationActorData& getFormationActorData(u16 modFormationID, u16 enemyActorIdx) {
    return gContext.formations.getResource(modFormationID).formationActorDataArray.formationDatas[enemyActorIdx];
}
