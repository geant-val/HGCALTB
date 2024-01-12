//**************************************************
// \file HGCALTBCHEHit.cc
// \brief: implementation of HGCALTBCHEHit class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 12 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBCHEHit.hh"

// Includers from Geant4
#include "G4UnitsTable.hh"

// Constructor and de-constructor
//
HGCALTBCHEHit::HGCALTBCHEHit() : G4VHit(), fCHESignals({}) {}

HGCALTBCHEHit::~HGCALTBCHEHit() {}

HGCALTBCHEHit::HGCALTBCHEHit(const HGCALTBCHEHit& right) : G4VHit()
{
  fCHESignals = right.fCHESignals;
}

// Operator = definition
//
const HGCALTBCHEHit& HGCALTBCHEHit::operator=(const HGCALTBCHEHit& right)
{
  fCHESignals = right.fCHESignals;

  return *this;
}

// Operator == definition
//
G4bool HGCALTBCHEHit::operator==(const HGCALTBCHEHit& right) const
{
  return (this == &right) ? true : false;
}

//**************************************************
