#ifndef MATERIA_H
#define MATERIA_H

#include <SisterRay/types.h>
#include <SisterRay/data_addresses.h>
#include "../sr_named_registry.h"

#pragma pack(push, 1)
typedef struct {
    u16     apLevel[4];
    u8      equipEffect;
    u32     statusEffect;
    u8      type;
    u8      data[6];
} MateriaData;
#pragma pack(pop)

class SrMateriaRegistry : public SrNamedResourceRegistry<MateriaData, std::string> {
public:
    SrMateriaRegistry(SrKernelStream* stream) : SrNamedResourceRegistry<MateriaData, std::string>(stream) {}
    SrMateriaRegistry() : SrNamedResourceRegistry<MateriaData, std::string>() {}
};

#endif
