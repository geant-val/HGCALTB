//**************************************************
// \file HGCALTBCEESD.hh
// \brief: definition of HGCALTBCEESD class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 January 2024
//**************************************************

#ifndef HGCALTBCEESD_h
#  define HGCALTBCEESD_h 1

// Includers from Geant4
//
#  include "G4VSensitiveDetector.hh"

// Includers form project files
//
// #include "HGCALTBCEEHit.hh"

// Forward declaration from Geant4
//
class G4Step;
class G4HCofThisEvent;

class HGCALTBCEESD : public G4VSensitiveDetector
{
  public:
    HGCALTBCEESD(const G4String& name, const G4String& hitsCollectionName);
    virtual ~HGCALTBCEESD();

    // virtual methods from base class
    //
    virtual void Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* history);
    virtual void EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    // ATLTileCalTBHitsCollection* fHitsCollection;
};

#endif  // HGCALTBCEESD_h 1

//**************************************************
