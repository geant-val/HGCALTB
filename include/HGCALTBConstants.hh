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

// CEE copynumbers for HGCalEECellCoarse (including HGCALEECellCoarseHalf)
constexpr G4int CEECellMinCpNo = 2000;
constexpr G4int CEECellMaxCpNo = 2132;

// CEE (small) silicon cells per big hexagon (layer)
constexpr G4int CEECells = CEECellMaxCpNo - CEECellMinCpNo;  // 132

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

// CHE copynumbers for HGCalHECellCoarse (including HGCALHECellCoarseHalf)
constexpr G4int CHECellMinCpNo = 2000;
constexpr G4int CHECellMaxCpNo = 2132;

// CHE big hexagons per layer
constexpr G4int CHEHexLayer = 7;

// CHE (small) silicon cells per big hexagon (layer)
constexpr G4int CHECells = CHEHexLayer * (CHECellMaxCpNo - CHECellMinCpNo);  // 7*132=924

// Small silicon cell constants
//

// small hexagon apothem
constexpr G4double SCApothem = 0.56227272727272 * cm;  // from gdml file

// small hexagon radius
constexpr G4double SCRadius = SCApothem * 2. / 1.7320508;  // apothem *2 / sqrt(3)

// small hexagon x-pitch
constexpr G4double SCXPitch = SCRadius + SCApothem / 2.;  // cell-to-cell x pitch (mm)

// small hexagon y-pitch
constexpr G4double SCYPitch = SCApothem;  // cell-to-cell y pitch (mm)

// column number in small cells (to determine x position)
constexpr std::array<G4int, CEECells + 1> CellColumn{
  0,  0,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  3,
  3,  3,  3,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  4,  5,  5,  5,  5,  5,  5,  5,  5,
  5,  5,  5,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  6,  7,  7,  7,  7,  7,  7,  7,  7,
  7,  7,  7,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  8,  9,  9,  9,  9,  9,  9,  9,  9,
  9,  9,  9,  10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11, 11, 11, 11, 11,
  11, 11, 11, 12, 12, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 13, 14, 14};

// column number in small cells (to determine y position)
constexpr std::array<G4int, CEECells + 1> CellRow{
  10, 12, 7,  9, 11, 13, 15, 4,  6,  8,  10, 12, 14, 16, 18, 1,  3,  5, 7, 9, 11, 13, 15,
  17, 19, 21, 0, 2,  4,  6,  8,  10, 12, 14, 16, 18, 20, 22, 1,  3,  5, 7, 9, 11, 13, 15,
  17, 19, 21, 0, 2,  4,  6,  8,  10, 12, 14, 16, 18, 20, 22, 1,  3,  5, 7, 9, 11, 13, 15,
  17, 19, 21, 0, 2,  4,  6,  8,  10, 12, 14, 16, 18, 20, 22, 1,  3,  5, 7, 9, 11, 13, 15,
  17, 19, 21, 0, 2,  4,  6,  8,  10, 12, 14, 16, 18, 20, 22, 1,  3,  5, 7, 9, 11, 13, 15,
  17, 19, 21, 4, 6,  8,  10, 12, 14, 16, 18, 7,  9,  11, 13, 15, 10, 12};

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
