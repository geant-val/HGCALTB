//**************************************************
// \file HGCALTBEventAction.cc
// \brief: Implementation of HGCALTBEventAction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBEventAction.hh"

#include "HGCALTBRunAction.hh"

// Includers from Geant4
//
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4Version.hh"
#if G4VERSION_NUMBER < 1100
#  include "g4root.hh"
#else
#  include "G4AnalysisManager.hh"
#endif

// constructor and de-constructor
//
HGCALTBEventAction::HGCALTBEventAction() : G4UserEventAction(), edep(0.) {}

HGCALTBEventAction::~HGCALTBEventAction() {}

// Define BeginOfEventAction() and EndOfEventAction() virtual methods
//
void HGCALTBEventAction::BeginOfEventAction(const G4Event*)
{
  // Initialize variables per event
  //
  edep = 0.;
}

void HGCALTBEventAction::EndOfEventAction(const G4Event*)
{
  // Access Event random seeds
  //
  // auto rndseed = G4RunManager::GetRunManager()->GetRandomNumberStatusForThisEvent();

  // Accumulate statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(0, edep);
  analysisManager->AddNtupleRow();
}

//**************************************************
