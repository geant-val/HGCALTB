//**************************************************
// \file HGCALTBTrackAction.cc
// \brief: Implementation of
//         HGCALTBTrackAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 15 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBTrackAction.hh"

// Includers from Geant4
//
#include "G4DynamicParticle.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "G4Track.hh"

// Includers from std
//
#include <string>

// Constructor and de-constructor
//
HGCALTBTrackAction::HGCALTBTrackAction(HGCALTBEventAction* EvtAction)
  : G4UserTrackingAction(), fEventAction(EvtAction)
{}

HGCALTBTrackAction::~HGCALTBTrackAction() {}

// PreUserTrackingAction definition
//
void HGCALTBTrackAction::PreUserTrackingAction(const G4Track* /*aTrack*/) {}

// PostUserTrackingAction definition
//
void HGCALTBTrackAction::PostUserTrackingAction(const G4Track* aTrack)
{
  if (aTrack->GetParentID() == 0) {  // primary track
    G4int InteractionLayer;
    const G4Step* step = aTrack->GetStep();
    const G4StepPoint* poststep = step->GetPostStepPoint();
    if (poststep->GetProcessDefinedStep()->GetProcessSubType() == 121) {  // HadInElastic process
      G4String volName = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName();
      G4String CEEsubname = "HGCalEE";
      size_t CEEfind = volName.find(CEEsubname);
      if (CEEfind != std::string::npos)
        InteractionLayer = 1;  // Had interacted in HGCALEE Section
      else
        InteractionLayer = 0;
      fEventAction->SetIntLayer(InteractionLayer);
    }
  }
}

//**************************************************
