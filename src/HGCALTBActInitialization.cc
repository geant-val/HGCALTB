//**************************************************
// \file HGCALTBActInitialization.cc
// \brief: implementation of
//         HGCALTBActInitialization class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 10 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBActInitialization.hh"

#include "HGCALTBPrimaryGenAction.hh"
#include "HGCALTBStepAction.hh"

// Constructor and de-constructor
//
HGCALTBActInitialization::HGCALTBActInitialization() : G4VUserActionInitialization() {}

HGCALTBActInitialization::~HGCALTBActInitialization() {}

// Build() and BuildForMaster() virtual methods
//
void HGCALTBActInitialization::BuildForMaster() const {}

void HGCALTBActInitialization::Build() const
{
  auto PrimaryGenAction = new HGCALTBPrimaryGenAction();
  SetUserAction(PrimaryGenAction);
  SetUserAction(new HGCALTBStepAction());
}

//**************************************************
