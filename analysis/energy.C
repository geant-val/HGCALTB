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

struct AnalysisOutput
{
    double CEEAvg;
    double CEESigma;
    double CHEAvg;
    double CHESigma;
};

AnalysisOutput DoAnalysis(const int RunNo, const double ene);

void energy()
{
  // beam energies
  const int runs = 8;
  const std::array<double, runs> energies{20., 50., 80., 100., 120., 200., 250., 300.};

  // re-create output file
  TFile* file(TFile::Open("HGCALTBpiene.root", "RECREATE"));
  file->Close();

  // final arrays
  std::array<double, runs> CEEresp{0.};
  std::array<double, runs> CHEresp{0.};

  // do analysis over loop
  for (std::size_t i = 0; i < runs; i++) {
    auto out = DoAnalysis(i, energies[i]);
    CEEresp[i] = out.CEEAvg / energies[i];
    CHEresp[i] = out.CHEAvg / energies[i];
  }

  TGraph GrCEEresp(runs, energies.data(), CEEresp.data());
  GrCEEresp.SetTitle("CEE response; Beam Energy [GeV]; Response");
  GrCEEresp.SetName("CEEresponse");
  TGraph GrCHEresp(runs, energies.data(), CHEresp.data());
  GrCHEresp.SetTitle("CHE response; Beam Energy [GeV]; Response");
  GrCHEresp.SetName("CHEresponse");
  TFile* outputfile(TFile::Open("HGCALTBpiene.root", "UPDATE"));
  outputfile->cd();
  GrCEEresp.Write();
  GrCHEresp.Write();
  outputfile->Close();
}

AnalysisOutput DoAnalysis(const int RunNo, const double ene)
{
  const std::string ene_name = std::to_string(static_cast<int>(ene));
  const string filename = "Data1/HGCALTBout_Run" + std::to_string(RunNo) + ".root";
  cout << "Analysis of " << filename << endl;
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

  const auto H1CEEname = ("H1CEE" + ene_name).c_str();
  const auto H1CHEname = ("H1CHE" + ene_name).c_str();
  const auto H1Totname = ("H1CTot" + ene_name).c_str();
  TH1F H1CEE(H1CEEname, H1CEEname, static_cast<int>(ene) * 2, 0., ene * 2.);
  TH1F H1CHE(H1CHEname, H1CHEname, static_cast<int>(ene) * 2, 0., ene * 2.);
  TH1F H1TOT(H1Totname, H1Totname, static_cast<int>(ene) * 2, 0., ene * 2.);

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

  TFile* outputfile(TFile::Open("HGCALTBpiene.root", "UPDATE"));
  outputfile->cd();
  H1CEE.Fit("gaus", "Q");
  H1CEE.Write();
  H1CHE.Fit("gaus", "Q");
  H1CHE.Write();
  H1TOT.Write();
  outputfile->Close();

  auto H1CEEAvg = H1CEE.GetFunction("gaus")->GetParameter(1);
  auto H1CEESigma = H1CEE.GetFunction("gaus")->GetParameter(2);
  auto H1CHEAvg = H1CHE.GetFunction("gaus")->GetParameter(1);
  auto H1CHESigma = H1CHE.GetFunction("gaus")->GetParameter(2);
  AnalysisOutput out{H1CEEAvg, H1CEESigma, H1CHEAvg, H1CHESigma};
  return out;
}
