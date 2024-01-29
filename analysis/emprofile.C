// Simple macro card to reconstruct longitdunal profiles of em-showers

void emprofile(const string inFileName = "HGCALTBout_Run0.root", const string Energy = "", const string PhysList = "")
{
  const string filename = inFileName;
  cout << "Analysis of " << filename << endl;
  TFile* file = TFile::Open(filename.c_str(), "READ");
  TTree* tree = (TTree*)file->Get("HGCALTBout");

  TFile* outputfile(TFile::Open(("EmProf"+Energy+"_"+PhysList+".root").c_str(), "RECREATE"));

  double CEETot{0.};
  tree->SetBranchAddress("CEETot", &CEETot);
  double CHETot{0.};
  tree->SetBranchAddress("CHETot", &CHETot);
  double AHCALTot{0.};
  tree->SetBranchAddress("AHCALTot", &AHCALTot);
  double HGCALTot{0.};
  tree->SetBranchAddress("HGCALTot", &HGCALTot);
  int IntLayer{0};
  tree->SetBranchAddress("IntLayer", &IntLayer);
  vector<double>* CEESignals = NULL;
  tree->SetBranchAddress("CEESignals", &CEESignals);
  vector<double>* CHESignals = NULL;
  tree->SetBranchAddress("CHESignals", &CHESignals);
  vector<double>* AHCALSignals = NULL;
  tree->SetBranchAddress("AHCALSignals", &AHCALSignals);

  const int layersNo = 28;
  std::array<double, layersNo/2> emprofile{0.};
  std::array<double, layersNo> fullemprofile{0.};

  for (std::size_t evtNo = 0; evtNo < tree->GetEntries(); evtNo++) {
    tree->GetEntry(evtNo);

    for(std::size_t i = 0; i < layersNo; i = i+2){
        emprofile.at(i/2) += CEESignals->at(i)+CEESignals->at(i+1);
    }
    for(std::size_t i = 0; i < layersNo; i++){
        fullemprofile.at(i) += CEESignals->at(i);
    }
  }
  
  for(const auto& a : emprofile) cout<<a<<endl;
  cout<<"-----------------------"<<endl;
  for(auto& a : emprofile) { a = a / tree->GetEntries(); }
  for(const auto& a : emprofile) cout<<a<<endl;

  for(auto& a : fullemprofile) { a = a / tree->GetEntries(); }

  std::array<double, layersNo/2> layers{};
  for (std::size_t i=0; i<layersNo/2; i++) { layers.at(i) = i; }
  std::array<double, layersNo> fulllayers{};
  for (std::size_t i=0; i<layersNo; i++) { fulllayers.at(i) = i; }
  
  TGraph grEmProfile(layersNo/2, layers.data(), emprofile.data());
  grEmProfile.SetTitle("EmProfile");
  grEmProfile.SetName("EmProfile");

  TGraph grFullEmProfile(layersNo, fulllayers.data(), fullemprofile.data());
  grFullEmProfile.SetTitle("FullEmProfile");
  grFullEmProfile.SetName("FullEmProfile");
  grFullEmProfile.GetXaxis()->SetTitle("Layer");
  grFullEmProfile.GetYaxis()->SetTitle("Measured energy [MIP]");
 
  outputfile->cd();
  grEmProfile.Write();
  grFullEmProfile.Write();
  outputfile->Close();

}
