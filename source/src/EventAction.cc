/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"

#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"

G4THitsMap<G4double>* EventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
    auto hitsCollection
        = static_cast<G4THitsMap<G4double>*>
        (event->GetHCofThisEvent()->GetHC(hcID));

    if (!hitsCollection)
    {
        G4ExceptionDescription msg;

        msg << "Cannot access hitsCollection ID " << hcID;

        G4Exception("EventAction::GetHitsCollection()",
            "MyCode0003", FatalException, msg);
    }

    return hitsCollection;
}


G4double EventAction::GetSum(G4THitsMap<G4double>* hitsMap) const
{
    G4double sumValue = 0.0;
    for (auto it : *hitsMap->GetMap())
    {
        // hitsMap->GetMap() returns map of type std::map<G4int, G4double*>
        sumValue += *(it.second);
    }
    return sumValue;
}



void EventAction::EndOfEventAction(const G4Event* event)
{
    // Get hist collections IDs
    if (m_HPGeHCID == -1)
    {
        m_HPGeHCID = G4SDManager::GetSDMpointer()->GetCollectionID("HPGe/Edep");
    }


    //HitsMap Shenanigans and mischief
    /*
    std::cout<<"HALLO DU DA"<<std::endl;
    auto meineMAP = GetHitsCollection(m_HPGeHCID, event)->GetMap();

    for (auto hi : *meineMAP)
    {
        std::cout<<hi.first<<" .. "<<*(hi.second)<<std::endl;
    }
    */


    // Get sum values from hits collections
    auto Edep = GetSum(GetHitsCollection(m_HPGeHCID, event));
    auto Evis = m_detectorResponse.EnergyResponse(Edep);

    // get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // fill histograms
    analysisManager->FillH1(1, Edep / CLHEP::MeV); // all energy depositions

    if (Edep != 0)
    {
        analysisManager->FillH1(2, Edep / CLHEP::MeV); // non-zero energy depositions
    }

    if (Evis != 0)
    {
        analysisManager->FillH1(3, Evis / CLHEP::MeV); // non-zero (visible) energy depositions
    }

    // fill ntuple
    //
    analysisManager->FillNtupleDColumn(2, 0, Edep / CLHEP::MeV);
    analysisManager->FillNtupleDColumn(2, 1, Evis / CLHEP::MeV);
    analysisManager->AddNtupleRow(2);
}

