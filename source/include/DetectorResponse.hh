/// \file DetectorResponse.hh
/// \brief Definition of the DetectorResponse class

/// \details This is an auxiliary class for the (HPGe) detector response. It
///          for energy resolution and trigger threshold.
///
///          Energy resolution is calculated as:
///             (sigma/MeV) = (A0 + A1*(E/MeV) + A2*(E/MeV)^2)^0.5
///
///          Trigger threshold is modeled by assigning a trigger probability of
///              1.0 / (1 + exp((Evis-m_trigThreshold) / (m_trigWidth)))
///          with Evis as the 'visible' energy (after applying resolution).
///
///          Parameters can be set with maccro commands
///            /detectorResponse/setA0 [double]
///            /detectorResponse/setA1 [double]
///            /detectorResponse/setA2 [double]
///            /detectorResponse/setTriggerThreshold [energy]
///            /detectorResponse/setTriggerWidth [energy]


#ifndef DetectorResponse_hh
#define DetectorResponse_hh

#include "G4UImessenger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "CLHEP/Units/SystemOfUnits.h"

#include <memory>
using std::shared_ptr;
using std::make_shared;

class DetectorResponse : public G4UImessenger
{
public:
    DetectorResponse();
    ~DetectorResponse() {}

    G4double EnergyResponse(G4double Edep);

private:
    G4double m_resA0 = 0.0005*0.0005;
    G4double m_resA1 = 1.07e-6;
    G4double m_resA2 = 0.0;

    G4double m_trigThreshold = 0.020 * CLHEP::MeV;
    G4double m_trigWidth     = 0.005 * CLHEP::MeV;

public:
    void SetNewValue(G4UIcommand* command, G4String newValue) override;

private:
    shared_ptr<G4UIcmdWithADouble> m_cmdSetA0, m_cmdSetA1, m_cmdSetA2;
    shared_ptr<G4UIcmdWithADoubleAndUnit> m_cmdSetTrigThreshold, m_cmdSetTrigWidth;
};

#endif
