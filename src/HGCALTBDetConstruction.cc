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

#include "HGCALTBAHCALSD.hh"
#include "HGCALTBCEESD.hh"
#include "HGCALTBCHESD.hh"

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
  auto CEESD = new HGCALTBCEESD("CEESD");
  G4SDManager::GetSDMpointer()->AddNewDetector(CEESD);
  auto CHESD = new HGCALTBCHESD("CHESD");
  G4SDManager::GetSDMpointer()->AddNewDetector(CHESD);
  auto AHCALSD = new HGCALTBAHCALSD("AHSD");
  G4SDManager::GetSDMpointer()->AddNewDetector(AHCALSD);

  // Assign to logical volume
  //
  auto LVStore = G4LogicalVolumeStore::GetInstance();
  for (auto volume : *LVStore) {
    if (volume->GetName() == "HGCalEECellCoarse") {
      G4cout << "--->Assigning HGCALTBCEESD to logical volume " << volume->GetName() << G4endl;
      volume->SetSensitiveDetector(CEESD);
    }
    if (volume->GetName() == "HGCalHECellCoarse") {
      G4cout << "--->Assigning HGCALTBCHESD to logical volume " << volume->GetName() << G4endl;
      volume->SetSensitiveDetector(CHESD);
    }
    if (volume->GetName() == "AHcalTileSensitive") {
      G4cout << "--->Assigning HGCALTBAHCALSD to logical volume " << volume->GetName() << G4endl;
      volume->SetSensitiveDetector(AHCALSD);
    }
  }

  // No fields involved
}

// DefineVisAttributes() private method
//
void HGCALTBDetConstruction::DefineVisAttributes() {}

//**************************************************
