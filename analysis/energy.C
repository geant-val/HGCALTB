// Simple macro card to reconstruct pion energies

// Calibration constants from TB paper
const double alpha = 10.5;  // MeV/MIP
const double beta = 80.0;  // MeV/MIP
const double delta = 0.4;  // MeV/MIP

// Apply calibration constants
double recene(const double CEETot, const double CHETOT, const double AHCALTot)
{
  auto ene = alpha * CEETot + beta * (CHETOT + delta * AHCALTot);
  return ene;
}

void energy()
{
  const string filename = "HGCALTBout_Run0.root";
  TFile* file = TFile::Open(filename.c_str(), "READ");
  TTree* tree = (TTree*)file->Get("HGCALTBout");

  double CEETot{0.};
  tree->SetBranchAddress("CEETot", &CEETot);
  double CHETot{0.};
  tree->SetBranchAddress("CHETot", &CHETot);
  double AHCALTot{0.};
  tree->SetBranchAddress("AHCALTot", &AHCALTot);
  double HGCALTot{0.};
  tree->SetBranchAddress("HGCALTot", &HGCALTot);
  vector<double>* CEESignals = NULL;
  tree->SetBranchAddress("CEESignals", &CEESignals);
  vector<double>* CHESignals = NULL;
  tree->SetBranchAddress("CHESignals", &CHESignals);
  vector<double>* AHCALSignals = NULL;
  tree->SetBranchAddress("AHCALSignals", &AHCALSignals);

  TH1F H1CEE("H1CEE", "H1CEE", 300, 0., 150.);
  TH1F H1CHE("H1CHE", "H1CHE", 300, 0., 150.);
  TH1F H1TOT("H1Tot", "H1Tot", 300, 0., 150.);

  bool CEEInteracted = false;

  for (std::size_t evtNo = 0; evtNo < tree->GetEntries(); evtNo++) {
    tree->GetEntry(evtNo);
    if (HGCALTot < 300.) continue;  // <300 MIP, not interacting event
    auto piene = recene(CEETot, CHETot, AHCALTot) / 1000;  // GeV

    if (CEETot > 200.) CEEInteracted = true;

    if (CEEInteracted)
      H1CEE.Fill(piene);
    else
      H1CHE.Fill(piene);

    H1TOT.Fill(piene);
    CEEInteracted = false;
  }

  TFile* outputfile(TFile::Open("HGCALTBpiene.root", "RECREATE"));
  H1CEE.Write();
  H1CHE.Write();
  H1TOT.Write();
  outputfile->Close();
}
