#include "geometry/Materials.hh"
#include "G4SystemOfUnits.hh"
#include "G4Element.hh"

// Initialisierung der statischen Variablen
std::map<G4String, G4Material*> Materials::materialTable;  // Definition der statischen Map

void Materials::DefineMaterials() {
    static bool alreadyDefined = false;
    if (alreadyDefined) return;  // Verhindert erneutes Ausführen
    alreadyDefined = true;

    // Elements definition

    G4Isotope* H1 = new G4Isotope("H1", 1, 1); //H
    G4Element* H = new G4Element("Hydrogen", "H", 1);
    H->AddIsotope(H1, 100 * CLHEP::perCent);

    G4Isotope* F19 = new G4Isotope("F19", 9, 19); //F
    G4Element* F = new G4Element("teflon_F", "F", 1);
    F->AddIsotope(F19, 100 * CLHEP::perCent);
    
    G4Isotope* C12 = new G4Isotope("C12", 6, 12); //C
    G4Isotope* C13 = new G4Isotope("C13", 6, 13); //C
    G4Element* C = new G4Element("teflon_C", "C", 2);
    C->AddIsotope(C12, 98.9 * CLHEP::perCent);
    C->AddIsotope(C13, 1.1 * CLHEP::perCent);
    
    G4Isotope* Cu63 = new G4Isotope("Cu63", 29, 63); //C
    G4Isotope* Cu65 = new G4Isotope("Cu65", 29, 65); //C
    G4Element* Cu = new G4Element("Brass", "Cu", 2);
    Cu->AddIsotope(Cu63, 69.15 * CLHEP::perCent);
    Cu->AddIsotope(Cu65, 30.85* CLHEP::perCent);
    
    G4Isotope* Zn64 = new G4Isotope("Zn64", 30, 64); //C
    G4Isotope* Zn66 = new G4Isotope("Zn66", 30, 66); //C
    G4Isotope* Zn67 = new G4Isotope("Zn67", 30, 67); //C
    G4Isotope* Zn68 = new G4Isotope("Zn68", 30, 68); //C
    G4Isotope* Zn70 = new G4Isotope("Zn70", 30, 70); //C
    G4Element* Zn = new G4Element("Brass", "Zn", 5);
    Zn->AddIsotope(Zn64, 48.6 * CLHEP::perCent);
    Zn->AddIsotope(Zn66, 27.9* CLHEP::perCent);
    Zn->AddIsotope(Zn67, 4.1 * CLHEP::perCent);
    Zn->AddIsotope(Zn68, 18.8 * CLHEP::perCent);
    Zn->AddIsotope(Zn70, 0.6 * CLHEP::perCent);
    
	G4Isotope* Mg24 = new G4Isotope("Mg24", 12, 24); //Mg
	G4Isotope* Mg25 = new G4Isotope("Mg25", 12, 25); //Mg
	G4Isotope* Mg26 = new G4Isotope("Mg26", 12, 26); //Mg
	G4Element* Mg = new G4Element("TS_Mg_of_Enstatit", "Mg", 3);
	Mg->AddIsotope(Mg24, 78.965 * CLHEP::perCent);
	Mg->AddIsotope(Mg25, 10.01 * CLHEP::perCent);
	Mg->AddIsotope(Mg26, 11.025 * CLHEP::perCent);

	G4Isotope* Na23 = new G4Isotope("Na23", 11, 23); //Na
	G4Element* Na = new G4Element("TS_Na_of_Enstatit", "Na", 1);
	Na->AddIsotope(Na23, 100 * CLHEP::perCent);

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

	G4Isotope* S32 = new G4Isotope("S32", 16, 32); //S
	G4Isotope* S33 = new G4Isotope("S33", 16, 33); //S
	G4Isotope* S34 = new G4Isotope("S34", 16, 34); //S
	G4Isotope* S36 = new G4Isotope("S36", 16, 36); //S 
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
	
    G4Isotope* As75 = new G4Isotope("As75", 33, 75);
    G4Element* As = new G4Element("TS_As_of_Lorandit", "As", 1);
    As->AddIsotope(As75, 100. * CLHEP::perCent);
    
    G4Isotope* Tl205 = new G4Isotope("Tl205", 81, 205);
    G4Isotope* Tl203 = new G4Isotope("Tl203", 81, 203);
    G4Element* Tl = new G4Element("TS_Tl_of_Lorandit", "Tl", 2);
    Tl->AddIsotope(Tl205, 70.485 * CLHEP::perCent);
    Tl->AddIsotope(Tl203, 29.515 * CLHEP::perCent);
    
    G4Isotope* Mn55 = new G4Isotope("Mn55", 25, 55); 
    G4Element* Mn = new G4Element("TS_Mn", "Mn", 1);
    Mn->AddIsotope(Mn55, 100*CLHEP::perCent);
    
     G4Isotope* Cr50 = new G4Isotope("Cr50", 24, 50); 
    G4Isotope* Cr52 = new G4Isotope("Cr52", 24, 52); 
    G4Isotope* Cr53 = new G4Isotope("Cr53", 24, 53);
    G4Isotope* Cr54 = new G4Isotope("Cr54", 24, 54);
    G4Element* Cr = new G4Element("TS_Cr", "Cr", 4);
    Cr->AddIsotope(Cr50, 4.345 *CLHEP::perCent);
    Cr->AddIsotope(Cr52, 83.789 *CLHEP::perCent);
    Cr->AddIsotope(Cr53, 9.501 *CLHEP::perCent);
    Cr->AddIsotope(Cr54, 2.365 *CLHEP::perCent);
    
    G4Isotope* Th230 = new G4Isotope("Th230", 90, 230);
	G4Isotope* Th232 = new G4Isotope("Th232", 90, 232);
	G4Element* Th = new G4Element("TS_Thorium", "Th", 2);
	Th->AddIsotope(Th230, 0.02 * CLHEP::perCent);
	Th->AddIsotope(Th232, 99.98 * CLHEP::perCent);
    
    G4Isotope* U234 = new G4Isotope("U234", 92, 234);
	G4Isotope* U235 = new G4Isotope("U235", 92, 235);
	G4Isotope* U238 = new G4Isotope("U238", 92, 238);
	G4Element* U = new G4Element("TS_Uranium", "U", 3);
	U->AddIsotope(U234, 0.0055 * CLHEP::perCent);
	U->AddIsotope(U235, 0.720 * CLHEP::perCent);
	U->AddIsotope(U238, 99.2745 * CLHEP::perCent);
		
    G4Isotope* Ce136 = new G4Isotope("Ce136", 58, 136);
    G4Isotope* Ce138 = new G4Isotope("Ce138", 58, 138);
    G4Isotope* Ce140 = new G4Isotope("Ce140", 58, 140);
    G4Isotope* Ce142 = new G4Isotope("Ce142", 58, 142);
    G4Element* Ce = new G4Element("TS_Cerium", "Ce", 4);
    Ce->AddIsotope(Ce136, 0.19 * CLHEP::perCent);
    Ce->AddIsotope(Ce138, 0.25 * CLHEP::perCent);
    Ce->AddIsotope(Ce140, 88.48 * CLHEP::perCent);
    Ce->AddIsotope(Ce142, 11.08 * CLHEP::perCent);

    ///////////////////////// Stahl Definition ///////////////////////
    
    //Teflon
    G4Material* Teflon = new G4Material("Teflon", 2.2 * CLHEP::g / CLHEP::cm3, 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    Teflon->AddElement(C, 2);
    Teflon->AddElement(F, 3);
    
    materialTable["Teflon"] = Teflon;

    // Brass
    G4Material* Brass = new G4Material("Brass", 8.6 * CLHEP::g / CLHEP::cm3, 2, kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    G4double weightFractio_CuBrass = 0.7; 
    G4double weightFractio_ZnBrass = 0.3; 
    Brass->AddElement(Cu, weightFractio_CuBrass);
    Brass->AddElement(Zn, weightFractio_ZnBrass);
    
    materialTable["Brass"] = Brass;
    
    // steel
    G4Material* steel = new G4Material("steel", 7.874 * CLHEP::g / CLHEP::cm3, 1,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    steel->AddElement(Fe, 1);

    materialTable["steel"] = steel;
    
    // cerium
    G4Material* cerium_5 = new G4Material("cerium_5", 5.599 * CLHEP::g / CLHEP::cm3, 1,
        kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    cerium_5->AddElement(Ce, 1);

    materialTable["cerium_5"] = cerium_5;

    // cerium_6
    G4Material* cerium_6 = new G4Material("cerium_6", 6.689 * CLHEP::g / CLHEP::cm3, 1,
        kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    cerium_6->AddElement(Ce, 1);

    materialTable["cerium_6"] = cerium_6;

    // vacuum_ideal
    G4double vacuumDensity = 1e-25 * CLHEP::g / CLHEP::cm3;
    G4double vacuumTemperature = 293.0 * CLHEP::kelvin;     // gleiche Temperatur wie cerium_6
    G4double vacuumPressure = 1.0 * CLHEP::atmosphere;   // kann gleich bleiben oder abgesenkt werden

    G4Material* vacuum_ideal = new G4Material("vacuum_ideal", 1, 1.01 * CLHEP::g / CLHEP::mole,
        vacuumDensity, kStateGas, vacuumTemperature, vacuumPressure);

    materialTable["vacuum_ideal"] = vacuum_ideal;

    ////////////////////////// Ausgedehnte Quellen Definition //////////////////////////
 
    G4Material* K2SO4 =  new G4Material("K2SO4", 1.60864 * CLHEP::g / CLHEP::cm3, 3, kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    K2SO4->AddElement(K, 2);
    K2SO4->AddElement(S, 1);
    K2SO4->AddElement(O, 4);
    
    materialTable["K2SO4"] = K2SO4;
    
    // Uran-Powder Definition
  
    // Gewichtsanteile
    G4double weightFraction_U = 0.0004; 
    G4double weightFraction_Si = 0.4677; 
    G4double weightFractio_O = 0.5319; 
    
    // Material für die Zusammensetzung von Enstatit definieren
    G4Material* USi = new G4Material("USi", 2.65 * CLHEP::g / CLHEP::cm3, 3, kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);

    // Teilmaterialien hinzufügen
    USi->AddElement(U, weightFraction_U);
    USi->AddElement(Si, weightFraction_Si);
    USi->AddElement(O, weightFractio_O);
    
    materialTable["USi"] = USi;
    
    // Thorium-Powder Definition
  
    // Gewichtsanteile
    G4double ThweightFraction_Th = 0.0008; 
    G4double ThweightFraction_U = 0.0000063; 
    G4double ThweightFraction_Si = 0.4675; 
    G4double ThweightFractio_O = 0.5317; 
    
    // Material für die Zusammensetzung von Enstatit definieren
    G4Material* ThSi = new G4Material("ThSi", 2.65 * CLHEP::g / CLHEP::cm3, 4, kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);

    // Teilmaterialien hinzufügen
    ThSi->AddElement(Th, ThweightFraction_Th);
    ThSi->AddElement(U, ThweightFraction_U);
    ThSi->AddElement(Si, ThweightFraction_Si);
    ThSi->AddElement(O, ThweightFractio_O);
    
    materialTable["ThSi"] = ThSi; 
    
    //////////////////////// Lorandit Definition //////////////////////////////////////////////

    G4Material* lorandit =  new G4Material("lorandit", 2.98592196393533 * CLHEP::g / CLHEP::cm3, 3, kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    lorandit->AddElement(As, 1);
    lorandit->AddElement(Tl, 1);
    lorandit->AddElement(S, 2);
    
    materialTable["lorandit"] = lorandit;

    /////////////////// Aubrit Meteorit ///////////////////

    G4Material* Enstatit = new G4Material("enstatit", 3.2 * CLHEP::g / CLHEP::cm3, 3, kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    Enstatit->AddElement(Mg, 1);
    Enstatit->AddElement(Si, 1);
    Enstatit->AddElement(O, 3);
    
    G4Material* Albit = new G4Material("albit", 2.62 * CLHEP::g / CLHEP::cm3,4,kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    Albit->AddElement(Na, 1);
    Albit->AddElement(Al, 1);
    Albit->AddElement(Si, 3);
    Albit->AddElement(O, 8);
    
    G4Material* Forsterit = new G4Material("forsterit", 3.275 * CLHEP::g / CLHEP::cm3, 3, kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
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
    
    materialTable["enstatit_comp"] = enstatit_comp;
    
/////////////// Haag Meteorit gebastelt //////////////////
	
	
	//Hagg Zusammensetzung
	G4int nCompSiO2;
    G4Material* SiO2 = new G4Material("SiO2", 2.65 * CLHEP::g / CLHEP::cm3, nCompSiO2 = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    SiO2->AddElement(Si, 1);
    SiO2->AddElement(O, 2);
    
    G4int nCompAl2O3;
    G4Material* Al2O3 = new G4Material("Al2O3", 3.99 * CLHEP::g / CLHEP::cm3, nCompAl2O3 = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    Al2O3->AddElement(Al, 2);
    Al2O3->AddElement(O, 3);
    
    G4int nCompFeO;
    G4Material* FeO = new G4Material("FeO", 5.74 * CLHEP::g / CLHEP::cm3, nCompFeO = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    FeO->AddElement(Fe, 1);
    FeO->AddElement(O, 1);
    
    G4int nCompFe2O3;
    G4Material* Fe2O3 = new G4Material("Fe2O3", 5.24 * CLHEP::g / CLHEP::cm3, nCompFe2O3 = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    Fe2O3->AddElement(Fe, 2);
    Fe2O3->AddElement(O, 3);

	G4int nCompMgO;
    G4Material* MgO = new G4Material("MgO", 3.58 * CLHEP::g / CLHEP::cm3, nCompMgO = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    MgO->AddElement(Mg, 1);
    MgO->AddElement(O, 1);
    
    G4int nCompNa2O;
    G4Material* Na2O = new G4Material("Na2O", 2.27 * CLHEP::g / CLHEP::cm3, nCompNa2O = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    Na2O->AddElement(Na, 2);
    Na2O->AddElement(O, 1);
    
    G4int nCompNiO;
    G4Material* NiO = new G4Material("NiO", 6.67 * CLHEP::g / CLHEP::cm3, nCompNiO = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    NiO->AddElement(Ni, 1);
    NiO->AddElement(O, 1);
    
    G4int nCompSO3;
    G4Material* SO3 = new G4Material("SO3", 1.92 * CLHEP::g / CLHEP::cm3, nCompSO3 = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    SO3->AddElement(S, 1);
    SO3->AddElement(O, 3);
    
    G4int nCompCaO;
    G4Material* CaO = new G4Material("CaO", 3.34 * CLHEP::g / CLHEP::cm3, nCompCaO = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    CaO->AddElement(Ca, 1);
    CaO->AddElement(O, 1);
    
    G4int nCompP2O5;
    G4Material* P2O5 = new G4Material("P2O5", 2.30 * CLHEP::g / CLHEP::cm3, nCompP2O5 = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    P2O5->AddElement(P, 2);
    P2O5->AddElement(O, 5);
    
    G4int nCompK2O;
    G4Material* K2O = new G4Material("K2O", 2.35 * CLHEP::g / CLHEP::cm3, nCompK2O = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    K2O->AddElement(K, 2);
    K2O->AddElement(O, 1);
    
    G4int nCompTiO2;
    G4Material* TiO2 = new G4Material("TiO2", 4.26 * CLHEP::g / CLHEP::cm3, nCompTiO2 = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    TiO2->AddElement(Ti, 1);
    TiO2->AddElement(O, 2);
    
    G4int nCompCr2O3;
    G4Material* Cr2O3 = new G4Material("Cr2O3", 5.22 * CLHEP::g / CLHEP::cm3, nCompCr2O3 = 2,
    kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    Cr2O3->AddElement(Cr, 2);
    Cr2O3->AddElement(O, 3);
    
    G4int nCompMnO;
    G4Material* MnO = new G4Material("MnO", 5.37 * CLHEP::g / CLHEP::cm3, nCompMnO = 2,
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
    
    materialTable["meteorit2_comp"] = meteorit2_comp;
    
    //Material for Drelow meteorite
    
     // Gewichtsanteil von MgO und SiO2
   // G4double weightFraction_drelow_SiO2 = 0.3981; 
    G4double weightFraction_drelow_Al2O3 = 0.0337; 
    G4double weightFraction_drelow_FeO = 0.4591; 
    G4double weightFraction_drelow_Fe2O3 = 0.0510; 
    G4double weightFraction_drelow_MgO = 0.3522; 
    G4double weightFraction_drelow_Na2O = 0.0149; 
    G4double weightFraction_drelow_NiO = 0.0432; 
    G4double weightFraction_drelow_P2O5 = 0.0034; 
   // G4double weightFraction_drelow_SO3 = 0.0516; 
    G4double weightFraction_drelow_K2O = 0.0017; 
    G4double weightFraction_drelow_CaO = 0.0283; 
    G4double weightFraction_drelow_TiO2 = 0.0015; 
    G4double weightFraction_drelow_Cr2O3 = 0.0054; 
    G4double weightFraction_drelow_MnO = 0.0055; 
    
    G4Material* drelow_comp = new G4Material("drelow_comp", 3.22069 * CLHEP::g / CLHEP::cm3, 12, kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);

    // Teilmaterialien hinzufügen
    //drelow_comp->AddMaterial(SiO2, weightFraction_drelow_SiO2);
    drelow_comp->AddMaterial(Al2O3, weightFraction_drelow_Al2O3);
    drelow_comp->AddMaterial(FeO, weightFraction_drelow_FeO);
    drelow_comp->AddMaterial(Fe2O3, weightFraction_drelow_Fe2O3);
    drelow_comp->AddMaterial(MgO, weightFraction_drelow_MgO);
    drelow_comp->AddMaterial(Na2O, weightFraction_drelow_Na2O);
    drelow_comp->AddMaterial(NiO, weightFraction_drelow_NiO);
    drelow_comp->AddMaterial(P2O5, weightFraction_drelow_P2O5);
   // drelow_comp->AddMaterial(SO3, weightFraction_drelow_SO3);
    drelow_comp->AddMaterial(K2O, weightFraction_drelow_K2O);
    drelow_comp->AddMaterial(CaO, weightFraction_drelow_CaO);
    drelow_comp->AddMaterial(TiO2, weightFraction_drelow_TiO2);
    drelow_comp->AddMaterial(Cr2O3, weightFraction_drelow_Cr2O3);
    drelow_comp->AddMaterial(MnO, weightFraction_drelow_MnO);
    
    materialTable["drelow_comp"] = drelow_comp;


    G4double pen_mass = 224 * CLHEP::g;
    G4double pen_volume = 10 * 10 * 3 * CLHEP::cm3; //Box
    G4Material* mat_pen = new G4Material("PEN", pen_mass / pen_volume, 3, kStateSolid, 293 * CLHEP::kelvin, 1 * CLHEP::atmosphere);
    mat_pen->AddElement(C, 14);
    mat_pen->AddElement(H, 10);
    mat_pen->AddElement(O, 4);
    materialTable["PEN"] = mat_pen;
    
}

G4Material* Materials::GetMaterial(const G4String& name) {
    if (materialTable.find(name) != materialTable.end()) {
        return materialTable[name];
    } else {
        G4cerr << "Material " << name << " nicht gefunden!" << G4endl;
        return nullptr;
    }
}

