#ifndef MATERIALS_HH
#define MATERIALS_HH

#include "G4Material.hh"
#include "G4String.hh"
#include <map>
#include <cmath>

class Materials {
public:
    static std::map<G4String, G4Material*> materialTable;  // Statische Map

    static void DefineMaterials();
    static G4Material* GetMaterial(const G4String& name);

};

#endif // MATERIALS_HH
