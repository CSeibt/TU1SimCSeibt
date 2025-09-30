#ifndef RockSample_hh
#define RockSample_hh

#include "GeometryObject.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4GenericMessenger.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4RotationMatrix.hh"
#include "CADmesh.hh"
#include "G4ThreeVector.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Transform3D.hh"
class RockSample : public GeometryObject
{
public:
    RockSample();
    virtual ~RockSample();

    void Construct(G4LogicalVolume *lWorldVolume, G4bool checkOverlaps);
    void DefineMaterials();
    void SetSamplePosition(G4ThreeVector newPos);
    void SomeFunction();
    virtual void SetNewValue(G4UIcommand* command, G4String newValue);

private:
    G4String fSTLfileName;
    G4double fSampleDistance;
    G4VPhysicalVolume* fRockSamplePhys;
    G4GenericMessenger* fMessenger; // Messenger für benutzerdefinierte Befehle
   
};

#endif
