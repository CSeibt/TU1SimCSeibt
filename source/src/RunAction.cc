/// \file RunAction.cc
/// \brief Implementation of the RunAction class

#include "RunAction.hh"

#include "G4AnalysisManager.hh"
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include <random>
#include <ctime>


RunAction::RunAction()
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

    // Creating histograms
    {
		 // one histogram storing all initally emitted gammas 
        analysisManager->CreateH1("Egamma", "Egamma Events", 200000, 0, 20);
        
        // one histogram storing all energy depositions (including zero)
        analysisManager->CreateH1("EdepAll", "Edep All Events", 200000, 0, 20);

        // one histogram storing only "visible" events with non-zero energy deposition
        analysisManager->CreateH1("Edep", "Edep Events", 200000, 0, 20);

        // one histogram storing only "visible" events with non-zero visible energy deposition
        analysisManager->CreateH1("Evis", "Evis Events", 200000, 0, 20);
        
       
    }

    // Creating ntuple
    //
    {
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

        // Ntuple 1: Gammas (EventAction)
        analysisManager->CreateNtuple("Detector", "Detector events");
        analysisManager->CreateNtupleDColumn("Edep");
        analysisManager->CreateNtupleDColumn("Evis");
        analysisManager->FinishNtuple();

        // Ntuple 1: Gammas (EventAction)
        analysisManager->CreateNtuple("angle", "angle  events");
        analysisManager->CreateNtupleDColumn("angle");
        analysisManager->FinishNtuple();

        // Ntuple 1: Gammas (TrackingAction)
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
}

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{
    // Get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    analysisManager->OpenFile(m_fileName);
    G4cout << "Using " << analysisManager->GetType() << G4endl;

    // Always(?) choose a 'random' randomSeed
    if (true)
    {
        G4cout << "*******************"
            << "\n*** AUTOSEED ON ***"
            << "\n*******************" << G4endl;
        long seeds[2];

        // New rng generator using current time to generate seeds:
        seeds[0] = std::time(nullptr);
        std::mt19937 rng(static_cast<unsigned int>(seeds[0]));
        std::uniform_int_distribution<long> uni(1, 1000000000);
        seeds[1] = seeds[0] - uni(rng);
#ifdef G4MULTITHREADED
        seeds[1] += G4Threading::G4GetThreadId();
#endif
        /*
        std::random_device rd;     // only used once to initialise (seed) engine
        std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
        std::uniform_int_distribution<int> uni(1, 1000000000); // guaranteed unbiased
        seeds[0] = uni(rng);
        seeds[1] = uni(rng);
        */
        G4Random::setTheSeeds(seeds);
    }

    // Print the random engine status?
    if (true)
    {
        G4Random::showEngineStatus();
        G4cout << G4endl;
    }

    // Save Random Engine Status?
    if (false)
    {
        // G4int runNumber = run->GetRunID();
        std::ostringstream fileName;
        fileName << "currentRandomEngine.rndm";
        G4cout << "**********************"
            << "\n*** RANDOM SAVE ON ***"
            << "\n**********************"
            << G4endl;
        G4Random::saveEngineStatus(fileName.str().c_str());
    }
}

void RunAction::EndOfRunAction(const G4Run* /*run*/)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
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
}
