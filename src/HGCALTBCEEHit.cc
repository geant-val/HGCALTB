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
HGCALTBCEEHit::HGCALTBCEEHit() : G4VHit(), fEdep(0.) {}

HGCALTBCEEHit::~HGCALTBCEEHit() {}

HGCALTBCEEHit::HGCALTBCEEHit(const HGCALTBCEEHit& right) : G4VHit()
{
  fEdep = right.fEdep;
}

// Operator = definition
//
const HGCALTBCEEHit& HGCALTBCEEHit::operator=(const HGCALTBCEEHit& right)
{
  fEdep = right.fEdep;

  return *this;
}

// Operator == definition
//
G4bool HGCALTBCEEHit::operator==(const HGCALTBCEEHit& right) const
{
  return (this == &right) ? true : false;
}

//**************************************************
