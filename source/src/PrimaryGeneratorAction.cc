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
#include "G4Tubs.hh"
#include "geometry/RockSample.hh" 
#include "geometry/PENSample.hh"
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
#include "G4AutoLock.hh"
#include "G4Threading.hh"
#include <vector>
#ifdef Integer
#undef Integer
#endif
#include "TRandom.h"

#include <TH3.h>
#include <TFile.h>

namespace {
    G4Mutex gRootMutex = G4MUTEX_INITIALIZER;
}

// Vectors to safe the directions of 2 gammas
static thread_local std::map<G4int, G4ThreeVector> gamma1Directionsp;
static thread_local std::map<G4int, G4ThreeVector> gamma2Directionsp;
static thread_local std::map<G4int, G4int> gamma1PrimaryID;


PrimaryGeneratorAction::PrimaryGeneratorAction()
    : m_particleGun(std::make_shared<G4ParticleGun>(1)),
    m_rockSample(nullptr), // Initialize to zero (in case no rock sample is defined/used)
    //
    m_sourceHeight(6.21 * CLHEP::mm),
    m_sourceDiameter(48.94 * CLHEP::mm),
    m_offsetOfSource(1.165 * CLHEP::mm),

    m_cmdSetVolumeHeight(std::make_shared<G4UIcmdWithADoubleAndUnit>("/generator/volumeHeight", this)),
    m_cmdSetVolumeDiameter(std::make_shared<G4UIcmdWithADoubleAndUnit>("/generator/volumeDiameter", this)),
    m_cmdSetVolumeOffset(std::make_shared<G4UIcmdWithADoubleAndUnit>("/generator/volumeOffset", this)),
    //
    m_posMode(point),
    m_cmdPosMode(std::make_shared<G4UIcmdWithAString>("/generator/posMode", this)),
    m_logicalVolumeName(""),
    m_cmdSetLogicalVolumeName(std::make_shared<G4UIcmdWithAString>("/generator/setLogicalVolumeName", this)),
    //
    m_directionMode(isotropic),
    m_cmdDirectionMode(std::make_shared<G4UIcmdWithAString>("/generator/directionMode", this)),
    m_customDirection(1, 0, 0),
    m_cmdSetCustomDirection(std::make_shared<G4UIcmdWith3Vector>("/generator/setCustomDirection", this)),
    //
    m_cmdUseDecay0File(std::make_shared<G4UIcmdWithAString>("/generator/setDecay0File", this))
{
    // Default particle gun, just a 0MeV gamma
    auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    m_particleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    m_particleGun->SetParticleDefinition(particleDefinition);
    m_particleGun->SetParticleEnergy(0.5 * MeV);

    // Define macro-commands, setting default values etc
    m_cmdSetVolumeHeight->SetParameterName("sourceHeight", false);
    m_cmdSetVolumeHeight->SetDefaultValue(6.21 * CLHEP::mm);
    G4cout << "Command /generator/volumeHeight registered" << G4endl;
    m_cmdSetVolumeDiameter->SetParameterName("sourceDiameter", false);
    m_cmdSetVolumeDiameter->SetDefaultValue(48.94 * CLHEP::mm);
    G4cout << "Command /generator/volumeDiameter registered" << G4endl;
    m_cmdSetVolumeOffset->SetParameterName("offsetOfSource", false);
    m_cmdSetVolumeOffset->SetDefaultValue(1.165 * CLHEP::mm);
    G4cout << "Command /generator/volumeOffset registered" << G4endl;
    //
    m_cmdPosMode->SetParameterName("posMode", false);
    m_cmdPosMode->SetDefaultValue("point");
    m_cmdPosMode->SetCandidates("inCylinder inSphere inRock inLogicalVolume point");
    G4cout << "Command /generator/posMode registered. Possible arguments: [inCylinder inSphere inRock inLogicalVolume point]" << G4endl;
    //
    m_cmdSetLogicalVolumeName->SetParameterName("logicalVolumeName", false);
    m_cmdSetLogicalVolumeName->SetDefaultValue("");
    G4cout << "Command /generator/setLogicalVolumeName registered" << G4endl;
    //
    m_cmdDirectionMode->SetParameterName("directionMode", false);
    m_cmdDirectionMode->SetDefaultValue("isotropic");
    m_cmdDirectionMode->SetCandidates("directional isotropic");
    // Note:  use 'm_cmdUseDecay0File' to change to decay0 mode!
    // Note2: use of 'directional' requires to adjust the direction vector with '/generator/setCustomDirection X Y Z'
    G4cout << "Command /generator/directionMode registered. Possible arguments: [directional isotropic]" << G4endl;
    m_cmdSetCustomDirection->SetParameterName("dirX", "dirY", "dirZ", false);
    m_cmdSetCustomDirection->SetDefaultValue(G4ThreeVector(1, 0, 0));
    G4cout << "Command /generator/setCustomDirection registered" << G4endl;
    //
    m_cmdUseDecay0File->SetParameterName("decayZeroFile", false);
    m_cmdUseDecay0File->SetDefaultValue("");
    G4cout << "Command /generator/setDecay0File registered" << G4endl;


    {
        G4AutoLock lock(&gRootMutex);
        // Verteilung Bestrahlung in Cerium Probe, laden Histogram
        TFile* file = TFile::Open("/SHARE/raw/users/pichotta/TU1/Simulation/projects/Cerium/Verteilung/activation_map_140_JENDL.root");
        if (!file || file->IsZombie()) {
            G4cerr << "Fehler: Konnte .root file nicht �ffnen!" << G4endl;
            exit(100);
        }

        h3 = (TH3F*)file->Get("h3D;1");
        if (!h3) {
            G4cerr << "Fehler: Histogramm 'hXZ; 1' nicht gefunden!" << G4endl;
            exit(101);
        }

        h3->SetDirectory(0); // vom File trennen
        file->Close();
        delete file;
        
        gRandom->SetSeed(0);
    }
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    if (m_decayZeroInputFile.is_open()) { m_decayZeroInputFile.close(); }
}

void PrimaryGeneratorAction::SetRockSample(shared_ptr<RockSample> rockSample)
{
    m_rockSample = rockSample;
}

bool PrimaryGeneratorAction::GenerateRandomPointInTessellatedSolid(G4TessellatedSolid* tessellatedSolid, G4ThreeVector& randomPosition)
{
    // Generierung und Transformierung
    G4ThreeVector minPoint, maxPoint;
    tessellatedSolid->BoundingLimits(minPoint, maxPoint);

    G4bool inside = false;
    G4int counter = 0;
    do {
        randomPosition.setX(G4UniformRand() * (maxPoint.x() - minPoint.x()) + minPoint.x());
        randomPosition.setY(G4UniformRand() * (maxPoint.y() - minPoint.y()) + minPoint.y());
        randomPosition.setZ(G4UniformRand() * (maxPoint.z() - minPoint.z()) + minPoint.z());

        inside = (tessellatedSolid->Inside(randomPosition) != kOutside);

        // Safety mechanism to avoid endless loops
        counter++;
        if (counter > 10000) { return false; }
    } while (!inside);
    return true;
}

bool PrimaryGeneratorAction::ParseDecayZeroFileSingleBlock(G4int& currentEventId, G4String& currentIsotopeName, std::vector<std::tuple<int, double, G4ThreeVector>>& particlesToGenerate)
{
    std::string line;
    while (std::getline(m_decayZeroInputFile, line))
    {
        // Skip empty lines until we reach an information block
        if (line.empty()) continue;

        // 1) First line: Read in header of type: "int int string"
        std::stringstream headerSS(line);
        double notUsedIsotopeDecayTime;
        headerSS >> currentEventId >> notUsedIsotopeDecayTime >> currentIsotopeName;
        /*G4cout << "Read in (full line): " << line << G4endl;
        G4cout << "Parsed -- currentEventId: " << currentEventId
            << ", notUsedIsotopeDecayTime: " << notUsedIsotopeDecayTime
            << ", currentIsotopeName: " << currentIsotopeName
            << G4endl;*/

            // 2) Second line: Read in nof particles to produce, type: "int"
        std::getline(m_decayZeroInputFile, line);
        int nofSecondaryParticles = 0;
        try
        {
            nofSecondaryParticles = std::stoi(line);
        }
        catch (const std::exception& e)
        {
            G4cerr << "ERROR: Try to parse decay0 file. Number passed for nofSecondaryParticles: '"
                << line << " doesnt match expectation (int)!"
                << "\n         !!! Production of particles skipped (and number set to 0) !!!" << G4endl;
            nofSecondaryParticles = 0;
        }
        /*  G4cout << "Read in (full line): " << line << G4endl;
          G4cout << "Parsed -- nofSecondaryParticles: " << nofSecondaryParticles << G4endl;
          */
          // 3) Read in 'nofSecondaryParticles' lines to get secondaries (== particles to generate)
        for (int i = 0; i < nofSecondaryParticles; ++i) {
            // Get next line and safe in a string
            std::getline(m_decayZeroInputFile, line);
            std::stringstream particleSS(line);

            // Safe string information in variables
            int particleId;
            double decayTime;
            double px, py, pz;
            particleSS >> particleId >> decayTime >> px >> py >> pz;
            /* G4cout << "Read in (full line): " << line << G4endl;
             G4cout << "Parsed -- particleId: " << particleId
                 << ", decayTime: " << decayTime
                 << ", MomDir vector: " << G4ThreeVector(px, py, pz)
                 << G4endl;*/

                 // Safe information in our vector
            particlesToGenerate.push_back(std::make_tuple(particleId, decayTime, G4ThreeVector(px, py, pz)));

        }
        return true;
    }

    // We only get here if there were no more lines with 'text' in the file
    return false;
}

G4AffineTransform GetTransformFromPhysicalVolume(G4VPhysicalVolume* physicalVolume)
{
    if (!physicalVolume) {
        G4cerr << "Physical volume not found!" << G4endl;
        exit(99);
    }

    // Get 'rotation' and 'translation of the physical volume
    G4RotationMatrix* rotation = physicalVolume->GetRotation();
    G4ThreeVector translation = physicalVolume->GetTranslation();

    // Create and return the Transformation matrix
    // Check if rotation is null (no rotation applied)
    if (rotation) {
        return G4AffineTransform(*rotation, translation);
    } else {
        // Use identity rotation if no rotation is applied
        return G4AffineTransform(translation);
    }

}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    // Generate position
    G4ThreeVector pos;
    switch (m_posMode)
    {
    case point:
        pos = m_particleGun->GetParticlePosition();
        break;

    case inCylinder:
    {
        // Retrieve the logical volume 
        G4LogicalVolume* logicalVolume = G4LogicalVolumeStore::GetInstance()->GetVolume("CylinderL"); 

        if (logicalVolume == nullptr) {
            logicalVolume = G4LogicalVolumeStore::GetInstance()->GetVolume("ExtendedSample");

            if (logicalVolume == nullptr) {
                G4cerr << "Neither 'CylinderL' nor 'ExtendedSample' found! Please define one of these volumes in TU1Detektor.cc." << G4endl;
                exit(99);
            }
        }
        // Get the solid from the logical volume (with name 'sample')
        G4VSolid* solid = logicalVolume->GetSolid();
        G4VPhysicalVolume* physicalVolume = nullptr;
        for (auto iter = G4PhysicalVolumeStore::GetInstance()->begin(); iter != G4PhysicalVolumeStore::GetInstance()->end(); ++iter) {
            if ((*iter)->GetLogicalVolume() == logicalVolume) {
                physicalVolume = *iter;
                break;
            }
        }
        if (!physicalVolume)
        {
            G4cerr << "Physical volume of Cylinder not found!" << G4endl;
            exit(99);
        }

        G4double sourceRadius = m_sourceDiameter / 2.;
        G4double randomRadius = sourceRadius * std::sqrt(G4UniformRand());
        G4double randomTheta = G4UniformRand() * CLHEP::twopi;
        pos = G4ThreeVector(
            randomRadius * std::cos(randomTheta),
            randomRadius * std::sin(randomTheta),
            m_offsetOfSource + G4UniformRand() * m_sourceHeight);

        G4AffineTransform transform = G4AffineTransform(physicalVolume->GetRotation(), physicalVolume->GetTranslation());
        G4ThreeVector localPos = transform.Inverse().TransformPoint(pos);

        if (solid->Inside(localPos) != kInside)
        {
            G4cerr << "Generated position is outside of the physical volume!" << G4endl;
            exit(101);
        }
    }
    break;

    case inSphere:
    {
        // Retrieve the logical volume with name 'sample'
        G4LogicalVolume* logicalVolume = G4LogicalVolumeStore::GetInstance()->GetVolume("SphereL");
        if (logicalVolume == nullptr)
        {
            G4cerr << "Sphere volume with name 'SphereL' not found! Define volume with this name in TU1Detektor.cc." << G4endl;
            exit(99);
        }
        // Get the solid from the logical volume (with name 'sample')
        G4VSolid* solid = logicalVolume->GetSolid();
        G4VPhysicalVolume* physicalVolume = nullptr;
        for (auto iter = G4PhysicalVolumeStore::GetInstance()->begin(); iter != G4PhysicalVolumeStore::GetInstance()->end(); ++iter) {
            if ((*iter)->GetLogicalVolume() == logicalVolume) {
                physicalVolume = *iter;
                break;
            }
        }
        if (!physicalVolume)
        {
            G4cerr << "Physical volume of Sphere not found!" << G4endl;
            exit(99);
        }

        G4double u, v, theta, phi, r, sinTheta, cosTheta, sinPhi, cosPhi;
        G4double radius = m_sourceDiameter / 2;

        u = G4UniformRand();
        v = G4UniformRand();
        theta = u * CLHEP::twopi;
        phi = std::acos(2.0 * v - 1.0);
        r = radius * std::cbrt(G4UniformRand());
        sinTheta = std::sin(theta);
        cosTheta = std::cos(theta);
        sinPhi = std::sin(phi);
        cosPhi = std::cos(phi);

        pos = G4ThreeVector(
            r * sinPhi * cosTheta,
            r * sinPhi * sinTheta,
            r * cosPhi + m_offsetOfSource + radius);

        G4AffineTransform transform = G4AffineTransform(physicalVolume->GetRotation(), physicalVolume->GetTranslation());
        G4ThreeVector localPos = transform.Inverse().TransformPoint(pos);

        if (solid->Inside(localPos) != kInside)
        {
            G4cerr << "Generated position is outside of the physical volume!" << G4endl;
            exit(101);
        }
    }
    break;

    case inRock:
    {
        // Retrieve the logical volume with name 'sample'
        G4LogicalVolume* logicalVolume = G4LogicalVolumeStore::GetInstance()->GetVolume("sample");
        if (logicalVolume == nullptr)
        {
            G4cerr << "Scan volume with name 'sample' not found! Define volume with this name in DetectorConstruction()." << G4endl;
            exit(99);
        }

        // Get the solid from the logical volume (with name 'sample')
        G4VSolid* solid = logicalVolume->GetSolid();
        if (G4TessellatedSolid* tessellatedSolid = dynamic_cast<G4TessellatedSolid*>(solid))
        {
            // Random (local) point within this (tessellated) solid
            G4ThreeVector randomLocalPosition(0, 0, 0);
            if (!GenerateRandomPointInTessellatedSolid(tessellatedSolid, randomLocalPosition))
            {
                G4cerr << "Failed to generate random position in tessellated solid!" << G4endl;
                exit(99);
            }

            // Move local position to 'global' position where this solid was placed
            //    1) Get the physical volume from the logical volume
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
            //    2) Transform our randomLocalPosition to physicalVolume position
            G4AffineTransform transform = GetTransformFromPhysicalVolume(physicalVolume);
            pos = transform.TransformPoint(randomLocalPosition);
        }
        else
        {
            G4cerr << "Unhandled solid type for scan volume. Must be type G4TessellatedSolid!" << G4endl;
            exit(99);
        }
    }
    break;
    case inLogicalVolume:
    {
        // Retrieve the logical volume with name 'sample'
        G4LogicalVolume* logicalVolume = G4LogicalVolumeStore::GetInstance()->GetVolume(m_logicalVolumeName);
        if (logicalVolume == nullptr)
        {
            G4cerr << "Volume with name '" << m_logicalVolumeName << "' not found! Define volume with this name in DetectorConstruction()." << G4endl;
            exit(99);
        }
        // Get the solid from the logical volume (with name 'sample')
        G4VSolid* solid = logicalVolume->GetSolid();
        if (G4Box* boxSolid = dynamic_cast<G4Box*>(solid))
        {
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

            G4double xHalfLength = boxSolid->GetXHalfLength();
            G4double yHalfLength = boxSolid->GetYHalfLength();
            G4double zHalfLength = boxSolid->GetZHalfLength();

            pos = G4ThreeVector(
                (G4UniformRand() * 2. - 1.) * xHalfLength,
                (G4UniformRand() * 2. - 1.) * yHalfLength,
                (G4UniformRand() * 2. - 1.) * zHalfLength);

            G4AffineTransform transform = GetTransformFromPhysicalVolume(physicalVolume);
            pos = transform.TransformPoint(pos);
        }
        else if (G4Tubs* tubsSolid = dynamic_cast<G4Tubs*>(solid))
        {
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

            G4double innerRadius = tubsSolid->GetInnerRadius();
            G4double outerRadius = tubsSolid->GetOuterRadius();
            G4double halfLengthZ = tubsSolid->GetZHalfLength();

            // Random point in cylinder
            G4double r = std::sqrt(G4UniformRand() * (outerRadius * outerRadius - innerRadius * innerRadius) + innerRadius * innerRadius);
            G4double phi = G4UniformRand() * CLHEP::twopi;
            G4double z = (G4UniformRand() * 2. - 1.) * halfLengthZ;

            pos = G4ThreeVector(r * std::cos(phi), r * std::sin(phi), z);

            G4AffineTransform transform = GetTransformFromPhysicalVolume(physicalVolume);
            pos = transform.TransformPoint(pos);
        }
        else
        {
            G4cerr << "Unhandled solid type for volume '" << m_logicalVolumeName << "'. Must be type G4Box!" << G4endl;
            exit(99);
        }
    }
    break;
    default:
        G4cerr << "Unhandled case for m_posMode in PrimaryGeneratorAction(2)" << G4endl;
        exit(99);
    }
    m_particleGun->SetParticlePosition(pos);


    // Generate direction (TODO: Must implement energy(?)/momentum)
    // ... and generate an event with given pos, momDirection (and energy TODO)
    // NOTE: In case of 'decay0' mode skip particle gun!
    switch (m_directionMode)
    {
    case directional:
        m_particleGun->SetParticleMomentumDirection(m_customDirection);
        m_particleGun->GeneratePrimaryVertex(anEvent);
        break;
    case isotropic:
        m_particleGun->SetParticleMomentumDirection(G4RandomDirection());
        m_particleGun->GeneratePrimaryVertex(anEvent);
        break;
    case decay0:
    {
        G4int currentEventId;
        G4String currentIsotopeName;
        std::vector<std::tuple<int,double,G4ThreeVector>> particlesToGenerate;
        if (!ParseDecayZeroFileSingleBlock(currentEventId, currentIsotopeName, particlesToGenerate)) {
            G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
                        "PG001", FatalException,
                        "End of decay0 file reached while more events requested.");
        }

        for (auto& currentParticle : particlesToGenerate)
        {
            int customPID = std::get<0>(currentParticle);
            auto momDir    = std::get<2>(currentParticle);
            if (momDir.mag2() == 0.) {
                // Avoid zero vector (give arbitrary direction)
                momDir = G4ThreeVector(0,0,1);
            }

            G4ParticleDefinition* pd = nullptr;
            if      (customPID == 3) pd = G4ParticleTable::GetParticleTable()->FindParticle(11);   // e-
            else if (customPID == 2) pd = G4ParticleTable::GetParticleTable()->FindParticle(-11);  // e+
            else if (customPID == 1) pd = G4ParticleTable::GetParticleTable()->FindParticle(22);   // gamma
            else {
                G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
                            "PG002", FatalException,
                            "Unknown particle ID in decay0 file (allowed 1,2,3).");
            }
            m_particleGun->SetParticleDefinition(pd);

            const G4double pMag = momDir.mag();      // momentum magnitude (assumed MeV/c units in file?)
            m_particleGun->SetParticleMomentum(pMag * MeV); // If file already in MeV/c this is OK; otherwise adjust
            m_particleGun->SetParticleMomentumDirection(momDir.unit());
            m_particleGun->SetParticleTime(std::get<1>(currentParticle) * CLHEP::second);

            m_particleGun->GeneratePrimaryVertex(anEvent);

            if (customPID == 1) {
                auto analysisManager = G4AnalysisManager::Instance();
                if (analysisManager && analysisManager->GetNtupleActivation(4)) { // guard
                    const auto& position  = m_particleGun->GetParticlePosition();
                    const auto& direction = m_particleGun->GetParticleMomentumDirection();
                    const G4double energy = m_particleGun->GetParticleEnergy();

                    analysisManager->FillNtupleDColumn(4,0, energy);
                    analysisManager->FillNtupleDColumn(4,1, direction.x());
                    analysisManager->FillNtupleDColumn(4,2, direction.y());
                    analysisManager->FillNtupleDColumn(4,3, direction.z());
                    analysisManager->FillNtupleDColumn(4,4, position.x());
                    analysisManager->FillNtupleDColumn(4,5, position.y());
                    analysisManager->FillNtupleDColumn(4,6, position.z());
                    analysisManager->AddNtupleRow(4);
                }
            }
        }
    }
    break;
    default:
        G4Exception("PrimaryGeneratorAction::GeneratePrimaries",
                    "PG003", FatalException,
                    "Unhandled direction mode.");
    }

    // Safe starting position in root file
    auto analysisManager = G4AnalysisManager::Instance();
        if (analysisManager->GetNofNtuples() > 0 && analysisManager->GetNtupleActivation(0)) {
            analysisManager->FillNtupleDColumn(0,0, pos.x()/CLHEP::cm);
            analysisManager->FillNtupleDColumn(0,1, pos.y()/CLHEP::cm);
            analysisManager->FillNtupleDColumn(0,2, pos.z()/CLHEP::cm);
            analysisManager->AddNtupleRow(0);
        }
    
}

void PrimaryGeneratorAction::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == m_cmdPosMode.get())
    {
        if (newValue == "inCylinder")
        {
            m_posMode = inCylinder;
            G4cout << "INFO: Set posMode to: 'inCylinder'" << G4endl;
        }
        else if (newValue == "inSphere")
        {
            m_posMode = inSphere;
            G4cout << "INFO: Set posMode to: 'inSphere'" << G4endl;
        }
        else if (newValue == "inRock")
        {
            m_posMode = inRock;
            G4cout << "INFO: Set posMode to: 'inRock'" << G4endl;
        }
        else if (newValue == "inLogicalVolume")
        {
            m_posMode = inLogicalVolume;
            G4cout << "INFO: Set posMode to: 'inLogicalVolume'" << G4endl;
        }
        else if (newValue == "point")
        {
            m_posMode = point;
            G4cout << "INFO: Set posMode to: 'point'" << G4endl;
        }
        else
        {
            G4cerr << "ERROR: Unhandled case for /generator/posMode -- ABORT" << G4endl;
            exit(99);
        }
    }
    else if (command == m_cmdSetLogicalVolumeName.get())
    {
        m_logicalVolumeName = newValue;
        G4cout << "INFO: Set logicalVolumeName to: '" << m_logicalVolumeName << "'" << G4endl;
    }
    else if (command == m_cmdDirectionMode.get())
    {
        if (newValue == "directional")
        {
            G4cout << "INFO: Set directionMode to: 'directional'"
                << "\n      Adjust direction with '/generator/setCustomDirection X Y Z'. Default is: (1,0,0)" << G4endl;
            m_directionMode = directional;
            // Close the decay0 file if one was requested before
            if (m_decayZeroInputFile.is_open())
            {
                G4cout << "      --- Clsoing previously requested decay0 file! --- " << G4endl;
                m_decayZeroInputFile.close();
            }
        }
        else if (newValue == "isotropic")
        {
            G4cout << "INFO: Set directionMode to: 'isotropic'" << G4endl;
            m_directionMode = isotropic;
            // Close the decay0 file if one was requested before
            if (m_decayZeroInputFile.is_open())
            {
                G4cout << "      --- Clsoing previously requested decay0 file! --- " << G4endl;
                m_decayZeroInputFile.close();
            }
        }
        else
        {
            G4cerr << "ERROR: Unhandled case for /generator/directionMode -- ABORT" << G4endl;
            exit(99);
        }
    }
    else if (command == m_cmdSetVolumeHeight.get())
    {
        m_sourceHeight = m_cmdSetVolumeHeight->GetNewDoubleValue(newValue);
        G4cout << "INFO: Set m_sourceHeight to: " << m_sourceHeight << G4endl;
    }
    else if (command == m_cmdSetVolumeDiameter.get())
    {
        m_sourceDiameter = m_cmdSetVolumeDiameter->GetNewDoubleValue(newValue);
        G4cout << "INFO: Set m_sourceDiameter to: " << m_sourceDiameter << G4endl;
    }
    else if (command == m_cmdSetVolumeOffset.get())
    {
        m_offsetOfSource = m_cmdSetVolumeOffset->GetNewDoubleValue(newValue);
        G4cout << "INFO: Set m_offsetOfSource to: " << m_offsetOfSource << G4endl;
    }
    else if (command == m_cmdSetCustomDirection.get())
    {
        m_customDirection = m_cmdSetCustomDirection->GetNew3VectorValue(newValue);
        G4cout << "INFO: Set m_cmdSetCustomDirection to: ("
            << m_customDirection.X << ", " << m_customDirection.Y << ", " << m_customDirection.Z << ")" << G4endl;
        if (m_directionMode != directional)
        {
            G4cout << "      Set directional to: 'isotropic'" << G4endl;
            m_directionMode = directional;
            // Close the decay0 file if one was requested before
            if (m_decayZeroInputFile.is_open())
            {
                G4cout << "      --- Clsoing previously requested decay0 file! --- " << G4endl;
                m_decayZeroInputFile.close();
            }
        }
    }
    else if (command == m_cmdUseDecay0File.get())
    {
        G4String fileName = newValue;
        if (fileName.empty())
        {
            G4cout << "INFO: Empty name passed to '/generator/setDecay0File'"
                << "\n      Set directionMode to: 'isotropic'" << G4endl;
            m_directionMode = isotropic;
            // Close the decay0 file if one was requested before
            if (m_decayZeroInputFile.is_open())
            {
                G4cout << "      --- Clsoing previously requested decay0 file! --- " << G4endl;
                m_decayZeroInputFile.close();
            }
        }
        else
        {
            // Try to open the corresponding text file
            m_decayZeroInputFile.open(fileName);
            if (m_decayZeroInputFile)
            {
                G4cout << "INFO: Use decay0 file'" << fileName << "' as input. Disable other direction modes!" << G4endl;
                m_directionMode = decay0;
            }
            else
            {
                G4cout << "*************************************************************************" << G4endl;
                G4cout << " WARNING: Provided decay zero file '" << fileName << "' cannot be found!" << G4endl;
                G4cout << "          Fall back to 'isotropic' directional mode!" << G4endl;
                G4cout << "*************************************************************************" << G4endl;
                m_directionMode = directional;
            }
        }

    }
    else
    {
        G4cerr << "ERROR: Unhandled command in PrimaryGeneratorAction::SetNewValue() -- ABORT" << G4endl;
        exit(99);
    }
}

