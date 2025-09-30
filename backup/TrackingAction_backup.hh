#ifndef TRACKINGACTION_HH
#define TRACKINGACTION_HH

#include "G4UserTrackingAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIdirectory.hh"
#include "G4UImessenger.hh"  // Hier den Header für G4UImessenger einbinden

class TrackingAction : public G4UserTrackingAction, public G4UImessenger {
public:
    TrackingAction();
    ~TrackingAction() override;

    void PreUserTrackingAction(const G4Track*) override;
    void PostUserTrackingAction(const G4Track*) override;

    void SetUseExternalDecayData(G4bool useData);
    void SetNewValue(G4UIcommand* cmd, G4String newValue) override;

private:
    G4bool f_useExternalDecayData;
    size_t f_currentDecayIndex;
    std::vector<std::vector<std::tuple<int, double, G4ThreeVector>>> f_decayData;

    // UI-Kommandos direkt hier
    G4UIcmdWithABool* fUseExternalDecayDataCmd;
    G4UIdirectory* fTrackingDir;
};

#endif // TRACKINGACTION_HH

