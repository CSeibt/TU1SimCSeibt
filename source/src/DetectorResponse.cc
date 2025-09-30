#include "DetectorResponse.hh"

#include "Randomize.hh"

DetectorResponse::DetectorResponse()
{
    m_cmdSetA0 = make_shared<G4UIcmdWithADouble>("/detectorResponse/setA0", this);
    m_cmdSetA0->SetParameterName("A0", false);

    m_cmdSetA1 = make_shared<G4UIcmdWithADouble>("/detectorResponse/setA1", this);
    m_cmdSetA1->SetParameterName("A1", false);

    m_cmdSetA2 = make_shared<G4UIcmdWithADouble>("/detectorResponse/setA2", this);
    m_cmdSetA2->SetParameterName("A2", false);

    m_cmdSetTrigThreshold = make_shared<G4UIcmdWithADoubleAndUnit>("/detectorResponse/setTriggerThreshold", this);
    m_cmdSetTrigThreshold->SetParameterName("threshold", false);

    m_cmdSetTrigWidth = make_shared<G4UIcmdWithADoubleAndUnit>("/detectorResponse/setTriggerWidth", this);
    m_cmdSetTrigWidth->SetParameterName("width", false);
}

G4double DetectorResponse::EnergyResponse(G4double Edep)
{
    if (Edep <= 0)
    {
        return 0.0;
    }

    G4double variance = m_resA0 + m_resA1 * (Edep / CLHEP::MeV) + m_resA2 * (Edep*Edep) / (CLHEP::MeV * CLHEP::MeV);
    G4double Evis = CLHEP::RandGauss::shoot(Edep / CLHEP::MeV, sqrt(variance));

    if (Evis <= 0)
    {
        return 0.0;
    }

    G4double trigger = 1.0 / (1 + exp((Evis-m_trigThreshold) / (m_trigWidth)));
    if (G4RandFlat::shoot() > trigger)
    {
        return Evis;
    }

    // did not trigger
    return 0.0;
}

void DetectorResponse::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == m_cmdSetA0.get())
    {
        m_resA0 = m_cmdSetA0->GetNewDoubleValue(newValue);
    }
    else if (command == m_cmdSetA1.get())
    {
        m_resA1 = m_cmdSetA1->GetNewDoubleValue(newValue);
    }
    else if (command == m_cmdSetA2.get())
    {
        m_resA2 = m_cmdSetA2->GetNewDoubleValue(newValue);
    }
    else if (command == m_cmdSetTrigThreshold.get())
    {
        m_trigThreshold = m_cmdSetTrigThreshold->GetNewDoubleValue(newValue);
    }
    else if (command == m_cmdSetTrigWidth.get())
    {
        m_trigWidth = m_cmdSetTrigWidth->GetNewDoubleValue(newValue);
    }
    else
    {
        G4cerr << "Unhandled command in RunAction::SetNewValue()" << G4endl;
        exit(99);
    }
}
