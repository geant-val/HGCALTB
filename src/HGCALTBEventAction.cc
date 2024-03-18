//**************************************************
// \file HGCALTBEventAction.cc
// \brief: Implementation of HGCALTBEventAction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBEventAction.hh"

#include "HGCALTBAHCALSD.hh"
#include "HGCALTBCEESD.hh"
#include "HGCALTBCHESD.hh"
#include "HGCALTBRunAction.hh"
#include "HGCALTBSignalHelper.hh"

// Includers from Geant4
//
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4Version.hh"
#if G4VERSION_NUMBER < 1100
#  include "g4root.hh"
#else
#  include "G4AnalysisManager.hh"
#endif
#include "G4ParticleGun.hh"
#include "G4SDManager.hh"

// Includers from std
//
#include <numeric>

// constructor and de-constructor
//
HGCALTBEventAction::HGCALTBEventAction(HGCALTBPrimaryGenAction* PGA)
  : G4UserEventAction(), edep(0.), fIntLayer(0), fPrimaryGenAction(PGA)
{
  fCEELayerSignals = std::vector<G4double>(HGCALTBConstants::CEELayers, 0.);
  fCHELayerSignals = std::vector<G4double>(HGCALTBConstants::CHELayers, 0.);
  fAHCALLayerSignals = std::vector<G4double>(HGCALTBConstants::AHCALLayers, 0.);
}

HGCALTBEventAction::HGCALTBEventAction() : G4UserEventAction(), edep(0.), fIntLayer(0)
{
  fCEELayerSignals = std::vector<G4double>(HGCALTBConstants::CEELayers, 0.);
  fCHELayerSignals = std::vector<G4double>(HGCALTBConstants::CHELayers, 0.);
  fAHCALLayerSignals = std::vector<G4double>(HGCALTBConstants::AHCALLayers, 0.);
}

HGCALTBEventAction::~HGCALTBEventAction() {}

// Define BeginOfEventAction() and EndOfEventAction() virtual methods
//
void HGCALTBEventAction::BeginOfEventAction(const G4Event*)
{
  // Initialize variables per event
  //
  edep = 0.;
  fIntLayer = 0;
  for (auto& value : fCEELayerSignals) {
    value = 0.;
  }
  for (auto& value : fCHELayerSignals) {
    value = 0.;
  }
  for (auto& value : fAHCALLayerSignals) {
    value = 0.;
  }
}

// GetCEEHitsCollection method()
//
HGCALTBCEEHitsCollection* HGCALTBEventAction::GetCEEHitsCollection(G4int hcID,
                                                                   const G4Event* event) const
{
  auto hitsCollection =
    static_cast<HGCALTBCEEHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));

  if (!hitsCollection) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("HGCALTBEventAction::GetHitsCollection()", "MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}

// GetCEEHitsCollection method()
//
HGCALTBCHEHitsCollection* HGCALTBEventAction::GetCHEHitsCollection(G4int hcID,
                                                                   const G4Event* event) const
{
  auto hitsCollection =
    static_cast<HGCALTBCHEHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));

  if (!hitsCollection) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("HGCALTBEventAction::GetHitsCollection()", "MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}

// GetAHCALHitsCollection method()
//
HGCALTBAHCALHitsCollection* HGCALTBEventAction::GetAHCALHitsCollection(G4int hcID,
                                                                       const G4Event* event) const
{
  auto hitsCollection =
    static_cast<HGCALTBAHCALHitsCollection*>(event->GetHCofThisEvent()->GetHC(hcID));

  if (!hitsCollection) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("HGCALTBEventAction::GetHitsCollection()", "MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}

void HGCALTBEventAction::EndOfEventAction(const G4Event* event)
{
  // Access Event random seeds
  //
  // auto rndseed = G4RunManager::GetRunManager()->GetRandomNumberStatusForThisEvent();

  // CEE Hits
  //
  auto CEEHCID =
    G4SDManager::GetSDMpointer()->GetCollectionID(HGCALTBCEESD::fCEEHitsCollectionName);
  HGCALTBCEEHitsCollection* CEEHC = GetCEEHitsCollection(CEEHCID, event);

  // lambda to apply calibration, add noise and apply a 0.5 MIP cut to CEE and CHE cells
  auto ApplyHGCALCut = [](G4double partialsum, G4double signal) -> G4double {
    auto calibsignal = signal / HGCALTBConstants::MIPSilicon;  // MIP calibration
    calibsignal += G4RandGauss::shoot(0., HGCALTBConstants::CEENoiseSigma);  // Noise
    if (calibsignal > HGCALTBConstants::CEEThreshold)  // Cut
      return partialsum + calibsignal;
    else
      return partialsum;
  };

  // auxiliary lambda function that takes an std::array of signals in big Si wafer
  // and returns a same-sized std::array with entries calibrated at MIP scale
  auto ApplyMIPCalib = [](const std::array<G4double, HGCALTBConstants::CEECells + 1>& Signals) {
    std::array<G4double, HGCALTBConstants::CEECells + 1> CalibSignals = {0.};
    for (std::size_t i = 0; i < CalibSignals.size(); i++) {
      CalibSignals[i] = Signals[i] / HGCALTBConstants::MIPSilicon;
    }
    return CalibSignals;
  };

  // Signal helper class
  HGCALTBSignalHelper SgnlHelper;
  // CEE layer of pion interaction
  G4int CEEIntLayer{99};
  // CEE nuclear interaction found
  G4bool CEENclInteraction{false};

  for (std::size_t i = 0; i < HGCALTBConstants::CEELayers; i++) {
    auto CEESignals = (*CEEHC)[i]->GetCEESignals();
    G4double CEELayerSignal =
      std::accumulate(CEESignals.begin(), CEESignals.end(), 0., ApplyHGCALCut);
    fCEELayerSignals[i] = CEELayerSignal;

    // Tag pion interaction layer in CEE
    //
    if (!CEENclInteraction) {  // pion has not interacted yet
      if (i <= 25) {
        if (!(SgnlHelper.IsInteraction(ApplyMIPCalib((*CEEHC)[i]->GetCEESignals()),
                                       ApplyMIPCalib((*CEEHC)[i + 1]->GetCEESignals()),
                                       ApplyMIPCalib((*CEEHC)[i + 2]->GetCEESignals()),
                                       fPrimaryGenAction->GetParticleGun()->GetParticleEnergy())))
          continue;
        else {
          CEENclInteraction = true;
          CEEIntLayer = i;
        }
      }
      else if (i < 27) {
        if (!(SgnlHelper.IsInteraction(ApplyMIPCalib((*CEEHC)[i]->GetCEESignals()),
                                       ApplyMIPCalib((*CEEHC)[i + 1]->GetCEESignals()),
                                       fPrimaryGenAction->GetParticleGun()->GetParticleEnergy())))
          continue;
        else {
          CEENclInteraction = true;
          CEEIntLayer = i;
        }
      }
    }  // end of CEE pion interaction tagging
  }

  // auxiliary lambda function that takes an std::array with signals in a CHE layer
  // of 7 pads and returns an std::array with only elements for the central pad
  auto ExtractCentralPad =
    [](const std::array<G4double, HGCALTBConstants::CHECells + 1>& CHESignals) {
      std::array<G4double, HGCALTBConstants::CEECells + 1> CentralPad = {0.};
      std::copy(CHESignals.begin(), CHESignals.begin() + (HGCALTBConstants::CEECells + 1),
                CentralPad.begin());
      return CentralPad;
    };

  // CHE layer of pion interaction
  G4int CHEIntLayer{99};
  // CHE nuclear interaction found
  G4bool CHENclInteraction{false};

  // CHE Hits
  //
  auto CHEHCID =
    G4SDManager::GetSDMpointer()->GetCollectionID(HGCALTBCHESD::fCHEHitsCollectionName);
  HGCALTBCHEHitsCollection* CHEHC = GetCHEHitsCollection(CHEHCID, event);

  for (std::size_t i = 0; i < HGCALTBConstants::CHELayers; i++) {
    auto CHESignals = (*CHEHC)[i]->GetCHESignals();
    G4double CHELayerSignal = 0.;
    if (i < HGCALTBConstants::CHESevenWaferLayers) {
      CHELayerSignal = std::accumulate(CHESignals.begin(), CHESignals.end(), 0., ApplyHGCALCut);
    }
    else {  // for last 3 CHE layers only add up cells for 1 silicon wafer
      CHELayerSignal =
        std::accumulate(CHESignals.begin(), CHESignals.begin() + (HGCALTBConstants::CEECells - 1),
                        0., ApplyHGCALCut);
    }
    fCHELayerSignals[i] = CHELayerSignal;

    // Tag pion interaction layer in CHE
    //
    if (!CHENclInteraction) {  // pion has not interacted yet
      if (i <= 10) {
        if (!(SgnlHelper.IsInteraction(
              ApplyMIPCalib(ExtractCentralPad((*CHEHC)[i]->GetCHESignals())),
              ApplyMIPCalib(ExtractCentralPad((*CHEHC)[i + 1]->GetCHESignals())),
              ApplyMIPCalib(ExtractCentralPad((*CHEHC)[i + 2]->GetCHESignals())),
              fPrimaryGenAction->GetParticleGun()->GetParticleEnergy())))
          continue;
        else {
          CHENclInteraction = true;
          CHEIntLayer = i;
        }
      }
      else if (i < 12) {
        if (!(SgnlHelper.IsInteraction(
              ApplyMIPCalib(ExtractCentralPad((*CHEHC)[i]->GetCHESignals())),
              ApplyMIPCalib(ExtractCentralPad((*CHEHC)[i + 1]->GetCHESignals())),
              fPrimaryGenAction->GetParticleGun()->GetParticleEnergy())))
          continue;
        else {
          CHENclInteraction = true;
          CHEIntLayer = i;
        }
      }
    }  // end of CHE pion interaction tagging
  }

  // AHCAL Hits
  //
  auto AHCALHCID =
    G4SDManager::GetSDMpointer()->GetCollectionID(HGCALTBAHCALSD::fAHCALHitsCollectionName);
  HGCALTBAHCALHitsCollection* AHCALHC = GetAHCALHitsCollection(AHCALHCID, event);

  // lambda to apply calibration, add noise and apply a 0.5 MIP cut to AHCAL cells
  auto ApplyAHCut = [MIPTile = HGCALTBConstants::MIPTile,
                     AHThreshold = HGCALTBConstants::AHCALThreshold](G4double partialsum,
                                                                     G4double signal) -> G4double {
    auto calibsignal = signal / MIPTile;  // MIP calibration
    calibsignal += G4RandGauss::shoot(0., HGCALTBConstants::AHCALNoiseSigma);  // Noise
    if (calibsignal > AHThreshold)  // Cut
      return partialsum + calibsignal;
    else
      return partialsum;
  };

  for (std::size_t i = 0; i < HGCALTBConstants::AHCALLayers; i++) {
    auto AHCALSignals = (*AHCALHC)[i]->GetAHSignals();
    G4double AHCALLayerSignal =
      std::accumulate(AHCALSignals.begin(), AHCALSignals.end(), 0., ApplyAHCut);
    fAHCALLayerSignals[i] = AHCALLayerSignal;
  }

  // Accumulate statistics
  //
  auto CEETot = std::accumulate(fCEELayerSignals.begin(), fCEELayerSignals.end(), 0.);
  auto CHETot = std::accumulate(fCHELayerSignals.begin(), fCHELayerSignals.end(), 0.);
  auto AHCALTot = std::accumulate(fAHCALLayerSignals.begin(), fAHCALLayerSignals.end(), 0.);
  auto HGCALTot = CEETot + CHETot + AHCALTot;
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->FillNtupleDColumn(0, edep);
  analysisManager->FillNtupleDColumn(1, CEETot);
  analysisManager->FillNtupleDColumn(2, CHETot);
  analysisManager->FillNtupleDColumn(3, AHCALTot);
  analysisManager->FillNtupleDColumn(4, HGCALTot);
  analysisManager->FillNtupleIColumn(5, fIntLayer);
  analysisManager->FillNtupleIColumn(
    6, fPrimaryGenAction->GetParticleGun()->GetParticleDefinition()->GetPDGEncoding());
  analysisManager->FillNtupleDColumn(7, fPrimaryGenAction->GetParticleGun()->GetParticleEnergy());
  analysisManager->FillNtupleIColumn(8, CEEIntLayer);
  analysisManager->FillNtupleIColumn(9, CHEIntLayer);
  analysisManager->AddNtupleRow();
}

//**************************************************
