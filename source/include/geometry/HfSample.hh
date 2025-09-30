#ifndef HfSample_hh
#define HfSample_hh

#include "GeometryObject.hh"

class HfSample : public GeometryObject
{
public:
    HfSample();

    virtual ~HfSample() {}

    void Construct(G4LogicalVolume *lWorldVolume, G4bool checkOverlaps);
    void DefineMaterials();
};

#endif
