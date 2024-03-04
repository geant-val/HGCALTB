//**************************************************
// \file HGCALTBSignalHelper.hh
// \brief: definition of HGCALTBSignalHelper class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 4 March 2024
//**************************************************

#ifndef HGCALTBSignalHelper_h
#  define HGCALTBSignalHelper_h 1

// Includers from project files
#  include "HGCALTBConstants.hh"

// Includers from Geant4
#  include "G4TwoVector.hh"

class HGCALTBSignalHelper
{
  public:
    HGCALTBSignalHelper() = default;
    ~HGCALTBSignalHelper() = default;

    G4TwoVector GetSiCellPos(const G4int CpNo) const;

  private:
    // return x-coordinate of small Si cell in mm from copy number
    G4double GetSiCellX(const G4int CpNo) const;
    // return x-coordinate of small Si cell in mm from copy number
    G4double GetSiCellY(const G4int CpNo) const;
};

inline G4double HGCALTBSignalHelper::GetSiCellX(const G4int CpNo) const
{
  G4int Column = HGCALTBConstants::CellColumn.at(CpNo);
  G4double X = -7. * HGCALTBConstants::SCXPitch + Column * HGCALTBConstants::SCXPitch;
  return X;
}

inline G4double HGCALTBSignalHelper::GetSiCellY(const G4int CpNo) const
{
  G4int Row = HGCALTBConstants::CellRow.at(CpNo);
  G4double Y = -12. * HGCALTBConstants::SCYPitch + Row * HGCALTBConstants::SCYPitch;
  return Y;
}

inline G4TwoVector HGCALTBSignalHelper::GetSiCellPos(const G4int CpNo) const
{
  G4TwoVector CellPos{GetSiCellX(CpNo), GetSiCellY(CpNo)};
  return CellPos;
}

#endif  // HGCALTBSignalHelper_h 1

//**************************************************
