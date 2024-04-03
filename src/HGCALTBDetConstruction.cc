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
#include "G4GeomTestVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"

// Includers from std
//
#include <string>

// Preprocessor macros
// #define CHECKOVERLAPS

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

#if G4VERSION_NUMBER > 1100
#  ifdef CHECKOVERLAPS
  CheckOverlaps(worldPV);
#  endif
#endif

  DefineVisAttributes();

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
    if (volume->GetName() == "HGCalEECellCoarseHalf") {
      G4cout << "--->Assigning HGCALTBCEESD to logical volume " << volume->GetName() << G4endl;
      volume->SetSensitiveDetector(CEESD);
    }
    if (volume->GetName() == "HGCalHECellCoarse") {
      G4cout << "--->Assigning HGCALTBCHESD to logical volume " << volume->GetName() << G4endl;
      volume->SetSensitiveDetector(CHESD);
    }
    if (volume->GetName() == "HGCalHECellCoarseHalf") {
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
void HGCALTBDetConstruction::DefineVisAttributes()
{
  // Lambda function to check if a string is included in another
  auto isSubstring = [](const std::string& mainString, const std::string& searchString) {
    return mainString.find(searchString) != std::string::npos;
  };

  // VisAttributes definitions
  auto SiWaferVisAttr = new G4VisAttributes();
  SiWaferVisAttr->SetForceSolid(true);
  SiWaferVisAttr->SetColor(G4Color::Green());
  SiWaferVisAttr->SetDaughtersInvisible(true);
  auto TotalInvisibleVisAttr = new G4VisAttributes();
  TotalInvisibleVisAttr->SetVisibility(false);
  TotalInvisibleVisAttr->SetDaughtersInvisible(true);
  auto TileVisAttr = new G4VisAttributes();
  TileVisAttr->SetForceSolid(false);
  TileVisAttr->SetColor(G4Color::Red());
  TileVisAttr->SetDaughtersInvisible(true);

  // Assign vis attributes
  //
  auto LVStore = G4LogicalVolumeStore::GetInstance();
  for (auto volume : *LVStore) {
    // beam line
    if (volume->GetName() == "HGCalBeam") volume->SetVisAttributes(TotalInvisibleVisAttr);
    if (volume->GetName() == "HGCalBeamDown") volume->SetVisAttributes(TotalInvisibleVisAttr);
    if (volume->GetName() == "HGCalBeamS5") volume->SetVisAttributes(TotalInvisibleVisAttr);
    if (volume->GetName() == "HGCalBeamS6") volume->SetVisAttributes(TotalInvisibleVisAttr);
    if (volume->GetName() == "CMSE") volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (volume->GetName() == "HGCal") volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    // cee
    if (volume->GetName() == "HGCalEE") volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalEEBlock"))
      volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalEEgap"))
      volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalEEAlcase"))
      volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalEEAbsorber"))
      volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalEECuPCB"))
      volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalEEPCB"))
      volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalEECuKapton"))
      volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalEESensitive"))
      volume->SetVisAttributes(SiWaferVisAttr);
    // che
    if (volume->GetName() == "HGCalHE") volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalHEBlock"))
      volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalHEgap"))
      volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalHEAbsorber"))
      volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalHECuPCB"))
      volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalHEPCB"))
      volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalHECuKapton"))
      volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "HGCalHESiliconSensitive"))
      volume->SetVisAttributes(SiWaferVisAttr);
    // ahcal
    if (volume->GetName() == "HGCalAH") volume->SetVisAttributes(G4VisAttributes::GetInvisible());
    if (isSubstring(volume->GetName(), "AHcalTileSensitive")) volume->SetVisAttributes(TileVisAttr);
  }
}

// CheckOverlaps() private method
//
#if G4VERSION_NUMBER > 1100
void HGCALTBDetConstruction::CheckOverlaps(G4VPhysicalVolume* PhysVol)
{
  G4cout << "-->CheckingOverlaps for volumes in " << PhysVol->GetName() << G4endl;
  // volume, tolerance, npoints, verbosity
  G4GeomTestVolume* testVolume = new G4GeomTestVolume(PhysVol, 0.0, 100000, true);
  testVolume->TestOverlapInTree();
}
#endif

//**************************************************
