//**************************************************
// \file HGCALTBCHESD.hh
// \brief: definition of HGCALTBCHESD class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 12 January 2024
//**************************************************

#ifndef HGCALTBCHESD_h
#  define HGCALTBCHESD_h 1

// Includers from Geant4
//
#  include "G4VSensitiveDetector.hh"

// Includers form project files
//
#  include "HGCALTBCHEHit.hh"

// Includers from std
//
#  include <unordered_map>

// #define DEBUGCHESD

// Forward declaration from Geant4
//
class G4Step;
class G4HCofThisEvent;

class HGCALTBCHESD : public G4VSensitiveDetector
{
  public:
    HGCALTBCHESD(const G4String& name);
    virtual ~HGCALTBCHESD();

    // virtual methods from base class
    //
    virtual void Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* history);
    virtual void EndOfEvent(G4HCofThisEvent* hitCollection);

    // This sensitive detector creates 1 hit collection
    //
    static const G4String fCHEHitsCollectionName;

  private:
    HGCALTBCHEHitsCollection* fHitsCollection;
    const std::unordered_map<G4int, G4int> CHEWaferMap;
    inline G4int FindWaferID(G4int cpno) const;
};

inline G4int HGCALTBCHESD::FindWaferID(G4int cpno) const
{
  auto key = CHEWaferMap.find(cpno);
  if (key != CHEWaferMap.end()) {
#  ifdef DEBUGCHESD
    G4cout << "CHE Wafer cpno " << cpno << " id " << key->second << G4endl;
#  endif
    return key->second;
  }
  else {
    G4ExceptionDescription msg;
    msg << "CHE Wafer cpno not in map";
    G4Exception("HGCALTBCHESD::FindWaferID", "MyCode0004", FatalException, msg);
    return 1;
  }
}

#endif  // HGCALTBCHESD_h 1

//**************************************************
