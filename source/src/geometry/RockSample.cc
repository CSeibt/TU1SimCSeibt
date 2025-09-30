#include "geometry/RockSample.hh"


#include "G4Box.hh"
#include "G4SubtractionSolid.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4RunManager.hh"
#include "G4PVPlacement.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4Transform3D.hh"
#include "geometry/Materials.hh"

using CLHEP::cm;
using CLHEP::g;
using CLHEP::cm3;
using CLHEP::perCent;
using CLHEP::degree;

RockSample::RockSample()
    : GeometryObject("RockSample"), fRockSamplePhys(nullptr)
{
    NewStringValue("STLfileName", "Path to .stl file to be loaded.", "");
    NewLengthValue("distanceFromCapRock", "Distance of Rock sample from cap.", 0 * CLHEP::cm); //0.527 * CLHEP::cm für meteorit2, -0.01 * CLHEP::cm aubrit, 0.791 cm für Drelow
    NewDoubleValue("Rotx", "Rotation of the Scan x axis", 0 *CLHEP::degree);//1.2 *CLHEP::degree für meteorit2,  7 *CLHEP::degree aubrit, -24.9 degree für Drelow
    NewDoubleValue("Roty", "Rotation of the Scan y axis", 0 *CLHEP::degree);//-3*CLHEP::degree für meteorit2,  90 *CLHEP::degree aubrit, 100.9 degree für Drelow
    NewDoubleValue("Rotz", "Rotation of the Scan z axis", 0 *CLHEP::degree);//90 *CLHEP::degree für meteorit2,  90 *CLHEP::degree aubrit, 0 degree für Drelow
    // NewLengthValue("sampleDiameter", "Diameter of the cylindrical samnple.", 1. * CLHEP::cm);
    // NewLengthValue("sampleLength", "Length of the cylindrical sample.", 1. * CLHEP::cm);
  //  NewLengthValue("sampleDistance", "Distance of the cylindrical sample from the detector surface.",1  * CLHEP::cm);

    //NewDensityValue("sampleDensity", "Density of the sample.", 3.19 * CLHEP::g/CLHEP::cm3); //3.19 for Haag
}

RockSample::~RockSample()
{
    delete fMessenger;
}

// Implementiere die DefineMaterials-Methode
void RockSample::DefineMaterials() {
    Materials::DefineMaterials();  // Aufruf der statischen Methode in der Materials-Klasse
}

void RockSample::Construct(G4LogicalVolume *lWorldVolume, G4bool checkOverlaps)
{
    const G4String name = "sample";
    G4cout << "Loading STL file: " << GetStringValue("STLfileName") << G4endl;

    auto rock_sample = CADMesh::TessellatedMesh::FromSTL(GetStringValue("STLfileName"));

    if (!rock_sample) {
        G4cerr << "Failed to load STL file: " << GetStringValue("STLfileName") << G4endl;
        return;
    }
   //G4double scaleFactor = 1000.0;  // Aubrit Scan muss skaliert werden
   // rock_sample->SetScale(scaleFactor);

    
    const G4double rotx = GetDoubleValue("Rotx");
    const G4double roty = GetDoubleValue("Roty");
    const G4double rotz = GetDoubleValue("Rotz");

    G4RotationMatrix* rotMatrix = new G4RotationMatrix();
    rotMatrix->rotateX(rotx);
    rotMatrix->rotateY(roty);
    rotMatrix->rotateZ(rotz);

    G4Material* matSample = Materials::GetMaterial("meteorit2_comp");  //"meteorit2_comp" or "enstatit_comp" or "drelow_comp"

    
    auto rock_logical= new G4LogicalVolume( rock_sample->GetSolid(), matSample, name);
    
    auto tessSolid = dynamic_cast<G4TessellatedSolid*>(rock_sample->GetSolid());
	if (tessSolid) {
		tessSolid->SetSolidClosed(true);
	} else {
		G4cerr << "Warning: Solid is not a G4TessellatedSolid!" << G4endl;
	}

    rock_logical->SetVisAttributes(G4Colour::Cyan());

    G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0, 0.5)); // Cyan mit 50% Transparenz
    visAttributes->SetVisibility(true);
    rock_logical->SetVisAttributes(visAttributes);

    G4cout << "Set visibility attributes for rock sample" << G4endl;

    const G4double distanceFromCapRock = GetLengthValue("distanceFromCapRock");  

    /*new G4PVPlacement(m_noRotation,
                      G4ThreeVector(0, 0, sampleDistance + 0.5 * sampleLength),
                      lSample, name,
                      lWorldVolume, false, 0, checkOverlaps);*/
    

    fRockSamplePhys = new G4PVPlacement(rotMatrix,  // Verwendung der Rotationsmatrix
        G4ThreeVector(0.5 * CLHEP::cm, -0.5 * CLHEP::cm, distanceFromCapRock), //(0.5 * CLHEP::cm, -0.5 * CLHEP::cm, distanceFromCapRock) für meteorit 2 and (0 * CLHEP::cm, 0 * CLHEP::cm, distanceFromCapRock) for aubrite
        rock_logical,
        name,
        lWorldVolume,
        false,
        0,
        checkOverlaps);
}

/*void RockSample::SetSamplePosition(G4ThreeVector newPos)
{
    if (fRockSamplePhys) {
        fRockSamplePhys->SetTranslation(newPos);
        G4RunManager::GetRunManager()->GeometryHasBeenModified();
    }
}*/



/*void RockSample::DefineMaterials()
{
    const G4double sampleDensity = GetDensityValue("sampleDensity");

    // Elements definition
    G4Isotope* Mg24 = new G4Isotope("Mg24", 12, 24); //Mg
    G4Isotope* Mg25 = new G4Isotope("Mg25", 12, 25); //Mg
    G4Isotope* Mg26 = new G4Isotope("Mg26", 12, 26); //Mg
    G4Element* Mg = new G4Element("TS_Mg_of_Enstatit", "Mg", 3);
    Mg->AddIsotope(Mg24, 78.965 * CLHEP::perCent);
    Mg->AddIsotope(Mg25, 10.01 * CLHEP::perCent);
    Mg->AddIsotope(Mg26, 11.025 * CLHEP::perCent);
    
    G4Isotope* Na22 = new G4Isotope("Na22", 11, 22); //Na
    G4Element* Na = new G4Element("TS_Na_of_Enstatit", "Na", 1);
    Na->AddIsotope(Na22, 100 * CLHEP::perCent);
    
    G4Isotope* Al27 = new G4Isotope("Al27", 13, 27); //Al
    G4Element* Al = new G4Element("TS_Al_of_Enstatit", "Al", 1);
    Al->AddIsotope(Al27, 100 * CLHEP::perCent);

    G4Isotope* Si28 = new G4Isotope("Si28", 14, 28); //Si
    G4Isotope* Si29 = new G4Isotope("Si29", 14, 29); //Si
    G4Isotope* Si30 = new G4Isotope("Si30", 14, 30); //Si
    G4Element* Si = new G4Element("TS_Si_of_Enstatit", "Si", 3);
    Si->AddIsotope(Si28, 92.2545 * CLHEP::perCent);
    Si->AddIsotope(Si29, 4.672 * CLHEP::perCent);
    Si->AddIsotope(Si30, 3.0735 * CLHEP::perCent);

    G4Isotope* O16 = new G4Isotope("O16", 8, 16); //O
    G4Isotope* O17 = new G4Isotope("O17", 8, 17); //O
    G4Isotope* O18 = new G4Isotope("O18", 8, 18); //O
    G4Element* O = new G4Element("TS_O_of_K2SO4", "O", 3);
    O->AddIsotope(O16, 99.757 * CLHEP::perCent);
    O->AddIsotope(O17, 0.0385 * CLHEP::perCent);
    O->AddIsotope(O18, 0.2045 * CLHEP::perCent);
    
    G4Isotope* Ni58 = new G4Isotope("Ni58", 28, 58); 
    G4Isotope* Ni60 = new G4Isotope("Ni60", 28, 60); 
    G4Isotope* Ni61 = new G4Isotope("Ni61", 28, 61); 
    G4Isotope* Ni62 = new G4Isotope("Ni62", 28, 62); 
    G4Isotope* Ni64 = new G4Isotope("Ni64", 28, 64); 
    G4Element* Ni = new G4Element("TS_Ni", "Ni", 5);
    Ni->AddIsotope(Ni58, 68.077 * CLHEP::perCent);
    Ni->AddIsotope(Ni60, 26.233 * CLHEP::perCent);
    Ni->AddIsotope(Ni61, 1.14 * CLHEP::perCent);
    Ni->AddIsotope(Ni62, 3.634 * CLHEP::perCent);
    Ni->AddIsotope(Ni64, 0.926 * CLHEP::perCent);
    
    G4Isotope* Fe54 = new G4Isotope("Fe54", 26, 54); //Fe
    G4Isotope* Fe56 = new G4Isotope("Fe56", 26, 56); //Fe
    G4Isotope* Fe57 = new G4Isotope("Fe57", 26, 57); //Fe
    G4Isotope* Fe58 = new G4Isotope("Fe58", 26, 58); //Fe 
    G4Element* Fe = new G4Element("TS_Fe", "Fe", 4);
    Fe->AddIsotope(Fe54, 5.8*CLHEP::perCent);
    Fe->AddIsotope(Fe56, 91.72*CLHEP::perCent);
    Fe->AddIsotope(Fe57, 2.2*CLHEP::perCent);
    Fe->AddIsotope(Fe58, 0.28*CLHEP::perCent);
    
    G4Isotope* S32 = new G4Isotope("S32", 16, 32); //Fe
    G4Isotope* S33 = new G4Isotope("S33", 16, 33); //Fe
    G4Isotope* S34 = new G4Isotope("S34", 16, 34); //Fe
    G4Isotope* S36 = new G4Isotope("S36", 16, 36); //Fe 
    G4Element* S = new G4Element("TS_S", "S", 4);
    S->AddIsotope(S32, 95.02*CLHEP::perCent);
    S->AddIsotope(S33, 0.75*CLHEP::perCent);
    S->AddIsotope(S34, 4.21*CLHEP::perCent);
    S->AddIsotope(S36, 0.02*CLHEP::perCent);
    
    G4Isotope* Ca40 = new G4Isotope("Ca40", 20, 40); //Ca
    G4Isotope* Ca42 = new G4Isotope("Ca42", 20, 42); //Ca
    G4Isotope* Ca43 = new G4Isotope("Ca43", 20, 43); //Ca
    G4Isotope* Ca44 = new G4Isotope("Ca44", 20, 44); //Ca 
    G4Isotope* Ca46 = new G4Isotope("Ca46", 20, 46); //Ca 
    G4Isotope* Ca48 = new G4Isotope("Ca48", 20, 48); //Ca 
    G4Element* Ca = new G4Element("TS_Ca", "Ca", 6);
    Ca->AddIsotope(Ca40, 96.941*CLHEP::perCent);
    Ca->AddIsotope(Ca42, 0.647*CLHEP::perCent);
    Ca->AddIsotope(Ca43, 0.135*CLHEP::perCent);
    Ca->AddIsotope(Ca44, 2.086*CLHEP::perCent);
    Ca->AddIsotope(Ca46, 0.004*CLHEP::perCent);
    Ca->AddIsotope(Ca48, 0.187*CLHEP::perCent);
    
    G4Isotope* P31 = new G4Isotope("P31", 15, 31); 
    G4Element* P = new G4Element("TS_P", "P", 1);
    P->AddIsotope(P31, 100*CLHEP::perCent);
    
    G4Isotope* K39 = new G4Isotope("K39", 19, 39); //K
    G4Isotope* K41 = new G4Isotope("K41", 19, 41); //K
    G4Element* K = new G4Element("TS_K_of_K2SO4", "K", 2);
    K->AddIsotope(K39, 93.258 * CLHEP::perCent);
    K->AddIsotope(K41, 6.742 * CLHEP::perCent);

    G4Isotope* Ti46 = new G4Isotope("Ti46", 22, 46); 
    G4Isotope* Ti47 = new G4Isotope("Ti47", 22, 47); 
    G4Isotope* Ti48 = new G4Isotope("Ti48", 22, 48); 
    G4Isotope* Ti49 = new G4Isotope("Ti49", 22, 49);
    G4Isotope* Ti50 = new G4Isotope("Ti50", 22, 50);  
    G4Element* Ti = new G4Element("TS_Ti", "Ti", 5);
    Ti->AddIsotope(Ti46, 8.0 *CLHEP::perCent);
    Ti->AddIsotope(Ti47, 7.3 *CLHEP::perCent);
    Ti->AddIsotope(Ti48, 73.8 *CLHEP::perCent);
    Ti->AddIsotope(Ti49, 5.5 *CLHEP::perCent);
    Ti->AddIsotope(Ti50, 5.4 *CLHEP::perCent);
    
    G4Isotope* Cr50 = new G4Isotope("Cr50", 24, 50); 
    G4Isotope* Cr52 = new G4Isotope("Cr52", 24, 52); 
    G4Isotope* Cr53 = new G4Isotope("Cr53", 24, 53);
    G4Isotope* Cr54 = new G4Isotope("Cr54", 24, 54);
    G4Element* Cr = new G4Element("TS_Cr", "Cr", 4);
    Cr->AddIsotope(Cr50, 4.345 *CLHEP::perCent);
    Cr->AddIsotope(Cr52, 83.789 *CLHEP::perCent);
    Cr->AddIsotope(Cr53, 9.501 *CLHEP::perCent);
    Cr->AddIsotope(Cr54, 2.365 *CLHEP::perCent);

    G4Isotope* Mn55 = new G4Isotope("Mn55", 25, 55); 
    G4Element* Mn = new G4Element("TS_Mn", "Mn", 1);
    Mn->AddIsotope(Mn55, 100*CLHEP::perCent);
    
    
    /////////////////// Hier wird das Aubrit material gebastelt ///////////////////
    G4int nCompenstatit;
    G4Material* Enstatit = new G4Material("enstatit", 3.2 * CLHEP::g / CLHEP::cm3, nCompenstatit = 3,
        kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    Enstatit->AddElement(Mg, 1);
    Enstatit->AddElement(Si, 1);
    Enstatit->AddElement(O, 3);

     G4int nCompalbit;
    G4Material* Albit = new G4Material("albit", 2.62 * CLHEP::g / CLHEP::cm3, nCompalbit = 4,
        kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    Albit->AddElement(Na, 1);
    Albit->AddElement(Al, 1);
    Albit->AddElement(Si, 3);
    Albit->AddElement(O, 8);
    
    G4int nCompforsterit;
    G4Material* Forsterit = new G4Material("forsterit", 3.275 * CLHEP::g / CLHEP::cm3, nCompforsterit = 3,
        kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    Forsterit->AddElement(Mg, 2);
    Forsterit->AddElement(Si, 1);
    Forsterit->AddElement(O, 4);

    // Gewichtsanteil von MgO und SiO2
    G4double weightFraction_Enstatit = 0.808; // 78.8% Enstatit Volumenanteil und 80.8% Massenanteil
    G4double weightFraction_Albit = 0.130; // 15.5% Albit Volumenanteil und 13.0% Massenanteil
    G4double weightFractio_Forsterit = 0.062; // 5.7% Forsterit Volumenanteil und 6.2% Massenanteil
    

    // Material für die Zusammensetzung von Enstatit definieren
    G4Material* enstatit_comp = new G4Material("enstatit_comp", 3.2 * CLHEP::g / CLHEP::cm3, 3, kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);

    // Teilmaterialien hinzufügen
    enstatit_comp->AddMaterial(Enstatit, weightFraction_Enstatit);
    enstatit_comp->AddMaterial(Albit, weightFraction_Albit);
    enstatit_comp->AddMaterial(Forsterit, weightFractio_Forsterit);


	///////////////Hier wird das Material für Meteorit 2 gebastelt //////////////////
	
	G4int nCompSiO2;
    G4Material* SiO2 = new G4Material("SiO2", 2.65 * CLHEP::g / CLHEP::cm3, nCompforsterit = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    SiO2->AddElement(Si, 1);
    SiO2->AddElement(O, 2);
    
    G4int nCompAl2O3;
    G4Material* Al2O3 = new G4Material("Al2O3", 3.99 * CLHEP::g / CLHEP::cm3, nCompforsterit = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    Al2O3->AddElement(Al, 2);
    Al2O3->AddElement(O, 3);
    
    G4int nCompFeO;
    G4Material* FeO = new G4Material("FeO", 5.74 * CLHEP::g / CLHEP::cm3, nCompforsterit = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    FeO->AddElement(Fe, 1);
    FeO->AddElement(O, 1);
    
    G4int nCompFe2O3;
    G4Material* Fe2O3 = new G4Material("Fe2O3", 5.24 * CLHEP::g / CLHEP::cm3, nCompforsterit = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    Fe2O3->AddElement(Fe, 2);
    Fe2O3->AddElement(O, 3);

	G4int nCompMgO;
    G4Material* MgO = new G4Material("MgO", 3.58 * CLHEP::g / CLHEP::cm3, nCompforsterit = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    MgO->AddElement(Mg, 1);
    MgO->AddElement(O, 1);
    
    G4int nCompNa2O;
    G4Material* Na2O = new G4Material("Na2O", 2.27 * CLHEP::g / CLHEP::cm3, nCompforsterit = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    Na2O->AddElement(Na, 2);
    Na2O->AddElement(O, 1);
    
    G4int nCompNiO;
    G4Material* NiO = new G4Material("NiO", 6.67 * CLHEP::g / CLHEP::cm3, nCompforsterit = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    NiO->AddElement(Ni, 1);
    NiO->AddElement(O, 1);
    
    G4int nCompSO3;
    G4Material* SO3 = new G4Material("SO3", 1.92 * CLHEP::g / CLHEP::cm3, nCompforsterit = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    SO3->AddElement(S, 1);
    SO3->AddElement(O, 3);
    
    G4int nCompCaO;
    G4Material* CaO = new G4Material("CaO", 3.34 * CLHEP::g / CLHEP::cm3, nCompforsterit = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    CaO->AddElement(Ca, 1);
    CaO->AddElement(O, 1);
    
    G4int nCompP2O5;
    G4Material* P2O5 = new G4Material("P2O5", 2.30 * CLHEP::g / CLHEP::cm3, nCompforsterit = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    P2O5->AddElement(P, 2);
    P2O5->AddElement(O, 5);
    
    G4int nCompK2O;
    G4Material* K2O = new G4Material("K2O", 2.35 * CLHEP::g / CLHEP::cm3, nCompforsterit = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    K2O->AddElement(K, 2);
    K2O->AddElement(O, 1);
    
    G4int nCompTiO2;
    G4Material* TiO2 = new G4Material("TiO2", 4.26 * CLHEP::g / CLHEP::cm3, nCompforsterit = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    TiO2->AddElement(Ti, 1);
    TiO2->AddElement(O, 2);
    
    G4int nCompCr2O3;
    G4Material* Cr2O3 = new G4Material("Cr2O3", 5.22 * CLHEP::g / CLHEP::cm3, nCompforsterit = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    Cr2O3->AddElement(Cr, 2);
    Cr2O3->AddElement(O, 3);
    
    G4int nCompMnO;
    G4Material* MnO = new G4Material("MnO", 5.37 * CLHEP::g / CLHEP::cm3, nCompforsterit = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    MnO->AddElement(Mn, 1);
    MnO->AddElement(O, 1);

	 // Gewichtsanteil von MgO und SiO2
    G4double weightFraction_SiO2 = 0.3981; 
    G4double weightFraction_Al2O3 = 0.0257; 
    G4double weightFractio_FeO = 0.2210; 
    G4double weightFractio_Fe2O3 = 0.0246; 
    G4double weightFractio_MgO = 0.2292; 
    G4double weightFractio_Na2O = 0.0087; 
    G4double weightFractio_NiO = 0.0099; 
    G4double weightFractio_P2O5 = 0.0024; 
    G4double weightFractio_SO3 = 0.0516; 
    G4double weightFractio_K2O = 0.0013; 
    G4double weightFractio_CaO = 0.0186; 
    G4double weightFractio_TiO2 = 0.0011; 
    G4double weightFractio_Cr2O3 = 0.0044; 
    G4double weightFractio_MnO = 0.0034; 
    
    // Material für die Zusammensetzung von Enstatit definieren
    G4Material* meteorit2_comp = new G4Material("meteorit2_comp", 3.19 * CLHEP::g / CLHEP::cm3, 14, kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);

    // Teilmaterialien hinzufügen
    meteorit2_comp->AddMaterial(SiO2, weightFraction_SiO2);
    meteorit2_comp->AddMaterial(Al2O3, weightFraction_Al2O3);
    meteorit2_comp->AddMaterial(FeO, weightFractio_FeO);
    meteorit2_comp->AddMaterial(Fe2O3, weightFractio_Fe2O3);
    meteorit2_comp->AddMaterial(MgO, weightFractio_MgO);
    meteorit2_comp->AddMaterial(Na2O, weightFractio_Na2O);
    meteorit2_comp->AddMaterial(NiO, weightFractio_NiO);
    meteorit2_comp->AddMaterial(P2O5, weightFractio_P2O5);
    meteorit2_comp->AddMaterial(SO3, weightFractio_SO3);
    meteorit2_comp->AddMaterial(K2O, weightFractio_K2O);
    meteorit2_comp->AddMaterial(CaO, weightFractio_CaO);
    meteorit2_comp->AddMaterial(TiO2, weightFractio_TiO2);
    meteorit2_comp->AddMaterial(Cr2O3, weightFractio_Cr2O3);
    meteorit2_comp->AddMaterial(MnO, weightFractio_MnO);
    
    
   /* // Material SiO2 definieren
    G4int nCompSiO2 = 2;
    G4Material* SiO2 = new G4Material("SiO2", 2.65 * CLHEP::g / CLHEP::cm3, nCompSiO2,
        kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    SiO2->AddElement(Si, 1);
    SiO2->AddElement(O, 2);

    // Material MgO definieren
    G4int nCompMg = 2;
    G4Material* MgO = new G4Material("MgO", 3.58 * CLHEP::g / CLHEP::cm3, nCompMg,
        kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    MgO->AddElement(Mg, 1);
    MgO->AddElement(O, 1);




    // Gewichtsanteil von MgO und SiO2
    G4double weightFractionMgO = 0.606; // 60.6% Magnesiumoxid
    G4double weightFractionSiO2 = 0.394; // 39.4% Siliziumdioxid

    // Gesamtdichte berechnen
    // G4double totalDensity = weightFractionMgO * MgO->GetDensity() + weightFractionSiO2 * SiO2->GetDensity();

    // Material für die Zusammensetzung von Enstatit definieren
    G4Material* enstatit_comp = new G4Material("enstatit_comp", 3.2 * CLHEP::g / CLHEP::cm3, 2, kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);

    // Teilmaterialien hinzufügen
    enstatit_comp->AddMaterial(MgO, weightFractionMgO);
    enstatit_comp->AddMaterial(SiO2, weightFractionSiO2); 

    // Granodiorite
    auto elSi = m_nistManager->FindOrBuildElement("Si");
    auto elTi = m_nistManager->FindOrBuildElement("Ti");
    auto elAl = m_nistManager->FindOrBuildElement("Al");
    auto elFe = m_nistManager->FindOrBuildElement("Fe");
    auto elMn = m_nistManager->FindOrBuildElement("Mn");
    auto elMg = m_nistManager->FindOrBuildElement("Mg");
    auto elCa = m_nistManager->FindOrBuildElement("Ca");
    auto elNa = m_nistManager->FindOrBuildElement("Na");
    auto elK = m_nistManager->FindOrBuildElement("K");
    auto elP = m_nistManager->FindOrBuildElement("P");
    auto elO = m_nistManager->FindOrBuildElement("O");

    const double dummyDensity = 1.0 * CLHEP::g / CLHEP::cm3;
    // we don't use the following materials directly, hence the density doesn't matter

    auto matSiO2 = new G4Material("SiO2", dummyDensity, 2, kStateSolid);
    matSiO2->AddElement(elSi, 1);
    matSiO2->AddElement(elO,  2);

    auto matTiO2 = new G4Material("TiO2", dummyDensity, 2, kStateSolid);
    matTiO2->AddElement(elTi, 1);
    matTiO2->AddElement(elO,  2);

    auto matAl2O3 = new G4Material("Al2O3", dummyDensity, 2, kStateSolid);
    matAl2O3->AddElement(elAl, 2);
    matAl2O3->AddElement(elO,  3);

    auto matFe2O3 = new G4Material("Fe2O3", dummyDensity, 2, kStateSolid);
    matFe2O3->AddElement(elFe, 2);
    matFe2O3->AddElement(elO,  3);

    auto matFeO = new G4Material("FeO", dummyDensity, 2, kStateSolid);
    matFeO->AddElement(elFe, 1);
    matFeO->AddElement(elO,  1);

    auto matMnO = new G4Material("MnO", dummyDensity, 2, kStateSolid);
    matMnO->AddElement(elMn, 1);
    matMnO->AddElement(elO,  1);

    auto matMgO = new G4Material("MgO", dummyDensity, 2, kStateSolid);
    matMgO->AddElement(elMg, 1);
    matMgO->AddElement(elO,  1);

    auto matCaO = new G4Material("CaO", dummyDensity, 2, kStateSolid);
    matCaO->AddElement(elCa, 1);
    matCaO->AddElement(elO,  1);

    auto matNa2O = new G4Material("Na2O", dummyDensity, 2, kStateSolid);
    matNa2O->AddElement(elNa, 2);
    matNa2O->AddElement(elO,  1);

    auto matK2O = new G4Material("K2O", dummyDensity, 2, kStateSolid);
    matK2O->AddElement(elK, 2);
    matK2O->AddElement(elO, 1);

    auto matP2O5 = new G4Material("P2O5", dummyDensity, 2, kStateSolid);
    matP2O5->AddElement(elP, 2);
    matP2O5->AddElement(elO, 5);

    auto matGranodiorite = new G4Material("Granodiorite", sampleDensity, 11, kStateSolid);
    matGranodiorite->AddMaterial(matSiO2,  .6691);
    matGranodiorite->AddMaterial(matTiO2,  .0055);
    matGranodiorite->AddMaterial(matAl2O3, .1592);
    matGranodiorite->AddMaterial(matFe2O3, .0140);
    matGranodiorite->AddMaterial(matFeO,   .0276);
    matGranodiorite->AddMaterial(matMnO,   .0008);
    matGranodiorite->AddMaterial(matMgO,   .0176);
    matGranodiorite->AddMaterial(matCaO,   .0388);
    matGranodiorite->AddMaterial(matNa2O,  .038);
    matGranodiorite->AddMaterial(matK2O,   .0276);
    matGranodiorite->AddMaterial(matP2O5,  .0018);



    auto matMonzonite = new G4Material("Monzonite", sampleDensity, 11, kStateSolid);
    //Composition
    matMonzonite->AddMaterial(matSiO2,	61.86*perCent);
    matMonzonite->AddMaterial(matTiO2,	0.20*perCent);
    matMonzonite->AddMaterial(matAl2O3,	16.37*perCent);
    matMonzonite->AddMaterial(matFe2O3,	2.38*perCent);
    matMonzonite->AddMaterial(matFeO,		1.71*perCent);
    matMonzonite->AddMaterial(matMnO,		0.10*perCent);
    matMonzonite->AddMaterial(matMgO,		2.30*perCent);
    matMonzonite->AddMaterial(matCaO,		4.85*perCent);
    matMonzonite->AddMaterial(matNa2O,		4.69*perCent); // in original simulation this was NaO - double-check for the future
    matMonzonite->AddMaterial(matK2O,	5.05*perCent);
    matMonzonite->AddMaterial(matP2O5,	0.49*perCent); 
} */

void RockSample::SetNewValue(G4UIcommand* command, G4String newValue)
{
    GeometryObject::SetNewValue(command, newValue);
    if (command->GetCommandName() == "STLfileName") {
        fSTLfileName = newValue;
    }
}

void RockSample::SomeFunction()
{
    if (fRockSamplePhys) {
        // Zugriff auf das physische Volumen
        G4cout << "Physical volume position: " << fRockSamplePhys->GetObjectTranslation() << G4endl;
    }
    else {
        G4cerr << "Physical volume not initialized!" << G4endl;
    }
}
