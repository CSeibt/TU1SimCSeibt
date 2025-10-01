/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"
#include <random>
#include <ctime>


RunAction::RunAction(bool isMaster) : G4UserRunAction(), fIsMaster(isMaster)
{
    // Define command to change output file name
    {
        m_cmdFileName = make_shared<G4UIcmdWithAString>("/run/fileName", this);
        m_cmdFileName->SetParameterName("file name", false);
    }

    // Create/get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // Merge ntuples (= in case of multithreading, merge to single output file)
    analysisManager->SetNtupleMerging(true);
    analysisManager->SetNtupleRowWise(true);  // Enable row-wise ntuple merging

    // Creating histograms 
    analysisManager->CreateH1("Egamma", "Egamma Events", 200000, 0, 20);

    // one histogram storing all energy depositions (including zero)
    analysisManager->CreateH1("EdepAll", "Edep All Events", 200000, 0, 20);

    // one histogram storing only "visible" events with non-zero energy deposition
    analysisManager->CreateH1("Edep", "Edep Events", 200000, 0, 20);

    // one histogram storing only "visible" events with non-zero visible energy deposition
    analysisManager->CreateH1("Evis", "Evis Events", 200000, 0, 20);
        // Creating ntuple

    // Ntuple 0: Ions (PrimaryGeneratorAction)
    analysisManager->CreateNtuple("ions", "Primary ions");
    analysisManager->CreateNtupleDColumn("posX");
    analysisManager->CreateNtupleDColumn("posY");
    analysisManager->CreateNtupleDColumn("posZ");
    analysisManager->FinishNtuple();

    // Ntuple 1: Gammas (TrackingAction)
    analysisManager->CreateNtuple("gammas", "Decay gammas");
    analysisManager->CreateNtupleDColumn("Egamma");
    analysisManager->CreateNtupleDColumn("GammaDirX");
    analysisManager->CreateNtupleDColumn("GammaDirY");
    analysisManager->CreateNtupleDColumn("GammaDirZ");
    analysisManager->CreateNtupleDColumn("posX");
    analysisManager->CreateNtupleDColumn("posY");
    analysisManager->CreateNtupleDColumn("posZ");
    analysisManager->FinishNtuple();

    // Ntuple 2: Gammas (EventAction)
    analysisManager->CreateNtuple("Detector", "Detector events");
    analysisManager->CreateNtupleDColumn("Edep");
    analysisManager->CreateNtupleDColumn("Evis");
    analysisManager->FinishNtuple();

    // Ntuple 3: Gammas (EventAction)
    analysisManager->CreateNtuple("angle", "angle  events");
    analysisManager->CreateNtupleDColumn("angle");
    analysisManager->FinishNtuple();

    // Ntuple 4: Gammas (TrackingAction)
    analysisManager->CreateNtuple("decay0", "Decay0 gammas");
    analysisManager->CreateNtupleDColumn("Egamma");
    analysisManager->CreateNtupleDColumn("GammaDirX");
    analysisManager->CreateNtupleDColumn("GammaDirY");
    analysisManager->CreateNtupleDColumn("GammaDirZ");
    analysisManager->CreateNtupleDColumn("posX");
    analysisManager->CreateNtupleDColumn("posY");
    analysisManager->CreateNtupleDColumn("posZ");
    analysisManager->FinishNtuple();
    

}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
    auto analysisManager = G4AnalysisManager::Instance();

    // Open file for ALL threads (Geant4 handles merging automatically)
    analysisManager->OpenFile(m_fileName);
    G4cout << "Using " << analysisManager->GetType() << G4endl;




// Debug: Verify ntuple creation
    G4cout << "Thread " << G4Threading::G4GetThreadId() << ": Created " 
           << analysisManager->GetNofNtuples() << " ntuples" << G4endl;

    // Handle RNG seeding
    if (fIsMaster)
    {
        // Seed RNG only in master
        long seeds[2];
        seeds[0] = std::time(nullptr);
        std::mt19937 rng(static_cast<unsigned int>(seeds[0]));
        std::uniform_int_distribution<long> uni(1, 1000000000);
        seeds[1] = seeds[0] - uni(rng);
        G4Random::setTheSeeds(seeds);
    }
    else
    {
        // Worker threads: set per-thread seeds
        auto tid = G4Threading::G4GetThreadId();
        std::random_device rd;
        auto seedBase = rd() ^ (std::uint64_t)std::time(nullptr);
        G4Random::setTheSeed((long)(seedBase + 1013904223ULL * (tid + 1)));
    }

    // Print status (all threads)
    G4Random::showEngineStatus();
    G4cout << G4endl;
}

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{

    auto analysisManager = G4AnalysisManager::Instance();

    // Debug: Check what we have before writing
    G4cout << "Thread " << G4Threading::G4GetThreadId() << ": EndOfRunAction" << G4endl;
    G4cout << "  Number of H1s: " << analysisManager->GetNofH1s() << G4endl;
    G4cout << "  Number of Ntuples: " << analysisManager->GetNofNtuples() << G4endl;
    
    for (int i = 0; i < analysisManager->GetNofNtuples(); ++i) {
        G4cout << "  Ntuple " << i << " entries: " << analysisManager->GetNtupleActivation(i) << G4endl;
    }

    // Write and close for ALL threads (Geant4 handles merging)
    analysisManager->Write();
    analysisManager->CloseFile();
    
    if (fIsMaster) {
        G4Random::showEngineStatus();
    }
}


void RunAction::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == static_cast<G4UIcommand*>(m_cmdFileName.get()))
    {
        m_fileName = newValue;
    }
    else
    {
        G4cerr << "Unhandled command in RunAction::SetNewValue()" << G4endl;
        exit(99);
    }
    return;
}
