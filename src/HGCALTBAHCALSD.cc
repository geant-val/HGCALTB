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
  fHitsCollection = new HGCALTBAHCALHitsCollection(SensitiveDetectorName, collectionName[0]);

  // Add this collection in the hits collection of this event
  //
  auto hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection(hcID, fHitsCollection);

  // Allocate hits in hit collection
  //
  for (G4int i = 0; i < HGCALTBConstants::AHCALLayers; i++) {
    fHitsCollection->insert(new HGCALTBAHHit());
  }
}

// ProcessHits virtual base method
//
G4bool HGCALTBAHCALSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  // Get CHE layer ID
  //
  auto AHCALLayerID = aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1) - 1;
  if (AHCALLayerID >= HGCALTBConstants::AHCALLayers || AHCALLayerID < 0) {
    G4ExceptionDescription msg;
    msg << "AHCAL layer copy number equal or greater than " << HGCALTBConstants::AHCALLayers
        << " or < 0";
    G4Exception("HGCALTBAHCALSD::ProcessHits()", "MyCode0004", FatalException, msg);
  }

  // Get tile ID
  //
  auto TileID = MapTileCpNo(aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(0));

  // Access the corresponding hit
  //
  auto hit = (*fHitsCollection)[AHCALLayerID];
  auto stepl = aStep->GetStepLength();
  auto charge = aStep->GetTrack()->GetDynamicParticle()->GetDefinition()->GetPDGCharge();
  auto edep = aStep->GetTotalEnergyDeposit();
  if (stepl > 0. && charge != 0) {
    edep = ApplyBirk(aStep);
  }
  hit->AddTileEdep(TileID, edep);

  return true;
}

// EndOfEvent virtual base method
//
void HGCALTBAHCALSD::EndOfEvent(G4HCofThisEvent*) {}

//**************************************************
