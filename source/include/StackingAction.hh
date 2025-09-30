/// \file StackingAction.hh
/// \brief Definition of the StackingAction class

#ifndef StackingAction_h
#define StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "G4IonTable.hh"
#include "globals.hh"

class G4VProcess;

class StackingAction : public G4UserStackingAction
{
public:
    StackingAction() {}
    ~StackingAction() {}

    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);

private:
    G4IonTable m_ionTable;
    G4VProcess* m_pRadioactiveDecay = nullptr;
};

#endif

