//**************************************************
// \file HGCALTBStepAction.cc
// \brief: implementation of HGCALTBStepAction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBStepAction.hh"

// Constructor and de-constructor
//
HGCALTBStepAction::HGCALTBStepAction() : G4UserSteppingAction() {}

// HGCALTBStepAction::HGCALTBStepAction(HGCALTBEventAction* EventAction)
//     : G4UserSteppingAction(),
//       fEventAction( EventAction ){}

HGCALTBStepAction::~HGCALTBStepAction() {}

// UserSteppingaction() virtual method from base class
//
void HGCALTBStepAction::UserSteppingAction(const G4Step* aStep) {}

//**************************************************
