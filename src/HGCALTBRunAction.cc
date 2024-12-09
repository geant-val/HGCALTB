//**************************************************
// \file HGCALTBRunAction.cc
// \brief: Implementation of HGCALTBRunAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBRunAction.hh"

// Includers from Geant4
//
#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Version.hh"
#if G4VERSION_NUMBER < 1100
#  include "g4root.hh"
#else
#  include "G4AnalysisManager.hh"
#endif
#include <string>

// Constructor and de-constructor
//
HGCALTBRunAction::HGCALTBRunAction(HGCALTBEventAction* eventAction, G4String filename)
  : G4UserRunAction(), fEventAction(eventAction), fFileName(filename)
{
  G4RunManager::GetRunManager()->SetPrintProgress(1);  // print each event number

  // Instantiate analysis manager
  auto analysisManager = G4AnalysisManager::Instance();  // using ROOT
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(1);

#if G4VERSION_NUMBER > 1050 && G4VERSION_NUMBER < 1060
  analysisManager->SetNtupleRowWise(false);
#endif

  analysisManager->CreateNtuple("HGCALTBout", "HGCALTBoutput");
  analysisManager->CreateNtupleDColumn("edep");
  analysisManager->CreateNtupleDColumn("CEETot");
  analysisManager->CreateNtupleDColumn("CHETot");
  analysisManager->CreateNtupleDColumn("AHCALTot");
  analysisManager->CreateNtupleDColumn("HGCALTot");
  analysisManager->CreateNtupleIColumn("IntLayer");
  analysisManager->CreateNtupleIColumn("PDGID");
  analysisManager->CreateNtupleDColumn("PrimaryEnergy");
  analysisManager->CreateNtupleIColumn("CEEIntLayer");
  analysisManager->CreateNtupleIColumn("CHEIntLayer");
  analysisManager->CreateNtupleDColumn("CEESignals", fEventAction->GetCEESignals());
  analysisManager->CreateNtupleDColumn("CHESignals", fEventAction->GetCHESignals());
  analysisManager->CreateNtupleDColumn("AHCALSignals", fEventAction->GetAHCALSignals());
  analysisManager->FinishNtuple();
}

// Define deconstructor
//
HGCALTBRunAction::~HGCALTBRunAction()
{
#if G4VERSION_NUMBER < 1100
  delete G4AnalysisManager::Instance();  // not needed for G4 v11 and up
#endif
}

// Define BeginOfRunAction() and EndOfRunAction() methods
//
void HGCALTBRunAction::BeginOfRunAction(const G4Run* Run)
{
  // Inform RunManager to save random seeds
  //
  // G4RunManager::GetRunManager()->SetRandomNumberStore( true );

  auto analysisManager = G4AnalysisManager::Instance();
  G4String outputfile;

  if (fFileName.empty()) {
    std::string runnumber = std::to_string(Run->GetRunID());
    outputfile = "HGCALTBout_Run" + runnumber + ".root";
  }
  else {
    outputfile = fFileName;
  }

  analysisManager->OpenFile(outputfile);

  // If using AdePT we may be interested in comparing the execution time
#ifdef USE_ADEPT
  fTimer.Start();
#endif
}

void HGCALTBRunAction::EndOfRunAction(const G4Run*)
{
#ifdef USE_ADEPT
  if(G4Threading::G4GetThreadId() < 0)
  {
    fTimer.Stop();
    G4cout << "Run time: " << fTimer.GetRealElapsed() << "\n";
  }
#endif

  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();
}

//**************************************************
