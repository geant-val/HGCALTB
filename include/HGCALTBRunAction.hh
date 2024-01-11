//**************************************************
// \file HGCALTBRunAction.hh
// \brief: Definition of HGCALTBRunAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 January 2024
//**************************************************

// Prevent including header multiple times
//
#ifndef HGCALTBRunAction_h
#  define HGCALTBRunAction_h 1

// Includers from Geant4
//
#  include "G4UserRunAction.hh"
#  include "globals.hh"

class ATLHECTBEventAction;

class G4Run;

class HGCALTBRunAction : public G4UserRunAction
{
  public:
    HGCALTBRunAction();
    virtual ~HGCALTBRunAction();
    // virtual methods from base class
    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);
};

#endif  // HGCALTBRunAction_h 1

//**************************************************
