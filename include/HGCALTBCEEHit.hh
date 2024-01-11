//**************************************************
// \file HGCALTBCEEHit.hh
// \brief: definition of HGCALTBCEEHit class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 January 2024
//**************************************************

#ifndef HGCALTBCEEHit_h
#  define HGCALTBCEEHit_h 1

// Includers from project files
//
#  include "HGCALTBConstants.hh"

// Includers from Geant4
//
#  include "G4THitsCollection.hh"
#  include "G4VHit.hh"

// Includers from C++
//
#  include <array>

class HGCALTBCEEHit : public G4VHit
{
  public:
    HGCALTBCEEHit();
    HGCALTBCEEHit(const HGCALTBCEEHit&);
    virtual ~HGCALTBCEEHit();

    // Operators (= and ==)
    //
    const HGCALTBCEEHit& operator=(const HGCALTBCEEHit&);
    G4bool operator==(const HGCALTBCEEHit&) const;

    // virtual methods from base class
    //
    virtual void Draw() {}
    virtual void Print(){};

    void AddCellEdep(G4int CellID, G4double Edep);

  private:
    std::array<G4double, HGCALTBConstants::CEECells> fCEESignals;
};

using HGCALTBCEEHitsCollection = G4THitsCollection<HGCALTBCEEHit>;

inline void HGCALTBCEEHit::AddCellEdep(G4int CellID, G4double Edep)
{
  fCEESignals[CellID] += Edep;
}

#endif  // HGCALTBCEEHit_h 1

//**************************************************
