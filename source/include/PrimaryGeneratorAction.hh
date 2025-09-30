/// \file PrimaryGeneratorAction.hh
/// \brief Definition der Klasse PrimaryGeneratorAction

#ifndef PrimaryGeneratorAction_hh
#define PrimaryGeneratorAction_hh

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4UImessenger.hh"
#include "geometry/RockSample.hh" // Include RockSample header

#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWith3Vector.hh"

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


// Forward declarations
class TH3F;

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
    bool GenerateRandomPointInTessellatedSolid(G4TessellatedSolid*, G4ThreeVector&);
    bool ParseDecayZeroFileSingleBlock(G4int&, G4String&, std::vector<std::tuple<int, double, G4ThreeVector>>&);

private:
    shared_ptr<G4ParticleGun> m_particleGun;
    shared_ptr<RockSample>    m_rockSample;

    // Adjust size of source.
    G4double m_sourceHeight;
    G4double m_sourceDiameter;
    G4double m_offsetOfSource; // In case of space between source(container) and 'where its lying on top'
    shared_ptr<G4UIcmdWithADoubleAndUnit> m_cmdSetVolumeHeight, m_cmdSetVolumeDiameter, m_cmdSetVolumeOffset;

    // Select starting position, adjust using the 'm_cmdPosMode'
    enum { point, inCylinder, inSphere, inRock, inLogicalVolume } m_posMode;
    shared_ptr<G4UIcmdWithAString> m_cmdPosMode;

    // Select a LogicalVolume to generate points inside
    G4String m_logicalVolumeName;
    shared_ptr<G4UIcmdWithAString> m_cmdSetLogicalVolumeName;
    // Select (starting) particle momentum direction), adjust using the 'm_cmdDirectionMode'
    // Note: Automatically set to 'decay0' if a file is provided!
    enum { directional, isotropic, decay0 } m_directionMode;
    shared_ptr<G4UIcmdWithAString> m_cmdDirectionMode;
    G4ThreeVector m_customDirection;
    shared_ptr<G4UIcmdWith3Vector> m_cmdSetCustomDirection;

    // Pass a decay zero file wich is used as input to generate primaries
    shared_ptr<G4UIcmdWithAString> m_cmdUseDecay0File;
    std::ifstream m_decayZeroInputFile;

private:
    G4VPhysicalVolume* fRockSamplePhys;

private:
    TH3F* h3 = nullptr;
};

#endif // PrimaryGeneratorAction_hh


