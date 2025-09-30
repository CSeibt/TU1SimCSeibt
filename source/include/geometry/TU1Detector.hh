#ifndef TU1Detector_hh
#define TU1Detector_hh

#include "GeometryObject.hh"

class TU1Detector : public GeometryObject
{
public:
    TU1Detector();

    virtual ~TU1Detector() {}

    void Construct(G4LogicalVolume *lWorldVolume, G4bool checkOverlaps);
    void DefineMaterials();

private:
};

#endif
