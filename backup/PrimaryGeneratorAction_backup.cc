/// \file PrimaryGeneratorAction.cc
/// \brief Implementation of the PrimaryGeneratorAction class

#include "PrimaryGeneratorAction.hh"

#include "G4VSolid.hh"
#include "G4TessellatedSolid.hh"
#include "G4Polycone.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "geometry/RockSample.hh" 
#include "geometry/TU1Detector.hh" 
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4RandomDirection.hh"
#include "Randomize.hh"
#include "G4AnalysisManager.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Transform3D.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4AffineTransform.hh"
#include "G4PhysicalVolumeStore.hh"

// Diese Methode gibt die Transformation des physischen Volumens zur�ck
G4AffineTransform GetTransformFromPhysicalVolume(G4VPhysicalVolume* physicalVolume)
{
    if (!physicalVolume) {
        G4cerr << "Physical volume not found!" << G4endl;
        return G4AffineTransform(); // Gibt eine Standard-Transformation zur�ck
    }

    // Holen Sie sich die Rotation und Translation des physischen Volumens
    G4RotationMatrix* rotation = physicalVolume->GetRotation();
    G4ThreeVector translation = physicalVolume->GetTranslation();

    // Erstellen Sie die Transformationsmatrix
    if (rotatation) {
        return G4AffineTransform(*rotation, translation);
    } else {
        // Verwenden Sie die Identit��tsrotation, wenn keine Rotation angewendet wird
        return G4AffineTransform(translation);
    }
}

PrimaryGeneratorAction::PrimaryGeneratorAction()
    : m_particleGun(std::make_shared<G4ParticleGun>(1)),
    m_cmdPosMode(std::make_shared<G4UIcmdWithAString>("/generator/mode", this)),
    m_cmdSetVolumeHeight(std::make_shared<G4UIcmdWithADoubleAndUnit>("/generator/volumeHeight", this)),
    m_cmdSetVolumeDiameter(std::make_shared<G4UIcmdWithADoubleAndUnit>("/generator/volumeDiameter", this)),
    m_cmdSetVolumeOffset(std::make_shared<G4UIcmdWithADoubleAndUnit>("/generator/volumeOffset", this)),
    m_posMode(mPointDirectional),
    m_height(6.21 * CLHEP::mm),
    m_diameter(48.94 * CLHEP::mm),
    m_offset(1.165 * CLHEP::mm),
    m_rockSample(nullptr) // Initialisiere den Zeiger auf RockSample
{

    auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    m_particleGun->SetParticleDefinition(particleDefinition);
    m_particleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 0.));
    m_particleGun->SetParticleEnergy(0. * MeV);

    m_cmdPosMode->SetParameterName("mode", false);
    m_cmdPosMode->SetCandidates("point-directional point-isotropic volume-cylinder volume-sphere volume-scan");
    G4cout << "Command /generator/mode registered" << G4endl;

    m_cmdSetVolumeHeight->SetParameterName("height", false);
    G4cout << "Command /generator/volumeHeight registered" << G4endl;

    m_cmdSetVolumeDiameter->SetParameterName("diameter", false);
    G4cout << "Command /generator/volumeDiameter registered" << G4endl;

    m_cmdSetVolumeOffset->SetParameterName("offset", false);
    G4cout << "Command /generator/volumeOffset registered" << G4endl;
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    // Der Destruktor kann leer bleiben, wenn keine speziellen Bereinigungen erforderlich sind
}

void PrimaryGeneratorAction::SetRockSample(shared_ptr<RockSample> rockSample)
{
    m_rockSample = rockSample;
}


bool PrimaryGeneratorAction::GenerateRandomPointInTessellatedSolid(G4TessellatedSolid* tessellatedSolid, G4ThreeVector& randomPosition, G4LogicalVolume* logicalVolume)
{
    /// Holen Sie sich das physische Volumen aus der Hierarchie
    G4VPhysicalVolume* physicalVolume = nullptr;
    for (auto iter = G4PhysicalVolumeStore::GetInstance()->begin(); iter != G4PhysicalVolumeStore::GetInstance()->end(); ++iter) {
        if ((*iter)->GetLogicalVolume() == logicalVolume) {
            physicalVolume = *iter;
            break;
        }
    }

    if (!physicalVolume) {
        G4cerr << "Physical volume not found!" << G4endl;
        return false;
    }

    // Holen Sie sich die Transformation
    G4AffineTransform transform = GetTransformFromPhysicalVolume(physicalVolume);

    // Generierung und Transformierung
    G4ThreeVector minPoint, maxPoint;
    tessellatedSolid->BoundingLimits(minPoint, maxPoint);

    // Transformiere die lokalen Grenzen in das globale Koordinatensystem
    G4ThreeVector globalMinPoint = transform.TransformPoint(minPoint);
    G4ThreeVector globalMaxPoint = transform.TransformPoint(maxPoint);



    G4bool inside = false;
    do {
        randomPosition.setX(G4UniformRand() * (maxPoint.x() - minPoint.x()) + minPoint.x());
        randomPosition.setY(G4UniformRand() * (maxPoint.y() - minPoint.y()) + minPoint.y());
        randomPosition.setZ(G4UniformRand() * (maxPoint.z() - minPoint.z()) + minPoint.z());

        // Transformiere die Position zur globalen Koordinatensystem
        G4ThreeVector globalPosition = transform.TransformPoint(randomPosition);
        G4ThreeVector localPosition = transform.Inverse().TransformPoint(globalPosition);


        inside = (tessellatedSolid->Inside(localPosition) != kOutside);

       /* G4cout << "-------------------------------" << G4endl;


        G4cout << "Generated position: ("
            << randomPosition.x() << ", "
            << randomPosition.y() << ", "
            << randomPosition.z() << ") - "
            << (inside ? "Inside" : "Outside") << G4endl;

        G4cout << "minPoint.x() : ("
            << minPoint.x() << ") - " << G4endl;
        G4cout << "maxPoint.x() : ("
            << maxPoint.x() << ") - " << G4endl;


        G4cout << "globalMinPoint : ("
            << globalMinPoint << ") - " << G4endl;
        G4cout << "globalMaxPoint : ("
            << globalMaxPoint << ") - " << G4endl;
        G4cout << "globalPosition : ("
            << globalPosition << ") - " << G4endl;*/

        /*G4cout << "Mittelpunkt global x : "
            << globalMaxPoint.x() + std::abs(globalMaxPoint.x() - globalMinPoint.x()) /2.<< G4endl;
        G4cout << "Mittelpunkt global y : "
            << globalMaxPoint.y() + std::abs(globalMaxPoint.y() - globalMinPoint.y()) / 2. << G4endl;
        G4cout << "Mittelpunkt global z : "
            << globalMinPoint.z() + std::abs(globalMaxPoint.z() - globalMinPoint.z()) / 2. << G4endl;*/


            /* G4cout << "Lenght x ("
                 << maxPoint.x() - minPoint.x() << ")" << G4endl;
             G4cout << "Lenght y ("
                 << maxPoint.y() - minPoint.y() << ")" << G4endl;
             G4cout << "Lenght z ("
                 << maxPoint.z() - minPoint.z() << ")" << G4endl; */

                 /*    G4cout << "global Lenght x ("
                         << globalMaxPoint.x() - globalMinPoint.x() << ")" << G4endl;
                     G4cout << "global Lenght y ("
                         << globalMaxPoint.y() - globalMinPoint.y() << ")" << G4endl;
                     G4cout << "global Lenght z ("
                         << globalMaxPoint.z() - globalMinPoint.z() << ")" << G4endl;*/

                         // G4cout << "_____________________________________________" << G4endl;
    } while (!inside);

    // Setze die zuf�llige Position zur�ck zum lokalen Koordinatensystem des Volumens
   // randomPosition = transform.Inverse().TransformPoint(randomPosition);

    return true;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // Generate direction
    switch (m_posMode)
    {
    case mPointDirectional:
        // Do not change direction
        break;

    case mPointIsotropic:
    case mVolumeCylinder:
    case mVolumeSphere:
        m_particleGun->SetParticleMomentumDirection(G4RandomDirection());
        break;

    case mVolumeScan:
        m_particleGun->SetParticleMomentumDirection(G4RandomDirection());
        break;

    default:
        G4cerr << "Unhandled case for m_posMode in PrimaryGeneratorAction (1)" << G4endl;
        exit(99);
    }

    // Generate position
    G4ThreeVector pos;
    G4ThreeVector pos_reset;
    switch (m_posMode)
    {
    case mPointDirectional:
    case mPointIsotropic:
        pos = m_particleGun->GetParticlePosition();
        break;

    case mVolumeCylinder:
    {
        G4double radiusC, height, offset, rC, thetaC;

        height = m_height;
        offset = m_offset;
        radiusC = m_diameter / 2.;

        rC = radiusC * std::sqrt(G4UniformRand());
        thetaC = G4UniformRand() * CLHEP::twopi;

        pos_reset = m_particleGun->GetParticlePosition();

        pos = G4ThreeVector(rC * std::cos(thetaC), rC * std::sin(thetaC), offset + G4UniformRand() * height);
        pos += pos_reset;
    }
    break;

    case mVolumeSphere:
    {
        pos_reset = m_particleGun->GetParticlePosition();

        G4double u, v, theta, phi, r, sinTheta, cosTheta, sinPhi, cosPhi;

        G4double radius = m_diameter / 2;

        u = G4UniformRand();
        v = G4UniformRand();
        theta = u * CLHEP::twopi;
        phi = std::acos(2.0 * v - 1.0);
        r = radius * std::cbrt(G4UniformRand());
        sinTheta = std::sin(theta);
        cosTheta = std::cos(theta);
        sinPhi = std::sin(phi);
        cosPhi = std::cos(phi);

        pos = G4ThreeVector(r * sinPhi * cosTheta, r * sinPhi * sinTheta, r * cosPhi);
        pos += pos_reset;
    }
    break;

    case mVolumeScan:
    {
        // Retrieve the logical volume
        auto logicalVolume = G4LogicalVolumeStore::GetInstance()->GetVolume("sample");
        if (logicalVolume == nullptr)
        {
            G4cerr << "Scan volume not found!" << G4endl;
            exit(99);
        }

        // Get the physical volume from the logical volume
        G4VPhysicalVolume* physicalVolume = nullptr;
        for (auto iter = G4PhysicalVolumeStore::GetInstance()->begin(); iter != G4PhysicalVolumeStore::GetInstance()->end(); ++iter) {
            if ((*iter)->GetLogicalVolume() == logicalVolume) {
                physicalVolume = *iter;
                break;
            }
        }

        if (!physicalVolume)
        {
            G4cerr << "Physical volume not found!" << G4endl;
            exit(99);
        }

        auto solid = logicalVolume->GetSolid();
        if (auto tessellatedSolid = dynamic_cast<G4TessellatedSolid*>(solid))
        {
            G4ThreeVector randomPosition;
            if (!GenerateRandomPointInTessellatedSolid(tessellatedSolid, randomPosition, logicalVolume))
            {
                G4cerr << "Failed to generate random position in tessellated solid!" << G4endl;
                exit(99);
            }

            G4AffineTransform transform = GetTransformFromPhysicalVolume(physicalVolume);
            G4ThreeVector globalPosition = transform.TransformPoint(randomPosition);

            pos = globalPosition;
        }
        else
        {
            G4cerr << "Unhandled solid type for scan volume!" << G4endl;
            exit(99);
        }
    }
    break;

    default:
        G4cerr << "Unhandled case for m_posMode in PrimaryGeneratorAction (2)" << G4endl;
        exit(99);
    }

    m_particleGun->SetParticlePosition(pos);
    m_particleGun->GeneratePrimaryVertex(anEvent);

    if (m_posMode == mVolumeCylinder || m_posMode == mVolumeSphere) {
        m_particleGun->SetParticlePosition(pos_reset);
    }

    // Set position in output tree
    /*{
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillNtupleDColumn(0, pos.x() / CLHEP::cm);
        analysisManager->FillNtupleDColumn(1, pos.y() / CLHEP::cm);
        analysisManager->FillNtupleDColumn(2, pos.z() / CLHEP::cm);

     
    }*/
    
    
	auto analysisManager = G4AnalysisManager::Instance();	
	analysisManager->FillNtupleDColumn(0, 0, pos.x() / CLHEP::cm); // Ntuple 0, Spalte 0
	analysisManager->FillNtupleDColumn(0, 1, pos.y() / CLHEP::cm); // Ntuple 0, Spalte 1
	analysisManager->FillNtupleDColumn(0, 2, pos.z() / CLHEP::cm); // Ntuple 0, Spalte 2

	analysisManager->AddNtupleRow(0);            // Ntuple 0 abschlie�en



}

void PrimaryGeneratorAction::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == m_cmdPosMode.get())
    {
        if (newValue == "point-directional")
        {
            m_posMode = mPointDirectional;
        }
        else if (newValue == "point-isotropic")
        {
            m_posMode = mPointIsotropic;
        }
        else if (newValue == "volume-cylinder")
        {
            m_posMode = mVolumeCylinder;
        }
        else if (newValue == "volume-sphere")
        {
            m_posMode = mVolumeSphere;
        }
        else if (newValue == "volume-scan")
        {
            m_posMode = mVolumeScan;
        }
        else
        {
            G4cerr << "Unhandled case for /generator/mode" << G4endl;
            exit(99);
        }
    }
    else if (command == m_cmdSetVolumeHeight.get())
    {
        m_height = m_cmdSetVolumeHeight->GetNewDoubleValue(newValue);
    }
    else if (command == m_cmdSetVolumeDiameter.get())
    {
        m_diameter = m_cmdSetVolumeDiameter->GetNewDoubleValue(newValue);
    }
    else if (command == m_cmdSetVolumeOffset.get())
    {
        m_offset = m_cmdSetVolumeOffset->GetNewDoubleValue(newValue);
    }
    else
    {
        G4cerr << "Unhandled command in PrimaryGeneratorAction::SetNewValue()" << G4endl;
        exit(99);
    }
}
