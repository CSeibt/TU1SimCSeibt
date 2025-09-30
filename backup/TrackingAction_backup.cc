#include "TrackingAction.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <cmath>  // Für cos, acos, sqrt

TrackingAction::TrackingAction()
    : f_useExternalDecayData(false), f_currentDecayIndex(0),
      fUseExternalDecayDataCmd(nullptr), fTrackingDir(nullptr)
{
    fTrackingDir = new G4UIdirectory("/trackingAction/");
    fTrackingDir->SetGuidance("Commands for tracking action");

    fUseExternalDecayDataCmd = new G4UIcmdWithABool("/trackingAction/setUseExternalDecayData", this);
    fUseExternalDecayDataCmd->SetGuidance("Use external decay data");
    fUseExternalDecayDataCmd->SetParameterName("useData", true);
    fUseExternalDecayDataCmd->SetDefaultValue(false);
    fUseExternalDecayDataCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

TrackingAction::~TrackingAction()
{
    delete fUseExternalDecayDataCmd;
    delete fTrackingDir;
}

void TrackingAction::SetUseExternalDecayData(G4bool useData)
{
    f_useExternalDecayData = useData;

    if (useData && f_decayData.empty()) {
        std::ifstream inputFile("../../decay0/genCo60.txt");
        if (!inputFile) {
            G4cerr << "Error opening decay data file!" << G4endl;
            return;
        }

        std::string line;
        while (std::getline(inputFile, line)) {
            if (line.empty()) continue;

            std::stringstream ss(line);
            std::string eventID;
            double decayTime;
            std::string nuclideName;

            ss >> eventID >> decayTime >> nuclideName;
            std::getline(inputFile, line);
            int numParticles = std::stoi(line);

            std::vector<std::tuple<int, double, G4ThreeVector>> particles;
            for (int i = 0; i < numParticles; ++i) {
                std::getline(inputFile, line);
                std::stringstream particleSS(line);

                int particleId;
                double decayTime;
                double px, py, pz;

                particleSS >> particleId >> decayTime >> px >> py >> pz;
                particles.push_back(std::make_tuple(particleId, decayTime, G4ThreeVector(px, py, pz)));
            }

            f_decayData.push_back(particles);
        }
    }
}

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
    G4String particleName = track->GetDefinition()->GetParticleName();
    G4double energy = track->GetKineticEnergy();

    if (particleName == "gamma" && track->GetParentID() > 0)
    {
        if (track->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay")
        {
            auto analysisManager = G4AnalysisManager::Instance();
            G4ThreeVector position = track->GetPosition();
            G4ThreeVector direction = track->GetMomentumDirection();

            if (f_useExternalDecayData) {
                // Wenn ParentID == 0, starten wir mit eventID = 0
                G4int eventID = (track->GetParentID() == 0) ? f_currentDecayIndex : track->GetParentID();

                // Überprüfe, ob der eventID gültig ist (innerhalb der verfügbaren Zerfallsdaten)
                if (eventID >= f_decayData.size()) {
                    G4cerr << "Invalid eventID: " << eventID << ", resetting to 0" << G4endl;
                    f_currentDecayIndex = 0;  // Zurücksetzen auf den ersten Index, wenn das Ende erreicht ist
                    eventID = 0;
                }

                auto& decayEvent = f_decayData[eventID];

                int gammaCount = 0;
                G4ThreeVector firstGammaDir, secondGammaDir;
                bool angleCalculated = false;  // Flag, um sicherzustellen, dass der Winkel nur einmal berechnet wird

                // Durchlaufe die Partikel dieses Zerfallsereignisses
                for (auto& particle : decayEvent) {
                    int particleId = std::get<0>(particle);
                    G4ThreeVector momentum = std::get<2>(particle);

                    if (particleId == 1) {  // Nur für Gamma-Teilchen
                        G4ThreeVector dir = momentum.unit();

                        // Wenn der Winkel noch nicht berechnet wurde, berechne ihn
                        if (gammaCount == 0) {
                            firstGammaDir = dir;
                        } else if (gammaCount == 1 && !angleCalculated) {
                            secondGammaDir = dir;

                            // Berechne den Winkel zwischen den beiden Gamma-Teilchen
                            G4double cosTheta = firstGammaDir.x() * secondGammaDir.x() +
                                                firstGammaDir.y() * secondGammaDir.y() +
                                                firstGammaDir.z() * secondGammaDir.z();

                            // Klammer den Wert in den Bereich [-1, 1] ein
                            cosTheta = std::min(std::max(cosTheta, -1.0), 1.0);
                            G4double theta = std::acos(cosTheta) * (180.0 / M_PI);  // Winkel in Grad
							G4cerr << "_____________________________" << G4endl;
							G4cerr << "eventID: " << eventID << G4endl;
                            G4cout << "Gamma-Gamma-Winkel: " << theta << " Grad" << G4endl;
                            G4cout << "Gamma 1  " << firstGammaDir  << G4endl;
                            G4cout << "Gamma 2  " << secondGammaDir  << G4endl;

                            // Speichere den Winkel
                            analysisManager->FillNtupleDColumn(3, 0, theta);
                            analysisManager->AddNtupleRow(3);

                            // Setze das Flag, um sicherzustellen, dass der Winkel nur einmal berechnet wird
                            angleCalculated = true;
                        }

                        gammaCount++;

                        // Speichern der Partikel- und Positionsdaten
                        analysisManager->FillNtupleDColumn(1, 0, energy);
                        analysisManager->FillNtupleDColumn(1, 1, dir.x());
                        analysisManager->FillNtupleDColumn(1, 2, dir.y());
                        analysisManager->FillNtupleDColumn(1, 3, dir.z());
                        analysisManager->FillNtupleDColumn(1, 4, position.x());
                        analysisManager->FillNtupleDColumn(1, 5, position.y());
                        analysisManager->FillNtupleDColumn(1, 6, position.z());
                        analysisManager->AddNtupleRow(1);
                    }
                }

                // Erhöhe den Index für das nächste Ereignis
                f_currentDecayIndex = (f_currentDecayIndex + 1) % f_decayData.size();  // Zyklisches Durchlaufen
            } else {
                analysisManager->FillNtupleDColumn(1, 0, energy);
                analysisManager->FillNtupleDColumn(1, 1, direction.x());
                analysisManager->FillNtupleDColumn(1, 2, direction.y());
                analysisManager->FillNtupleDColumn(1, 3, direction.z());
                analysisManager->FillNtupleDColumn(1, 4, position.x());
                analysisManager->FillNtupleDColumn(1, 5, position.y());
                analysisManager->FillNtupleDColumn(1, 6, position.z());
                analysisManager->AddNtupleRow(1);
            }
        }
    }
}



void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
    // Nachverarbeitung, wenn das Teilchen die Verfolgung beendet hat
}

void TrackingAction::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == fUseExternalDecayDataCmd) {
        SetUseExternalDecayData(fUseExternalDecayDataCmd->GetNewBoolValue(newValue));
    }
}
