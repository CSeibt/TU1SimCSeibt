#ifndef TRACKINGACTION_HH
#define TRACKINGACTION_HH

#include "G4UserTrackingAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"  // Hier den Header für G4UImessenger einbinden

class TrackingAction : public G4UserTrackingAction {
public:
    TrackingAction();
    ~TrackingAction() override;

    void PreUserTrackingAction(const G4Track*) override;
    void PostUserTrackingAction(const G4Track*) override;
};

#endif // TRACKINGACTION_HH