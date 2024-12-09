//**************************************************
// \file HGCALTB.cc
// \brief: main() of HGCALTB
// \author: Lorenzo Pezzotti (CERN EP-SFT-sim)
//          @lopezzot
// \start date: 10 January 2024
//**************************************************

// Includers from project files
//
#include "HGCALTBActInitialization.hh"
#include "HGCALTBDetConstruction.hh"

// Includers from Geant4
//
#ifdef G4MULTITHREADED
#  include "G4MTRunManager.hh"
#  include "G4Threading.hh"
#else
#  include "G4RunManager.hh"
#endif
// #include "G4RunManagerFactory.hh" //only available from 10.7 on
#include "G4PhysListFactory.hh"
#include "G4UIExecutive.hh"
#include "G4UIcommand.hh"
#include "G4UImanager.hh"
#include "G4Version.hh"
#include "G4VisExecutive.hh"

#ifdef USE_ADEPT
#include "FTFP_BERT_AdePT.hh"
#endif

// CLI string outputs
namespace CLIOutputs
{
void PrintHelp()
{
  G4cout << "Usage: HGCALTB [OPTION...]\n\n"
         << "Options:\n"
         << "  -m MACRO        path to macro file to run\n"
         << "  -u UISESSION    string of the Geant4 UI session to use\n"
         << "  -t THREADS      number of threads to use in the simulation\n"
         << "  -p PHYSICSLIST  string of the physics list to use\n"
         << "  -f FILENAME     optional string with custom file name\n"
         << "  -h              print this help and exit\n"
         << G4endl;
}
void PrintError()
{
  G4cerr << "Wrong usage, see 'HGCALTB -h' for more information" << G4endl;
}
}  // namespace CLIOutputs

// G4err output for PhysListFactory usage error
//
namespace PrintPLFactoryUsageError
{
void PLFactoryUsageError()
{
  G4cerr << "Wrong PLFactory usage: no name for selected PL. " << G4endl;
}
}  // namespace PrintPLFactoryUsageError

int main(int argc, char** argv)
{
  // CLI variables
  G4String macro;
  G4String session;
  G4String custom_pl = "FTFP_BERT";  // default physics list
  G4String custom_filename = "";  // default file name selected by RunAction
#ifdef G4MULTITHREADED
  G4int nThreads = G4Threading::G4GetNumberOfCores();
#endif
#ifdef USE_ADEPT
  G4bool adept{false};
#endif

  // CLI parsing
  for (G4int i = 1; i < argc; i = i + 2) {
    if (G4String(argv[i]) == "-m")
      macro = argv[i + 1];
    else if (G4String(argv[i]) == "-u")
      session = argv[i + 1];
    else if (G4String(argv[i]) == "-p")
      custom_pl = argv[i + 1];
    else if (G4String(argv[i]) == "-f")
      custom_filename = argv[i + 1];
#ifdef G4MULTITHREADED
    else if (G4String(argv[i]) == "-t") {
      nThreads = G4UIcommand::ConvertToInt(argv[i + 1]);
    }
#endif
#ifdef USE_ADEPT
    else if (G4String(argv[i]) == "--adept") {
      adept = true;
      i-=1;
    }
#endif
    else if (G4String(argv[i]) == "-h") {
      CLIOutputs::PrintHelp();
      return 0;
    }
    else {
      CLIOutputs::PrintError();
      return 1;
    }
  }

  // Activate interaction mode if no macro card is provided and define UI
  // session
  //
  G4UIExecutive* ui = nullptr;
  if (!macro.size()) {  // if macro card is not passed
    ui = new G4UIExecutive(argc, argv, session);
  }

  // Construct the run manager
  //
#ifdef G4MULTITHREADED
  auto runManager = new G4MTRunManager;
  if (nThreads > 0) {
    runManager->SetNumberOfThreads(nThreads);
  }
#else
  auto runManager = new G4RunManager;
#endif

  // Manadatory Geant4 classes
  //
  auto physListFactory = new G4PhysListFactory();
  if (!physListFactory->IsReferencePhysList(custom_pl)) {  // if custom_pl is not a PLname exit
    PrintPLFactoryUsageError::PLFactoryUsageError();
    return 1;
  }
  G4VUserPhysicsList *physicsList;
  #ifndef USE_ADEPT
    physicsList = physListFactory->GetReferencePhysList(custom_pl);
  #else
    if(adept)
      physicsList = new FTFP_BERT_AdePT();
    else
      physicsList = physListFactory->GetReferencePhysList(custom_pl);
  #endif
  runManager->SetUserInitialization(physicsList);

  runManager->SetUserInitialization(new HGCALTBDetConstruction());

  runManager->SetUserInitialization(new HGCALTBActInitialization(custom_filename));

  // Visualization manager construction
  //
  auto visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();

  // Get the pointer to the User Interface manager
  //
  auto UImanager = G4UImanager::GetUIpointer();

  if (!ui) {
    // execute an argument macro file if exist (second parser argument)
    G4String command = "/control/execute ";
    UImanager->ApplyCommand(command + macro);
  }
  else {
    UImanager->ApplyCommand("/control/execute init_vis.mac");
    if (ui->IsGUI()) {
      UImanager->ApplyCommand("/control/execute gui.mac");
    }
    // start interactive session
    ui->SessionStart();
    delete ui;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program
  //
  delete visManager;
  delete runManager;
}

//**************************************************
