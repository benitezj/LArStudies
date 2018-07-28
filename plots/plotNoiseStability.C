#include "globals.h"

void plotNoiseStability(){
  gROOT->ProcessLine(".x LArStudies/run/RootLogon.C");

  std::vector<std::string> runs=getListOfRuns();

  //TString Reference="00264034";
  TString Reference="00268731";

  ////////////////////////////////////////////////////////////////////
  ///////Read the reference run
  ////////////////////////////////////////////////////////////////////
  float RefRMS[NDET][NSIDE][NFT][NBFEB];
  float RefRMSCoh[NDET][NSIDE][NFT][NBFEB];
  float RefFrac[NDET][NSIDE][NFT][NBFEB];
  for(int det=0;det<NDET;det++)
    for(int side=0;side<NSIDE;side++){
      TString filename=PLOTSPATH+"/CosmicCalo_Coherent_"+Reference+"/NoiseType_Det"+(long)det+"_Side"+(long)side+"_NoiseFracFEB.txt";
      cout<<"Reference values from:"<<filename<<endl;
      ifstream INFILE(filename.Data());
      if(INFILE.is_open()) {
	for(int F=0;F<NFT;F++){
	  for(int slot = 0; slot<NBFEB; slot++){
	    int FT; int SLOT; float rms; float rmsCoh; float frac;
	    INFILE>>FT>>SLOT>>rms>>rmsCoh>>frac;
	    if(F==FT&&slot==SLOT){
	      RefRMS[det][side][F][slot]=rms;
	      RefRMSCoh[det][side][F][slot]=rmsCoh;
	      RefFrac[det][side][F][slot]=frac;
	      cout<<"Det="<<det<<" side="<<side<<" FT="<<F<<" or  slot="<<slot<<"  rms="<<rms<<endl;
	    }else{
	      cout<<"Det="<<det<<" side="<<side<<" FT="<<F<<" or  slot="<<slot<<"  does not match !!: "<<filename<<endl;
	    }
	  }
	}
      }
    }


  ////////////////////////////////////////////////////////////////////
  /////read all the runs
  /////////////////////////////////////////////////////////////////
  TGraphErrors RMSHistory[NDET][NSIDE][NFT][NBFEB];
  TGraphErrors RMSCohHistory[NDET][NSIDE][NFT][NBFEB];
  TGraphErrors FracHistory[NDET][NSIDE][NFT][NBFEB];
  TGraphErrors FracDiffHistory[NDET][NSIDE][NFT][NBFEB];
  for(int det=0;det<NDET;det++)
    for(int side=0;side<NSIDE;side++)
      for(int i=0;i<runs.size();i++){
	TString filename=PLOTSPATH+"/CosmicCalo_Coherent_"+runs.at(i).c_str()+"/NoiseType_Det"+(long)det+"_Side"+(long)side+"_NoiseFracFEB.txt";
	cout<<filename<<endl;
	ifstream INFILE(filename.Data());
	if (INFILE.is_open()) {
	  for(int F=0;F<NFT;F++){
	    for(int slot = 0; slot<NBFEB; slot++){
	      int FT; int SLOT; float rms; float rmsCoh; float frac;
	      INFILE>>FT>>SLOT>>rms>>rmsCoh>>frac;
	      
	      if(F==FT && slot==SLOT
		 && RefRMS[det][side][F][slot] > 0 
		 && RefRMSCoh[det][side][F][slot] > 0 
		 && RefFrac[det][side][F][slot] != 0. ){
		RMSHistory[det][side][F][slot].SetPoint(i,i+1,rms/RefRMS[det][side][F][slot]);
		RMSCohHistory[det][side][F][slot].SetPoint(i,i+1,rmsCoh/RefRMSCoh[det][side][F][slot]);
		FracHistory[det][side][F][slot].SetPoint(i,i+1,frac/RefFrac[det][side][F][slot]);
		FracDiffHistory[det][side][F][slot].SetPoint(i,i+1,frac - RefFrac[det][side][F][slot]);
	      }
	    }
	  }
	 
	  INFILE.close();
	}else cout<<"Unable to open file "<<filename<<endl;
       
      }



  ///////////////////////////////////////////////////////////////
  /////Make the plots
  /////////////////////////////////////////////////////////////
 
  float minRMS=0.99;
  float maxRMS=1.01;
  float minRMSCoh=0.90;
  float maxRMSCoh=1.10;
  float minFrac=0.0;
  float maxFrac=10.0;
  float maxFracDiff=0.1;
  
  // DrawHistories(PLOTSPATH+"/HistoryRMS",&RMSHistory,runs.size(),minRMS,maxRMS);
  // return;


  TCanvas Canvas("Canvas","",700,700);
  //Canvas.SetLeftMargin(0.0);
  Canvas.SetRightMargin(0.0);
  Canvas.SetTopMargin(0.0);
  //Canvas.SetBottomMargin(0.0);

  TH1F HFrame("HFrame","",runs.size(),0.5,0.5+runs.size());
  HFrame.SetNdivisions(0,"x");
  HFrame.SetNdivisions(3,"y");
  HFrame.GetXaxis()->SetTitle("");
  HFrame.GetXaxis()->SetLabelSize(.08);
  HFrame.GetYaxis()->SetTitleSize(0.18);
  HFrame.GetYaxis()->SetTitleOffset(0.15);
  HFrame.GetYaxis()->SetLabelSize(0.1);
 
  int NFTPLOT=8;
  int NEvtForStatUnc=5000;

  /////////////////////////////////////////////////////////
  //Plot the RMS (simple noise)
  ////////////////////////////////////////////////////////
  for(int det=0;det<NDET;det++)
    for(int side=0;side<NSIDE;side++){
      Canvas.Clear();
      Canvas.Divide(1,32/NFTPLOT);
      for(int F=0;F<NFT;F++){
	if(F%NFTPLOT==0){
	  TVirtualPad* pad=Canvas.cd(F/NFTPLOT  + 1);
	  //pad->SetLeftMargin(0.0);
	  pad->SetRightMargin(0.0);
	  pad->SetTopMargin(0.0);
	  pad->SetBottomMargin(0.0);
	  //HFrame.GetYaxis()->SetTitle("Simple RMS / Nominal");
	  HFrame.GetYaxis()->SetTitle(TString("FT")+(long)F+"-"+(long)(F+NFTPLOT-1)+"   ");
	  HFrame.GetYaxis()->SetRangeUser(minRMS,maxRMS);
	  HFrame.DrawClone("hist");
	  drawHorizontalLine(&HFrame,1);
	  drawVerticalLines(&HFrame,minRMS,maxRMS);
	  drawHorizontalLine(&HFrame,1+1.65/sqrt(2*NEvtForStatUnc*128),2);
	  drawHorizontalLine(&HFrame,1-1.65/sqrt(2*NEvtForStatUnc*128),2);
	}

	for(int slot = 0; slot<NBFEB; slot++)
	  RMSHistory[det][side][F][slot].Draw("lsame");
      }
      Canvas.Print(PLOTSPATH+"/HistoryRMS_Det"+(long)det+"_Side"+(long)side+".gif");
    }


  /////////////////////////////////////////////////////////
  //Plot the coherent RMS (coherent noise)
  ////////////////////////////////////////////////////////
  for(int det=0;det<NDET;det++)
    for(int side=0;side<NSIDE;side++){
      Canvas.Clear();
      Canvas.Divide(1,32/NFTPLOT);
      for(int F=0;F<NFT;F++){
	if(F%NFTPLOT==0){
	  TVirtualPad* pad=Canvas.cd(F/NFTPLOT  + 1);
	  //pad->SetLeftMargin(0.0);
	  pad->SetRightMargin(0.0);
	  pad->SetTopMargin(0.0);
	  pad->SetBottomMargin(0.0);
	  //HFrame.GetYaxis()->SetTitle("Coherent RMS / Nominal");
	  HFrame.GetYaxis()->SetTitle(TString("FT")+(long)F+"-"+(long)(F+NFTPLOT-1)+"   ");
	  HFrame.GetYaxis()->SetRangeUser(minRMSCoh,maxRMSCoh);
	  HFrame.DrawClone("hist");
	  drawHorizontalLine(&HFrame,1);
	  drawVerticalLines(&HFrame,minRMSCoh,maxRMSCoh);
	  drawHorizontalLine(&HFrame,1+1.65/sqrt(2*NEvtForStatUnc),2);
	  drawHorizontalLine(&HFrame,1-1.65/sqrt(2*NEvtForStatUnc),2);
	}

	for(int slot = 0; slot<NBFEB; slot++)
	  RMSCohHistory[det][side][F][slot].Draw("lsame");
      }
      Canvas.Print(PLOTSPATH+"/HistoryRMSCoh_Det"+(long)det+"_Side"+(long)side+".gif");
    }


  ////////////////////////////////////////////////////////
  //Plot the coherent noise fraction
  ////////////////////////////////////////////////////////
  for(int det=0;det<NDET;det++)
    for(int side=0;side<NSIDE;side++){
      Canvas.Clear();
      Canvas.Divide(1,32/NFTPLOT);
      for(int F=0;F<NFT;F++){
	if(F%NFTPLOT==0){
	  TVirtualPad* pad=Canvas.cd(F/NFTPLOT  + 1);
	  //pad->SetLeftMargin(0.0);
	  pad->SetRightMargin(0.0);
	  pad->SetTopMargin(0.0);
	  pad->SetBottomMargin(0.0);
	  //HFrame.GetYaxis()->SetTitle("Coherent Noise Fraction / Nominal");
	  HFrame.GetYaxis()->SetTitle(TString("FT")+(long)F+"-"+(long)(F+NFTPLOT-1)+"   ");
	  HFrame.GetYaxis()->SetRangeUser(minFrac,maxFrac);
	  HFrame.DrawClone("hist");
	  drawHorizontalLine(&HFrame,1);
	  drawVerticalLines(&HFrame,minFrac,maxFrac);
	}
	for(int slot = 0; slot<NBFEB; slot++)
	  FracHistory[det][side][F][slot].Draw("lsame");
      }
      Canvas.Print(PLOTSPATH+"/HistoryFrac_Det"+(long)det+"_Side"+(long)side+".gif");
    }


  ////////////////////////////////////////////////////////
  //Plot the coherent noise fraction
  ////////////////////////////////////////////////////////
  for(int det=0;det<NDET;det++)
    for(int side=0;side<NSIDE;side++){
      Canvas.Clear();
      Canvas.Divide(1,32/NFTPLOT);
      for(int F=0;F<NFT;F++){
	if(F%NFTPLOT==0){
	  TVirtualPad* pad=Canvas.cd(F/NFTPLOT  + 1);
	  pad->SetRightMargin(0.0);
	  pad->SetTopMargin(0.0);
	  pad->SetBottomMargin(0.0);
	  HFrame.GetYaxis()->SetTitle(TString("FT")+(long)F+"-"+(long)(F+NFTPLOT-1)+"   ");
	  HFrame.GetYaxis()->SetRangeUser(-maxFracDiff,maxFracDiff);
	  HFrame.DrawClone("hist");
	  drawHorizontalLine(&HFrame,1);
	  drawVerticalLines(&HFrame,-maxFracDiff,maxFracDiff);
	  drawHorizontalLine(&HFrame,0.03,2);//~80CL on runs with 5000 events
	  drawHorizontalLine(&HFrame,-0.03,2);
	}
	for(int slot = 0; slot<NBFEB; slot++)
	  FracDiffHistory[det][side][F][slot].Draw("lsame");
      }
      Canvas.Print(PLOTSPATH+"/HistoryFracDiff_Det"+(long)det+"_Side"+(long)side+".gif");
    }


  gROOT->ProcessLine(".q");
  
}




void plotNoiseStabilityFT(){
  ///This function uses the RMS values averaged over all FEBs in FT
  gROOT->ProcessLine(".x LArCalorimeter/LArStudies/run/RootLogon.C");

  std::vector<std::string> runs=getListOfRuns();

 //TString Reference="00264034";
  TString Reference="00268731";


  ////////////////////////////////////////////////////////////////////
  ///////Read the reference run
  ////////////////////////////////////////////////////////////////////
  float RefRMS[NDET][NSIDE][NFT];
  float RefRMSCoh[NDET][NSIDE][NFT];
  float RefFrac[NDET][NSIDE][NFT];
  for(int det=0;det<NDET;det++)
    for(int side=0;side<NSIDE;side++){
      TString filename=PLOTSPATH+"/CosmicCalo_Coherent_"+Reference+"/NoiseType_Det"+(long)det+"_Side"+(long)side+"_NoiseFrac.txt";
      cout<<"Reference values from:"<<filename<<endl;
      ifstream INFILE(filename.Data());
      if(INFILE.is_open()) {
	for(int F=0;F<NFT;F++){
	  int FT; float rms; float rmsCoh; float frac;
	  INFILE>>FT>>rms>>rmsCoh>>frac;
	  if(F==FT){
	    RefRMS[det][side][F]=rms;
	    RefRMSCoh[det][side][F]=rmsCoh;
	    RefFrac[det][side][F]=frac;
	  }else{
	    cout<<"FT="<<F<<" number does not match !!: "<<filename<<endl;
	  }
	}
      }
    }


  ////////////////////////////////////////////////////////////////////
  /////read all the runs
  /////////////////////////////////////////////////////////////////
  TGraphErrors RMSHistory[NDET][NSIDE][NFT];
  TGraphErrors RMSCohHistory[NDET][NSIDE][NFT];
  TGraphErrors FracHistory[NDET][NSIDE][NFT];
  for(int det=0;det<NDET;det++)
    for(int side=0;side<NSIDE;side++)
      for(int i=0;i<runs.size();i++){
	TString filename=PLOTSPATH+"/CosmicCalo_Coherent_"+runs.at(i).c_str()+"/NoiseType_Det"+(long)det+"_Side"+(long)side+"_NoiseFrac.txt";
	cout<<filename<<endl;
	ifstream INFILE(filename.Data());
	if (INFILE.is_open()) {
	  for(int F=0;F<NFT;F++){
	    int FT; float rms; float rmsCoh; float frac;
	    INFILE>>FT>>rms>>rmsCoh>>frac;

	    if(F==FT && RefRMS[det][side][F] > 0 && RefRMSCoh[det][side][F] > 0 && RefFrac[det][side][F] > 0 ){
	      RMSHistory[det][side][F].SetPoint(i,i+1,rms/RefRMS[det][side][F]);
	      RMSCohHistory[det][side][F].SetPoint(i,i+1,rmsCoh/RefRMSCoh[det][side][F]);
	      FracHistory[det][side][F].SetPoint(i,i+1,frac/RefFrac[det][side][F]);
	    }
	  }
	 
	  INFILE.close();
	}else cout<<"Unable to open file "<<filename<<endl;
       
      }



  ///////////////////////////////////////////////////////////////
  /////Make the plots
  /////////////////////////////////////////////////////////////
  TCanvas Canvas("Canvas","",700,700);
  //Canvas.SetLeftMargin(0.0);
  Canvas.SetRightMargin(0.0);
  Canvas.SetTopMargin(0.0);
  //Canvas.SetBottomMargin(0.0);

  TH1F HFrame("HFrame","",runs.size(),0.5,0.5+runs.size());
  HFrame.SetNdivisions(0,"x");
  HFrame.SetNdivisions(3,"y");
  HFrame.GetXaxis()->SetTitle("");
  HFrame.GetXaxis()->SetLabelSize(.08);
  HFrame.GetYaxis()->SetTitleSize(0.15);
  HFrame.GetYaxis()->SetTitleOffset(0.15);
  HFrame.GetYaxis()->SetLabelSize(0.1);
 
  float minRMS=0.995;
  float maxRMS=1.005;
  float minRMSCoh=0.90;
  float maxRMSCoh=1.10;
  float minFrac=0.0;
  float maxFrac=2.0;

  int NFTPLOT=8;

  /////////////////////////////////////////////////////////
  //Plot the RMS (simple noise)
  ////////////////////////////////////////////////////////
  for(int det=0;det<NDET;det++)
    for(int side=0;side<NSIDE;side++){
      Canvas.Clear();
      Canvas.Divide(1,4);
      for(int F=0;F<NFT;F++){
	if(F%NFTPLOT==0){
	  TVirtualPad* pad=Canvas.cd(F/NFTPLOT  + 1);
	  //pad->SetLeftMargin(0.0);
	  pad->SetRightMargin(0.0);
	  pad->SetTopMargin(0.0);
	  pad->SetBottomMargin(0.0);
	  //HFrame.GetYaxis()->SetTitle("Simple RMS / Nominal");
	  HFrame.GetYaxis()->SetTitle(TString("FT")+(long)F+" - FT"+(long)(F+NFTPLOT-1));
	  HFrame.GetYaxis()->SetRangeUser(minRMS,maxRMS);
	  HFrame.DrawClone("hist");
	  drawHorizontalLine(&HFrame,1);
	  drawVerticalLines(&HFrame,minRMS,maxRMS);
	}
	RMSHistory[det][side][F].Draw("lsame");
      }
      Canvas.Print(PLOTSPATH+"/HistoryRMS_Det"+(long)det+"_Side"+(long)side+".gif");
    }


  /////////////////////////////////////////////////////////
  //Plot the coherent RMS (coherent noise)
  ////////////////////////////////////////////////////////
  for(int det=0;det<NDET;det++)
    for(int side=0;side<NSIDE;side++){
      Canvas.Clear();
      Canvas.Divide(1,4);
      for(int F=0;F<NFT;F++){
	if(F%NFTPLOT==0){
	  TVirtualPad* pad=Canvas.cd(F/NFTPLOT  + 1);
	  //pad->SetLeftMargin(0.0);
	  pad->SetRightMargin(0.0);
	  pad->SetTopMargin(0.0);
	  pad->SetBottomMargin(0.0);
	  //HFrame.GetYaxis()->SetTitle("Coherent RMS / Nominal");
	  HFrame.GetYaxis()->SetTitle(TString("FT")+(long)F+" - FT"+(long)(F+NFTPLOT-1));
	  HFrame.GetYaxis()->SetRangeUser(minRMSCoh,maxRMSCoh);
	  HFrame.DrawClone("hist");
	  drawHorizontalLine(&HFrame,1);
	  drawVerticalLines(&HFrame,minRMSCoh,maxRMSCoh);
	}
	RMSCohHistory[det][side][F].Draw("lsame");
      }
      Canvas.Print(PLOTSPATH+"/HistoryRMSCoh_Det"+(long)det+"_Side"+(long)side+".gif");
    }


  ////////////////////////////////////////////////////////
  //Plot the coherent noise fraction (RMSCoh - RMS) / RMS
  ////////////////////////////////////////////////////////
  for(int det=0;det<NDET;det++)
    for(int side=0;side<NSIDE;side++){
      Canvas.Clear();
      Canvas.Divide(1,4);
      for(int F=0;F<NFT;F++){
	if(F%NFTPLOT==0){
	  TVirtualPad* pad=Canvas.cd(F/NFTPLOT  + 1);
	  //pad->SetLeftMargin(0.0);
	  pad->SetRightMargin(0.0);
	  pad->SetTopMargin(0.0);
	  pad->SetBottomMargin(0.0);
	  //HFrame.GetYaxis()->SetTitle("Coherent Noise Fraction / Nominal");
	  HFrame.GetYaxis()->SetTitle(TString("FT")+(long)F+" - FT"+(long)(F+NFTPLOT-1));
	  HFrame.GetYaxis()->SetRangeUser(minFrac,maxFrac);
	  HFrame.DrawClone("hist");
	  drawHorizontalLine(&HFrame,1);
	  drawVerticalLines(&HFrame,minFrac,maxFrac);
	}
	FracHistory[det][side][F].Draw("lsame");
      }
      Canvas.Print(PLOTSPATH+"/HistoryFrac_Det"+(long)det+"_Side"+(long)side+".gif");
    }


  gROOT->ProcessLine(".q");
  
}











// void DrawHistories(TString outname, TGraphErrors * History[NDET][NSIDE][NFT][NBFEB], int Nruns, float miny, float maxy){
//   TCanvas Canvas("Canvas","",700,700);
//   //Canvas.SetLeftMargin(0.0);
//   Canvas.SetRightMargin(0.0);
//   Canvas.SetTopMargin(0.0);
//   //Canvas.SetBottomMargin(0.0);

//   TH1F HFrame("HFrame","",Nruns,0.5,0.5+Nruns);
//   HFrame.SetNdivisions(0,"x");
//   HFrame.SetNdivisions(3,"y");
//   HFrame.GetXaxis()->SetTitle("");
//   HFrame.GetXaxis()->SetLabelSize(.08);
//   HFrame.GetYaxis()->SetTitleSize(0.18);
//   HFrame.GetYaxis()->SetTitleOffset(0.15);
//   HFrame.GetYaxis()->SetLabelSize(0.1);

//   int NFTPLOT=8;

//   for(int det=0;det<NDET;det++)
//     for(int side=0;side<NSIDE;side++){
//       Canvas.Clear();
//       Canvas.Divide(1,NFT/NFTPLOT);
//       for(int F=0;F<NFT;F++){
// 	if(F%NFTPLOT==0){
// 	  TVirtualPad* pad=Canvas.cd(F/NFTPLOT  + 1);
// 	  //pad->SetLeftMargin(0.0);
// 	  pad->SetRightMargin(0.0);
// 	  pad->SetTopMargin(0.0);
// 	  pad->SetBottomMargin(0.0);
// 	  //HFrame.GetYaxis()->SetTitle("Simple RMS / Nominal");
// 	  HFrame.GetYaxis()->SetTitle(TString("FT")+(long)F+"-"+(long)(F+NFTPLOT-1)+"   ");
// 	  HFrame.GetYaxis()->SetRangeUser(miny,maxy);
// 	  HFrame.DrawClone("hist");
// 	  drawHorizontalLine(&HFrame,1);
// 	  drawVerticalLines(&HFrame,miny,maxy);
// 	}

// 	for(int slot = 0; slot<NBFEB; slot++)
// 	  History[det][side][F][slot]->Draw("lsame");
//       }
//       Canvas.Print(outname+"_Det"+(long)det+"_Side"+(long)side+".gif");
//     }


// }


