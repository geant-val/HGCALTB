//**************************************************
// \file HGCALTBCEESD.cc
// \brief: implementation of HGCALTBCEESD
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBCEESD.hh"

#include "HGCALTBConstants.hh"

// Includers from Geant4
//
#include "G4HCofThisEvent.hh"
#include "G4Poisson.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"

// Constructor and de-constructor
//
HGCALTBCEESD::HGCALTBCEESD(const G4String& name, const G4String& hitsCollectionName)
  : G4VSensitiveDetector(name), fHitsCollection(nullptr)
{
  collectionName.insert(hitsCollectionName);
}

HGCALTBCEESD::~HGCALTBCEESD() {}

// Intiliaze virtual base method
//
void HGCALTBCEESD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  //
  fHitsCollection = new HGCALTBCEEHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in the hits collection of this event
  //
  auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);

  // Allocate hits in hit collection
  //
  for (G4int i = 0; i < HGCALTBConstants::CEECells; i++) {
    fHitsCollection->insert(new HGCALTBCEEHit());
  }
}

// ProcessHits virtual base method
//
G4bool HGCALTBCEESD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  // Get CEE layer ID
  //
  auto CEELayerID = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(2) / 3;
  if (CEELayerID > HGCALTBConstants::CEELayers) {
    G4ExceptionDescription msg;
    msg << "CEE layer copy number greater than " << HGCALTBConstants::CEELayers;
    G4Exception("HGCALTBCEESD::ProcessHits()", "MyCode0004", FatalException, msg);
  }

  // Access the corresponding hit
  //
  auto CellID = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0)
                - HGCALTBConstants::CEECellMinCpNo;
  if (CellID > HGCALTBConstants::CEECells) {
    G4ExceptionDescription msg;
    msg << "CEE cell copy number greater than " << HGCALTBConstants::CEECells;
    G4Exception("HGCALTBCEESD::ProcessHits()", "MyCode0004", FatalException, msg);
  }
  auto hit = (*fHitsCollection)[CEELayerID];
  hit->AddCellEdep(CellID, aStep->GetTotalEnergyDeposit());

  return true;
}

// EndOfEvent virtual base method
//
void HGCALTBCEESD::EndOfEvent(G4HCofThisEvent*) {}

//**************************************************
