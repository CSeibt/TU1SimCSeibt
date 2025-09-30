#include "geometry/PENSample.hh"

using CLHEP::cm;
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Ellipsoid.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PVPlacement.hh"
#include "geometry/Materials.hh"

PENSample::PENSample() : GeometryObject("PENSample") {
    NewLengthValue("sampleHalfLengthX", "Half length of the sample in x direction.", 5*CLHEP::cm);
    NewLengthValue("sampleHalfLengthY", "Half length of the sample in y direction.", 5*CLHEP::cm);
    NewLengthValue("sampleHalfLengthZ", "Half length of the sample in z direction.", 1.5*CLHEP::cm);
    NewLengthValue("sampleDistance", "Distance of the sample from the detector surface.", 0.*CLHEP::cm);
}

void PENSample::DefineMaterials() {
    Materials::DefineMaterials();  // Aufruf der statischen Methode in der Materials-Klasse
}

void PENSample::Construct(G4LogicalVolume* lWorldVolume, G4bool checkOverlaps) {
    const G4String name = "PENSample";
    const G4double sampleHalfLengthX = GetLengthValue("sampleHalfLengthX");
    const G4double sampleHalfLengthY = GetLengthValue("sampleHalfLengthY");
    const G4double sampleHalfLengthZ = GetLengthValue("sampleHalfLengthZ");
    const G4double sampleDistance = GetLengthValue("sampleDistance") + sampleHalfLengthZ;

    G4Material* mat_pen = Materials::GetMaterial("PEN");

    auto pen_sample_solid = new G4Box(name, sampleHalfLengthX, sampleHalfLengthY, sampleHalfLengthZ);
    //auto pen_sample_solid = new G4Tubs(name, 0, sampleRadius, sampleHalfLengthZ, 0, 2 * M_PI);
    auto pen_sample_logical = new G4LogicalVolume(pen_sample_solid, mat_pen, name);
    pen_sample_logical->SetVisAttributes(G4Colour::White());

    new G4PVPlacement(m_noRotation,
                      G4ThreeVector(0, 0, sampleDistance),
                      pen_sample_logical, name,
                      lWorldVolume, false, 0, checkOverlaps);
}