
#ifndef NoiseTupleD3PD_h
#define NoiseTupleD3PD_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

using namespace std;


class NoiseTupleD3PD {

public :

  NoiseTupleD3PD(){};
  NoiseTupleD3PD(char * file);
  virtual ~NoiseTupleD3PD();

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   UInt_t          RunNumber;
   ULong64_t       EventNumber;
   UInt_t          timestamp;
   UInt_t          timestamp_ns;
   UInt_t          lbn;
   UInt_t          bcid;
   UInt_t          detmask0;
   UInt_t          detmask1;
   Float_t         actualIntPerXing;
   Float_t         averageIntPerXing;
   UInt_t          pixelFlags;
   UInt_t          sctFlags;
   UInt_t          trtFlags;
   UInt_t          larFlags;
   UInt_t          tileFlags;
   UInt_t          muonFlags;
   UInt_t          fwdFlags;
   UInt_t          coreFlags;
   UInt_t          backgroundFlags;
   UInt_t          lumiFlags;
   UInt_t          pixelError;
   UInt_t          sctError;
   UInt_t          trtError;
   UInt_t          larError;
   UInt_t          tileError;
   UInt_t          muonError;
   UInt_t          fwdError;
   UInt_t          coreError;
   Bool_t          streamDecision_Egamma;
   Bool_t          streamDecision_Muons;
   Bool_t          streamDecision_JetTauEtmiss;
   UInt_t          l1id;
   Bool_t          isSimulation;
   Bool_t          isCalibration;
   Bool_t          isTestBeam;
   Int_t           lardigit_n;
   vector<int>     *lardigit_layer;
   vector<int>     *lardigit_ieta;
   vector<int>     *lardigit_calo;
   vector<int>     *lardigit_iphi;
   vector<int>     *lardigit_barrel_ec;
   vector<int>     *lardigit_pos_neg;
   vector<int>     *lardigit_FT;
   vector<int>     *lardigit_slot;
   vector<int>     *lardigit_channel;
   vector<int>     *lardigit_gain;
   vector<vector<int> > *lardigit_Samples;
   vector<unsigned int> *lardigit_offlineId;
   vector<vector<unsigned int> > *lardigit_sca_add;
   
  int    lardigit_layer_;
  int    lardigit_ieta_;
  int    lardigit_calo_;
  int    lardigit_iphi_;
  int    lardigit_barrel_ec_;
  int    lardigit_pos_neg_;
  int    lardigit_FT_;
  int    lardigit_slot_;
  int    lardigit_channel_;
  int    lardigit_gain_;
  int    lardigit_Samples0_;
  float  lardigit_pedestal_;

   Int_t           larrawchannel_n;
   vector<unsigned int> *larrawchannel_offlineID;
   vector<unsigned int> *larrawchannel_onlineID;
   vector<float>   *larrawchannel_energy;
   vector<float>   *larrawchannel_time;
   vector<float>   *larrawchannel_quality;

   // List of branches
   TBranch        *b_RunNumber;   //!
   TBranch        *b_EventNumber;   //!
   TBranch        *b_timestamp;   //!
   TBranch        *b_timestamp_ns;   //!
   TBranch        *b_lbn;   //!
   TBranch        *b_bcid;   //!
   TBranch        *b_detmask0;   //!
   TBranch        *b_detmask1;   //!
   TBranch        *b_actualIntPerXing;   //!
   TBranch        *b_averageIntPerXing;   //!
   TBranch        *b_pixelFlags;   //!
   TBranch        *b_sctFlags;   //!
   TBranch        *b_trtFlags;   //!
   TBranch        *b_larFlags;   //!
   TBranch        *b_tileFlags;   //!
   TBranch        *b_muonFlags;   //!
   TBranch        *b_fwdFlags;   //!
   TBranch        *b_coreFlags;   //!
   TBranch        *b_backgroundFlags;   //!
   TBranch        *b_lumiFlags;   //!
   TBranch        *b_pixelError;   //!
   TBranch        *b_sctError;   //!
   TBranch        *b_trtError;   //!
   TBranch        *b_larError;   //!
   TBranch        *b_tileError;   //!
   TBranch        *b_muonError;   //!
   TBranch        *b_fwdError;   //!
   TBranch        *b_coreError;   //!
   TBranch        *b_streamDecision_Egamma;   //!
   TBranch        *b_streamDecision_Muons;   //!
   TBranch        *b_streamDecision_JetTauEtmiss;   //!
   TBranch        *b_l1id;   //!
   TBranch        *b_isSimulation;   //!
   TBranch        *b_isCalibration;   //!
   TBranch        *b_isTestBeam;   //!
   TBranch        *b_lardigit_n;   //!
   TBranch        *b_lardigit_layer;   //!
   TBranch        *b_lardigit_ieta;   //!
   TBranch        *b_lardigit_calo;   //!
   TBranch        *b_lardigit_iphi;   //!
   TBranch        *b_lardigit_barrel_ec;   //!
   TBranch        *b_lardigit_pos_neg;   //!
   TBranch        *b_lardigit_FT;   //!
   TBranch        *b_lardigit_slot;   //!
   TBranch        *b_lardigit_channel;   //!
   TBranch        *b_lardigit_gain;   //!
   TBranch        *b_lardigit_Samples;   //!
   TBranch        *b_lardigit_offlineId;   //!
   TBranch        *b_lardigit_sca_add;   //!
   TBranch        *b_larrawchannel_n;   //!
   TBranch        *b_larrawchannel_offlineID;   //!
   TBranch        *b_larrawchannel_onlineID;   //!
   TBranch        *b_larrawchannel_energy;   //!
   TBranch        *b_larrawchannel_time;   //!
   TBranch        *b_larrawchannel_quality;   //!


   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
  virtual void     Loop(TString outpath, int NMAX=100000000,bool writeCalib=0);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

  TTree * makeTree(TString name);

  TString calibpath_;
  void setCalibrationPath(TString path){calibpath_=path;}

  std::ifstream infile;
  void getTranslatorLayerEtaPhi(int id,int &layer, float &eta,float &phi);

};

#endif
