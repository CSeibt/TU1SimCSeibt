#include "GeometryObject.hh"

GeometryObject::GeometryObject(const G4String& name) : m_name(name)
{
    m_nistManager = G4NistManager::Instance();

    G4String commandName = G4String("/geometry/") + m_name + "/enable";
    m_cmdEnable = make_shared<G4UIcmdWithABool>(commandName.data(), this);
    m_cmdEnable->SetParameterName("enabled", false);
    m_cmdEnable->AvailableForStates(G4State_PreInit);
    m_cmdEnable->SetGuidance("toggles building of this geometry object");
}

void GeometryObject::SetNewValue(G4UIcommand* command, G4String newValue)
{
    if (command == m_cmdEnable.get())
    {
        m_enabled = m_cmdEnable->GetNewBoolValue(newValue);
        return;
    }

    for (auto& thisCmd : m_boolCommands)
    {
        if (command == thisCmd.second.get())
        {
            m_boolValues[thisCmd.first] = thisCmd.second->GetNewBoolValue(newValue);
            return;
        }
    }

    for (auto& thisCmd : m_lengthCommands)
    {
        if (command == thisCmd.second.get())
        {
            m_lengthValues[thisCmd.first] = thisCmd.second->GetNewDoubleValue(newValue);
            return;
        }
    }

    for (auto& thisCmd : m_doubleCommands)
    {
        if (command == thisCmd.second.get())
        {
            m_doubleValues[thisCmd.first] = thisCmd.second->GetNewDoubleValue(newValue);
            return;
        }
    }

    for (auto& thisCmd : m_densityCommands)
    {
        if (command == thisCmd.second.get())
        {
            m_densityValues[thisCmd.first] = thisCmd.second->GetNewDoubleValue(newValue);
            return;
        }
    }

    for (auto& thisCmd : m_stringCommands)
    {
        if (command == thisCmd.second.get())
        {
            m_stringValues[thisCmd.first] = newValue;
            return;
        }
    }

    G4ExceptionDescription msg;
    msg << "Unhandled command " << command->GetCommandPath();

    G4Exception("GeometryObject::SetNewValue",
        "MyCodeXYZ", FatalException, msg);

    return;
}


void GeometryObject::NewBoolValue(const G4String& name, const G4String& description, G4bool defaultValue)
{
    G4String commandName = G4String("/geometry/") + m_name + "/setBool/" + name;
    auto cmd = make_shared<G4UIcmdWithABool>(commandName.data(), this);
    cmd->SetParameterName(name, false);
    cmd->AvailableForStates(G4State_PreInit);
    cmd->SetGuidance(description);

    m_boolCommands[name] = cmd;
    m_boolValues[name] = defaultValue;
}

void GeometryObject::NewLengthValue(const G4String& name, const G4String& description, G4double defaultValue)
{
    G4String commandName = G4String("/geometry/") + m_name + "/setLength/" + name;
    auto cmd = make_shared<G4UIcmdWithADoubleAndUnit>(commandName.data(), this);
    cmd->SetParameterName(name, false);
    cmd->SetUnitCategory("Length");
    cmd->AvailableForStates(G4State_PreInit);
    cmd->SetGuidance(description);

    m_lengthCommands[name] = cmd;
    m_lengthValues[name] = defaultValue;
}

void GeometryObject::NewDoubleValue(const G4String& name, const G4String& description, G4double defaultValue)
{
    G4String commandName = G4String("/geometry/") + m_name + "/setDouble/" + name;
    auto cmd = make_shared<G4UIcmdWithADoubleAndUnit>(commandName.data(), this);
    cmd->SetParameterName(name, false);
    cmd->SetUnitCategory("Double");
    cmd->AvailableForStates(G4State_PreInit);
    cmd->SetGuidance(description);

    m_doubleCommands[name] = cmd;
    m_doubleValues[name] = defaultValue;
}

void GeometryObject::NewDensityValue(const G4String& name, const G4String& description, G4double defaultValue)
{
    G4String commandName = G4String("/geometry/") + m_name + "/setDensity/" + name;
    auto cmd = make_shared<G4UIcmdWithADoubleAndUnit>(commandName.data(), this);
    cmd->SetParameterName(name, false);
    cmd->SetUnitCategory("Volumic Mass");
    cmd->AvailableForStates(G4State_PreInit);
    cmd->SetGuidance(description);

    m_densityCommands[name] = cmd;
    m_densityValues[name] = defaultValue;
}

void GeometryObject::NewStringValue(const G4String& name, const G4String& description, G4String defaultValue)
{
    G4String commandName = G4String("/geometry/") + m_name + "/setString/" + name;
    auto cmd = make_shared<G4UIcmdWithAString>(commandName.data(), this);
    cmd->SetParameterName(name, false);
    cmd->AvailableForStates(G4State_PreInit);
    cmd->SetGuidance(description);

    m_stringCommands[name] = cmd;
    m_stringValues[name] = defaultValue;
}

G4bool GeometryObject::GetBoolValue(const G4String& name) const
{
    return m_boolValues.at(name);
}

G4double GeometryObject::GetLengthValue(const G4String& name) const
{
    return m_lengthValues.at(name);
}

G4double GeometryObject::GetDoubleValue(const G4String& name) const
{
    return m_doubleValues.at(name);
}

G4double GeometryObject::GetDensityValue(const G4String& name) const
{
    return m_densityValues.at(name);
}

G4String GeometryObject::GetStringValue(const G4String& name) const
{
    return m_stringValues.at(name);
}
