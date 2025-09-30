/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

/// \details DetectorConstruction class to construct geometry for Geant4.
///          Apart from the world volume, this class holds a list of
///          GeometryObject objects, which are built, if they have been enabled
///          via macro commands.

#ifndef B4dDetectorConstruction_h
#define B4dDetectorConstruction_h 1

#include "GeometryObject.hh"

#include "G4VUserDetectorConstruction.hh"
#include "G4UImessenger.hh"
#include "CADmesh.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"

#include "G4SystemOfUnits.hh"
#include "globals.hh"

#include <memory>
using std::shared_ptr;
using std::make_shared;

#include <vector>
using std::vector;

class G4VPhysicalVolume;


class DetectorConstruction : public G4VUserDetectorConstruction,
    public G4UImessenger
{
public:
    DetectorConstruction();
    ~DetectorConstruction() override {};

public:
    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

public:
    void SetNewValue (G4UIcommand* command, G4String newValue) override;

private:
    // List of geometry objects
    vector<shared_ptr<GeometryObject>> m_geometryObjects;

    // data members
    G4bool m_checkOverlaps = true; // option to activate checking of volumes overlaps
};

#endif
