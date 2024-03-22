//**************************************************
// \file HGCALTBAHCAL.hh
// \brief: definition of HGCALTBAHCAL class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 14 January 2024
//**************************************************

#ifndef HGCALTBAHCALSD_h
#  define HGCALTAHCALSD_h 1

// Includers from Geant4
//
#  include "G4VSensitiveDetector.hh"

// Includers form project files
//
#  include "HGCALTBAHHit.hh"
#  include "HGCALTBConstants.hh"

// #  define DEBUGAHCALSD

// Forward declaration from Geant4
//
class G4Step;
class G4HCofThisEvent;

class HGCALTBAHCALSD : public G4VSensitiveDetector
{
  public:
    HGCALTBAHCALSD(const G4String& name);
    virtual ~HGCALTBAHCALSD();

    // virtual methods from base class
    //
    virtual void Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* history);
    virtual void EndOfEvent(G4HCofThisEvent* hitCollection);

    // This sensitive detector creates 1 hit collection
    //
    static const G4String fAHCALHitsCollectionName;

  private:
    HGCALTBAHCALHitsCollection* fHitsCollection;
    inline G4int MapTileCpNo(G4int cpno) const;
    inline G4double GetBirk(const G4Step* aStep) const;
    inline G4double GetSimpleBirk(const G4Step* aStep) const;
};

inline G4double HGCALTBAHCALSD::GetBirk(const G4Step* const aStep) const
{
  // from https://github.com/cms-sw/cmssw/blob/master/SimG4CMS/Calo/src/CaloSD.cc#L727
  double weight = 1.;
  double charge = aStep->GetPreStepPoint()->GetCharge();
  double length = aStep->GetStepLength();
  double birk1 = 0.0052;
  double birk2 = 0.142;
  double birk3 = 1.75;

  if (charge != 0. && length > 0.) {
    double density = aStep->GetPreStepPoint()->GetMaterial()->GetDensity() / (g / cm3);
    double dedx = aStep->GetTotalEnergyDeposit() / length;
    double rkb = birk1 / density;
    double c = birk2 * rkb * rkb;
    if (std::abs(charge) >= 2.) {
      rkb /= birk3;
    }  // based on alpha particle data
    weight = 1. / (1. + rkb * dedx + c * dedx * dedx);
  }

  return weight;
}

inline G4double HGCALTBAHCALSD::GetSimpleBirk(const G4Step* aStep) const
{
  const G4double kBirk = 0.126;  // mm/MeV
  const auto edep = aStep->GetTotalEnergyDeposit();  // MeV
  const auto stepl = aStep->GetStepLength();  // mm
  G4double weight = 1. / (1. + kBirk * (edep / stepl));
  if (weight > 1.) weight = 1.;
  return weight;
}

inline G4int HGCALTBAHCALSD::MapTileCpNo(G4int cpno) const
{
  /*quadrant 1 (up-right):
  101..112
  .
  .
  1201..1212

  quadrant 2 (up-left):
  10101..10112
  .
  .
  11201..11212

  quadrant 3 (down-right):
  100101..100112
  .
  .
  101201..101212

  quadrant 4 (down-left):
  110101..110112
  .
  .
  111201..111212*/

  // Find quadrant
  G4int quadrant = 0;
  if (cpno <= 1212)
    quadrant = 1;
  else if (cpno > 1212 && cpno <= 11212)
    quadrant = 2;
  else if (cpno > 11212 && cpno <= 101212)
    quadrant = 3;
  else if (cpno > 101212 && cpno <= 111212)
    quadrant = 4;
  else {
    G4ExceptionDescription msg;
    msg << "Wrong AHCAL cpno" << cpno;
    G4Exception("HGCALTBAHCALSD::MapTileCpNo", "MyCode0004", FatalException, msg);
  }

  if (quadrant == 2)
    cpno = cpno - 10000;
  else if (quadrant == 3)
    cpno = cpno - 100000;
  else if (quadrant == 4)
    cpno = cpno - 110000;

  // Find row (1...12)
  G4int row = 99;
  row = cpno / 100;
  if (row < 1 || row > HGCALTBConstants::AHCALrow / 2) {
    G4ExceptionDescription msg;
    msg << "Wrong AHCAL row in quadrant 1:" << row;
    G4Exception("HGCALTBAHCALSD::MapTileCpNo", "MyCode0004", FatalException, msg);
  }
  row = row - 1;  // make row number start from 0

  // Find column (1..12)
  G4int column = 99;
  column = cpno - (row + 1) * 100;
  if (column < 1 || row > HGCALTBConstants::AHCALcolumn / 2) {
    G4ExceptionDescription msg;
    msg << "Wrong AHCAL column in quadrant 1:" << column;
    G4Exception("HGCALTBAHCALSD::MapTileCpNo", "MyCode0004", FatalException, msg);
  }
  column = column - 1;  // make column number start from 0

  // Construct final cpno from 0 to 575 (24x24 tiles per layer)
  quadrant = quadrant - 1;  // make quadrant number start from 0
  G4int newcpno = row + column * HGCALTBConstants::AHCALrow / 2
                  + quadrant * HGCALTBConstants::AHCALtileperlayer / 4;
  if (newcpno >= HGCALTBConstants::AHCALtileperlayer) {
    G4ExceptionDescription msg;
    msg << "Wrong AHCAL newcpno: " << newcpno;
    G4Exception("HGCALTBAHCALSD::MapTileCpNo", "MyCode0004", FatalException, msg);
  }

#  ifdef DEBUGAHCALSD
  G4cout << "cpno " << cpno << " quadrant " << quadrant << " column " << column << " row " << row
         << " newcpno " << newcpno << G4endl;
#  endif

  return newcpno;
}

#endif  // HGCALTBAHCALSD_h 1

//**************************************************
