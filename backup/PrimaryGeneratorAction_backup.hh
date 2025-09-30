/// \file PrimaryGeneratorAction.hh
/// \brief Definition der Klasse PrimaryGeneratorAction

#ifndef PrimaryGeneratorAction_hh
#define PrimaryGeneratorAction_hh

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4UImessenger.hh"
#include "geometry/RockSample.hh" // Include RockSample header

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "CLHEP/Units/SystemOfUnits.h"
#include "globals.hh"

#include "G4TessellatedSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"  // Für G4VPhysicalVolume
#include "G4Transform3D.hh"      // Für G4Transform3D
#include "G4PhysicalVolumeStore.hh"
#include "G4Transform3D.hh"

#include <memory>
using std::shared_ptr;
using std::make_shared;

class G4ParticleGun;
class G4Event;

/// The primary generator action class with particle gun.
class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction,
    public G4UImessenger
{
public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event* event) override;
    void SetNewValue(G4UIcommand* command, G4String newValue) override;

    void SetRockSample(shared_ptr<RockSample> rockSample); // Methode zum Setzen des RockSample

private:
    shared_ptr<G4ParticleGun> m_particleGun;
    shared_ptr<RockSample> m_rockSample;

    enum { mPointDirectional, mPointIsotropic, mVolumeCylinder, mVolumeSphere, mVolumeScan } m_posMode;
    shared_ptr<G4UIcmdWithAString> m_cmdPosMode;

    G4double m_height = 21.75 * CLHEP::mm;
    G4double m_diameter = 6.93 / 2 * CLHEP::cm;
    G4double m_offset = 0. * CLHEP::cm;
    shared_ptr<G4UIcmdWithADoubleAndUnit> m_cmdSetVolumeHeight, m_cmdSetVolumeDiameter, m_cmdSetVolumeOffset;

    bool GenerateRandomPointInTessellatedSolid(G4TessellatedSolid* tessellatedSolid, G4ThreeVector& randomPosition, G4LogicalVolume* logicalVolume);

    //G4AffineTransform GetTransformFromPhysicalVolume(G4VPhysicalVolume* physicalVolume);

private:
    G4VPhysicalVolume* fRockSamplePhys;
};

#endif // PrimaryGeneratorAction_hh
