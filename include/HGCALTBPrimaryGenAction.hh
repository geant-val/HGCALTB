//**************************************************
// \file HGCALTBPrimaryGenAction.hh
// \brief: definition of
//         HGCALTBPrimaryGenAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 10 January 2024
//**************************************************

#ifndef HGCALTBPrimaryGenAction_h
#  define HGCALTBPrimaryGenAction_h 1

// Includers from Geant4
//
#  include "G4Types.hh"
#  include "G4VUserPrimaryGeneratorAction.hh"

// Forward declaration from Geant4
//
class G4ParticleGun;
class G4Event;

class HGCALTBPrimaryGenAction : public G4VUserPrimaryGeneratorAction
{
  public:
    HGCALTBPrimaryGenAction();
    virtual ~HGCALTBPrimaryGenAction();

    // virtual method from base class
    virtual void GeneratePrimaries(G4Event* event);

    // get particle gun
    const G4ParticleGun* GetParticleGun() const;

  private:
    G4ParticleGun* fParticleGun;
};

inline const G4ParticleGun* HGCALTBPrimaryGenAction::GetParticleGun() const
{
  return fParticleGun;
}

#endif  // HGCALTBPrimaryGenAction_h

//**************************************************
