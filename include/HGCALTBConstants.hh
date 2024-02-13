//**************************************************
// \file HGCALTBConstants.hh
// \brief: definition of HGCALTBConstants namespace
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 January 2024
//**************************************************

#ifndef HGCALTBConstants_h
#  define HGCALTBConstants_h 1

// Includers from Geant4
//
#  include "G4SystemOfUnits.hh"
#  include "G4Types.hh"

// Includers from C++
//
#  include <array>

namespace HGCALTBConstants
{

// Time cut
constexpr G4double TimeCut = 500.;  // ns

// HGCALCEE constants
//

// CEE longitudinal layers (one big hexagon per layer)
constexpr G4int CEELayers = 28;

// CEE copynumbers for HGCalEECellCoarse (exluding HGCALEECellCoarseHalf)
constexpr G4int CEECellMinCpNo = 2003;
constexpr G4int CEECellMaxCpNo = 2129;

// CEE (small) silicon cells per big hexagon (layer)
constexpr G4int CEECells = CEECellMaxCpNo - CEECellMinCpNo;  // 126

// CEE Threshold
constexpr G4double CEEThreshold = 0.5;  // MIP

// CEE noise
// intrinsic noise - https://arxiv.org/pdf/2012.06336.pdf fig. 22
constexpr G4double CEENoiseSigma = 0.12;  // MIP

// HGCALCHE constants
//

// CHE longitudinal layers
constexpr G4int CHELayers = 12;

// CHE longitudinal layers with 7 silicon wafers
constexpr G4int CHESevenWaferLayers = 9;

// CHE copynumbers for HGCalHECellCoarse (exluding HGCALHECellCoarseHalf)
constexpr G4int CHECellMinCpNo = 2003;
constexpr G4int CHECellMaxCpNo = 2129;

// CHE big hexagons per layer
constexpr G4int CHEHexLayer = 7;

// CHE (small) silicon cells per big hexagon (layer)
constexpr G4int CHECells = CHEHexLayer * (CHECellMaxCpNo - CHECellMinCpNo);  // 7*126=882

// AHCAL constants
//

// AHCAL row and column plastic tiles
constexpr G4int AHCALrow = 24;
constexpr G4int AHCALcolumn = 24;

// AHCAL tiles per layer
constexpr G4int AHCALtileperlayer = AHCALrow * AHCALcolumn;

// AHCAL layers
constexpr G4int AHCALLayers = 39;

// AHCAL MIP threshold
constexpr G4double AHCALThreshold = 0.5;  // MIP

// AHCAL noise
constexpr G4double AHCALNoiseSigma = 0.10;  // MIP

// MIP calibration
//
constexpr G4double MIPSilicon = 0.085;  // MeV
constexpr G4double MIPTile = 0.475;  // MeV

}  // namespace HGCALTBConstants

#endif  // HGCALTBConstants_h 1

//**************************************************
