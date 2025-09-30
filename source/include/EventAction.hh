/// \file EventAction.hh
/// \brief Definition of the EventAction class

#ifndef EventAction_hh
#define EventAction_hh 1

#include "G4UserEventAction.hh"
#include "G4THitsMap.hh"

#include "DetectorResponse.hh"

/// Event action class

class EventAction : public G4UserEventAction
{
public:
    EventAction() {}
    ~EventAction() override {}

    void  BeginOfEventAction(const G4Event* /*event*/) override {}
    void    EndOfEventAction(const G4Event* event) override;

private:

    G4THitsMap<G4double>* GetHitsCollection(G4int hcID,
        const G4Event* event) const;

    G4double GetSum(G4THitsMap<G4double>* hitsMap) const;

    DetectorResponse m_detectorResponse;

    G4int m_HPGeHCID = -1;
};

#endif

