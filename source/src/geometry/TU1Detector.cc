#include "geometry/TU1Detector.hh"

#include "G4Polycone.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Sphere.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PVPlacement.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "geometry/Materials.hh"

TU1Detector::TU1Detector() : GeometryObject("TU1Detector")
{
    NewLengthValue("capDiameter", "Diameter of detector cap.", 115 * CLHEP::mm);
    NewLengthValue("capLength", "Length of detector cap.", 196 * CLHEP::mm);
    NewLengthValue("capThickness", "Thickness of detector cap.", 1.5 * CLHEP::mm);
    NewLengthValue("capThicknessside", "Thickness of detector cap on the cap side", 2.5 * CLHEP::mm);

    NewLengthValue("distanceFromCap", "Distance of HPGe crystal from cap.", 5.2 * CLHEP::mm);

    NewLengthValue("crystalDiameter", "Diameter of HPGe crystal.", 91.2 * CLHEP::mm);
    NewLengthValue("crystalLength", "Length of HPGe crystal.", 91.2 * CLHEP::mm);

    NewLengthValue("boreHoleDiameter", "Borehole diameter of HPGe crystal.", 14 * CLHEP::mm);
    NewLengthValue("boreHoleLength", "Length of borehole of HPGe crystal.", 65 * CLHEP::mm);

    NewLengthValue("deadLayerFront", "Dead layer of front face of HPGe.", 0.2 * CLHEP::mm);
    NewLengthValue("deadLayerOutside", "Dead layer of outside (radial) surface of HPGe.", 1. * CLHEP::mm);
    NewLengthValue("deadLayerInside", "Dead layer of inside (radial) surface of HPGe.", 0.2 * CLHEP::mm);
    NewLengthValue("deadLayerBack", "Dead layer of back/rear surface of HPGe.", 0.2 * CLHEP::mm);

    NewBoolValue("crystalHolderOn", "Toggels on/off Aluminium Crystal Holder", false);
    NewLengthValue("crystalHolderThickness", "Thickness of Aluminium Crystal Holder", 2.65 * CLHEP::mm);
    NewLengthValue("crystalHolderLength", "Length of Aluminium Crystal Holder", 101 * CLHEP::mm);

    NewBoolValue("coolingFingerOn", "Switch On/Off Cooling Finger", false);
    NewLengthValue("coolingFingerLength", "Length of Cooling Finger", 14 * CLHEP::cm);
    NewLengthValue("coolingFingerDiameter", "Diameter of Cooling Finger", 30 * CLHEP::mm);
    NewLengthValue("coolingPinDiameter", "Diameter of inner contact pin", 5 * CLHEP::mm);
    NewLengthValue("coolingBaseDiameter", "cooling Base cylinder Diameter", 95 * CLHEP::mm);
    NewLengthValue("coolingBaseHeight", "cooling Base cylinder height", 45 * CLHEP::mm);
    NewLengthValue("coolingOutDiameter", "Diameter of Tube leading to Cryostat.", 42 * CLHEP::mm);
    NewLengthValue("coolingOutLength", "Length of Tube leading to Cryostat.", 30 * CLHEP::cm);

    NewBoolValue("sampleHolderOn", "Switch On/Off Sample Holder.", false);
    NewLengthValue("sampleDistance", "Distance to middle of sample", 7 * CLHEP::cm);
    NewLengthValue("holderLegBaseLength", "Length of base of Sample holder Legs.", 3 * CLHEP::mm);
    NewLengthValue("holderLegDiameter", "Diameter of Legs' Zylinders", 0.6 * CLHEP::cm);
    NewLengthValue("holderTopWidth", "Width of holding Block.", 95 * CLHEP::mm);
    NewLengthValue("holderTopHeight", "Height of holding Block.", 0.5 * CLHEP::cm);
    NewLengthValue("holderHoleInnerDiameter", "Inner Diameter of the cut out in holding  Block", 2.02 * CLHEP::cm);
    NewLengthValue("holderHoleOuterDiameter", "Outer Diameter of the cut out in holding  Block", 3.35 * CLHEP::cm);
    NewBoolValue("holderRoundedCorners", "Switch On/Off Rounded Corners.", false);
    NewLengthValue("cornerDiameter", "Diameter of Rounded Corners of Sample Holder", 7 * CLHEP::mm);
    NewLengthValue("bottomThicknessSampleHolder", "Thickness of bottom layer of Sample Holder", 2.5 * CLHEP::mm);

    NewBoolValue("innerCopperRing", "Toggels on/off inner Copper Shilding Ring", false);
    NewBoolValue("innerCopperDisk", "Toggels on/off inner Copper Shilding Disk", false);
    NewBoolValue("innerCopperDiskbottom", "Toggels on/off inner Copper Shilding Disk on the bottom", false);

    NewBoolValue("plasticCasingOn", "Switch On/Off Plastic Casing of point sample.", false);
    NewBoolValue("plasticFoilOn", "Switch On/Off Plastic Foil", false);

    NewBoolValue("extendedSampleOn", "Switch On/Off Plastic Casing of volume sample.", false);
    NewStringValue("extendedSampleShape", "Shape of extended sample. choose between cylinder and sphere", "cylinder");
    NewLengthValue("extendedSampleFilling", "Height of extendedSample in plastic casing.", 1.5 * CLHEP::cm);
    NewLengthValue("plasticInnerDiameter", "Inner Duíameter of Plastic Sample Holder Casing.", 6.93 * CLHEP::cm);
    NewLengthValue("plasticInnerHeight", "Inner Height of Plastic Sample Holder Casing.", 2.175 * CLHEP::cm);
    NewLengthValue("plasticOuterDiameter", "Outer Duíameter of Plastic Sample Holder Casing.", 7.125 * CLHEP::cm);
    NewLengthValue("plasticOuterHeight", "Outer Height of Plastic Sample Holder Casing.", 2.36 * CLHEP::cm);
    NewLengthValue("sampleOffset", "Offset of extended Sample from sample Distance", 0 * CLHEP::cm);
  //  NewLengthValue("extendedsampleDistance", "Distance to middle of extended sample", 1 * CLHEP::cm);

    NewLengthValue("sphereDiameter", "Diameter of extended source sphere", 2.5 * CLHEP::cm);
    NewLengthValue("sphereOffset", "Offset of extended source sphere along z-axis", 0 * CLHEP::cm);
    NewLengthValue("cylinderDiameter", "Diameter of extended source cylinder", 90. * CLHEP::mm);
    NewLengthValue("cylinderOffset", "Offset of extended source cylinder along z-axis", 0 * CLHEP::cm);
    NewLengthValue("cylinderLength", "Length of cylinder sample", 1 * CLHEP::cm);

    NewStringValue("sampleMaterialName", "Name of defined Material used for extended source", "cerium_5");
    NewDensityValue("sampleMaterialDensity", "Density of Material used for extended source", -1 * CLHEP::g / CLHEP::cm3);

}

// Implementiere die DefineMaterials-Methode
void TU1Detector::DefineMaterials() {
    Materials::DefineMaterials();  // Aufruf der statischen Methode in der Materials-Klasse
}

void TU1Detector::Construct(G4LogicalVolume* lWorldVolume, G4bool checkOverlaps)
{ 
    /// = Detector Cap =

    {
		
		
        const G4String name = "cap";

        const G4double capDiameter = GetLengthValue("capDiameter");
        const G4double capLength = GetLengthValue("capLength");
        const G4double capThickness = GetLengthValue("capThickness");
        const G4double capThicknessside = GetLengthValue("capThicknessside");

        auto matAl = m_nistManager->FindOrBuildMaterial("G4_Al");


        const G4double zValues[] = { -capLength,
                                    -capThickness,
                                    -capThickness,
                                    0
        };

        const G4double rIn[] = { 0.5 * capDiameter - capThicknessside,
                                0.5 * capDiameter - capThicknessside,
                                0,
                                0
        };

        const G4double rOut[] = { 0.5 * capDiameter,
                                 0.5 * capDiameter,
                                 0.5 * capDiameter,
                                 0.5 * capDiameter
        };

        auto sCap = new G4Polycone(name, 0, CLHEP::twopi, 4, zValues, rIn, rOut);

        auto lCap = new G4LogicalVolume(sCap, matAl, name);

        lCap->SetVisAttributes(G4Colour::Gray());

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(),
            lCap, name,
            lWorldVolume, false, 0, checkOverlaps);
    }


    /// = Germanium Crystal =
    // Two volumes;:
    //  Mother volume (M) including dead layer,
    //  Daughter volume (D) of sensitive detector

    {   
        const G4String nameM = "HPGe";
        const G4String nameD = "sensitiveHPGe";

        const G4double capThickness = GetLengthValue("capThickness");
        const G4double distanceFromCap = GetLengthValue("distanceFromCap");

        const G4double crystalDiameter = GetLengthValue("crystalDiameter");
        const G4double crystalLength = GetLengthValue("crystalLength");
        const G4double boreHoleDiameter = GetLengthValue("boreHoleDiameter");
        const G4double boreHoleLength = GetLengthValue("boreHoleLength");

        const G4double deadLayerFront = GetLengthValue("deadLayerFront");
        const G4double deadLayerOutside = GetLengthValue("deadLayerOutside");
        const G4double deadLayerInside = GetLengthValue("deadLayerInside");
        const G4double deadLayerBack = GetLengthValue("deadLayerBack");

        auto matGe = m_nistManager->FindOrBuildMaterial("G4_Ge");


        // Mother

        const G4double zValuesM[] = { -crystalLength,
                                     -crystalLength + boreHoleLength,
                                     -crystalLength + boreHoleLength,
                                     0
        };

        const G4double rInM[] = { 0.5 * boreHoleDiameter,
                                 0.5 * boreHoleDiameter,
                                 0,
                                 0
        };

        const G4double rOutM[] = { 0.5 * crystalDiameter,
                                  0.5 * crystalDiameter,
                                  0.5 * crystalDiameter,
                                  0.5 * crystalDiameter
        };

        auto sCrystalM = new G4Polycone(nameM, 0, CLHEP::twopi, 4, zValuesM, rInM, rOutM);

        auto lCrystalM = new G4LogicalVolume(sCrystalM, matGe, nameM);

        lCrystalM->SetVisAttributes(G4Colour::Yellow());

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(0, 0, -distanceFromCap - capThickness),
            lCrystalM, nameM,
            lWorldVolume, false, 0, checkOverlaps);


        // Daughter

        const G4double zValuesD[] = { -crystalLength + deadLayerBack,
                                     -crystalLength + boreHoleLength + deadLayerInside,
                                     -crystalLength + boreHoleLength + deadLayerInside,
                                     -deadLayerFront
        };

        const G4double rInD[] = { 0.5 * boreHoleDiameter + deadLayerInside,
                                 0.5 * boreHoleDiameter + deadLayerInside,
                                 0,
                                 0
        };

        const G4double rOutD[] = { 0.5 * crystalDiameter - deadLayerOutside,
                                  0.5 * crystalDiameter - deadLayerOutside,
                                  0.5 * crystalDiameter - deadLayerOutside,
                                  0.5 * crystalDiameter - deadLayerOutside
        };

        auto sCrystalD = new G4Polycone(nameD, 0, CLHEP::twopi, 4, zValuesD, rInD, rOutD);

        auto lCrystalD = new G4LogicalVolume(sCrystalD, matGe, nameD);

        lCrystalD->SetVisAttributes(G4Colour::Red());

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(),
            lCrystalD, nameD,
            lCrystalM, false, 0, checkOverlaps);
    }

    // = Aluminium Crystal Holder

    if (GetBoolValue("crystalHolderOn")) {
		
        const G4double crystalDiameter = GetLengthValue("crystalDiameter");
        const G4double crystalLength = GetLengthValue("crystalLength");
        const G4double distanceFromCap = GetLengthValue("distanceFromCap");
        const G4double capThickness = GetLengthValue("capThickness");
        const G4double AlHolderThickness = GetLengthValue("crystalHolderThickness");
        const G4double AlHolderLength = GetLengthValue("crystalHolderLength");
        const G4double boreHoleDiameter = GetLengthValue("boreHoleDiameter");
        

        const G4double zposition = - distanceFromCap - capThickness - 15.7 * CLHEP::mm - AlHolderLength;
        const G4double holderdiameter = 0.5 * crystalDiameter + 0.75 * CLHEP::mm + AlHolderThickness;

        auto matAl = m_nistManager->FindOrBuildMaterial("G4_Al");

       /* auto sAluHolder = new G4Tubs("AluHolder", 0.5 * crystalDiameter + 0.75 * CLHEP::mm, 0.5 * crystalDiameter + 0.75 * CLHEP::mm + AlHolderThickness, 0.5 * crystalLength, 0, CLHEP::twopi);

        auto lAluHolder = new G4LogicalVolume(sAluHolder, matAl, "AluHolder");

        lAluHolder->SetVisAttributes(G4Colour(230 / 255, 242 / 255, 1, 0.9));

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(0, 0, -0.5 * crystalLength - distanceFromCap - capThickness),
            lAluHolder, "AluHolder",
            lWorldVolume, false, 0, checkOverlaps); */


        const G4double zValues[] = { zposition,
                                    zposition + 8 * CLHEP::mm,
                                   zposition + 8 * CLHEP::mm,
                                   zposition + AlHolderLength
        };

        const G4double rIn[] = { 0.5 * 1.8 * CLHEP::cm,
                                 0.5 * 1.8 * CLHEP::cm,
                                 holderdiameter - AlHolderThickness,
                                 holderdiameter - AlHolderThickness
        };

        const G4double rOut[] = { holderdiameter,
                                 holderdiameter,
                                  holderdiameter,
                                 holderdiameter
        };

        auto sCrystalHolder = new G4Polycone("Crystal Holder", 0, CLHEP::twopi, 4, zValues, rIn, rOut);

        auto lCrystalHolder = new G4LogicalVolume(sCrystalHolder, matAl, "Crystal Holder");

        //lCrystalHolder->SetVisAttributes(G4Colour(230 / 255, 242 / 255, 1, 0.9));
        lCrystalHolder->SetVisAttributes(G4Colour::Gray());
   

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(),
            lCrystalHolder, "Crystal Holder",
            lWorldVolume, false, 0, checkOverlaps);

        // hier wird der untere Teil aus Blei definiert (siehe techische Zeichnung)
        auto matPb = m_nistManager->FindOrBuildMaterial("G4_Pb");

        const G4double zValuesPb[] = { zposition + 8 * CLHEP::mm,
                                   zposition + 8 * CLHEP::mm + 5 * CLHEP::mm,
                                   zposition + 8 * CLHEP::mm + 10 * CLHEP::mm
                                   

        };

        const G4double rInPb[] = { 0.5 *  1.2 * CLHEP::cm,
                                 0.5 * 1.2 * CLHEP::cm,
                                 0.5 * 3 * CLHEP::cm

        };

        const G4double rOutPb[] = { 0.5 * crystalDiameter,
                                  0.5 * crystalDiameter,
                                  0.5 * crystalDiameter

        };

        auto sCrystalHolderPb = new G4Polycone("Crystal Holder Blei", 0, CLHEP::twopi, 3, zValuesPb, rInPb, rOutPb);

        auto lCrystalHolderPb = new G4LogicalVolume(sCrystalHolderPb, matPb, "Crystal Holder");

        lCrystalHolderPb->SetVisAttributes(G4Colour(0.1, 0.1, 0.1, 1));

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(),
            lCrystalHolderPb, "Crystal Holder Blei",
            lWorldVolume, false, 0, checkOverlaps);

    }

    // = Inner Round Copper Shielding approximation

    

    if (GetBoolValue("innerCopperDisk")) { Materials::DefineMaterials();
        const G4double outerDiameterD = 24.5 * CLHEP::cm;
        const G4double cuLengthD = GetLengthValue("capLength");
        auto matCu = m_nistManager->FindOrBuildMaterial("G4_Cu");
        auto sRoundShieldD = new G4Tubs("copperDisk", 0, 0.5 * outerDiameterD, 0.5 * 5 * CLHEP::cm, 0, CLHEP::twopi);

        auto lRoundShieldD = new G4LogicalVolume(sRoundShieldD, matCu, "copperDisk");

        lRoundShieldD->SetVisAttributes(G4Colour::Brown());


        new G4PVPlacement(m_noRotation,
            G4ThreeVector(0, 0, 4 * CLHEP::cm + 2.5 * CLHEP::cm),
            lRoundShieldD, "copperDisk",
            lWorldVolume, false, 0, checkOverlaps);
    }

    if (GetBoolValue("innerCopperDiskbottom")) { Materials::DefineMaterials();
        const G4double outerDiameterD = 14.5 * CLHEP::cm;
        const G4double cuLengthD = GetLengthValue("capLength");
        auto matCu = m_nistManager->FindOrBuildMaterial("G4_Cu");
        auto sRoundShieldD = new G4Tubs("copperDisk", 0, 0.5 * outerDiameterD, 0.5 * 5 * CLHEP::cm, 0, CLHEP::twopi);

        auto lRoundShieldD = new G4LogicalVolume(sRoundShieldD, matCu, "copperDiskbottom");

        lRoundShieldD->SetVisAttributes(G4Colour::Brown());


        new G4PVPlacement(m_noRotation,
            G4ThreeVector(0, 0, -27.5 * CLHEP::cm),
            lRoundShieldD, "copperDiskbottom",
            lWorldVolume, false, 0, checkOverlaps);
    }

    if (GetBoolValue("innerCopperRing") && GetBoolValue("coolingFingerOn")) {

       

        ////////////////////////////////
        // CU Ring
        const G4double cuLength = 30. * CLHEP::cm;
        const G4double innerDiameter = 14.5 * CLHEP::cm;
        const G4double outerDiameter = 24.5 * CLHEP::cm;
        
        const G4double coolingOutDiameter = GetLengthValue("coolingOutDiameter");
        const G4double coolingOutLength = GetLengthValue("coolingOutLength");
        const G4double capThickness = GetLengthValue("capThickness");
        const G4double capLength = GetLengthValue("capLength");
        const G4double coolingBaseHeight = GetLengthValue("coolingBaseHeight");
        const G4double distanceFromCap = GetLengthValue("distanceFromCap");
        const G4double crystalLength = GetLengthValue("crystalLength");
        const G4double boreHoleLength = GetLengthValue("boreHoleLength");
        const G4double AlHolderLength = GetLengthValue("crystalHolderLength");


        const G4double fingerDiameter = GetLengthValue("coolingFingerDiameter");
        const G4double pinDiameter = GetLengthValue("coolingPinDiameter");
        const G4double fingerLength = capLength - capThickness -distanceFromCap -crystalLength + boreHoleLength;

        auto matCu = m_nistManager->FindOrBuildMaterial("G4_Cu");
        auto matAl = m_nistManager->FindOrBuildMaterial("G4_Al");
         G4Material* matTeflon = Materials::GetMaterial("Teflon");
         G4Material* matBrass = Materials::GetMaterial("Brass");
        auto sRoundShield = new G4Tubs("copperRing", 0.5 * innerDiameter, 0.5 * outerDiameter, 0.5 * cuLength + 2 * CLHEP::cm, 0, CLHEP::twopi);

       
				// Subtraktionsvolumen: Loch für Kühlfinger
		auto subtractionVolume = new G4Tubs("SubtractionVolume", 0, 
			0.5 * coolingOutDiameter + 2 * CLHEP::mm, 
			0.5 * coolingOutLength, 0, CLHEP::twopi);

		auto subOutTubeRot = new G4RotationMatrix();
		subOutTubeRot->rotateY(CLHEP::pi / 2);

		// Korrekte Translation für die Subtraktion
		G4ThreeVector subtractionTranslation(
			0.5 * coolingOutLength + 3 * CLHEP::cm, 
			0, 
			-(30. * CLHEP::cm * 0.5 - 5 * CLHEP::cm + 1.2 * CLHEP::cm) + 0.5 * coolingBaseHeight
		);

		auto modifiedVolume = new G4SubtractionSolid("ModifiedVolume",
			sRoundShield,
			subtractionVolume,
			subOutTubeRot, 
			subtractionTranslation // Korrekte Translation für die Subtraktion
		);

		auto lRoundShield = new G4LogicalVolume(modifiedVolume, matCu, "copperRing");
		lRoundShield->SetVisAttributes(G4Colour::Brown());

		// Korrekte Position für die Platzierung
		G4ThreeVector placementPosition(0, 0, -0.5 * cuLength + 2 * CLHEP::cm);

		new G4PVPlacement(
			m_noRotation, 
			placementPosition,  // Hier korrekt übergeben
			lRoundShield, "copperRing",
			lWorldVolume, false, 0, checkOverlaps);


        ///////  /// Cooling Finger
        const G4String nameF = "coolingFinger";
        const G4String nameB = "coolingBase";
        const G4String nameO = "coolingOut";


        const G4double coolingBaseDiameter = GetLengthValue("coolingBaseDiameter");
        
        
         // Cooling Finger part made of insulator teflon
        const G4double zpos = - distanceFromCap - capThickness - 15.7 * CLHEP::mm - AlHolderLength- 0.2 * CLHEP::cm;
        const G4double zValuesInsulator[] = {zpos - 1 * CLHEP::cm,
									         zpos ,
									         zpos ,
								     	     zpos + 0.2 * CLHEP::cm ,
									         zpos + 0.2 * CLHEP::cm ,
									         zpos + 0.2 * CLHEP::cm + 1.6 * CLHEP::cm
									  
        };
        const G4double rInInsulator[] = {0.5 * 0.3 * CLHEP::cm,
								  0.5 * 0.3 * CLHEP::cm,
								  0.5 * 0.3 * CLHEP::cm,
								  0.5 * 0.3 * CLHEP::cm,
								  0.5 * 0.3 * CLHEP::cm,
								  0.5 * 0.3 * CLHEP::cm
        };

        const G4double rOutInsulator[] = { 0.5 * 1. * CLHEP::cm,
									0.5 * 1. * CLHEP::cm,
									0.5 * 6 *CLHEP::cm,
									0.5 * 6 *CLHEP::cm,
									0.5 * 1. * CLHEP::cm,
									0.5 * 1. * CLHEP::cm
        };
        

        auto sFingerInsulator = new G4Polycone(nameF, 0, CLHEP::twopi, 6, zValuesInsulator, rInInsulator, rOutInsulator);
        auto lFingerInsulator = new G4LogicalVolume(sFingerInsulator, matTeflon, nameF);

        lFingerInsulator->SetVisAttributes(G4Colour::Blue());

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(0, 0, 0),
            lFingerInsulator, nameF,
            lWorldVolume, false, 0, checkOverlaps);
        
        
        const G4double zValuesInsulator2[] = {-distanceFromCap - capThickness -crystalLength - 0.2 * CLHEP::cm,
										      -distanceFromCap - capThickness -crystalLength 
        };
        const G4double rInInsulator2[] = {0,
										  0
        };
        const G4double rOutInsulator2[] = {0.5 * 1.6*CLHEP::cm,
										   0.5 * 1.6 *CLHEP::cm    
        };
      
        auto sFingerInsulator2 = new G4Polycone(nameF, 0, CLHEP::twopi, 2, zValuesInsulator2, rInInsulator2, rOutInsulator2);
        auto lFingerInsulator2 = new G4LogicalVolume(sFingerInsulator2, matTeflon, nameF);

        lFingerInsulator2->SetVisAttributes(G4Colour::Blue());

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(0, 0, 0), 
            lFingerInsulator2, nameF,
            lWorldVolume, false, 0, checkOverlaps);
            
        
        // Cooling Finger part made of aluminium

        
        const G4double zValuesAl[] = { - capLength,
									   zpos - 3.1 * CLHEP::cm,
									   zpos - 3.1 * CLHEP::cm,
                                       zpos - 1. * CLHEP::cm,
                                       zpos - 1. * CLHEP::cm,
                                       zpos
        };
        const G4double rInAl[] = {0.5 * 1.8 * CLHEP::cm,
								0.5 * 1.8 * CLHEP::cm,
                                0,
                                0,
                                0.5 * 1. * CLHEP::cm,
                                0.5 * 1. * CLHEP::cm
        };

        const G4double rOutAl[] = { 0.5 * fingerDiameter,
                                 0.5 * fingerDiameter,
                                 0.5 * fingerDiameter,
                                 0.5 * fingerDiameter,
                                 0.5 * 6*CLHEP::cm,
                                 0.5 * 6 *CLHEP::cm    
        };
      
        auto sFingerAl = new G4Polycone(nameF, 0, CLHEP::twopi, 6, zValuesAl, rInAl, rOutAl);
        auto lFingerAl = new G4LogicalVolume(sFingerAl, matAl, nameF);

        lFingerAl->SetVisAttributes(G4Colour::Brown());

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(0, 0, 0), 
            lFingerAl, nameF,
            lWorldVolume, false, 0, checkOverlaps);
            
        // Cooling Finger part made of Brass
        
        const G4double zValuesBrass[] = { zpos - 0.8 * CLHEP::cm,
										 zpos + 2.55 * CLHEP::cm
										 
        };
        const G4double rInBrass[] = {0,
								  0
        };
        const G4double rOutBrass[] = {0.5 * 0.3*CLHEP::cm,
                                 0.5 * 0.3 *CLHEP::cm    
        };
      
        auto sFingerBrass = new G4Polycone(nameF, 0, CLHEP::twopi, 2, zValuesBrass, rInBrass, rOutBrass);
        auto lFingerBrass = new G4LogicalVolume(sFingerBrass, matBrass, nameF);

        lFingerBrass->SetVisAttributes(G4Colour::Brown());

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(0, 0, 0), 
            lFingerBrass, nameF,
            lWorldVolume, false, 0, checkOverlaps);
   
		// Cooling Finger part made of Cu
		
		const G4double zValuesCopper[] = {- capLength,
									       zpos - 3.1 * CLHEP::cm
        };
        const G4double rInCopper[] = {0,
									  0
        };
        const G4double rOutCopper[] = {0.5 * 1.8 *CLHEP::cm,
									   0.5 * 1.8 *CLHEP::cm    
        };
      
        auto sFingerCopper = new G4Polycone(nameF, 0, CLHEP::twopi, 2, zValuesCopper, rInCopper, rOutCopper);
        auto lFingerCopper = new G4LogicalVolume(sFingerCopper, matCu, nameF);

        lFingerCopper->SetVisAttributes(G4Colour::Brown());

        new G4PVPlacement(m_noRotation,
            G4ThreeVector(0, 0, 0), 
            lFingerCopper, nameF,
            lWorldVolume, false, 0, checkOverlaps);


        // Cooling Base
        auto sCoolingBase = new G4Tubs(nameB, 0, 0.5 * coolingBaseDiameter, 0.5 * coolingBaseHeight, 0, CLHEP::twopi);

        /* auto lCoolingBase = new G4LogicalVolume(sCoolingBase, matCu, nameB);

           lCoolingBase->SetVisAttributes(G4Colour::Brown());

           new G4PVPlacement(m_noRotation,
                        G4ThreeVector(0, 0, -capThickness - capLength - 0.5*coolingBaseHeight),
                        lCoolingBase, nameB,
                        lWorldVolume, false, 0, checkOverlaps); */

                        // Outward Pipe

        auto sCoolingOut = new G4Tubs(nameO, 0, 0.5 * coolingOutDiameter, 0.5 * coolingOutLength, 0, CLHEP::twopi);

        auto OutTubeRot = new G4RotationMatrix();
        OutTubeRot->rotateY(CLHEP::pi / 2);

        G4ThreeVector translation(0.5 * coolingOutLength + 3* CLHEP::cm, 0, 0);

        auto sCoolingCryo = new G4UnionSolid(nameO, sCoolingBase, sCoolingOut, OutTubeRot, translation);

        auto lCoolingCryo = new G4LogicalVolume(sCoolingCryo, matCu, nameO);

        lCoolingCryo->SetVisAttributes(G4Colour::Brown());

        new G4PVPlacement(0,
            G4ThreeVector(0, 0, -capThickness - capLength - 0.5 * coolingBaseHeight),
            lCoolingCryo, nameO,
            lWorldVolume, false, 0, checkOverlaps);
    }
    else{

        /// = Cooling Finger =
         if (GetBoolValue("coolingFingerOn")) {

          const G4String nameF = "coolingFinger";
          const G4String nameB = "coolingBase";
          const G4String nameO = "coolingOut";

          const G4double capThickness = GetLengthValue("capThickness");
          const G4double capLength = GetLengthValue("capLength");
          const G4double distanceFromCap = GetLengthValue("distanceFromCap");
          const G4double crystalLength = GetLengthValue("crystalLength");
          const G4double boreHoleLength = GetLengthValue("boreHoleLength");

          const G4double fingerDiameter = GetLengthValue("coolingFingerDiameter");
          const G4double coolingBaseDiameter = GetLengthValue("coolingBaseDiameter");
          const G4double coolingBaseHeight = GetLengthValue("coolingBaseHeight");

          const G4double coolingOutDiameter = GetLengthValue("coolingOutDiameter");
          const G4double coolingOutLength = GetLengthValue("coolingOutLength");
          const G4double pinDiameter = GetLengthValue("coolingPinDiameter");
          const G4double fingerLength = capLength - capThickness -distanceFromCap -crystalLength + boreHoleLength;

          // const G4double fingerPosition = boreHoleLength - crystalLength - distanceFromCap - capThickness - 0.5 * fingerLength;
          const G4double fingerPosition = - capLength + 0.5 * fingerLength;

          auto matCu = m_nistManager->FindOrBuildMaterial("G4_Cu");

          // Finger in Borehole part
         // auto sFinger = new G4Tubs(nameF, 0, 0.5 * fingerDiameter, 0.5 * fingerLength, 0, CLHEP::twopi);
              
          const G4double zValues[] = {- capLength,
                                      - capLength+ 7.2 * CLHEP::cm,
                                      - capLength+ 7.2 * CLHEP::cm,
                                      - capLength + fingerLength
          };

          const G4double rIn[] = {0,
                                  0,
                                  0,
                                  0
          };

          const G4double rOut[] = {0.5 * fingerDiameter,
                                   0.5 * fingerDiameter,
                                   0.5 * pinDiameter,
                                   0.5 * pinDiameter
          };

          auto sFinger = new G4Polycone(nameF, 0, CLHEP::twopi, 4, zValues, rIn, rOut);
          auto lFinger = new G4LogicalVolume(sFinger, matCu, nameF);
          lFinger->SetVisAttributes(G4Colour::Brown());

          new G4PVPlacement(m_noRotation,
            G4ThreeVector(0, 0, fingerPosition),
            lFinger, nameF,
            lWorldVolume, false, 0, checkOverlaps);

             // Cooling Base
          auto sCoolingBase = new G4Tubs(nameB, 0, 0.5 * coolingBaseDiameter, 0.5 * coolingBaseHeight, 0, CLHEP::twopi);

          /* auto lCoolingBase = new G4LogicalVolume(sCoolingBase, matCu, nameB);

             lCoolingBase->SetVisAttributes(G4Colour::Brown());

             new G4PVPlacement(m_noRotation,
                          G4ThreeVector(0, 0, -capThickness - capLength - 0.5*coolingBaseHeight),
                          lCoolingBase, nameB,
                          lWorldVolume, false, 0, checkOverlaps); */

                          // Outward Pipe

          auto sCoolingOut = new G4Tubs(nameO, 0, 0.5 * coolingOutDiameter, 0.5 * coolingOutLength, 0, CLHEP::twopi);

          auto OutTubeRot = new G4RotationMatrix();
          OutTubeRot->rotateY(CLHEP::pi / 2);

          G4ThreeVector translation(17 * CLHEP::cm, 0, 0);
 
          auto sCoolingCryo = new G4UnionSolid(nameO, sCoolingBase, sCoolingOut, OutTubeRot, translation);

          auto lCoolingCryo = new G4LogicalVolume(sCoolingCryo, matCu, nameO);

          lCoolingCryo->SetVisAttributes(G4Colour::Brown());

          new G4PVPlacement(0,
              G4ThreeVector(0, 0, -capThickness - capLength - 0.5 * coolingBaseHeight),
              lCoolingCryo, nameO,
              lWorldVolume, false, 0, checkOverlaps);


         }

         if (GetBoolValue("innerCopperRing")) {
             const G4double cuLength = 30. * CLHEP::cm;
             const G4double innerDiameter = 14.5 * CLHEP::cm;
             const G4double outerDiameter = 24.5 * CLHEP::cm;

             auto matCu = m_nistManager->FindOrBuildMaterial("G4_Cu");
             auto sRoundShield = new G4Tubs("copperRing", 0.5 * innerDiameter, 0.5 * outerDiameter, 0.5 * cuLength + 2 * CLHEP::cm, 0, CLHEP::twopi);

             auto lRoundShield = new G4LogicalVolume(sRoundShield, matCu, "copperRing");

             lRoundShield->SetVisAttributes(G4Colour::Brown());


              new G4PVPlacement(m_noRotation,
                 G4ThreeVector(0, 0, -0.5 * cuLength + 2 * CLHEP::cm),
                 lRoundShield, "copperRing",
                 lWorldVolume, false, 0, checkOverlaps);
          }

     }

    // = Extended Sample Geometry
    if (GetBoolValue("extendedSampleOn")) {

        if (GetStringValue("extendedSampleShape") == "samplecontainer") {
            G4double plasticInnerDiameter, plasticInnerHeight, plasticOuterDiameter, plasticOuterHeight, sampleHeight;
            plasticInnerDiameter = GetLengthValue("plasticInnerDiameter");
            plasticInnerHeight = GetLengthValue("plasticInnerHeight");
            plasticOuterDiameter = GetLengthValue("plasticOuterDiameter");
            plasticOuterHeight = GetLengthValue("plasticOuterHeight");
            sampleHeight = GetLengthValue("extendedSampleFilling");
            G4double offset = GetLengthValue("sampleOffset");
            G4String materialName = GetStringValue("sampleMaterialName");
            
            G4double density = GetDensityValue("sampleMaterialDensity");

            G4Material* matSample;
            if (density < 0) {
                matSample = m_nistManager->FindOrBuildMaterial(materialName);
            }
            else {
                matSample = m_nistManager->BuildMaterialWithNewDensity(materialName + "1", materialName, density);
            }
			
            auto matPET = m_nistManager->FindOrBuildMaterial("G4_POLYETHYLENE");
            const G4String namePC = "SampleContainer";
            const G4double plasticThickness = 0.5 * (plasticOuterHeight - plasticInnerHeight);

            auto sOuterSampleContainer = new G4Tubs("OuterPlasticHolder", 0, 0.5 * plasticOuterDiameter, 0.5 * plasticOuterHeight, 0, CLHEP::twopi);
            auto sInnerSampleContainer = new G4Tubs("InnerPlasticHolder", 0, 0.5 * plasticInnerDiameter, 0.5 * plasticInnerHeight, 0, CLHEP::twopi);
            auto sSampleContainer = new G4SubtractionSolid("SampleContainer", sOuterSampleContainer, sInnerSampleContainer, 0, G4ThreeVector(0, 0, 0));

            auto lSampleContainer = new G4LogicalVolume(sSampleContainer, matPET, namePC);
            lSampleContainer->SetVisAttributes(G4Colour(0.3, 0.3, 0.5, 0.5));

            new G4PVPlacement(0,
                G4ThreeVector(0, 0, 0.5 * plasticOuterHeight + offset), //+sampleDistance
                lSampleContainer, namePC,
                lWorldVolume, false, 0, checkOverlaps);

            if (sampleHeight > 0) {
                auto sExtendedSample = new G4Tubs("extendedSampleSample", 0, 0.5 * plasticInnerDiameter, 0.5 * sampleHeight, 0, CLHEP::twopi);
                auto lExtendedSample = new G4LogicalVolume(sExtendedSample, matSample, "ExtendedSample");
                lExtendedSample->SetVisAttributes(G4Colour(1, 0.3, 0.3, 1));

                new G4PVPlacement(0,
                    G4ThreeVector(0, 0, 0.5 * sampleHeight + plasticThickness + offset), //sampleDistance + 0.5 * sampleHeight + plasticThickness + 0.175 * CLHEP::mm + offset),
                    lExtendedSample, "ExtendedSample",
                    lWorldVolume, false, 0, checkOverlaps);
            }
        }

        // Sphere approximation
        if ( 
         
            GetStringValue("extendedSampleShape") == "sphere") {
            G4double diameter = GetLengthValue("sphereDiameter");
            G4double offset = GetLengthValue("sphereOffset");
            G4String materialName = GetStringValue("sampleMaterialName");
            G4double density = GetDensityValue("sampleMaterialDensity");

            G4Material* matSample;
            if (density < 0) {
                matSample = m_nistManager->FindOrBuildMaterial(materialName);
            }
            else {
                matSample = m_nistManager->BuildMaterialWithNewDensity(materialName + "1", materialName, density);
            }

            auto sSphere = new G4Sphere("Sphere", 0, 0.5 * diameter, 0, CLHEP::twopi, 0, CLHEP::pi);
            auto lSphere = new G4LogicalVolume(sSphere, matSample, "SphereL");
            lSphere->SetVisAttributes(G4Colour(0.3, 0.3, 0.3, 0.9));

            new G4PVPlacement(0,
                G4ThreeVector(0, 0, offset + 0.5 * diameter),
                lSphere, "Sphere",
                lWorldVolume, false, 0, checkOverlaps);
        }

        // Cylinder approximation
        if (GetStringValue("extendedSampleShape") == "cylinder") {
            G4double diameter = GetLengthValue("cylinderDiameter");
            G4double length = GetLengthValue("cylinderLength");
            G4double offset = GetLengthValue("cylinderOffset");
            G4String materialName = GetStringValue("sampleMaterialName");
            G4double density = GetDensityValue("sampleMaterialDensity");

            G4Material* matSample;
            if (density < 0) {
                matSample = m_nistManager->FindOrBuildMaterial(materialName);
            }
            else {
                matSample = m_nistManager->BuildMaterialWithNewDensity(materialName + "1", materialName, density);
            }
            auto scyl = new G4Tubs("Cylinder", 0, 0.5 * diameter, 0.5 * length, 0, CLHEP::twopi);
            auto lcyl = new G4LogicalVolume(scyl, matSample, "CylinderL");
            lcyl->SetVisAttributes(G4Colour(0.3, 0.3, 0.3, 0.9));

            new G4PVPlacement(0,
                G4ThreeVector(0, 0, offset + 0.5 * length),
                lcyl, "Cylinder",
                lWorldVolume, false, 0, checkOverlaps);
        }
    }
    /// = Sample Holder Detailed =

    if (GetBoolValue("sampleHolderOn"))
    {

        const G4String name = "holder";
        const G4String nameLeg = "Leg";

        const G4double capDiameter = GetLengthValue("capDiameter");

        const G4double sampleDistance = GetLengthValue("sampleDistance");

        const G4double holderLegBaseLength = GetLengthValue("holderLegBaseLength");
        const G4double holderLegDiameter = GetLengthValue("holderLegDiameter");

        const G4double holderTopWidth = GetLengthValue("holderTopWidth");
        const G4double holderTopHeight = GetLengthValue("holderTopHeight");
        const G4double bottomthickness = GetLengthValue("bottomThicknessSampleHolder");

        const G4double holderHoleInnerDiameter = GetLengthValue("holderHoleInnerDiameter");
        const G4double holderHoleOuterDiameter = GetLengthValue("holderHoleOuterDiameter");

        const G4bool holderRoundedCorners = GetBoolValue("holderRoundedCorners");
        const G4double cornerDiameter = GetLengthValue("cornerDiameter");

        auto matCu = m_nistManager->FindOrBuildMaterial("G4_Cu");

        const G4double holderLegLength = sampleDistance + holderLegBaseLength - 0.5 * holderTopHeight;

        const G4bool PlasticCasingOn = GetBoolValue("plasticCasingOn");
        const G4bool PlasticFoilOn = GetBoolValue("plasticFoilOn");


        // Automatically switch Smaple Holder Top part based on how high the sample is situated
        if (holderLegLength > holderLegBaseLength) {

            auto sLegPole = new G4Tubs("LegPole", 0, 0.5 * holderLegDiameter, 0.5 * holderLegLength, 0, CLHEP::twopi);
            auto sCapSubtraction = new G4Tubs("capSubtraction", 0, 0.5 * capDiameter, holderLegBaseLength, 0, CLHEP::twopi);

            auto sLeg = new G4SubtractionSolid(nameLeg, sLegPole, sCapSubtraction, 0, G4ThreeVector(-0.5 * capDiameter, 0, -0.5 * holderLegLength));

            auto lLeg = new G4LogicalVolume(sLeg, matCu, nameLeg);

            lLeg->SetVisAttributes(G4Colour(0.7, 0.082, 0.263, 1));

            auto LegRot1 = new G4RotationMatrix();
            LegRot1->rotateZ(CLHEP::pi / 2);

            auto LegRot2 = new G4RotationMatrix();
            LegRot2->rotateZ(CLHEP::pi);

            auto LegRot3 = new G4RotationMatrix();
            LegRot3->rotateZ(CLHEP::pi / 2 * 3);

            new G4PVPlacement(0,
                G4ThreeVector(0.5 * capDiameter, 0, 0.5 * holderLegLength - holderLegBaseLength),
                lLeg, nameLeg,
                lWorldVolume, false, 0, checkOverlaps);

            new G4PVPlacement(LegRot1,
                G4ThreeVector(0, -0.5 * capDiameter, 0.5 * holderLegLength - holderLegBaseLength),
                lLeg, nameLeg,
                lWorldVolume, false, 0, checkOverlaps);

            new G4PVPlacement(LegRot2,
                G4ThreeVector(-0.5 * capDiameter, 0, 0.5 * holderLegLength - holderLegBaseLength),
                lLeg, nameLeg,
                lWorldVolume, false, 0, checkOverlaps);

            new G4PVPlacement(LegRot3,
                G4ThreeVector(0, 0.5 * capDiameter, 0.5 * holderLegLength - holderLegBaseLength),
                lLeg, nameLeg,
                lWorldVolume, false, 0, checkOverlaps);


            // Holder Top
                auto sQuad = new G4Box("Quader", 0.5*holderTopWidth, 0.5*holderTopWidth, 0.5*holderTopHeight);
                const G4double dickeboden = 1 * CLHEP::mm;

                const G4double zValuesCut[] = { -holderTopHeight/2.-1 * CLHEP::mm
                                              , -holderTopHeight/2. + dickeboden
                                              , -holderTopHeight/2. + dickeboden
                                              , holderTopHeight/2.+1 * CLHEP::mm
                                              };
                const G4double rInValuesCut[] = { 0
                                                , 0
                                                , 0
                                                , 0
                                                };
                const G4double rOutValuesCut[] = { 0.5*holderHoleInnerDiameter
                                                 , 0.5*holderHoleInnerDiameter
                                                 , 0.5*holderHoleOuterDiameter
                                                 , 0.5*holderHoleOuterDiameter
                                                 };

            auto sCut = new G4Polycone("holderTopCut", 0, CLHEP::twopi, 4, zValuesCut, rInValuesCut, rOutValuesCut);

            //-----------
            // rounded Corners Calculations
            G4VSolid* sRectRoundedFull;
            if (holderRoundedCorners) {
                auto sRectInit = new G4Box("Corner", 0.25 * holderTopWidth, 0.25 * holderTopWidth, 0.5 * holderTopHeight);
                auto sRect = new G4DisplacedSolid("Quadrant", sRectInit, 0, G4ThreeVector(0.25 * holderTopWidth, 0.25 * holderTopWidth, 0));
                auto sCornerCut = new G4Box("Corner", 0.5 * cornerDiameter, 0.5 * cornerDiameter, 0.6 * holderTopHeight);
                auto sRectNoCorner = new G4SubtractionSolid("cornerless", sRect, sCornerCut, 0, G4ThreeVector(0.5 * (holderTopWidth), 0.5 * (holderTopWidth), 0));
                auto sCornerRound = new G4Tubs("RoundCornerPiece", 0, 0.5 * cornerDiameter, 0.5 * holderTopHeight, 0, CLHEP::twopi);
                auto sRectRounded = new G4UnionSolid("roundedCorner", sRectNoCorner, sCornerRound, 0, G4ThreeVector(0.5 * (holderTopWidth - cornerDiameter), 0.5 * (holderTopWidth - cornerDiameter), 0));
                auto sRectRoundedHalf = new G4UnionSolid("roundedHalf", sRectRounded, sRectRounded, LegRot1, G4ThreeVector());
                sRectRoundedFull = new G4UnionSolid("roundedHalf", sRectRoundedHalf, sRectRoundedHalf, LegRot2, G4ThreeVector());
            }
            //-----------

            G4VSolid* sHolder;
            if (holderRoundedCorners) { sHolder = new G4SubtractionSolid(name, sRectRoundedFull, sCut, 0, G4ThreeVector(0, 0, 0)); }
            else { sHolder = new G4SubtractionSolid(name, sQuad, sCut, 0, G4ThreeVector(0, 0, 0)); }
            //auto sHolder = new G4SubtractionSolid(name, sQuad, sCut, 0, G4ThreeVector(0, 0, 0));

            auto lHolder = new G4LogicalVolume(sHolder, matCu, name);

            //if(holderRoundedCorners){lHolder = new G4LogicalVolume(sRectRoundedFull, matCu, name);}

            auto holderRot = new G4RotationMatrix();
            holderRot->rotateZ(CLHEP::pi / 4);

            lHolder->SetVisAttributes(G4Colour(0.788, 0.082, 0.263, 1));

            new G4PVPlacement(holderRot,
                G4ThreeVector(0, 0, sampleDistance),
                lHolder, name,
                lWorldVolume, false, 0, checkOverlaps);
        }

        else {
		    

            auto LegRot1 = new G4RotationMatrix();
            LegRot1->rotateZ(CLHEP::pi / 2);

            auto LegRot2 = new G4RotationMatrix();
            LegRot2->rotateZ(CLHEP::pi);

            G4cout<<"Value 'bottomThicknessSampleHolder' must be below 5 mm "<<G4endl;
                
            const G4double holderHeight = 1.5 * holderTopHeight;    //added height for base of holder around detector cap
            
            auto sQuad = new G4Box("Quader", 0.5 * holderTopWidth, 0.5 * holderTopWidth, 0.5 * holderHeight);

              const G4double zValuesCut[] = { -0.5 * holderHeight -1* CLHEP::mm
                                          , -0.5 * holderHeight + holderHeight/3.
                                          , -0.5 * holderHeight + holderHeight/3.
                                          , -0.5 * holderHeight + holderHeight/3. + bottomthickness
                                          , -0.5 * holderHeight + holderHeight/3. + bottomthickness
                                          , +0.5 * holderHeight +1* CLHEP::mm
            };
            const G4double rInValuesCut[] = { 0
                                            , 0
                                            , 0
                                            , 0
                                            , 0
                                            , 0
            };
            const G4double rOutValuesCut[] = { 0.5 * capDiameter
                                             , 0.5 * capDiameter
                                             , 0.5 * holderHoleInnerDiameter
                                             , 0.5 * holderHoleInnerDiameter
                                             , 0.5 * holderHoleOuterDiameter
                                             , 0.5 * holderHoleOuterDiameter
            };

            auto sCut = new G4Polycone("holderTopCut", 0, CLHEP::twopi, 6, zValuesCut, rInValuesCut, rOutValuesCut);
            
             //-----------
            // rounded Corners Calculations
            G4VSolid* sRectRoundedFull;
            if (holderRoundedCorners) {
                auto sRectInit = new G4Box("Corner", 0.25 * holderTopWidth, 0.25 * holderTopWidth, 0.5 * holderHeight);
                auto sRect = new G4DisplacedSolid("Quadrant", sRectInit, 0, G4ThreeVector(0.25 * holderTopWidth, 0.25 * holderTopWidth, 0));
                auto sCornerCut = new G4Box("Corner", 0.5 * cornerDiameter, 0.5 * cornerDiameter, 0.6 * holderHeight);
                auto sRectNoCorner = new G4SubtractionSolid("cornerless", sRect, sCornerCut, 0, G4ThreeVector(0.5 * (holderTopWidth), 0.5 * (holderTopWidth), 0));
                auto sCornerRound = new G4Tubs("RoundCornerPiece", 0, 0.5 * cornerDiameter, 0.5 * holderHeight, 0, CLHEP::twopi);
                auto sRectRounded = new G4UnionSolid("roundedCorner", sRectNoCorner, sCornerRound, 0, G4ThreeVector(0.5 * (holderTopWidth - cornerDiameter), 0.5 * (holderTopWidth - cornerDiameter), 0));
                auto sRectRoundedHalf = new G4UnionSolid("roundedHalf", sRectRounded, sRectRounded, LegRot1, G4ThreeVector());
                sRectRoundedFull = new G4UnionSolid("roundedHalf", sRectRoundedHalf, sRectRoundedHalf, LegRot2, G4ThreeVector());
            }
            //-----------

            G4VSolid* sHolder;
            if (holderRoundedCorners) { sHolder = new G4SubtractionSolid(name, sRectRoundedFull, sCut, 0, G4ThreeVector(0, 0, +1/6* holderTopHeight)); }
            else { sHolder = new G4SubtractionSolid(name, sQuad, sCut, 0, G4ThreeVector(0, 0, +1/6* holderTopHeight)); }
            //auto sHolder = new G4SubtractionSolid(name, sQuad, sCut, 0, G4ThreeVector(0, 0, 0));






            //auto sHolder = new G4SubtractionSolid(name, sQuad, sCut, 0, G4ThreeVector(0, 0, 0));

            auto lHolder = new G4LogicalVolume(sHolder, matCu, name);

            lHolder->SetVisAttributes(G4Colour(0.788, 0.082, 0.263, 1));

            new G4PVPlacement(0,
                G4ThreeVector(0, 0, 0.5* holderHeight - holderHeight/3.),
                lHolder, name,
                lWorldVolume, false, 0, checkOverlaps);
        }


        // = Sample Plastic Casing


        if (PlasticCasingOn) {
            const G4String namePC = "plasticCasing";
            auto matPET = m_nistManager->FindOrBuildMaterial("G4_POLYETHYLENE");
            auto matAl = m_nistManager->FindOrBuildMaterial("G4_Al");


            auto sPlasticCasing = new G4Tubs(
                namePC,
                0,
                0.5 * holderHoleOuterDiameter - 4.5 * CLHEP::mm,  // Adjusted outer radius
                0.12 * CLHEP::mm,
                0,
                CLHEP::twopi
            );

            auto lPlasticCasing = new G4LogicalVolume(sPlasticCasing, matPET, namePC);

            lPlasticCasing->SetVisAttributes(G4Colour(0.3, 0.3, 0.5, 0.78));

            new G4PVPlacement(0,
                G4ThreeVector(0, 0, sampleDistance),
                lPlasticCasing, namePC,
                lWorldVolume, false, 0, checkOverlaps);

            auto sAlCasing = new G4Tubs(namePC, 0.5 * holderHoleOuterDiameter - 4.5 * CLHEP::mm, 0.5 * holderHoleOuterDiameter, 0.5 * 3 * CLHEP::mm, 0, CLHEP::twopi);

            auto lAlCasing = new G4LogicalVolume(sAlCasing, matAl, namePC);

            lAlCasing->SetVisAttributes(G4Colour(0.9, 0.9, 0.9, 1));

            new G4PVPlacement(0,
                G4ThreeVector(0, 0, sampleDistance),
                lAlCasing, namePC,
                lWorldVolume, false, 0, checkOverlaps);

        }

        if (PlasticFoilOn) {
            const G4String namePC = "plasticFoil";
            auto matPET = m_nistManager->FindOrBuildMaterial("G4_POLYETHYLENE");

            auto sPlasticFoil = new G4Tubs(namePC, 0, 0.5 * 20.2 * CLHEP::mm, 0.005 * CLHEP::mm, 0, CLHEP::twopi);

            auto lPlasticFoil = new G4LogicalVolume(sPlasticFoil, matPET, namePC);

            lPlasticFoil->SetVisAttributes(G4Colour(0.6, 0.7, 0.5, 0.78));

            new G4PVPlacement(0,
                G4ThreeVector(0, 0, 0.005 * CLHEP::mm),
                lPlasticFoil, namePC,
                lWorldVolume, false, 0, checkOverlaps);

        }

    }

}


