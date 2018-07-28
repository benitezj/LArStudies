
#include <iostream>
#include <vector>
#include <string.h>

/////detector numbers from here:
//https://atlasop.cern.ch/lar/doc/AtLarOper.html/Hardware_memento.html
#define NDET 5
#define NSIDE 2
#define NFT 32
#define NBFEB 15
#define NBChannel 128
#define NBLayer 4

//noise histogram binning
#define NHistBins 100
#define HistLow -50
#define HistHigh 50

///limits for histograms showing mean and rms noise distributions
#define MeanLow -2
#define MeanHigh 2
#define RMSLow 0
#define RMSHigh 12

///Threshold for coherent noise fraction
#define MinNEvent 500
#define FracHigh 0.2
#define MaxFrac 0.05

///Used for first Noise presentation:
//TString PLOTSPATH="/afs/cern.ch/user/b/benitezj/www/LArStudies/Noise";
//TString NTUPLEPATH="./NTUPLES/LArSamples/CosmicCalo_Coherent";


TString PLOTSPATH="/afs/cern.ch/user/b/benitezj/www/LArStudies/Noise2";
TString NTUPLEPATH="./NTUPLES/LArSamples/CosmicCalo_Coherent2";


std::vector<std::string> getListOfRuns(){

std::vector<std::string> runs;  
//runs.push_back("data15_comm.00263659.physics_CosmicCalo.merge.RAW");
 // runs.push_back("00263659");
 // runs.push_back("00263936");
 // runs.push_back("00263962");
 // runs.push_back("00263964");
 // runs.push_back("00263965");
 // runs.push_back("00264034");
 // runs.push_back("00265532");
 // runs.push_back("00265535");
 // runs.push_back("00265536");
 // runs.push_back("00265537");
 // runs.push_back("00265545");
 // runs.push_back("00265573");
 // runs.push_back("00266211");
 // runs.push_back("00266502");
 // runs.push_back("00266503");
 // runs.push_back("00266534");

// runs.push_back("00267236");
// runs.push_back("00267270");
// runs.push_back("00267332");
// runs.push_back("00267354");

/////New set of runs July4
 runs.push_back("00268731");
// runs.push_back("00268989");
// runs.push_back("00268991");
// runs.push_back("00268992");
// runs.push_back("00268994");
// runs.push_back("00269155");
// runs.push_back("00269310");
// runs.push_back("00269317");
// runs.push_back("00269373");
// runs.push_back("00269377");
// runs.push_back("00269417");
// runs.push_back("00269419");
// runs.push_back("00269421");
// runs.push_back("00269899");
// runs.push_back("00269932");
// runs.push_back("00270298");
// runs.push_back("00270394");
// runs.push_back("00270417");
// runs.push_back("00270419");
// runs.push_back("00270420");
// runs.push_back("00270440");


 for(int i=0;i<runs.size();i++)
   cout<<"Run: "<<runs.at(i)<<endl;

 return runs;
}



////////////////////Useful Functions
void drawHorizontalLine(TH1F* Hist,float value,int color=1){
  if(!Hist){cout<<"drawHorizontalLine Histo is NULL"<<endl;return;}
  TLine line;
  line.SetLineWidth(1);
  line.SetLineColor(color);
  line.DrawLine(Hist->GetXaxis()->GetXmin(),value,Hist->GetXaxis()->GetXmax(),value);
}

void drawVerticalLines(TH1F* H,float ymin,float ymax,int color=1){
  TLine line;
  line.SetLineWidth(1);
  line.SetLineColor(color);
  for(int b=2;b<=H->GetNbinsX();b++){
    line.DrawLine(H->GetBinLowEdge(b),ymin,H->GetBinLowEdge(b),ymax);
  }
}

void drawLabels(TH1F* H,int n,int firstlabel,float ymin){
  TLatex label;
  label.SetTextSize(0.03);
  label.SetTextColor(2);
  int counter=0;
  for(int b=1;b<H->GetNbinsX();b++){
    if((b-1)%n==0)
      label.DrawText(H->GetBinCenter(b),ymin,TString("")+(long)(firstlabel+counter++));
  }
}



void drawXLabels(TAxis* A, std::vector<std::string> labels, float size=0.03){
  if(!A){cout<<"TAxis is NULL"<<endl;  return; }

  if(labels.size() != A->GetNbins()){cout<<"Number of labels does not match"<<endl;  return; }

  TText t;
  t.SetTextAngle(30);
  t.SetTextSize(size);
  t.SetTextAlign(33);
  for (int i=0;i<A->GetNbins();i++) {
    t.DrawText(A->GetBinCenter(i+1),0.1,labels[i].c_str());
  }
  
}
