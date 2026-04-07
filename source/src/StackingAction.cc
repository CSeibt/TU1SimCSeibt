/// \file StackingAction.cc
/// \brief Implementation of the StackingAction class

#include "StackingAction.hh"

#include "G4Track.hh"
#include "G4GenericIon.hh"
#include "G4ProcessManager.hh"
#include "G4VProcess.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"


G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* track)
{
    //if (track->GetTotalEnergy() < 5*CLHEP::keV) return fKill;
    if (!m_pRadioactiveDecay)
    {
        // search for radioactive decay process
        auto ion = G4GenericIon::Definition();
        auto pman = ion->GetProcessManager();

        auto pvec = pman->GetProcessList();
        const auto pvecLength = pman->GetProcessListLength();

        for (int iProcess = 0; iProcess < pvecLength; iProcess += 1)
        {
            auto process = (*pvec)[iProcess];
            if (process->GetProcessName() == "RadioactiveDecay")
            {
                m_pRadioactiveDecay = process;
                break;
            }
        }

        if (!m_pRadioactiveDecay)
        {
            G4cerr << "Failed to retrieve radioactive decay process in StackingAction!" << G4endl;
            exit(99);
        }


    }

    // We want to avoid the decay of daughter nuclei, i.e. only track first radioactive decay
    // until daughter is in ground state. Then kill daughter.

    if (track->GetCreatorProcess() == m_pRadioactiveDecay
            && track->GetParentID() != 0 // not the primary
       )
    {
        auto particleDefinition = track->GetParticleDefinition();
        if (!m_ionTable.IsIon(particleDefinition))
        {
            // not an ion
            return fUrgent;
        }

        if (static_cast<const G4GenericIon*>(particleDefinition)->GetExcitationEnergy() == 0)
        {
            // secondary ion with zero excitation energy
            return fKill;
        }
    }

    

    return fUrgent;
}
