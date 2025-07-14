// Simple macro card to reconstruct longitdunal profiles of em-showers
// Evolves from emprofile.C

/// Statistics calculator using Welford’s algorithm.
/// Usage:
///   - Call `add(x)` for each new data point.
///   - Call `GetMean()` or `GetMeanError()` to retrieve statistics.
struct OnlineStats {
    int count = {0};
    double mean = {0.0};
    double M2 = {0.0};

    void add(double x) {
        count++;
        double delta = x - mean;
        mean += delta / count;
        double delta2 = x - mean;
        M2 += delta * delta2;
    }
    
    double GetMean() const { return mean; }

    double GetVariance() const { return (count > 1) ? M2 / (count - 1) : 0.0; }

    double Getstddev() const { return std::sqrt(GetVariance()); }
    
    double GetMeanError() const { return Getstddev()/std::sqrt(count); }
    
};


void emprofile2(const string inFileName = "HGCALTBout_Run0.root", const string Energy = "",
               const string PhysList = "")
{
  const string filename = inFileName;
  cout << "Analysis of " << filename << endl;
  TFile* file = TFile::Open(filename.c_str(), "READ");
  TTree* tree = (TTree*)file->Get("HGCALTBout");

  TFile* outputfile(
    TFile::Open(("EmProf" + Energy + "_" + PhysList + ".root").c_str(), "RECREATE"));

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
  
  std::array<OnlineStats, layersNo / 2> emprofile{};
  std::array<OnlineStats, layersNo> fullemprofile{};

  for (std::size_t evtNo = 0; evtNo < tree->GetEntries(); evtNo++) {
    tree->GetEntry(evtNo);

    for (std::size_t i = 0; i < layersNo; i = i + 2) {
      emprofile.at(i / 2).add( CEESignals->at(i) + CEESignals->at(i + 1));
    }
    for (std::size_t i = 0; i < layersNo; i++) {
      fullemprofile.at(i).add(CEESignals->at(i)); 
    }
  }

  TGraphErrors grEmProfile;
  for (std::size_t i = 0; i < emprofile.size(); i++) {
    grEmProfile.AddPoint( i, emprofile.at(i).GetMean());
    grEmProfile.SetPointError(i , 0.0 /*x-error*/,  emprofile.at(i).GetMeanError() );
  }
  grEmProfile.SetTitle("EmProfile");
  grEmProfile.SetName("EmProfile");

  TGraphErrors grFullEmProfile;
  for (std::size_t i = 0; i < fullemprofile.size(); i++) {
    grFullEmProfile.AddPoint( i , fullemprofile.at(i).GetMean() );
    grFullEmProfile.SetPointError( i , 0.0 /*x-error*/,  fullemprofile.at(i).GetMeanError() );
  }
  grFullEmProfile.SetTitle("FullEmProfile");
  grFullEmProfile.SetName("FullEmProfile");
  grFullEmProfile.GetXaxis()->SetTitle("Layer");
  grFullEmProfile.GetYaxis()->SetTitle("Measured energy [MIP]");
  
  outputfile->cd();
  grEmProfile.Write();
  grFullEmProfile.Write();
  outputfile->Close();
}
