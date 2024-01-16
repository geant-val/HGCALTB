//**************************************************
// \file HGCALTBEventAction.hh
// \brief: Definition of HGCALTBEventAction class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 11 January 2024
//**************************************************

// Prevent including header multiple times
//
#ifndef HGCALTBEventAction_h
#  define HGCALTBEventAction_h 1

#  include <vector>

// Includers from Gean4
//
#  include "G4UserEventAction.hh"
#  include "globals.hh"

// Includers from project files
//
#  include "HGCALTBAHHit.hh"
#  include "HGCALTBCEEHit.hh"
#  include "HGCALTBCHEHit.hh"
#  include "HGCALTBConstants.hh"

class HGCALTBEventAction : public G4UserEventAction
{
  public:
    HGCALTBEventAction();
    virtual ~HGCALTBEventAction();

    // virtual methods from base class
    virtual void BeginOfEventAction(const G4Event* event);
    virtual void EndOfEventAction(const G4Event* event);

    void Addedep(G4double stepedep);
    void SetIntLayer(G4int IntTrack);
    std::vector<G4double>& GetCEESignals() { return fCEELayerSignals; };
    std::vector<G4double>& GetCHESignals() { return fCHELayerSignals; };
    std::vector<G4double>& GetAHCALSignals() { return fAHCALLayerSignals; };

  private:
    HGCALTBCEEHitsCollection* GetCEEHitsCollection(G4int hcID, const G4Event* event) const;
    HGCALTBCHEHitsCollection* GetCHEHitsCollection(G4int hcID, const G4Event* event) const;
    HGCALTBAHCALHitsCollection* GetAHCALHitsCollection(G4int hcID, const G4Event* event) const;
    G4double edep;  // energy deposited in every volume
    G4int fIntLayer;  // 1 if primary interacted in CEE, 0 otherwise
    std::vector<G4double> fCEELayerSignals;  // signals per CEE layer
    std::vector<G4double> fCHELayerSignals;  // signals per CHE layer
    std::vector<G4double> fAHCALLayerSignals;  // signals per AHCAL layer
};

inline void HGCALTBEventAction::Addedep(G4double stepedep)
{
  edep += stepedep;
}

inline void HGCALTBEventAction::SetIntLayer(G4int IntTrack)
{
  fIntLayer = IntTrack;
}

#endif  // HGCALTBEventAction_h 1

//**************************************************
