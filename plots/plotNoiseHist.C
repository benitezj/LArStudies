#include "globals.h"

void plotCohNoiseFromHisto2(TFile * INFILE, TString outpath, unsigned int Det, unsigned int Side, unsigned int firstFT=0, unsigned int lastFT=31, TH1F*HUNC=0){

  gStyle->SetTitleSize  (0.04,"Y");
  gStyle->SetTitleSize  (0.06,"X");
  gStyle->SetTitleOffset(0.60,"Y");
  gStyle->SetTitleOffset(0.80,"X");
  gStyle->SetLabelSize  (0.06,"Y");
  gStyle->SetLabelSize  (0.06,"X");

  
  TString outfilename=TString("NoiseType")+"_Det"+(long)Det+"_Side"+(long)Side;

  int NFT=lastFT-firstFT+1;
  TH1F HRMSAVG("HRMSAVG","",NFT*NBFEB,0,NFT*NBFEB);
  TH1F HRMSCoh("HRMSCoh","",NFT*NBFEB,0,NFT*NBFEB);
  TH1F HFrac("HFrac","",NFT*NBFEB,0,NFT*NBFEB);
  TH1F HNChan("HNChan","",NFT*NBFEB,0,NFT*NBFEB);
   
  TH1F HFT("HFT","",NFT,0,NFT*NBFEB);

  
 
  ///Text file containing the coherent noise fractions per FEB 
  std::filebuf fbNoiseFracFEB;
  fbNoiseFracFEB.open (outpath+"/"+outfilename+"_NoiseFracFEB.txt",std::ios::out);
  std::ostream osNoiseFracFEB(&fbNoiseFracFEB);

  ///Text file containing the FEBS with large noise fraction 
  std::filebuf fbNoisyFEB;
  fbNoisyFEB.open (outpath+"/"+outfilename+"_NoisyFEB.txt",std::ios::out);
  std::ostream osNoisyFEB(&fbNoisyFEB);

  for(int FT = firstFT; FT<=lastFT; FT++){

    for(int slot = 0; slot<NBFEB; slot++){//in NoiseTupleCoherent.cxx FEB starts from 0
      TString histID=TString("")+"_Det"+(long)(Det)+"_Side"+(long)(Side)+"_FT"+(long)(FT)+"_Slot"+(long)(slot);
      //cout<<histID<<endl;

      TString HistoName=TString("HistoRMSChannel")+histID;
      TH1F* Histo=(TH1F*)(INFILE->Get(HistoName));
      if(!Histo){cout<<HistoName<<"  not found"<<endl; continue;}

      TString HistoNameCoh=TString("HistoRMSCoh")+histID;
      TH1F* HistoCoh=(TH1F*)(INFILE->Get(HistoNameCoh));
      if(!HistoCoh){cout<<HistoNameCoh<<"  not found"<<endl; continue;}

      int nEvt=HistoCoh->GetBinContent(1);

      //Compute the rms sums
      float Sum_RMS=0.;
      float Sum_RMS2=0.;
      float Sum_RMS4=0.;//needed for uncertainty estimation
      int   NGoodChan=0;
      for(int c=1;c<=NBChannel;c++){
	if( Histo->GetBinContent(c) > 0 ){//remove channels with rms=0
	  NGoodChan++;
	  float RMS=0.;
	  if(nEvt>0) RMS = sqrt(Histo->GetBinContent(c)/nEvt);
	  Sum_RMS += RMS; //rms for each channel
	  Sum_RMS2 += RMS*RMS ; //rms^2 for each channel
	  Sum_RMS4 += RMS*RMS*RMS*RMS ; //rms^2 for each channel
	}
      }

      float RMSCOH =0.;
      if(nEvt>0) RMSCOH = sqrt(HistoCoh->GetBinContent(3)/nEvt);
      

      float frac=0.;
      if( Sum_RMS > 0. ){
	if(RMSCOH*RMSCOH - Sum_RMS2 > 0 ) frac =   sqrt(  RMSCOH*RMSCOH - Sum_RMS2 )/ Sum_RMS ;
	if(RMSCOH*RMSCOH - Sum_RMS2 < 0 ) frac = - sqrt( -RMSCOH*RMSCOH + Sum_RMS2 )/ Sum_RMS ;
      }

      float RMSAvg=0;
      float RMSCohAvg=0.;
      if(NGoodChan>0){
	RMSAvg = Sum_RMS/NGoodChan ;
	RMSCohAvg = RMSCOH/sqrt(NGoodChan) ;
      }

      
      //Fill a histogram with the uncertainty on the fraction:
      float unc=0.;
      if(nEvt>0){
	unc= 1.65 * sqrt(2. / nEvt) * sqrt( ( RMSCOH*RMSCOH*RMSCOH*RMSCOH + Sum_RMS4 ) / fabs( RMSCOH*RMSCOH - Sum_RMS2 ) ) /  Sum_RMS;
	if(HUNC) HUNC->Fill( unc );
      }

      ///always write out the fractions and rms's
      osNoiseFracFEB<<FT
		    <<" "<<slot
		    <<" "<<RMSAvg
		    <<" "<<RMSCohAvg
		    <<" "<<frac
		    <<endl;
      
      ///Plot only FEB's which have have enough events
      if( nEvt > MinNEvent ) {

	HRMSAVG.SetBinContent(slot+1+NBFEB*(FT-firstFT) , RMSAvg );  // Average RMS per FEB
	HRMSCoh.SetBinContent(slot+1+NBFEB*(FT-firstFT) , RMSCohAvg ); // Coherent RMS divided by number of channels in FEB
	HFrac.SetBinContent(slot+1+NBFEB*(FT-firstFT), frac );
	HNChan.SetBinContent(slot+1+NBFEB*(FT-firstFT), HistoCoh->GetBinContent(2)/nEvt);

	///fill the noisy FEB list
	if(frac>MaxFrac) 
	  osNoisyFEB<<Det
		    <<" "<<Side
		    <<" "<<FT
		    <<" "<<slot+1
		    <<" "<<frac
		    <<endl;
	
      }

    }

  }

  fbNoiseFracFEB.close();
  fbNoisyFEB.close();

  TCanvas Canvas("Canvas","",1500,500);
  Canvas.SetLeftMargin(0.05);


  Canvas.Clear();
  HNChan.GetYaxis()->SetTitle("N channels");
  HNChan.GetYaxis()->SetRangeUser(0,150);
  HNChan.GetXaxis()->SetTitle("FEB");
  HNChan.Draw("histp");
  drawVerticalLines(&HFT,0,150);
  drawHorizontalLine(&HFT,128); 
  Canvas.Print(outpath+"/"+outfilename+"_NChan.gif");

  Canvas.Clear();
  HRMSAVG.GetYaxis()->SetTitle("Avg. RMS");
  HRMSAVG.GetYaxis()->SetRangeUser(RMSLow,RMSHigh);
  HRMSAVG.GetXaxis()->SetTitle("FEB");
  HRMSAVG.Draw("histp");
  drawVerticalLines(&HFT,RMSLow,RMSHigh);
  drawHorizontalLine(&HFT,8); 
  drawHorizontalLine(&HFT,4);
  Canvas.Print(outpath+"/"+outfilename+"_RMS.gif");

  Canvas.Clear();
  HRMSCoh.GetYaxis()->SetTitle("RMS_sum / sqrt(N)");
  HRMSCoh.GetYaxis()->SetRangeUser(RMSLow,RMSHigh);
  HRMSCoh.GetXaxis()->SetTitle("FEB");
  HRMSCoh.Draw("histp");
  drawVerticalLines(&HFT,RMSLow,RMSHigh);
  drawHorizontalLine(&HFT,8); 
  drawHorizontalLine(&HFT,4);
  Canvas.Print(outpath+"/"+outfilename+"_RMSCoh.gif");

  Canvas.Clear();
  HFrac.GetYaxis()->SetTitle("Fraction");
  HFrac.GetYaxis()->SetRangeUser(-MaxFrac,FracHigh);
  HFrac.GetXaxis()->SetTitle("FEB");
  HFrac.Draw("histp");
  drawHorizontalLine(&HFrac,0);
  drawHorizontalLine(&HFrac,MaxFrac,2);
  drawVerticalLines(&HFT,-MaxFrac,FracHigh);
  Canvas.Print(outpath+"/"+outfilename+"_Frac.gif");
  
}


void plotNoiseForRun(TString infile,TString outtag="test",  int type=0, bool draw=1){
  cout<<"infile : "<<infile<<endl;
  TFile INFILE(infile,"read");
  if(INFILE.IsZombie()){
    cout<<"Bad input file :"<<infile<<endl;
    return;
  }


  TString outpath=PLOTSPATH+"/"+outtag;
  cout<<"outpath : "<<outpath<<endl;
  system((TString("rm -rf ")+outpath).Data());
  system((TString("mkdir ")+outpath).Data());


  // if(type==128||type==0)
  //   for(int det=0;det<NDET;det++)
  //     for(int side=0;side<NSIDE;side++)
  // 	plotCohNoiseFromHisto(&INFILE,outpath,det,side,128,0,0,31,draw);
  
  // if(type==64||type==0)
  //   for(int det=0;det<NDET;det++)
  //     for(int side=0;side<NSIDE;side++)
  // 	for(int i=0;i<2;i++)
  // 	  plotCohNoiseFromHisto(&INFILE,outpath,det,side,64,i,0,31,draw);
  
  // if(type==32||type==0)
  //   for(int det=0;det<NDET;det++)
  //     for(int side=0;side<NSIDE;side++)
  // 	for(int i=0;i<4;i++)
  // 	  plotCohNoiseFromHisto(&INFILE,outpath,det,side,32,i,0,31,draw);
  

  TH1F HFracUnc("HFracUnc","",100,0,.1);//histogram will be filled with all FEBS in all detectors

  for(int det=0;det<NDET;det++)
    for(int side=0;side<NSIDE;side++)
      plotCohNoiseFromHisto2(&INFILE,outpath,det,side,0,31,&HFracUnc);


  //gStyle->SetOptStat(1);
  TCanvas Canvas("Canvas");
  Canvas.Clear();
  HFracUnc.GetXaxis()->SetTitle("absolute uncertainty on noise fraction");
  HFracUnc.Draw("histp");
  Canvas.Print(outpath+"/FracUnc.gif");
}


void plotNoiseHist( int type=128, int draw=1){
  //set draw=1 for reference run to see the distributions.
  //set type=0 to plot all coherent combinations: 32, 64, 128. For stability profile type=128 is enough
  
  gROOT->ProcessLine(".x ./LArStudies/run/RootLogon.C");
  //gStyle->SetMarkerSize(4);
  gStyle->SetMarkerStyle(8);
  gStyle->SetMarkerColor(4);


  std::vector<std::string> runs=getListOfRuns();
  for(int i=0;i<runs.size();i++){
    TString file=NTUPLEPATH+"/data15_comm."+runs.at(i).c_str()+".physics_CosmicCalo.merge.RAW/merged.root";
    plotNoiseForRun(file,TString("CosmicCalo_Coherent_")+runs[i].c_str(),type,draw);
  }
  
  gROOT->ProcessLine(".q");
}












////////////////////Used for fist presentation:

void plotCohNoiseFromHisto(TFile * INFILE, TString outpath, unsigned int Det, unsigned int Side, unsigned type=128, unsigned index=0, unsigned int firstFT=0, unsigned int lastFT=31, bool draw=0){
  
  TString outfilename=TString("NoiseType")+(long)type+"i"+(long)index+"_Det"+(long)Det+"_Side"+(long)Side;

  int NFT=lastFT-firstFT+1;
  TH1F HMean("HMean","",NFT*NBFEB,0,NFT*NBFEB);
  TH1F HRMS("HRMS","",NFT*NBFEB,0,NFT*NBFEB);
  TH1F HMeanCoh("HMeanCoh","",NFT*NBFEB,0,NFT*NBFEB);
  TH1F HRMSCoh("HRMSCoh","",NFT*NBFEB,0,NFT*NBFEB);
  TH1F HFrac("HFrac","",NFT*NBFEB,0,NFT*NBFEB);
  TH1F HFT("HFT","",NFT,0,NFT*NBFEB);
  

  std::filebuf fbNoiseFracFT;
  fbNoiseFracFT.open (outpath+"/"+outfilename+"_NoiseFracFT.txt",std::ios::out);
  std::ostream osNoiseFracFT(&fbNoiseFracFT);

  std::filebuf fbNoiseFracFEB;
  fbNoiseFracFEB.open (outpath+"/"+outfilename+"_NoiseFracFEB.txt",std::ios::out);
  std::ostream osNoiseFracFEB(&fbNoiseFracFEB);

  std::filebuf fbNoisyFEB;
  fbNoisyFEB.open (outpath+"/"+outfilename+"_NoisyFEB.txt",std::ios::out);
  std::ostream osNoisyFEB(&fbNoisyFEB);

  TCanvas CanvasDist("CanvasDist");

  for(int FT = firstFT; FT<=lastFT; FT++){
    CanvasDist.Clear();
    CanvasDist.Divide(4,4);

    TH1F HistoNoiseAvg(TString("HistoNoiseAvg")+(long)FT,"",NHistBins,HistLow,HistHigh);
    TH1F HistoNoiseAvgCoh(TString("HistoNoiseAvgCoh")+(long)FT,"",NHistBins,HistLow,HistHigh);

    TH1F* Histo[NBFEB];
    TH1F* HistoCoh[NBFEB];
    for(int slot = 0; slot<NBFEB; slot++){//in NoiseTupleCoherent.cxx FEB starts from 0
      CanvasDist.cd(slot+1);
      TString histID=TString("")+(long)type+"_Det"+(long)(Det)+"_Side"+(long)(Side)+"_FT"+(long)(FT)+"_Slot"+(long)(slot)+"_"+(long)index;
      TString HistoName=TString("Histo")+histID;
      TString HistoCohName=TString("HistoCoh")+histID;
      Histo[slot]=(TH1F*)(INFILE->Get(HistoName));
      HistoCoh[slot]=(TH1F*)(INFILE->Get(HistoCohName));
      if(!Histo[slot]){cout<<Histo<<"  not found"<<endl; continue;}
      if(!HistoCoh[slot]){cout<<HistoCoh<<"  not found"<<endl; continue;}

      float frac=0.;
      if(Histo[slot]->GetRMS()>0)
	frac=(HistoCoh[slot]->GetRMS() - Histo[slot]->GetRMS()) / Histo[slot]->GetRMS();
      
      osNoiseFracFEB<<FT<<" "<<slot
		    <<" "<<Histo[slot]->GetRMS()
		    <<" "<<HistoCoh[slot]->GetRMS()
		    <<" "<<frac<<endl;
      
      ///fill only FEB's which have entries
      if( HistoCoh[slot]->Integral() > MinNEvent && Histo[slot]->GetRMS() > 0.) {

	HMean.SetBinContent(slot+1+NBFEB*(FT-firstFT),Histo[slot]->GetMean());
	HRMS.SetBinContent(slot+1+NBFEB*(FT-firstFT),Histo[slot]->GetRMS());
	HMeanCoh.SetBinContent(slot+1+NBFEB*(FT-firstFT),HistoCoh[slot]->GetMean());
	HRMSCoh.SetBinContent(slot+1+NBFEB*(FT-firstFT),HistoCoh[slot]->GetRMS());

	HFrac.SetBinContent(slot+1+NBFEB*(FT-firstFT), frac);

	///fill the noisy FEB list
	if(frac>MaxFrac) 
	  osNoisyFEB<<Det
		    <<" "<<Side
		    <<" "<<FT
		    <<" "<<slot+1
		    <<" "<<frac
		    <<endl;
	

	if(draw){
	  Histo[slot]->Scale(HistoCoh[slot]->Integral()/Histo[slot]->Integral());
	  if(HistoCoh[slot]->GetMaximum() > Histo[slot]->GetMaximum()){
	    HistoCoh[slot]->Draw("histp");
	    Histo[slot]->Draw("histsame");
	  }else {
	    Histo[slot]->Draw("hist");
	    HistoCoh[slot]->Draw("histpsame");
	  }
	}

	HistoNoiseAvg.Add(Histo[slot]);
	HistoNoiseAvgCoh.Add(HistoCoh[slot]);
	
      }

    }
    if(draw)CanvasDist.Print(outpath+"/"+outfilename+"_FT"+(long)FT+".gif");
    

    ///Write out the average per FT
    float frac=0.;
    if(HistoNoiseAvgCoh.Integral()>MinNEvent && HistoNoiseAvg.GetRMS()>0.)
      frac=(HistoNoiseAvgCoh.GetRMS()-HistoNoiseAvg.GetRMS())/HistoNoiseAvg.GetRMS();
    
    osNoiseFracFT<<FT
		 <<" "<<HistoNoiseAvg.GetRMS()
		 <<" "<<HistoNoiseAvgCoh.GetRMS()
		 <<" "<<frac<<endl;
  }

  fbNoiseFracFT.close();
  fbNoiseFracFEB.close();
  fbNoisyFEB.close();

  TCanvas Canvas("Canvas","",1500,500);
  Canvas.SetLeftMargin(0.05);
  gStyle->SetTitleOffset(0.50,"Y");


  Canvas.Clear();
  HMean.GetYaxis()->SetTitle("Mean");
  HMean.GetYaxis()->SetRangeUser(MeanLow,MeanHigh);
  HMean.GetXaxis()->SetTitle("FEB number");
  HMean.Draw("histp");
  drawHorizontalLine(&HMean,0);
  drawVerticalLines(&HFT,MeanLow,MeanHigh);
  Canvas.Print(outpath+"/"+outfilename+"_Mean.gif");

  Canvas.Clear();
  HRMS.GetYaxis()->SetTitle("RMS");
  HRMS.GetYaxis()->SetRangeUser(RMSLow,RMSHigh);
  HRMS.GetXaxis()->SetTitle("FEB number");
  HRMS.Draw("histp");
  drawVerticalLines(&HFT,RMSLow,RMSHigh);
  Canvas.Print(outpath+"/"+outfilename+"_RMS.gif");

  Canvas.Clear();
  HMeanCoh.GetYaxis()->SetTitle("Mean");
  HMeanCoh.GetYaxis()->SetRangeUser(MeanLow,MeanHigh);
  HMeanCoh.GetXaxis()->SetTitle("FEB number");
  HMeanCoh.Draw("histp");
  drawHorizontalLine(&HMeanCoh,0);
  drawVerticalLines(&HFT,MeanLow,MeanHigh);
  Canvas.Print(outpath+"/"+outfilename+"_MeanCoh.gif");

  Canvas.Clear();
  HRMSCoh.GetYaxis()->SetTitle("RMS");
  HRMSCoh.GetYaxis()->SetRangeUser(RMSLow,RMSHigh);
  HRMSCoh.GetXaxis()->SetTitle("FEB number");
  HRMSCoh.Draw("histp");
  drawVerticalLines(&HFT,RMSLow,RMSHigh);
  Canvas.Print(outpath+"/"+outfilename+"_RMSCoh.gif");

  Canvas.Clear();
  HFrac.GetYaxis()->SetTitle("Coherent Noise Fraction");
  HFrac.GetYaxis()->SetRangeUser(-MaxFrac,FracHigh);
  HFrac.GetXaxis()->SetTitle("FEB number");
  HFrac.Draw("histp");
  drawHorizontalLine(&HMeanCoh,0);
  drawHorizontalLine(&HMeanCoh,MaxFrac,2);
  drawVerticalLines(&HFT,-MaxFrac,FracHigh);
  Canvas.Print(outpath+"/"+outfilename+"_Frac.gif");
  
}

