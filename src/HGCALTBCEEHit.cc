//**************************************************
// \file HGCALTBCEEHit.cc
// \brief: implementation of HGCALTBCEEHit class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBCEEHit.hh"

// Includers from Geant4
#include "G4UnitsTable.hh"

// Constructor and de-constructor
//
HGCALTBCEEHit::HGCALTBCEEHit() : G4VHit(), fCEESignals({}) {}

HGCALTBCEEHit::~HGCALTBCEEHit() {}

HGCALTBCEEHit::HGCALTBCEEHit(const HGCALTBCEEHit& right) : G4VHit()
{
  fCEESignals = right.fCEESignals;
}

// Operator = definition
//
const HGCALTBCEEHit& HGCALTBCEEHit::operator=(const HGCALTBCEEHit& right)
{
  fCEESignals = right.fCEESignals;

  return *this;
}

// Operator == definition
//
G4bool HGCALTBCEEHit::operator==(const HGCALTBCEEHit& right) const
{
  return (this == &right) ? true : false;
}

//**************************************************
