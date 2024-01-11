//**************************************************
// \file HGCALTBStepAction.hh
// \brief: definition of HGCALTBStepAction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 January 2024
//**************************************************

#ifndef HGCALTBStepAction_h
#  define HGCALTBStepAction_h 1

// Includers from Geant4
//
#  include "G4Step.hh"
#  include "G4UserSteppingAction.hh"

// Includers from project files
//
// #include "HGCALTBEventAction.hh"

class HGCALTBStepAction : public G4UserSteppingAction
{
  public:
    HGCALTBStepAction();
    // HGCALTBStepAction( ATLTileCalTBEventAction* EvtAction );
    virtual ~HGCALTBStepAction();

    // virtual methods from base class
    virtual void UserSteppingAction(const G4Step* aStep);

  private:
    // HGCALTBEventAction* fEventAction;
    void PrintCEEInfo(const G4Step* aStep);
};

#endif  // HGCALTBStepAction_h 1

//**************************************************
