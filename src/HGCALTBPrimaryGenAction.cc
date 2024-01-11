//**************************************************
// \file HGCALTBPrimaryGenAction.cc
// \brief: implementation of
//         HGCALTBPrimaryGenAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 10 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBPrimaryGenAction.hh"

// Includers from Geant4
//
#include "G4Event.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"

// Constructor and de-constructor
//
HGCALTBPrimaryGenAction::HGCALTBPrimaryGenAction()
  : G4VUserPrimaryGeneratorAction(), fParticleGun(nullptr)
{
  fParticleGun = new G4ParticleGun(1);  // set primary particle(s) to 1

  // default particle gun parameters (can be changed via UI)
  //
  auto particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle("pi+");
  fParticleGun->SetParticleDefinition(particleDefinition);
  fParticleGun->SetParticleEnergy(10. * GeV);
  fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -900*cm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}

HGCALTBPrimaryGenAction::~HGCALTBPrimaryGenAction()
{
  delete fParticleGun;
}

// Define GeneratePrimaries() virtual method
//
void HGCALTBPrimaryGenAction::GeneratePrimaries(G4Event* event)
{
  fParticleGun->GeneratePrimaryVertex(event);
}

//**************************************************
