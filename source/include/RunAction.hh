/// \file RunAction.hh
/// \brief Definition of the RunAction class

/// \details The RunAction class creates the output histograms and tuples, and
///          writes them to a file at the end of the run.
///
///          The output file name can be set via macro commmand:
///            /run/fileName [output.root]

#ifndef RunAction_hh
#define RunAction_hh 1

#include "G4UserRunAction.hh"
#include "G4UImessenger.hh"

#include "G4UIcmdWithAString.hh"

#include "globals.hh"

#include <memory>
using std::shared_ptr;
using std::make_shared;

class G4Run;

class RunAction : public G4UserRunAction,
    public G4UImessenger
{
public:
    explicit RunAction(bool isMaster);
    ~RunAction() override = default;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

public:
    void SetNewValue (G4UIcommand* command, G4String newValue) override;

private:
    bool fIsMaster;
    G4String m_fileName = "TU1.root";
    shared_ptr<G4UIcmdWithAString> m_cmdFileName;
};

#endif
