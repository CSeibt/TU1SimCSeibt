#ifndef PENSAMPLE_HH
#define PENSAMPLE_HH

#include "GeometryObject.hh"

class PENSample : public GeometryObject
{
public:
    PENSample();
    virtual ~PENSample() {};

    void Construct(G4LogicalVolume* lWorldVolume, G4bool checkOverlaps);
    void DefineMaterials();
};

#endif // PENSAMPLE_HH