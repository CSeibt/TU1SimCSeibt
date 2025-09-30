/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class

#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"

#include "geometry/TU1Detector.hh"
#include "geometry/Shielding.hh"
#include "geometry/RockSample.hh"
#include "geometry/HfSample.hh"
#include "geometry/PENSample.hh"


DetectorConstruction::DetectorConstruction()
{
    /// Add new geometries here

    m_geometryObjects.push_back(make_shared<TU1Detector>());
        m_geometryObjects.back()->SetEnabled(true);
    // The TU1 Detector is special in that it is enabled by default

    // All other pieces of geometry need to be enabled in the macro
    m_geometryObjects.push_back(make_shared<Shielding>());
    m_geometryObjects.push_back(make_shared<RockSample>());
    m_geometryObjects.push_back(make_shared<HfSample>());
    m_geometryObjects.push_back(make_shared<PENSample>());
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4RotationMatrix* noRotation = nullptr;
    auto nistManager = G4NistManager::Instance();

    // Default Materials
    nistManager->FindOrBuildMaterial("G4_Air");

    /// = World Volume =

    G4VPhysicalVolume* pWorld = nullptr;
    G4LogicalVolume*   lWorld = nullptr;

    {
        const G4String name = "world";

        const G4double worldLength = 2.0 * CLHEP::m;

        auto matAir = nistManager->FindOrBuildMaterial("G4_AIR");

        auto sWorld = new G4Box(name,
                                0.5*worldLength,
                                0.5*worldLength,
                                0.5*worldLength);

        lWorld = new G4LogicalVolume(sWorld, matAir, name);

        lWorld->SetVisAttributes(G4VisAttributes::GetInvisible()); // invisible

        pWorld = new G4PVPlacement(noRotation,
                                   G4ThreeVector(),
                                   lWorld, name,
                                   nullptr, false, 0, m_checkOverlaps);
    }

    for (auto geometryObject : m_geometryObjects)
    {
        if (geometryObject->Enabled())
        {
            G4cout << "Building " << geometryObject->GetName() << G4endl;

            geometryObject->DefineMaterials();
            geometryObject->Construct(lWorld, m_checkOverlaps);
        }
    }

    return pWorld;
}


void DetectorConstruction::ConstructSDandField()
{
    auto detector = new G4MultiFunctionalDetector("HPGe");
    G4SDManager::GetSDMpointer()->AddNewDetector(detector);

    auto psEdep = new G4PSEnergyDeposit("Edep");
    detector->RegisterPrimitive(psEdep);

    SetSensitiveDetector("sensitiveHPGe", detector);
}

void DetectorConstruction::SetNewValue(G4UIcommand* command, G4String newValue)
{
    {
        G4cerr << "Unhandled command in DetectorConstruction::SetNewValue()" << G4endl;
        exit(99);
    }
}
