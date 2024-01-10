//**************************************************
// \file HGCALTBDetConstruction.hh
// \brief: definition of HGCALTBDetConstruction
//         class
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 10 January 2024
//**************************************************

#ifndef HGCALTBDetConstruction_h
#  define HGCALTBDetConstruction_h 1

// Includers from Geant4
//
#  include "G4VUserDetectorConstruction.hh"

class ATLTileCalTBDetConstruction : public G4VUserDetectorConstruction
{
  public:
    ATLTileCalTBDetConstruction();
    ~ATLTileCalTBDetConstruction();
    // virtual methods from base class
    //
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

  private:
    void DefineVisAttributes();
};

#endif  // HGCALTBDetConstruction_h

//**************************************************
