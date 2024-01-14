//**************************************************
// \file HGCALTBAHHit.cc
// \brief: implementation of HGCALTBAHHit class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 14 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBAHHit.hh"

// Includers from Geant4
#include "G4UnitsTable.hh"

// Constructor and de-constructor
//
HGCALTBAHHit::HGCALTBAHHit() : G4VHit(), fAHSignals({}) {}

HGCALTBAHHit::~HGCALTBAHHit() {}

HGCALTBAHHit::HGCALTBAHHit(const HGCALTBAHHit& right) : G4VHit()
{
  fAHSignals = right.fAHSignals;
}

// Operator = definition
//
const HGCALTBAHHit& HGCALTBAHHit::operator=(const HGCALTBAHHit& right)
{
  fAHSignals = right.fAHSignals;

  return *this;
}

// Operator == definition
//
G4bool HGCALTBAHHit::operator==(const HGCALTBAHHit& right) const
{
  return (this == &right) ? true : false;
}

//**************************************************
