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

// Includers from C++
#  include <array>
#  include <numeric>

// #  define DEBUGHELPER

class HGCALTBSignalHelper
{
  public:
    HGCALTBSignalHelper() = default;
    ~HGCALTBSignalHelper() = default;

    // Get x and y (mm) of small Si cell from copy number
    G4TwoVector GetSiCellPos(const G4int CpNo) const;
    // Get layer with nuclear interaction (using 3 layers)
    G4bool IsInteraction(const std::array<G4double, HGCALTBConstants::CEECells + 1> Layer1,
                         const std::array<G4double, HGCALTBConstants::CEECells + 1> Layer2,
                         const std::array<G4double, HGCALTBConstants::CEECells + 1> Layer3,
                         const G4double PrimaryEnergy) const;
    // Get layer with nuclear interaction (using 2 layers)
    G4bool IsInteraction(const std::array<G4double, HGCALTBConstants::CEECells + 1> Layer1,
                         const std::array<G4double, HGCALTBConstants::CEECells + 1> Layer2,
                         const G4double PrimaryEnergy) const;

  private:
    // Return x-coordinate of small Si cell in mm from copy number
    G4double GetSiCellX(const G4int CpNo) const;
    // Return x-coordinate of small Si cell in mm from copy number
    G4double GetSiCellY(const G4int CpNo) const;
    // Calculate Center Of Granvity (COG) x and y (mm) position of small Si cell
    G4TwoVector GetCOGPos(const std::array<G4double, HGCALTBConstants::CEECells + 1> Signals) const;
    // Compute signal in radius
    G4double ComputeSignalRadius(const std::array<G4double, HGCALTBConstants::CEECells + 1> Signals,
                                 const G4double Radius) const;
    // Get threshold for tagging pion nuclear interaction (from CMS paper)
    // goes from 12 to 40 MIPs, from 20 to 200 GeV pions
    G4int ComputeLayerThreshold(const G4double PrimaryEnergy) const;
};

inline G4double HGCALTBSignalHelper::GetSiCellX(const G4int CpNo) const
{
  G4int Column = HGCALTBConstants::CellColumn.at(CpNo);
  G4double X = -7. * HGCALTBConstants::SCXPitch + Column * HGCALTBConstants::SCXPitch;
  return -1. * X;
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

inline G4TwoVector HGCALTBSignalHelper::GetCOGPos(
  const std::array<G4double, HGCALTBConstants::CEECells + 1> Signals) const
{
  G4double COGX{0.};
  G4double COGY{0.};

  for (std::size_t counter = 0; counter <= HGCALTBConstants::CEECells; counter++) {
    auto Pos = GetSiCellPos(counter);
    COGX += Signals.at(counter) * Pos.x();
    COGY += Signals.at(counter) * Pos.y();
  }

  auto TotSignal = std::accumulate(Signals.begin(), Signals.end(), 0.);
  G4TwoVector COGPos{0., 0.};

  if (TotSignal != 0.) {
    COGPos[0] = COGX / TotSignal;
    COGPos[1] = COGY / TotSignal;
  }

  return COGPos;
}

inline G4double HGCALTBSignalHelper::ComputeSignalRadius(
  const std::array<G4double, HGCALTBConstants::CEECells + 1> Signals,
  const G4double Radius /*mm*/) const
{
  G4double SgnlRadius{0.};
  const auto COGPos = GetCOGPos(Signals);

  for (std::size_t counter = 0; counter <= HGCALTBConstants::CEECells; counter++) {
    auto CellPos = GetSiCellPos(counter);
    if ((CellPos - COGPos).mag() < Radius) SgnlRadius += Signals.at(counter);
  }

  return SgnlRadius;
}

inline G4bool HGCALTBSignalHelper::IsInteraction(
  const std::array<G4double, HGCALTBConstants::CEECells + 1> Layer1,
  const std::array<G4double, HGCALTBConstants::CEECells + 1> Layer2,
  const std::array<G4double, HGCALTBConstants::CEECells + 1> Layer3,
  const G4double PrimaryEnergy) const
{
#  ifdef DEBUGHELPER
  G4cout << "PrimaryEnergy " << PrimaryEnergy << " Threshold "
         << ComputeLayerThreshold(PrimaryEnergy) << " SignalRadius 10 cm "
         << ComputeSignalRadius(Layer1, 100) << G4endl;
#  endif

  if (ComputeSignalRadius(Layer1, 100) < ComputeLayerThreshold(PrimaryEnergy)) {
    return false;
  }  // 10 cm

  G4double RLayer = (ComputeSignalRadius(Layer1, 20) + ComputeSignalRadius(Layer2, 20)
                     + ComputeSignalRadius(Layer3, 20))
                    / (ComputeSignalRadius(Layer1, 100) + ComputeSignalRadius(Layer2, 100)
                       + ComputeSignalRadius(Layer3, 100));

  if (RLayer > 0.96) {
    return false;
  }
  else {
    return true;
  }
}

inline G4bool HGCALTBSignalHelper::IsInteraction(
  const std::array<G4double, HGCALTBConstants::CEECells + 1> Layer1,
  const std::array<G4double, HGCALTBConstants::CEECells + 1> Layer2,
  const G4double PrimaryEnergy) const
{
#  ifdef DEBUGHELPER
  G4cout << "PrimaryEnergy " << PrimaryEnergy << " Threshold "
         << ComputeLayerThreshold(PrimaryEnergy) << " SignalRadius 10 cm "
         << ComputeSignalRadius(Layer1, 100) << G4endl;
#  endif

  if (ComputeSignalRadius(Layer1, 100) < ComputeLayerThreshold(PrimaryEnergy)) {
    return false;
  }  // 10 cm

  G4double RLayer = (ComputeSignalRadius(Layer1, 20) + ComputeSignalRadius(Layer2, 20))
                    / (ComputeSignalRadius(Layer1, 100) + ComputeSignalRadius(Layer2, 100));

  if (RLayer > 0.96) {
    return false;
  }
  else {
    return true;
  }
}

inline G4int HGCALTBSignalHelper::ComputeLayerThreshold(const G4double PrimaryEnergy) const
{
  auto Energy = static_cast<int>(PrimaryEnergy * GeV);  // from MeV to GeV
  if (Energy <= 20) return 12;  // MIPs

  auto Threshold = 12 + (Energy - 20) * (28 / 180);  // goes from 12 to 40 MIPs from 20 to 200 GeV

  return Threshold;
}

#endif  // HGCALTBSignalHelper_h 1

//**************************************************
