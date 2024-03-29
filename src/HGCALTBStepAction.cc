//**************************************************
// \file HGCALTBStepAction.cc
// \brief: implementation of HGCALTBStepAction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBStepAction.hh"

// Constructor and de-constructor
//
HGCALTBStepAction::HGCALTBStepAction() : G4UserSteppingAction() {}

HGCALTBStepAction::HGCALTBStepAction(HGCALTBEventAction* EventAction)
  : G4UserSteppingAction(), fEventAction(EventAction)
{}

HGCALTBStepAction::~HGCALTBStepAction() {}

// UserSteppingaction() virtual method from base class
//
void HGCALTBStepAction::UserSteppingAction(const G4Step* aStep)
{
  // Print CEE info for steps in silicon
  //
  // PrintCEEInfo(aStep);
  // PrintCHEInfo(aStep);
  // PrintAHCALInfo(aStep);

  // Add edep at each step
  //
  fEventAction->Addedep(aStep->GetTotalEnergyDeposit());
}

void HGCALTBStepAction::PrintCEEInfo(const G4Step* aStep)
{
  // Print information from steps in CEE active (silicon) elements
  // NB: using only silicon cells inside the main hexagon, i.e. "HGCalEECellCoarse"
  //     not using half silicon cells outside the main hexagon, i.e. "HGCalEECellCoarseHalf"
  // NB: The parent volume with useful copynumber for layer information is the granmother
  //
  if (aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() == "HGCalEECellCoarse")
  {
    G4String HGCALSectionName =
      aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume(4)->GetName();
    if (HGCALSectionName != "HGCalEE") return;
    G4cout
      << "Volume " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() << " "
      << "Logical "
      << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName()
      << " "
      << "Mat " << aStep->GetPreStepPoint()->GetMaterial()->GetName() << " "
      << "StepLength " << aStep->GetStepLength() << " mm "
      << "cpNo " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber() << " "
      << "GranMother " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume(2)->GetName()
      << " "
      << "GranMother cpNo " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(2)
      << " " << G4endl;
  }
}

void HGCALTBStepAction::PrintCHEInfo(const G4Step* aStep)
{
  // Print information from steps in CHE active (silicon) elements
  // NB: using only silicon cells inside the main hexagon, i.e. "HGCalEECellCoarse"
  //     not using half silicon cells outside the main hexagon, i.e. "HGCalEECellCoarseHalf"
  // NB: The parent volume with useful copynumber for hexagon information (0...6) is the mother
  // NB: The parent volume with useful copynumber for layer information is the granmother
  //
  if (aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() == "HGCalHECellCoarse")
  {
    G4String HGCALSectionName =
      aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume(4)->GetName();
    if (HGCALSectionName != "HGCalHE") return;
    G4cout
      << "Volume " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() << " "
      << "Logical "
      << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName()
      << " "
      << "Mat " << aStep->GetPreStepPoint()->GetMaterial()->GetName() << " "
      << "StepLength " << aStep->GetStepLength() << " mm "
      << "cpNo " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber() << " "
      << "Mother " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume(1)->GetName() << " "
      << "Mother cpNo " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1) << " "
      << "GranMother " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume(2)->GetName()
      << " "
      << "GranMother cpNo " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(2)
      << " " << G4endl;
  }
}

void HGCALTBStepAction::PrintAHCALInfo(const G4Step* aStep)
{
  // Print information from steps in AHCAL active (plastic) elements
  // NB: using only plastic cells
  // NB: The parent volume with useful copynumber for layer information is the mother
  //
  if (aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName()
      == "AHcalTileSensitive")
  {
    G4String HGCALSectionName =
      aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume(2)->GetName();
    if (HGCALSectionName != "HGCalAH") return;
    G4cout
      << "Volume " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetName() << " "
      << "Logical "
      << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume()->GetName()
      << " "
      << "Mat " << aStep->GetPreStepPoint()->GetMaterial()->GetName() << " "
      << "StepLength " << aStep->GetStepLength() << " mm "
      << "cpNo " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber() << " "
      << "Mother " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume(1)->GetName() << " "
      << "Mother cpNo " << aStep->GetPreStepPoint()->GetTouchableHandle()->GetCopyNumber(1) << " "
      << " " << G4endl;
  }
}

//**************************************************
