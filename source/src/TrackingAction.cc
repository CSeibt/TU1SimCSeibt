#include "TrackingAction.hh"
#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4ios.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4ThreeVector.hh"
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <cmath>

// Vectors to safe the directions of 2 gammas
static thread_local std::map<G4int, G4ThreeVector> gamma1Directions;
static thread_local std::map<G4int, G4ThreeVector> gamma2Directions;
static thread_local std::map<G4int, G4int> gamma1PrimaryID;

double roundToStep(double val, double step = 1e-6) {
	return std::round(val / step) * step;
}

TrackingAction::TrackingAction()
{
}

TrackingAction::~TrackingAction()
{
}

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
	G4String particleName = track->GetDefinition()->GetParticleName();
	
	G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	G4int parentID = track->GetParentID();
	
	if (particleName == "gamma" && parentID > 0)
	{
		//G4cout << "_______________________ " << G4endl;
		//G4cout << "eventID 1 " << eventID << G4endl;
		//G4cout << "parentID 1 " << parentID << G4endl;
		//G4cout << "trackid 1 " << track->GetTrackID() << G4endl;
		//G4cout << "particle name  " << track->GetDefinition()->GetParticleName() << G4endl;
		//G4cout << "gamma energy  " << track->GetKineticEnergy() << G4endl;


		if (track->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay")
		{
			auto analysisManager = G4AnalysisManager::Instance();
			G4ThreeVector position = track->GetPosition();
			G4ThreeVector direction = track->GetMomentumDirection();
			G4double energy = track->GetKineticEnergy();

			// rounded because for point sources in 0 0 z position slightly negative values close to zero are not shown in position histograms
			analysisManager->FillH1(0, energy / CLHEP::MeV); // all energy depositions
			analysisManager->FillNtupleDColumn(1, 0, energy);
			analysisManager->FillNtupleDColumn(1, 1, direction.x());
			analysisManager->FillNtupleDColumn(1, 2, direction.y());
			analysisManager->FillNtupleDColumn(1, 3, direction.z());
			analysisManager->FillNtupleDColumn(1, 4, position.x());
			analysisManager->FillNtupleDColumn(1, 5, position.y());
			analysisManager->FillNtupleDColumn(1, 6, position.z());
			//G4cout << "position.x() " << roundToStep(position.x()) << G4endl;
			//G4cout << "position.y() " << roundToStep(position.y()) << G4endl;
			//G4cout << "position.z() " << roundToStep(position.z()) << G4endl;
			//G4cout << "track ID gamma " << track->GetTrackID() << G4endl;
			analysisManager->AddNtupleRow(1);

			//in case of 2 gammas from one decay, the angle between gammas is calculated and printed
			if(false)
			{
				// Nur speichern, wenn Slot noch frei
				if (parentID == 2 & gamma1Directions.find(eventID) == gamma1Directions.end())
				{
					gamma1Directions[eventID] = direction;
					gamma1PrimaryID[eventID] = parentID;
					//G4cout << "hello " << G4endl;
					//G4cout << "eventID 1 " << eventID << G4endl;
					//G4cout << "parentID 1 " << parentID << G4endl;
					//G4cout << "trackid 1 " << track->GetTrackID() << G4endl;
					//G4cout << "_______________________ " << G4endl;
					//G4cout << "gamma energy 1 " << track->GetKineticEnergy() << G4endl;
				}
				else if (track->GetKineticEnergy()>1331 * CLHEP::keV & gamma2Directions.find(eventID) == gamma2Directions.end())
				{
				
					// Vergleich der Prim�rteilchen-IDs
					//if (parentID == gamma1PrimaryID[eventID])
					{
						
						//G4cout << "eventID 2 " << eventID << G4endl;
						//G4cout << "parentID 2 " << parentID << G4endl;
						//G4cout << "trackid 2 " << track->GetTrackID() << G4endl;
					//	G4cout << "gamma energy 2 " << track->GetKineticEnergy() << G4endl;

						gamma2Directions[eventID] = direction;

						const G4ThreeVector& dir1 = gamma1Directions[eventID];
						const G4ThreeVector& dir2 = gamma2Directions[eventID];
						
						 // Normiere zur Sicherheit (falls nicht schon normiert)
                        G4ThreeVector v1 = dir1.unit();
                        G4ThreeVector v2 = dir2.unit();

                        // Berechne den Winkel zwischen den beiden Vektoren (immer 0��180�)
                        G4double angleRad = std::acos(v1.dot(v2));
                     

						/*const G4ThreeVector normal = G4ThreeVector(0, 0, 1);

						G4ThreeVector cross = dir1.cross(dir2);
						G4double dot = dir1.dot(dir2);

						G4double angleRad = std::atan2(cross.mag(), dot);

						// Vorzeichen des Winkels �ber Normalenvektor
						if (normal.dot(cross) < 0) {
							angleRad = 2.0 * CLHEP::pi - angleRad;
						}*/

						G4double angleDeg = angleRad * (180.0 / CLHEP::pi);
						//G4cout << "Event " << currentEventId << ": Angle = " << angleDeg << "�" << G4endl;

						auto analysisManager = G4AnalysisManager::Instance();
						analysisManager->FillNtupleDColumn(3, 0, angleDeg);
						analysisManager->AddNtupleRow(3);
					}
				}
			}
		}
	}
}

void TrackingAction::PostUserTrackingAction(const G4Track* track)
{
}
