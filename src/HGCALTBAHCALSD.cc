//**************************************************
// \file HGCALTBAHCALSD.cc
// \brief: implementation of HGCALTBAHCALSD
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 14 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBAHCALSD.hh"

// Includers from Geant4
//
#include "G4HCofThisEvent.hh"
#include "G4Poisson.hh"
#include "G4SDManager.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"

const G4String HGCALTBAHCALSD::fAHCALHitsCollectionName = "AHCALHitsCollectionName";

// Constructor and de-constructor
//
HGCALTBAHCALSD::HGCALTBAHCALSD(const G4String& name)
  : G4VSensitiveDetector(name), fHitsCollection(nullptr)
{
  collectionName.insert(fAHCALHitsCollectionName);
}

HGCALTBAHCALSD::~HGCALTBAHCALSD() {}

// Intiliaze virtual base method
//
void HGCALTBAHCALSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  //
  /*fHitsCollection = new HGCALTBCHEHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in the hits collection of this event
  //
  auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);

  // Allocate hits in hit collection
  //
  for (G4int i = 0; i < HGCALTBConstants::CHELayers; i++) {
    fHitsCollection->insert(new HGCALTBCHEHit());
  }*/
}

// ProcessHits virtual base method
//
G4bool HGCALTBAHCALSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  /*  // Get CHE layer ID
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
  */
  return true;
}

// EndOfEvent virtual base method
//
void HGCALTBAHCALSD::EndOfEvent(G4HCofThisEvent*) {}

//**************************************************
