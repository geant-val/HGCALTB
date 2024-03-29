//**************************************************
// \file  HFCALTBTrackAction.hh
// \brief: Definition of HGCALTBTrackAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 15 January 2024
//**************************************************

#ifndef HGCALTBTrackAction_h
#  define HGCALTBTrackAction_h

// Includers from Geant4
//
#  include "G4UserTrackingAction.hh"

// Includers from project files
//
#  include "HGCALTBEventAction.hh"

class HGCALTBTrackAction : public G4UserTrackingAction
{
  public:
    HGCALTBTrackAction(HGCALTBEventAction* EvtAction);
    virtual ~HGCALTBTrackAction();

    // virtual methods from base class
    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);

  private:
    HGCALTBEventAction* fEventAction;
};

#endif  // HGCALTBTrackAction_h 1

//**************************************************
