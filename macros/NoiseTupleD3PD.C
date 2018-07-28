#include "NoiseTupleD3PD.h"


NoiseTupleD3PD::NoiseTupleD3PD(char * file) :
  fChain(0),
  infile("off_lay_eta_phi.txt") 
{

  TTree * tree=0;
  TFile * f = new TFile(file);
  f->GetObject("caloD3PD",tree);
  if(!tree){ std::cout<<"No tree found"<<std::endl; exit(0); }
  Init(tree);
}

NoiseTupleD3PD::~NoiseTupleD3PD()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t NoiseTupleD3PD::GetEntry(Long64_t entry)
{
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t NoiseTupleD3PD::LoadTree(Long64_t entry)
{

   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void NoiseTupleD3PD::Init(TTree *tree)
{

   // Set object pointer
   lardigit_layer = 0;
   lardigit_ieta = 0;
   lardigit_calo = 0;
   lardigit_iphi = 0;
   lardigit_barrel_ec = 0;
   lardigit_pos_neg = 0;
   lardigit_FT = 0;
   lardigit_slot = 0;
   lardigit_channel = 0;
   lardigit_gain = 0;
   lardigit_Samples = 0;
   lardigit_offlineId = 0;
   lardigit_sca_add = 0;
   larrawchannel_offlineID = 0;
   larrawchannel_onlineID = 0;
   larrawchannel_energy = 0;
   larrawchannel_time = 0;
   larrawchannel_quality = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("RunNumber", &RunNumber, &b_RunNumber);
   fChain->SetBranchAddress("EventNumber", &EventNumber, &b_EventNumber);
   fChain->SetBranchAddress("timestamp", &timestamp, &b_timestamp);
   fChain->SetBranchAddress("timestamp_ns", &timestamp_ns, &b_timestamp_ns);
   fChain->SetBranchAddress("lbn", &lbn, &b_lbn);
   fChain->SetBranchAddress("bcid", &bcid, &b_bcid);
   fChain->SetBranchAddress("detmask0", &detmask0, &b_detmask0);
   fChain->SetBranchAddress("detmask1", &detmask1, &b_detmask1);
   fChain->SetBranchAddress("actualIntPerXing", &actualIntPerXing, &b_actualIntPerXing);
   fChain->SetBranchAddress("averageIntPerXing", &averageIntPerXing, &b_averageIntPerXing);
   fChain->SetBranchAddress("pixelFlags", &pixelFlags, &b_pixelFlags);
   fChain->SetBranchAddress("sctFlags", &sctFlags, &b_sctFlags);
   fChain->SetBranchAddress("trtFlags", &trtFlags, &b_trtFlags);
   fChain->SetBranchAddress("larFlags", &larFlags, &b_larFlags);
   fChain->SetBranchAddress("tileFlags", &tileFlags, &b_tileFlags);
   fChain->SetBranchAddress("muonFlags", &muonFlags, &b_muonFlags);
   fChain->SetBranchAddress("fwdFlags", &fwdFlags, &b_fwdFlags);
   fChain->SetBranchAddress("coreFlags", &coreFlags, &b_coreFlags);
   fChain->SetBranchAddress("backgroundFlags", &backgroundFlags, &b_backgroundFlags);
   fChain->SetBranchAddress("lumiFlags", &lumiFlags, &b_lumiFlags);
   fChain->SetBranchAddress("pixelError", &pixelError, &b_pixelError);
   fChain->SetBranchAddress("sctError", &sctError, &b_sctError);
   fChain->SetBranchAddress("trtError", &trtError, &b_trtError);
   fChain->SetBranchAddress("larError", &larError, &b_larError);
   fChain->SetBranchAddress("tileError", &tileError, &b_tileError);
   fChain->SetBranchAddress("muonError", &muonError, &b_muonError);
   fChain->SetBranchAddress("fwdError", &fwdError, &b_fwdError);
   fChain->SetBranchAddress("coreError", &coreError, &b_coreError);
   fChain->SetBranchAddress("streamDecision_Egamma", &streamDecision_Egamma, &b_streamDecision_Egamma);
   fChain->SetBranchAddress("streamDecision_Muons", &streamDecision_Muons, &b_streamDecision_Muons);
   fChain->SetBranchAddress("streamDecision_JetTauEtmiss", &streamDecision_JetTauEtmiss, &b_streamDecision_JetTauEtmiss);
   fChain->SetBranchAddress("l1id", &l1id, &b_l1id);
   fChain->SetBranchAddress("isSimulation", &isSimulation, &b_isSimulation);
   fChain->SetBranchAddress("isCalibration", &isCalibration, &b_isCalibration);
   fChain->SetBranchAddress("isTestBeam", &isTestBeam, &b_isTestBeam);
   fChain->SetBranchAddress("lardigit_n", &lardigit_n, &b_lardigit_n);
   fChain->SetBranchAddress("lardigit_layer", &lardigit_layer, &b_lardigit_layer);
   fChain->SetBranchAddress("lardigit_ieta", &lardigit_ieta, &b_lardigit_ieta);
   fChain->SetBranchAddress("lardigit_calo", &lardigit_calo, &b_lardigit_calo);
   fChain->SetBranchAddress("lardigit_iphi", &lardigit_iphi, &b_lardigit_iphi);
   fChain->SetBranchAddress("lardigit_barrel_ec", &lardigit_barrel_ec, &b_lardigit_barrel_ec);
   fChain->SetBranchAddress("lardigit_pos_neg", &lardigit_pos_neg, &b_lardigit_pos_neg);
   fChain->SetBranchAddress("lardigit_FT", &lardigit_FT, &b_lardigit_FT);
   fChain->SetBranchAddress("lardigit_slot", &lardigit_slot, &b_lardigit_slot);
   fChain->SetBranchAddress("lardigit_channel", &lardigit_channel, &b_lardigit_channel);
   fChain->SetBranchAddress("lardigit_gain", &lardigit_gain, &b_lardigit_gain);
   fChain->SetBranchAddress("lardigit_Samples", &lardigit_Samples, &b_lardigit_Samples);
   fChain->SetBranchAddress("lardigit_offlineId", &lardigit_offlineId, &b_lardigit_offlineId);
   fChain->SetBranchAddress("lardigit_sca_add", &lardigit_sca_add, &b_lardigit_sca_add);
   fChain->SetBranchAddress("larrawchannel_n", &larrawchannel_n, &b_larrawchannel_n);
   fChain->SetBranchAddress("larrawchannel_offlineID", &larrawchannel_offlineID, &b_larrawchannel_offlineID);
   fChain->SetBranchAddress("larrawchannel_onlineID", &larrawchannel_onlineID, &b_larrawchannel_onlineID);
   fChain->SetBranchAddress("larrawchannel_energy", &larrawchannel_energy, &b_larrawchannel_energy);
   fChain->SetBranchAddress("larrawchannel_time", &larrawchannel_time, &b_larrawchannel_time);
   fChain->SetBranchAddress("larrawchannel_quality", &larrawchannel_quality, &b_larrawchannel_quality);
   Notify();
}

Bool_t NoiseTupleD3PD::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void NoiseTupleD3PD::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t NoiseTupleD3PD::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}


void NoiseTupleD3PD::getTranslatorLayerEtaPhi(int id,int &layer, float &eta, float &phi){
  
  std::string line;
  int ID;
  while (std::getline(infile, line)){
    std::istringstream iss(line);
    iss >> ID;
    iss >> layer;
    iss >> eta;
    iss >> phi;
    cout<<ID<<" "<<layer<<" "<<eta<<" "<<phi<<endl;

    if( ID == id){
      break;
    }
  }

  infile.clear();
  infile.seekg(0, ios::beg);

}



TTree *  NoiseTupleD3PD::makeTree(TString name){

  TTree* FlatTree =new TTree(name.Data(),name.Data());

   FlatTree->Branch("run",&RunNumber,"run/I");
   FlatTree->Branch("event",&EventNumber,"event/I");

   FlatTree->Branch("det",&lardigit_barrel_ec_,"det/I");
   FlatTree->Branch("side",&lardigit_pos_neg_,"side/I");
   FlatTree->Branch("FT",&lardigit_FT_,"FT/I");
   FlatTree->Branch("slot",&lardigit_slot_,"slot/I");
   FlatTree->Branch("channel",&lardigit_channel_,"channel/I");
   FlatTree->Branch("layer",&lardigit_layer_,"layer/I");
   FlatTree->Branch("sample0",&lardigit_Samples0_,"sample0/I");
   FlatTree->Branch("pedestal",&lardigit_pedestal_,"sample0/F");

   return FlatTree;
}

void NoiseTupleD3PD::Loop(TString outpath, int NMAX, bool writeCalib)
{

   if (fChain == 0) return;

   TFile OUTFILE(outpath+"/tuple.root","recreate");
   
   TTree * Tree[2][2][32];
   for(int b=0;b<2;b++)
     for(int s=0;s<2;s++)
       for(int F=0;F<32;F++){
	 Tree[b][s][F]=makeTree(TString("tuple_")+(long)b+"_"+(long)s+"_"+(long)F);
	 Tree[b][s][F]->SetDirectory(&OUTFILE);
       }

   ///Fill Calibration on first pass
   float SampleMean[2][2][32][15][128];
   int SampleCounter[2][2][32][15][128];
   for(int b=0;b<2;b++)
     for(int s=0;s<2;s++)
       for(int F=0;F<32;F++)
	 for(int f=0;f<15;f++)//15 in EC
	   for(int c=0;c<128;c++){
	     SampleMean[b][s][F][f][c]=0.;
	     SampleCounter[b][s][F][f][c]=0;
	   }


   //Load Calibration on second pass
   float SampleMeanCalib[2][2][32][15][128];
   ifstream calibfile;
   calibfile.open(calibpath_+"/LArNoise_Calib.txt");
   if(calibfile.is_open()){
     cout<<"Reading calibration from path: "<<calibpath_<<endl; 
    for(int b=0;b<2;b++)
     for(int s=0;s<2;s++)
       for(int F=0;F<32;F++)
	 for(int f=0;f<15;f++)//15 in EC
	   for(int c=0;c<128;c++){
	     int B,S,FT,FEB,C;
	     calibfile>>B
		      >>S
		      >>FT
		      >>FEB
		      >>C;
	     if(B==b&&S==s&&FT==F&&FEB==f&&C==c)
	       calibfile>>SampleMeanCalib[b][s][F][f][c];
	     else SampleMeanCalib[b][s][F][f][c] = 980;
		   
	     //cout<<B<<" "<<S<<" "<<FT<<" "<<FEB<<" "<<C<<" "<<SampleMeanCalib[b][s][F][f][c]<<endl;
	   }
   calibfile.close();
   }



   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries && jentry<NMAX ; jentry++) {

     if(jentry%100==0) cout<<jentry<<"/"<<nentries<<endl;

     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     
     //std::cout<<EventNumber<<" "<<timestamp<<" "<<lardigit_n<<std::endl;
     
     for(int ch = 0; ch < lardigit_n; ch++){

       lardigit_layer_ = (*lardigit_layer)[ch];
       lardigit_barrel_ec_ = (*lardigit_barrel_ec)[ch];
       lardigit_pos_neg_ = (*lardigit_pos_neg)[ch];
       lardigit_FT_ = (*lardigit_FT)[ch];
       lardigit_slot_ = (*lardigit_slot)[ch];
       lardigit_channel_ = (*lardigit_channel)[ch];
       lardigit_Samples0_ = ((*lardigit_Samples)[ch])[0];
       lardigit_pedestal_ = SampleMeanCalib[lardigit_barrel_ec_][lardigit_pos_neg_][lardigit_FT_][lardigit_slot_-1][lardigit_channel_];

       if(lardigit_Samples0_<2000){//remover overflow
	 SampleMean[lardigit_barrel_ec_][lardigit_pos_neg_][lardigit_FT_][lardigit_slot_-1][lardigit_channel_]+=lardigit_Samples0_;
	 SampleCounter[lardigit_barrel_ec_][lardigit_pos_neg_][lardigit_FT_][lardigit_slot_-1][lardigit_channel_]++;
       }
       
       Tree[lardigit_barrel_ec_][lardigit_pos_neg_][lardigit_FT_]->Fill();
     
     }

   }

   for(int b=0;b<2;b++)
     for(int s=0;s<2;s++)
       for(int F=0;F<32;F++){
	 Tree[b][s][F]->Write();
       }
   
   OUTFILE.ls();
   OUTFILE.Close();


   if(writeCalib){
     ofstream myfile;
     system(TString("rm -f ")+outpath+"/LArNoise_Calib.txt");
     myfile.open(calibpath_+"/LArNoise_Calib.txt");
     for(int b=0;b<2;b++)
       for(int s=0;s<2;s++)
	 for(int F=0;F<32;F++)
	   for(int f=0;f<15;f++)//15 in EC
	     for(int c=0;c<128;c++){
	       float average = SampleCounter[b][s][F][f][c] > 0 ? SampleMean[b][s][F][f][c]/SampleCounter[b][s][F][f][c] : 980;
	     
	       myfile<<b<<" "
		     <<s<<" "
		     <<F<<" "
		     <<f<<" "
		     <<c<<" "
		     <<average
		     <<endl;
	     }
   
     myfile.close();
   }
}
