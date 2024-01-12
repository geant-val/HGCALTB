// Simple macro card to plot the cell energy per layer in CEE and CHE.
// Can be used with 200 GeV mu- data to study the MIP calibration,
// either with raw data (MeV in silicon cells) or calibrated data
//(MIP in silicon cells).
void calibration()
{
  const string filename = "HGCALTBout_Run0.root";
  TFile* file = TFile::Open(filename.c_str(), "READ");
  TTree* tree = (TTree*)file->Get("HGCALTBout");

  double CEETot{0.};
  tree->SetBranchAddress("CEETot", &CEETot);
  double CHETot{0.};
  tree->SetBranchAddress("CHETot", &CHETot);
  vector<double>* CEESignals = NULL;
  tree->SetBranchAddress("CEESignals", &CEESignals);
  vector<double>* CHESignals = NULL;
  tree->SetBranchAddress("CHESignals", &CHESignals);

  TH1F H1CEE24("H1CEE24", "H1CEE24", 500, 0., 5);
  TH1F H1CHE4("H1CHE4", "H1CHE4", 500, 0., 5);

  for (std::size_t evtNo = 0; evtNo < tree->GetEntries(); evtNo++) {
    tree->GetEntry(evtNo);

    H1CEE24.Fill(CEESignals->at(24));
    H1CHE4.Fill(CHESignals->at(4));
  }

  TFile* outputfile(TFile::Open("HGCALTBcalib.root", "RECREATE"));
  H1CEE24.Write();
  H1CHE4.Write();
  outputfile->Close();
}
