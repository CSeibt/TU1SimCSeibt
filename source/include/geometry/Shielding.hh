#ifndef Shielding_hh
#define Shielding_hh

#include "GeometryObject.hh"

class Shielding : public GeometryObject
{
public:
    Shielding();

    virtual ~Shielding() {}

    void Construct(G4LogicalVolume *lWorldVolume, G4bool checkOverlaps);
    void DefineMaterials();
};

#endif
