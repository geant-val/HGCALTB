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
constexpr G4int CEECells = CEECellMinCpNo - CEECellMaxCpNo;  // 126

}  // namespace HGCALTBConstants

#endif  // HGCALTBConstants_h 1

//**************************************************
