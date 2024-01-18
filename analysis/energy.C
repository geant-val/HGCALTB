// Simple macro card to reconstruct pion energies
// Usage: root 'energy.C("path-to-data")'

// Calibration constants from TB paper
double alpha = 10.5;  // MeV/MIP
double beta = 80.0;  // MeV/MIP
double delta = 0.4;  // MeV/MIP

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

AnalysisOutput DoAnalysis(const int RunNo, const double ene, const bool Write, const string path);

void energy(const string path)
{
  // beam energies
  const int runs = 8;
  const std::array<double, runs> energies{20., 50., 80., 100., 120., 200., 250., 300.};

  // re-create output file
  TFile* file(TFile::Open("HGCALTBpiene.root", "RECREATE"));
  file->Close();

  // final arrays before correction
  std::array<double, runs> CEEresp{0.};
  std::array<double, runs> CHEresp{0.};
  std::array<double, runs> CEEresl{0.};
  std::array<double, runs> CHEresl{0.};
  // final arrays after correction
  std::array<double, runs> CEErespcorr{0.};
  std::array<double, runs> CHErespcorr{0.};
  std::array<double, runs> CEEreslcorr{0.};
  std::array<double, runs> CHEreslcorr{0.};

  // do analysis over loop
  cout << "Using calibration constants alpha " << alpha << " beta " << beta << " delta " << delta
       << endl;
  for (std::size_t i = 0; i < runs; i++) {
    auto out = DoAnalysis(i, energies[i], true, path);
    CEEresp[i] = out.CEEAvg / energies[i];
    CHEresp[i] = out.CHEAvg / energies[i];
    CEEresl[i] = out.CEESigma / out.CEEAvg;
    CHEresl[i] = out.CHESigma / out.CHEAvg;
  }

  // apply calibration corrections as TB paper
  beta = beta / CHEresp[1];  // use run with pi- at 50 GeV to recalibrate
  alpha = alpha / 1.035;  // from TB paper with e+
  cout << "Using calibration constants alpha " << alpha << " beta " << beta << " delta " << delta
       << endl;
  for (std::size_t i = 0; i < runs; i++) {
    auto out = DoAnalysis(i, energies[i], false, path);
    CEErespcorr[i] = out.CEEAvg / energies[i];
    CHErespcorr[i] = out.CHEAvg / energies[i];
    CEEreslcorr[i] = out.CEESigma / out.CEEAvg;
    CHEreslcorr[i] = out.CHESigma / out.CHEAvg;
  }

  TFile* outputfile(TFile::Open("HGCALTBpiene.root", "UPDATE"));
  outputfile->cd();

  TGraph GrCEEresp(runs, energies.data(), CEEresp.data());
  GrCEEresp.SetTitle("CEE response; Beam Energy [GeV]; Response");
  GrCEEresp.SetName("CEEresponse");
  TGraph GrCHEresp(runs, energies.data(), CHEresp.data());
  GrCHEresp.SetTitle("CHE response; Beam Energy [GeV]; Response");
  GrCHEresp.SetName("CHEresponse");
  TGraph GrCEEresl(runs, energies.data(), CEEresl.data());
  GrCEEresl.SetTitle("CEE resolution; Beam Energy [GeV]; Resolution");
  GrCEEresl.SetName("CEEresolution");
  TGraph GrCHEresl(runs, energies.data(), CHEresl.data());
  GrCHEresl.SetTitle("CHE resolution; Beam Energy [GeV]; Resolution");
  GrCHEresl.SetName("CHEresolution");
  GrCEEresp.Write();
  GrCHEresp.Write();
  GrCEEresl.Write();
  GrCHEresl.Write();

  TGraph GrCEErespcorr(runs, energies.data(), CEErespcorr.data());
  GrCEErespcorr.SetTitle("CEE response corrected; Beam Energy [GeV]; Response");
  GrCEErespcorr.SetName("CEEresponsecorrected");
  TGraph GrCHErespcorr(runs, energies.data(), CHErespcorr.data());
  GrCHErespcorr.SetTitle("CHE response corrected; Beam Energy [GeV]; Response");
  GrCHErespcorr.SetName("CHEresponsecorrected");
  TGraph GrCEEreslcorr(runs, energies.data(), CEEreslcorr.data());
  GrCEEreslcorr.SetTitle("CEE resolution corrected; Beam Energy [GeV]; Resolution");
  GrCEEreslcorr.SetName("CEEresolutioncorrected");
  TGraph GrCHEreslcorr(runs, energies.data(), CHEreslcorr.data());
  GrCHEreslcorr.SetTitle("CHE resolution corrected; Beam Energy [GeV]; Resolution");
  GrCHEreslcorr.SetName("CHEresolutioncorrected");
  GrCEErespcorr.Write();
  GrCHErespcorr.Write();
  GrCEEreslcorr.Write();
  GrCHEreslcorr.Write();

  outputfile->Close();
}

AnalysisOutput DoAnalysis(const int RunNo, const double ene, const bool Write,
                          const string path = "")
{
  const std::string ene_name = std::to_string(static_cast<int>(ene));
  const string filename = path + "/HGCALTBout_Run" + std::to_string(RunNo) + ".root";
  cout << "-->Analysis of " << filename << endl;
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
  TH1F H1CEE(H1CEEname, H1CEEname, static_cast<int>(ene) * 4, 0., ene * 2.);
  TH1F H1CHE(H1CHEname, H1CHEname, static_cast<int>(ene) * 4, 0., ene * 2.);
  TH1F H1TOT(H1Totname, H1Totname, static_cast<int>(ene) * 4, 0., ene * 2.);

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
  H1CHE.Fit("gaus", "Q");
  if (Write) {
    H1CEE.Write();
    H1CHE.Write();
    H1TOT.Write();
  }
  outputfile->Close();

  auto H1CEEAvg = H1CEE.GetFunction("gaus")->GetParameter(1);
  auto H1CEESigma = H1CEE.GetFunction("gaus")->GetParameter(2);
  auto H1CHEAvg = H1CHE.GetFunction("gaus")->GetParameter(1);
  auto H1CHESigma = H1CHE.GetFunction("gaus")->GetParameter(2);
  AnalysisOutput out{H1CEEAvg, H1CEESigma, H1CHEAvg, H1CHESigma};
  return out;
}
