//**************************************************
// \file HGCALTBCHESD.cc
// \brief: implementation of HGCALTBCHESD
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 12 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBCHESD.hh"

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

const G4String HGCALTBCHESD::fCHEHitsCollectionName = "CHEHitsCollectionName";

// Constructor and de-constructor
//
HGCALTBCHESD::HGCALTBCHESD(const G4String& name)
  : G4VSensitiveDetector(name),
    fHitsCollection(nullptr),
    CHEWaferMap({{0, 0}, {10002, 1}, {100101, 2}, {101, 3}, {10101, 4}, {110101, 5}, {2, 6}})
{
  collectionName.insert(fCHEHitsCollectionName);
}

HGCALTBCHESD::~HGCALTBCHESD() {}

// Intiliaze virtual base method
//
void HGCALTBCHESD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  //
  fHitsCollection = new HGCALTBCHEHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in the hits collection of this event
  //
  auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);

  // Allocate hits in hit collection
  //
  for (G4int i = 0; i < HGCALTBConstants::CHELayers; i++) {
    fHitsCollection->insert(new HGCALTBCHEHit());
  }
}

// ProcessHits virtual base method
//
G4bool HGCALTBCHESD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  // Apply time cut
  //
  if (aStep->GetTrack()->GetGlobalTime() > HGCALTBConstants::TimeCut) return false;

  // Get CHE layer ID
  //
  auto CHELayerID = (aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(2) - 1) / 3;
  if (CHELayerID > HGCALTBConstants::CHELayers || CHELayerID < 0) {
    G4ExceptionDescription msg;
    msg << "CHE layer copy number greater than " << HGCALTBConstants::CHELayers << " or < 0";
    G4Exception("HGCALTBCHESD::ProcessHits()", "MyCode0004", FatalException, msg);
  }

  // Get CHE silicon wafer ID (0...6)
  //
  auto WaferID = FindWaferID(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1));

  // Access the corresponding hit
  //
  auto CellID = (aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0)
                 - HGCALTBConstants::CHECellMinCpNo)
                + (WaferID * HGCALTBConstants::CEECells);
  if (CellID > HGCALTBConstants::CHECells || CellID < 0) {
    G4ExceptionDescription msg;
    msg << "CHE cell copy number greater than " << HGCALTBConstants::CHECells << "or < 0";
    G4Exception("HGCALTBCHESD::ProcessHits()", "MyCode0004", FatalException, msg);
  }
  auto hit = (*fHitsCollection)[CHELayerID];
  hit->AddCellEdep(CellID, aStep->GetTotalEnergyDeposit());

  return true;
}

// EndOfEvent virtual base method
//
void HGCALTBCHESD::EndOfEvent(G4HCofThisEvent*) {}

//**************************************************
