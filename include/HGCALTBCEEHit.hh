//**************************************************
// \file HGCALTBCEEHit.hh
// \brief: definition of HGCALTBCEEHit class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 January 2024
//**************************************************

#ifndef HGCALTBCEEHit_h
#  define HGCALTBCEEHit_h 1

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

    // Methods to handle data
    //
    void AddEdep(G4double dEdep);

    // Get methods
    //
    G4double GetEdep() const;

  private:
    // Total energy deposition in the CEE layer
    G4double fEdep;
};

using HGCALTBCEEHitsCollection = G4THitsCollection<HGCALTBCEEHit>;

inline void HGCALTBCEEHit::AddEdep(G4double dEdep)
{
  fEdep += dEdep;
}

inline G4double HGCALTBCEEHit::GetEdep() const
{
  return fEdep;
}

#endif  // HGCALTBCEEHit_h 1

//**************************************************
