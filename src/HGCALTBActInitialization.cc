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

#include "HGCALTBEventAction.hh"
#include "HGCALTBPrimaryGenAction.hh"
#include "HGCALTBRunAction.hh"
#include "HGCALTBStepAction.hh"
#include "HGCALTBTrackAction.hh"
#ifdef USE_CELERITAS
#include "Celeritas.hh"
#endif

// Constructor and de-constructor
//
HGCALTBActInitialization::HGCALTBActInitialization(G4String filename)
  : G4VUserActionInitialization(), fFileName(filename)
{}

HGCALTBActInitialization::~HGCALTBActInitialization() {}

// Build() and BuildForMaster() virtual methods
//
void HGCALTBActInitialization::BuildForMaster() const
{
  auto EventAction = new HGCALTBEventAction();
  SetUserAction(new HGCALTBRunAction(EventAction, fFileName));

#ifdef USE_CELERITAS
  CelerSimpleOffload().BuildForMaster(&CelerSetupOptions(), &CelerSharedParams());
#endif
}

void HGCALTBActInitialization::Build() const
{
#ifdef USE_CELERITAS
  CelerSimpleOffload().Build(&CelerSetupOptions(), &CelerSharedParams(), &CelerLocalTransporter());
#endif

  auto PrimaryGenAction = new HGCALTBPrimaryGenAction();
  auto EventAction = new HGCALTBEventAction(PrimaryGenAction);
  SetUserAction(PrimaryGenAction);
  SetUserAction(new HGCALTBStepAction(EventAction));
  SetUserAction(new HGCALTBRunAction(EventAction, fFileName));
  SetUserAction(EventAction);
  SetUserAction(new HGCALTBTrackAction(EventAction));
}

//**************************************************
