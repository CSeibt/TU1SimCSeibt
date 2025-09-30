#include "geometry/HfSample.hh"

using CLHEP::cm;

#include "G4Tubs.hh"
#include "G4Polycone.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PVPlacement.hh"

HfSample::HfSample() : GeometryObject("HfSample")
{
    NewLengthValue("beakerBottomThickness", "Thickness of the bottom of the beaker.", 1*CLHEP::mm);
    NewLengthValue("beakerSideThickness", "Thickness of the side of the beaker.", 1*CLHEP::mm);
    NewLengthValue("beakerOuterDiameter", "Outer diameter of the beaker.", 29*CLHEP::mm);
    NewLengthValue("beakerOuterHeight", "Outer height of the beaker.", 69*CLHEP::mm);

    NewLengthValue("liquidLevel", "Level of liquid in the beaker.", 24*CLHEP::mm);
}


void HfSample::Construct(G4LogicalVolume *lWorldVolume, G4bool checkOverlaps)
{
    /// = Hafnium Liquid Sample =

    const G4String name = "sample";
    const G4String cuName = "sampleHolder";
    const G4String beakerName = "sampleBeaker";

    const G4double beakerBottomThickness = GetLengthValue("beakerBottomThickness");
    const G4double beakerSideThickness = GetLengthValue("beakerSideThickness");
    const G4double beakerOuterDiameter = GetLengthValue("beakerOuterDiameter");
    const G4double beakerOuterHeight = GetLengthValue("beakerOuterHeight");

    const G4double liquidLevel = GetLengthValue("liquidLevel");

    auto matCu      = m_nistManager->FindOrBuildMaterial("G4_Cu");
    auto matPlastic = m_nistManager->FindOrBuildMaterial("G4_POLYCARBONATE");
    auto matWater   = m_nistManager->FindOrBuildMaterial("G4_WATER");

    const G4double tolerance = 1e-3*CLHEP::mm;

    // Cu holder - this is approximated as round on the outside. Should not affect the direct path of gammas.
    {
        G4double zValues[] = {0*CLHEP::mm + tolerance, 1*CLHEP::mm, 1*CLHEP::mm, 5*CLHEP::mm - tolerance};
        G4double innerRadius[] = {0.5*20.2*CLHEP::mm, 0.5*20.2*CLHEP::mm, 0.5*30*CLHEP::mm, 0.5*30*CLHEP::mm};
        G4double outerRadius[] = {57.6*CLHEP::mm, 57.6*CLHEP::mm, 57.6*CLHEP::mm, 57.6*CLHEP::mm};

        auto sCuHolder = new G4Polycone(cuName, 0, CLHEP::twopi, 4, zValues, innerRadius, outerRadius);

        auto lCuHolder = new G4LogicalVolume(sCuHolder, matCu, cuName);

        lCuHolder->SetVisAttributes(G4Colour::Brown());

        new G4PVPlacement(m_noRotation,
                          G4ThreeVector(0, 0, 0),
                          lCuHolder, cuName,
                          lWorldVolume, false, 0, checkOverlaps);
    }

    // Plastic beaker
    {
        G4double zValues[] = {1*CLHEP::mm + tolerance, 1*CLHEP::mm + beakerBottomThickness, 1*CLHEP::mm + beakerBottomThickness, 1*CLHEP::mm + beakerOuterHeight};
        G4double innerRadius[] = {0, 0, 0.5*beakerOuterDiameter - beakerSideThickness + tolerance, 0.5*beakerOuterDiameter - beakerSideThickness + tolerance};
        G4double outerRadius[] = {0.5*beakerOuterDiameter - tolerance, 0.5*beakerOuterDiameter - tolerance, 0.5*beakerOuterDiameter - tolerance, 0.5*beakerOuterDiameter - tolerance};

        auto sBeaker = new G4Polycone(beakerName, 0, CLHEP::twopi, 4, zValues, innerRadius, outerRadius);

        auto lBeaker = new G4LogicalVolume(sBeaker, matPlastic, beakerName);

        lBeaker->SetVisAttributes(G4Colour::White());

        new G4PVPlacement(m_noRotation,
                          G4ThreeVector(0, 0, 0),
                          lBeaker, beakerName,
                          lWorldVolume, false, 0, checkOverlaps);
    }

    // Liquid volume
    {
        auto sSample = new G4Tubs(name, 0, 0.5*beakerOuterDiameter - beakerSideThickness - tolerance, 0.5*liquidLevel, 0, CLHEP::twopi);

        auto lSample = new G4LogicalVolume(sSample, matWater, name);

        lSample->SetVisAttributes(G4Colour::Cyan());

        new G4PVPlacement(m_noRotation,
                          G4ThreeVector(0, 0, 1*CLHEP::mm + beakerBottomThickness + 0.5*liquidLevel),
                          lSample, name,
                          lWorldVolume, false, 0, checkOverlaps);
    }
}

void HfSample::DefineMaterials()
{
    // only using default materials
}
