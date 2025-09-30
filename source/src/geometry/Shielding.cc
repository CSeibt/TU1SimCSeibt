#include "geometry/Shielding.hh"

using CLHEP::cm;

#include "G4Polycone.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PVPlacement.hh"

Shielding::Shielding() : GeometryObject("Shielding")
{
    NewLengthValue("innerXY", "Width of inner volume in X- and Y- direction.", 25 * cm);
    NewLengthValue("innerHeight", "Height of inner volume.", 40 * cm);

    NewLengthValue("copperThickness", "Thickness of (inner) copper layer.", 5 * cm);
    NewLengthValue("leadThickness", "Thickness of (outer) lead layer.", 15 * cm);

    NewBoolValue("simulateLid", "if False, simulates completely enclosed Detector. If True, opens up Detector or rather implements lid hole.", false);
    NewBoolValue("lidOn", "Turns Lid on or off. If simulateLid is True but lidOn is false, only the hole is simulated.", true);
    NewLengthValue("shieldingHoleDiameterCu", "Diameter of lower Copper shielding Hole for lid.", 25 * cm);
    NewLengthValue("shieldingHoleDiameterPb3Bq", "Diameter of 3 Bq Lead shielding Hole for lid.", 27 * cm);
    NewLengthValue("shieldingHoleDiameterPb50BqLower", "Diameter of lower 50Bq Lead shielding Hole for lid.", 30 * cm);
    NewLengthValue("shieldingHoleDiameterPb50BqUpper", "Diameter of upper 50Bq Lead shielding Hole for lid.", 35 * cm);
    NewLengthValue("lidPosition", "Position of the Lid along the z-axis, starting from its closed state", 20 * cm);
    NewLengthValue("detectorLoc", "The relative location of the Middle of the shielding to the middle of the Detector setup", 5 * cm);

    // for Cryostat Hole
    NewBoolValue("coolingFingerOn", "Switch On/Off Cooling Finger", false);
    NewLengthValue("capLength", "Length of detector cap.", 196 * CLHEP::mm);
    NewLengthValue("capThickness", "Thickness of detector cap.", 1.6 * CLHEP::mm);
    NewLengthValue("coolingBaseHeight", "cooling Base cylinder height", 45 * CLHEP::mm);
    NewLengthValue("coolingOutDiameter", "Diameter of Tube leading to Cryostat.", 42 * CLHEP::mm);
}


void Shielding::Construct(G4LogicalVolume* lWorldVolume, G4bool checkOverlaps)
{
    /// = Shielding =
    {
        const G4String nameCu = m_name + "Cu";
        const G4String namePb = m_name + "Pb";

        // Detector Location
        const G4double detectorLoc = GetLengthValue("detectorLoc");

        // inner volume
        const G4double innerXY = GetLengthValue("innerXY");
        const G4double innerHeight = GetLengthValue("innerHeight");

        // copper layer (inner)
        const G4double copperThickness = GetLengthValue("copperThickness");

        // lead layer (outer)
        const G4double leadThickness = GetLengthValue("leadThickness");

        auto matCu = m_nistManager->FindOrBuildMaterial("G4_Cu");
        auto matPb = m_nistManager->FindOrBuildMaterial("G4_Pb");

        // Cryostat Pipe Hole
        const G4double capThickness = GetLengthValue("capThickness");
        const G4double capLength = GetLengthValue("capLength");
        const G4double coolingBaseHeight = GetLengthValue("coolingBaseHeight");
        const G4double coolingOutDiameter = GetLengthValue("coolingOutDiameter");

        // cutout
        const G4double shieldingHoleDiameterCu = GetLengthValue("shieldingHoleDiameterCu");
        const G4double shieldingHoleDiameterPb3Bq = GetLengthValue("shieldingHoleDiameterPb3Bq");
        const G4double shieldingHoleDiameterPb50BqLower = GetLengthValue("shieldingHoleDiameterPb50BqLower");
        const G4double shieldingHoleDiameterPb50BqUpper = GetLengthValue("shieldingHoleDiameterPb50BqUpper");
        const G4bool simulateLid = GetBoolValue("simulateLid");

        // Cut Out for Lid
        const G4double zValuesCut[] = { 0.5 * innerHeight - 1 * CLHEP::mm 
                                      , 0.5 * innerHeight + 0.5 * copperThickness 
                                      , 0.5 * innerHeight + 0.5 * copperThickness 
                                      , 0.5 * innerHeight + copperThickness + leadThickness / 3 
                                      , 0.5 * innerHeight + copperThickness + leadThickness / 3 
                                      , 0.5 * innerHeight + copperThickness + 2 * leadThickness / 3
                                      , 0.5 * innerHeight + copperThickness + 2 * leadThickness / 3 
                                      , 0.5 * innerHeight + copperThickness + leadThickness + 1 * CLHEP::mm
        };
        const G4double rInValuesCut[] = { 0
                                          , 0
                                          , 0
                                          , 0
                                          , 0
                                          , 0
                                          , 0
                                          , 0
        };
        const G4double rOutValuesCut[] = { 0.5 * shieldingHoleDiameterCu 
                                           , 0.5 * shieldingHoleDiameterCu
                                           , 0.5 * shieldingHoleDiameterPb3Bq 
                                           , 0.5 * shieldingHoleDiameterPb3Bq 
                                           , 0.5 * shieldingHoleDiameterPb50BqLower 
                                           , 0.5 * shieldingHoleDiameterPb50BqLower 
                                           , 0.5 * shieldingHoleDiameterPb50BqUpper 
                                           , 0.5 * shieldingHoleDiameterPb50BqUpper 
        };

        auto sLidCut = new G4Polycone("lidCut", 0, CLHEP::twopi, 8, zValuesCut, rInValuesCut, rOutValuesCut);

        // Inner volume (for subtraction)
        auto sInnerVolume = new G4Box("tmpInnerVolume", 0.5 * innerXY, 0.5 * innerXY, 0.5 * innerHeight );

        // Inner shielding

        auto sInnerShieldingTmp = new G4Box("tmpCu", 0.5 * innerXY + copperThickness, 0.5 * innerXY + copperThickness, 0.5 * innerHeight + copperThickness );

        G4VSolid* sInnerShielding;

        // Lid Case Difference
        if (simulateLid) {
            auto sInnerShieldingTmp2 = new G4SubtractionSolid("tmpCu2", sInnerShieldingTmp, sInnerVolume);
            sInnerShielding = new G4SubtractionSolid(nameCu, sInnerShieldingTmp2, sLidCut);
        }

        else {
            sInnerShielding = new G4SubtractionSolid(nameCu, sInnerShieldingTmp, sInnerVolume);
        }

        // Cooling Case difference
        if (GetBoolValue("coolingFingerOn")) {
            auto sCryoTube = new G4Tubs("CryoHole", 0, 0.5 * coolingOutDiameter + 2 * CLHEP::mm, 1.5 * (copperThickness + leadThickness), 0, CLHEP::twopi);
            auto OutTubeRot = new G4RotationMatrix();
            OutTubeRot->rotateY(CLHEP::pi / 2);
            sInnerShielding = new G4SubtractionSolid(nameCu, sInnerShielding, sCryoTube, OutTubeRot, G4ThreeVector(0.5 * innerXY + 1.45 * (copperThickness + leadThickness), 0, -capThickness - capLength - 0.5 * coolingBaseHeight + detectorLoc));
        }

        auto lInnerShielding = new G4LogicalVolume(sInnerShielding, matCu, nameCu);

        lInnerShielding->SetVisAttributes(G4Colour::Brown());

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(0, 0, -detectorLoc),
            lInnerShielding, nameCu,
            lWorldVolume, false, 0, checkOverlaps);

        // Outer shielding

        auto sOuterShieldingTmp = new G4Box("tmpPb", 0.5 * innerXY + copperThickness + leadThickness, 0.5 * innerXY + copperThickness + leadThickness, 0.5 * innerHeight + copperThickness + leadThickness);

        G4VSolid* sOuterShielding;

        if (simulateLid) {
            auto sOuterShieldingTmp2 = new G4SubtractionSolid("tmpPb2", sOuterShieldingTmp, sInnerShieldingTmp);
            sOuterShielding = new G4SubtractionSolid(namePb, sOuterShieldingTmp2, sLidCut);
        }

        else {
            sOuterShielding = new G4SubtractionSolid(namePb, sOuterShieldingTmp, sInnerShieldingTmp);
        }

        // Cooling Case difference
        if (GetBoolValue("coolingFingerOn")) {
            auto sCryoTube = new G4Tubs("CryoHole", 0, 0.5 * coolingOutDiameter + 2 * CLHEP::mm, 1.5 * (copperThickness + leadThickness), 0, CLHEP::twopi);
            auto OutTubeRot = new G4RotationMatrix();
            OutTubeRot->rotateY(CLHEP::pi / 2);
            sOuterShielding = new G4SubtractionSolid(nameCu, sOuterShielding, sCryoTube, OutTubeRot, G4ThreeVector(0.5 * innerXY + 1.45 * (copperThickness + leadThickness), 0, -capThickness - capLength - 0.5 * coolingBaseHeight + detectorLoc));
        }

        auto lOuterShielding = new G4LogicalVolume(sOuterShielding, matPb, namePb);

        lInnerShielding->SetVisAttributes(G4Colour::Brown());

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(0, 0, -detectorLoc ),
            lOuterShielding, namePb,
            lWorldVolume, false, 0, checkOverlaps);
    }

    /// = Lid =
    if (GetBoolValue("simulateLid") && GetBoolValue("lidOn"))
    {
        const G4String nameCu = m_name + "Cu";
        const G4String namePb = m_name + "Pb";
        const G4String nameLCu = "LidCu";
        const G4String nameLPb = "LidPb";

        // Detector Location
        const G4double detectorLoc = GetLengthValue("detectorLoc");

        // inner volume
        const G4double innerXY = GetLengthValue("innerXY");
        const G4double innerHeight = GetLengthValue("innerHeight");

        // copper layer (inner)
        const G4double copperThickness = GetLengthValue("copperThickness");

        // lead layer (outer)
        const G4double leadThickness = GetLengthValue("leadThickness");

        auto matCu = m_nistManager->FindOrBuildMaterial("G4_Cu");
        auto matPb = m_nistManager->FindOrBuildMaterial("G4_Pb");

        // Lid
        const G4double shieldingHoleDiameterCu = GetLengthValue("shieldingHoleDiameterCu");
        const G4double shieldingHoleDiameterPb3Bq = GetLengthValue("shieldingHoleDiameterPb3Bq");
        const G4double shieldingHoleDiameterPb50BqLower = GetLengthValue("shieldingHoleDiameterPb50BqLower");
        const G4double shieldingHoleDiameterPb50BqUpper = GetLengthValue("shieldingHoleDiameterPb50BqUpper");

        const G4double lidPosition = (GetLengthValue("lidPosition") < 0) ? 0 : GetLengthValue("lidPosition"); //check if lidPosition is >0

        //  Lid Shape Cu
        const G4double zValuesCu[] = { 0 
                                      ,0.5 * copperThickness 
                                      ,0.5 * copperThickness 
                                      ,copperThickness 
        };
        const G4double rInValuesCu[] = { 0
                                          , 0
                                          , 0
                                          , 0
        };
        const G4double rOutValuesCu[] = { 0.5 * shieldingHoleDiameterCu
                                           , 0.5 * shieldingHoleDiameterCu
                                           , 0.5 * shieldingHoleDiameterPb3Bq
                                           , 0.5 * shieldingHoleDiameterPb3Bq
        };

        //  Lid Shape Pb
        const G4double zValuesPb[] = { copperThickness 
                                      ,copperThickness + leadThickness / 3 
                                      ,copperThickness + leadThickness / 3 
                                      ,copperThickness + 2 * leadThickness / 3 
                                      ,copperThickness + 2 * leadThickness / 3 
                                      ,copperThickness + leadThickness 
        };
        const G4double rInValuesPb[] = { 0
                                          , 0
                                          , 0
                                          , 0
                                          , 0
                                          , 0
        };
        const G4double rOutValuesPb[] = { 0.5 * shieldingHoleDiameterPb3Bq
                                           , 0.5 * shieldingHoleDiameterPb3Bq
                                           , 0.5 * shieldingHoleDiameterPb50BqLower
                                           , 0.5 * shieldingHoleDiameterPb50BqLower
                                           , 0.5 * shieldingHoleDiameterPb50BqUpper
                                           , 0.5 * shieldingHoleDiameterPb50BqUpper
        };

        // Lid Solid volume
        auto sLidCu = new G4Polycone("lidCut", 0, CLHEP::twopi, 4, zValuesCu, rInValuesCu, rOutValuesCu);
        auto sLidPb = new G4Polycone("lidCut", 0, CLHEP::twopi, 6, zValuesPb, rInValuesPb, rOutValuesPb);


        auto lLidCu = new G4LogicalVolume(sLidCu, matCu, nameLCu);

        lLidCu->SetVisAttributes(G4Colour::Brown());

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(0, 0, 0.5 * innerHeight + lidPosition - detectorLoc ),
            lLidCu, nameLCu,
            lWorldVolume, false, 0, checkOverlaps);

        auto lLidPb = new G4LogicalVolume(sLidPb, matPb, nameLPb);

        lLidPb->SetVisAttributes(G4Colour::Gray());

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(0, 0, 0.5 * innerHeight + lidPosition - detectorLoc),
            lLidPb, nameLPb,
            lWorldVolume, false, 0, checkOverlaps);

    }
}

void Shielding::DefineMaterials()
{
    // only using default materials
}
