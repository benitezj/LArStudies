{
  gSystem->Load("libCintex");
  ROOT::Cintex::Cintex::Enable();
  gSystem->Load("libTree");
  gSystem->Load("libPhysics");
  gSystem->Load("libMathMore");
  gSystem->Load("libMinuit");
  gSystem->Load("libLArCafJobsDict");
  gSystem->Load("libLArStudiesDict.so");

  gStyle->SetCanvasColor(0);
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetPadLeftMargin (0.15);
  gStyle->SetPadRightMargin (0.05);
  gStyle->SetTitleOffset(1.600,"Y");
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetPalette(1);
  gStyle->SetHistMinimumZero();
  gStyle->SetErrorX(0);
  gStyle->SetMarkerStyle(7);
  gStyle->SetMarkerColor(kBlack);
  //gStyle->SetMarkerSize(1);
 
  using namespace LArSamples;
  using namespace LArSamples::Definitions;
}
