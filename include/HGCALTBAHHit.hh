//**************************************************
// \file HGCALTBAHHit.hh
// \brief: definition of HGCALTBAHHit class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 14 January 2024
//**************************************************

#ifndef HGCALTBAHHit_h
#  define HGCALTBAHHit_h 1

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

class HGCALTBAHHit : public G4VHit
{
  public:
    HGCALTBAHHit();
    HGCALTBAHHit(const HGCALTBAHHit&);
    virtual ~HGCALTBAHHit();

    // Operators (= and ==)
    //
    const HGCALTBAHHit& operator=(const HGCALTBAHHit&);
    G4bool operator==(const HGCALTBAHHit&) const;

    // virtual methods from base class
    //
    virtual void Draw() {}
    virtual void Print(){};

    void AddTileEdep(G4int TileID, G4double Edep);

    // return a copy of the fCEESignals array
    //
    std::array<G4double, HGCALTBConstants::AHCALtileperlayer> GetAHSignals() const
    {
      return fAHSignals;
    }

  private:
    std::array<G4double, HGCALTBConstants::AHCALtileperlayer> fAHSignals;
};

using HGCALTBAHCALHitsCollection = G4THitsCollection<HGCALTBAHHit>;

inline void HGCALTBAHHit::AddTileEdep(G4int TileID, G4double Edep)
{
  fAHSignals[TileID] += Edep;
}

#endif  // HGCALTBAHHit_h 1

//**************************************************
