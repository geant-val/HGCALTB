//**************************************************
// \file HGCALTBCHEHit.hh
// \brief: definition of HGCALTBCHEHit class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 12 January 2024
//**************************************************

#ifndef HGCALTBCHEHit_h
#  define HGCALTBCHEHit_h 1

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

class HGCALTBCHEHit : public G4VHit
{
  public:
    HGCALTBCHEHit();
    HGCALTBCHEHit(const HGCALTBCHEHit&);
    virtual ~HGCALTBCHEHit();

    // Operators (= and ==)
    //
    const HGCALTBCHEHit& operator=(const HGCALTBCHEHit&);
    G4bool operator==(const HGCALTBCHEHit&) const;

    // virtual methods from base class
    //
    virtual void Draw() {}
    virtual void Print(){};

    void AddCellEdep(G4int CellID, G4double Edep);

    // return a copy of the fCHESignals array
    //
    std::array<G4double, HGCALTBConstants::CHECells + 1> GetCHESignals() const
    {
      return fCHESignals;
    }

  private:
    std::array<G4double, HGCALTBConstants::CHECells + 1> fCHESignals;
};

using HGCALTBCHEHitsCollection = G4THitsCollection<HGCALTBCHEHit>;

inline void HGCALTBCHEHit::AddCellEdep(G4int CellID, G4double Edep)
{
  fCHESignals[CellID] += Edep;
}

#endif  // HGCALTBCHEHit_h 1

//**************************************************
