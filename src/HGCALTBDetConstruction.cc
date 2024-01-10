//**************************************************
// \file HGCALTBDetConstruction.cc
// \brief: implementation of
//         HGCALTBDetConstruction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 10 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBDetConstruction.hh"

// Includers from Geant4
//
#include "G4GDMLParser.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"

// Constructors and de-constructor
//
HGCALTBDetConstruction::HGCALTBDetConstruction() : G4VUserDetectorConstruction() {}

HGCALTBDetConstruction::~HGCALTBDetConstruction() {}

// virtual Construct() method from base class
//
G4VPhysicalVolume* HGCALTBDetConstruction::Construct()
{
  G4GDMLParser Parser;
  Parser.Read("TBHGCal181Oct.gdml", false);
  auto worldPV = Parser.GetWorldVolume();

  // DefineVisAttributes();

  return worldPV;
}

// virtual ConstructSDandField() method from base class
//
void HGCALTBDetConstruction::ConstructSDandField()
{
  // Sensitive detectors
  //

  // No fields involved
}

// DefineVisAttributes() private method
//
void HGCALTBDetConstruction::DefineVisAttributes() {}

//**************************************************
