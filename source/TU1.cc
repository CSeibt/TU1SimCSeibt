/// \file TU1.cc
/// \brief Geant4 simulation of TU1 detector for gamma-rays

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"
#include "G4SteppingVerbose.hh"
#include "G4UIcommand.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"
#include "G4PhysListFactory.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4DecayPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4RootAnalysisManager.hh"


namespace
{
    void PrintUsage()
    {
        G4cerr << " Usage: " << G4endl;
        G4cerr << " exampleB4d [-m macro ] [-u UIsession] [-t nThreads] [-vDefault]"
            << G4endl;
        G4cerr << "   note: -t option is available only for multi-threaded mode."
            << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc, char** argv)
{
    // Evaluate command line arguments
    //
    if (argc > 7)
    {
        PrintUsage();
        return 1;
    }

    G4String macro;
    G4String session;
    G4bool verboseBestUnits = true;
#ifdef G4MULTITHREADED
    G4int nThreads = 0;
#endif
    for (G4int i = 1; i < argc; i = i + 2)
    {
        if (G4String(argv[i]) == "-m") macro = argv[i + 1];
        else if (G4String(argv[i]) == "-u") session = argv[i + 1];
#ifdef G4MULTITHREADED
        else if (G4String(argv[i]) == "-t")
        {
            nThreads = G4UIcommand::ConvertToInt(argv[i + 1]);
        }
#endif
        else if (G4String(argv[i]) == "-vDefault")
        {
            verboseBestUnits = false;
            --i;  // this option is not followed with a parameter
        }
        else
        {
            PrintUsage();
            return 1;
        }
    }

    // Detect interactive mode (if no macro provided) and define UI session
    //
    G4UIExecutive* ui = nullptr;
    if (!macro.size())
    {
        ui = new G4UIExecutive(argc, argv, session);
    }

    // Optionally: choose a different Random engine...
    // NOTE: Seed is set in RunAction.cc and based on linux <random>
    G4Random::setTheEngine(new CLHEP::RanecuEngine);

    // Use G4SteppingVerboseWithUnits
    if (verboseBestUnits)
    {
        G4int precision = 4;
        G4SteppingVerbose::UseBestUnit(precision);
    }

    // Construct the default run manager
    //
    auto* runManager =
        G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
#ifdef G4MULTITHREADED
    if (nThreads > 0)
    {
        G4cout << "===== Multi-threaded mode with " << nThreads << " threads =====" << G4endl;
        runManager->SetNumberOfThreads(nThreads);
        G4cout << " " << G4endl;
    }
#endif

    // Detector construction
    //
    {
        auto detConstruction = new DetectorConstruction();
        runManager->SetUserInitialization(detConstruction);
    }

    // Define physics list
    {
        // Define physics list
        auto factory = new G4PhysListFactory();
        auto physList = factory->GetReferencePhysList("FTFP_BERT_LIV");

        // Add necessary physics for decay processes
        // physList->RegisterPhysics(new G4DecayPhysics); // General decay physics
        physList->RegisterPhysics(new G4RadioactiveDecayPhysics); // Radioactive decay physics
       // physList->RegisterPhysics(new G4EmStandardPhysics); // 

        runManager->SetUserInitialization(physList);
    }

    // action initialization
    {
        auto actionInitialization = new ActionInitialization();
        runManager->SetUserInitialization(actionInitialization);
    }

    // Initialize visualization
    auto visManager = new G4VisExecutive;
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    auto UImanager = G4UImanager::GetUIpointer();
    // Apply some settings
    UImanager->ApplyCommand("/control/macroPath ../mac/");
    UImanager->ApplyCommand("/control/execute preInit.mac");

    // Process macro or start UI session
    //
    if (macro.size())
    {
        // batch mode
        G4String command = "/control/execute ";
        UImanager->ApplyCommand(command + macro);
    }
    else
    {
        // interactive mode : define UI session
        UImanager->ApplyCommand("/control/execute defaultGeometryInit.mac");
        UImanager->ApplyCommand("/control/execute vis.mac");
        UImanager->ApplyCommand("/control/execute defaultRun.mac");
        ui->SessionStart();
        
        delete ui;
    }


    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !

    delete visManager;
    delete runManager;
}
