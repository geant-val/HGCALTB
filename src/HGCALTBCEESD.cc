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
  for (std::size_t i = 0; i < 1; i++) {
    fHitsCollection->insert(new HGCALTBCEEHit());
  }
}

// ProcessHits virtual base method
//
G4bool HGCALTBCEESD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  return true;
}

// EndOfEvent virtual base method
//
void HGCALTBCEESD::EndOfEvent(G4HCofThisEvent*) {}

//**************************************************
