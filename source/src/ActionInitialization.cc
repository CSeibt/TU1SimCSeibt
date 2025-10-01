/// \file ActionInitialization.cc
/// \brief Implementation of the ActionInitialization class

#include "ActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "StackingAction.hh"
#include "TrackingAction.hh"
#include "DetectorResponse.hh"

void ActionInitialization::BuildForMaster() const
{
    SetUserAction(new RunAction(true));
}

void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGeneratorAction);
    SetUserAction(new RunAction(false));
    SetUserAction(new EventAction);
    SetUserAction(new StackingAction);
    SetUserAction(new TrackingAction);
}
