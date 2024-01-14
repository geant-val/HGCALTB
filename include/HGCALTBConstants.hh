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

// HGCALCEE constants
//

// CEE longitudinal layers (one big hexagon per layer)
constexpr G4int CEELayers = 28;

// CEE copynumbers for HGCalEECellCoarse (exluding HGCALEECellCoarseHalf)
constexpr G4int CEECellMinCpNo = 2003;
constexpr G4int CEECellMaxCpNo = 2129;

// CEE (small) silicon cells per big hexagon (layer)
constexpr G4int CEECells = CEECellMaxCpNo - CEECellMinCpNo;  // 126

// HGCALCHE constants
//

// CHE longitudinal layers
constexpr G4int CHELayers = 12;

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
constexpr G4int AHCALtileperlayer = AHCALrow * AHCALcolumn;

// MIP calibration
//
constexpr G4double MIPSilicon = 0.085;  // MeV

}  // namespace HGCALTBConstants

#endif  // HGCALTBConstants_h 1

//**************************************************
