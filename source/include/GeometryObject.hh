/// \file GeometryObject.hh
/// \brief Definition of the GeometryObject class

/// \details This is an auxiliary base class representing a single
///          (non-sensitive) piece of geometry, to allow for modular setups.
///          New geometry objects should derive from this class, and be added
///          to the DetectorConstruction::DetectorConstruction() method.
///
///          Each derived class from a GeometryObject has a name. To include it
///          in the geometry, it should be enabled via macro command:
///
///             /geometry/[GeometryObjectName]/enable True
///
///          The class offers methods to create named length, density, boolean,
///          or string values, which can then be set by macro commands:
///
///             /geometry/[GeometryObjectName]/setLength/[valueName] xyz cm
///             /geometry/[GeometryObjectName]/setDensity/[valueName] xyz g/cm3
///             /geometry/[GeometryObjectName]/setBool/[valueName] True/False
///             /geometry/[GeometryObjectName]/setString/[valueName] [stringValue]
///

#ifndef GeometryObject_hh
#define GeometryObject_hh 1

#include "G4UImessenger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4LogicalVolume.hh"

#include "G4NistManager.hh"

#include <memory>
using std::shared_ptr;
using std::make_shared;

#include <vector>
using std::vector;

#include <map>
using std::map;

class GeometryObject : public G4UImessenger
{
public:
    GeometryObject(const G4String& name);
    virtual ~GeometryObject() {}

    virtual void Construct(G4LogicalVolume* lWorldVolume, G4bool checkOverlaps) = 0; // purely virtual, needs to be implemented by daughters
    virtual void DefineMaterials() = 0; // purely virtual, needs to be implemented by daughters

    const G4String& GetName() const { return m_name; }

    G4bool Enabled() const { return m_enabled; }
    void SetEnabled(G4bool enabled) { m_enabled = enabled; }


protected:
    G4String m_name = "";

    G4RotationMatrix* m_noRotation = nullptr;

    G4NistManager* m_nistManager = nullptr;

    G4bool m_enabled = false; // object will be built if this is true
    shared_ptr<G4UIcmdWithABool> m_cmdEnable;

    // command and value storage for geometry parameters
    void NewBoolValue(const G4String& name, const G4String& description, G4bool defaultValue);
    map<G4String, shared_ptr<G4UIcmdWithABool>> m_boolCommands;
    map<G4String, G4bool>   m_boolValues;
    G4bool GetBoolValue(const G4String& name) const;

    void NewLengthValue(const G4String& name, const G4String& description, G4double defaultValue);
    map<G4String, shared_ptr<G4UIcmdWithADoubleAndUnit>> m_lengthCommands;
    map<G4String, G4double> m_lengthValues;
    G4double GetLengthValue(const G4String& name) const;

    void NewDoubleValue(const G4String& name, const G4String& description, G4double defaultValue);
    map<G4String, shared_ptr<G4UIcmdWithADoubleAndUnit>> m_doubleCommands;
    map<G4String, G4double> m_doubleValues;
    G4double GetDoubleValue(const G4String& name) const;

    void NewDensityValue(const G4String& name, const G4String& description, G4double defaultValue);
    map<G4String, shared_ptr<G4UIcmdWithADoubleAndUnit>> m_densityCommands;
    map<G4String, G4double> m_densityValues;
    G4double GetDensityValue(const G4String& name) const;

    void NewStringValue(const G4String& name, const G4String& description, G4String defaultValue);
    map<G4String, shared_ptr<G4UIcmdWithAString>> m_stringCommands;
    map<G4String, G4String> m_stringValues;
    G4String GetStringValue(const G4String& name) const;

public:
    virtual void SetNewValue(G4UIcommand* command, G4String newValue);

};


#endif
