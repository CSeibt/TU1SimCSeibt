#include "tets.h"

tets::tets()
{
    if(GetBoolValue("sampleHolder")){

        const G4String name = "holder";
        const G4String nameLeg = "Leg";

        const G4double capDiameter    = GetLengthValue("capDiameter");

        const G4double holderLegLength    = GetLengthValue("holderLegLength");
        const G4double holderLegDiameter    = GetLengthValue("holderLegDiameter");
        const G4double holderLegXY    = GetLengthValue("holderLegXY");

        const G4double holderTopWidth    = GetLengthValue("holderLegHeight");
        const G4double holderTopHeight    = GetLengthValue("holderLegHeight");

        const G4double holderHoleDiameter    = GetLengthValue("holderHoleDiameter");

        auto matCu = m_nistManager->FindOrBuildMaterial("G4_Cu");


        auto sLeg = new G4Tubs(nameLeg, 0, 0.5*holderLegDiameter, 0.5*holderLegLength, 0, CLHEP::twopi);

        auto lLeg = new G4LogicalVolume(sLeg, matCu, nameLeg);

        lLeg->SetVisAttributes(G4Colour::Grey());

        new G4PVPlacement(m_noRotation,
                          G4ThreeVector(0, 0, 1*CLHEP::cm),
                          lLeg, nameLeg,
                          lWorldVolume, false, 0, checkOverlaps);
}
    NewBoolValue("sampleHolder", "Switch On/Off Sample Holder.", true);
    NewLengthValue("holderLegLength", "Length of the Sample holder Legs.", 10 * CLHEP::cm);
    NewLengthValue("holderLegDiameter", "The diameter of the Legs' Zylinders", 0.5 * CLHEP::cm);
    NewLengthValue("holderLegXY", "", 10 * CLHEP::cm);
    NewLengthValue("holderTopWidth", "", 10 * CLHEP::cm);
    NewLengthValue("holderTopHeight", "", 10 * CLHEP::cm);
    NewLengthValue("holderHoleDiameter", "", 10 * CLHEP::cm);
    */
