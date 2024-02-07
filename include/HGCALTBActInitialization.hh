//**************************************************
// \file HGCALTBActInitialization.hh
// \brief: definition of HGCALTBActInitialization
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 10 January 2024
//**************************************************

#ifndef HGCALTBActInitialization_h
#  define HGCALTBActInitialization_h 1

// Includers from Geant4
//
#  include "G4String.hh"
#  include "G4VUserActionInitialization.hh"

class HGCALTBActInitialization : public G4VUserActionInitialization
{
  public:
    HGCALTBActInitialization(G4String filename);
    virtual ~HGCALTBActInitialization();
    // vitual methods from base class
    virtual void BuildForMaster() const;
    virtual void Build() const;

  private:
    G4String fFileName;
};

#endif  // HGCALTBActInitialization_h

//**************************************************
