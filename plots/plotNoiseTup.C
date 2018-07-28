
void plotBarrelFEBfromHisto(TFile * INFILE, TString outpath, unsigned type, unsigned int PosNeg, unsigned int firstFT, unsigned int lastFT, bool draw=0);

void plotNoise(TString infile,TString outtag="test"){

  TString outpath="/afs/cern.ch/user/b/benitezj/www/LArStudies/Noise";
  outpath=outpath+"/"+outtag;
  system((TString("rm -rf ")+outpath).Data());
  system((TString("mkdir ")+outpath).Data());

  TFile INFILE(infile,"read");
  plotBarrelFEBfromHisto(&INFILE,outpath,128,0,0,31);//A
  plotBarrelFEBfromHisto(&INFILE,outpath,128,1,0,31);//C
  
  gROOT->ProcessLine(".q");
}




void plotBarrelFEBfromHisto(TFile * INFILE, TString outpath, unsigned type, unsigned int PosNeg, unsigned int firstFT, unsigned int lastFT, bool draw){
  TString outfilename=TString("BarrelSide")+(long)PosNeg+"_FEB"+(long)type+"_FT"+(long)firstFT+"To"+(long)lastFT;
  TCanvas Canvas("Canvas","",1500,500);

  int NFT=lastFT-firstFT+1;
  TH1F HMean("HMean","",NFT*NBFEB,0,NFT*NBFEB);
  TH1F HRMS("HRMS","",NFT*NBFEB,0,NFT*NBFEB);
  TH1F HMeanCoh("HMeanCoh","",NFT*NBFEB,0,NFT*NBFEB);
  TH1F HRMSCoh("HRMSCoh","",NFT*NBFEB,0,NFT*NBFEB);
  TH1F HFrac("HFrac","",NFT*NBFEB,0,NFT*NBFEB);
  
  
  for(int i = firstFT; i<=lastFT; i++){
    Canvas.Clear();
    if(draw)Canvas.Divide(4,4);
    TH1F* Histo[NBFEB];
    TH1F* HistoCoh[NBFEB];
    for(int l = 0; l<NBFEB; l++){
      if(draw)Canvas.cd(l+1);
      TString HistoName=TString("Histo")+(long)type+"_Det"+(long)(0)+"_Side"+(long)(PosNeg)+"_FT"+(long)(i)+"_Slot"+(long)(l)+"_0";
      TString HistoCohName=TString("HistoCoh")+(long)type+"_Det"+(long)(0)+"_Side"+(long)(PosNeg)+"_FT"+(long)(i)+"_Slot"+(long)(l)+"_0";
      Histo[l]=(TH1F*)(INFILE->Get(HistoName));
      HistoCoh[l]=(TH1F*)(INFILE->Get(HistoCohName));
      if(!Histo[l]){cout<<Histo<<"  not found"<<endl; continue;}
      if(!HistoCoh[l]){cout<<HistoCoh<<"  not found"<<endl; continue;}
      HMean.SetBinContent(l+1+NBFEB*(i-firstFT),Histo[l]->GetMean());
      HRMS.SetBinContent(l+1+NBFEB*(i-firstFT),Histo[l]->GetRMS());
      HMeanCoh.SetBinContent(l+1+NBFEB*(i-firstFT),HistoCoh[l]->GetMean());
      HRMSCoh.SetBinContent(l+1+NBFEB*(i-firstFT),HistoCoh[l]->GetRMS());
      float frac=0.;
      if(Histo[l]->GetRMS()) frac=(HistoCoh[l]->GetRMS() - Histo[l]->GetRMS()) / Histo[l]->GetRMS();
      //cout<<i<<" "<<l<<" "<<frac<<endl;
      HFrac.SetBinContent(l+1+NBFEB*(i-firstFT), frac);
    }
    if(draw)Canvas.Print(outpath+"/"+outfilename+"_FT"+(long)i+".gif");

  }


  Canvas.Clear();
  HMean.GetYaxis()->SetTitle("Mean");
  HMean.GetYaxis()->SetRangeUser(MeanLow,MeanHigh);
  HMean.GetXaxis()->SetTitle("FEB number");
  HMean.Draw("histp");
  drawHorizontalLine(&HMean);
  Canvas.Print(outpath+"/"+outfilename+"_Mean.gif");

  Canvas.Clear();
  HRMS.GetYaxis()->SetTitle("RMS");
  HRMS.GetYaxis()->SetRangeUser(RMSLow,RMSHigh);
  HRMS.GetXaxis()->SetTitle("FEB number");
  HRMS.Draw("histp");
  Canvas.Print(outpath+"/"+outfilename+"_RMS.gif");

  Canvas.Clear();
  HMeanCoh.GetYaxis()->SetTitle("Mean");
  HMeanCoh.GetYaxis()->SetRangeUser(MeanLow,MeanHigh);
  HMeanCoh.GetXaxis()->SetTitle("FEB number");
  HMeanCoh.Draw("histp");
  drawHorizontalLine(&HMeanCoh);
  Canvas.Print(outpath+"/"+outfilename+"_MeanCoh.gif");

  Canvas.Clear();
  HRMSCoh.GetYaxis()->SetTitle("RMS");
  HRMSCoh.GetYaxis()->SetRangeUser(RMSLow,RMSHigh);
  HRMSCoh.GetXaxis()->SetTitle("FEB number");
  HRMSCoh.Draw("histp");
  Canvas.Print(outpath+"/"+outfilename+"_RMSCoh.gif");

  Canvas.Clear();
  HFrac.GetYaxis()->SetTitle("Coherent Noise Fraction");
  HFrac.GetXaxis()->SetTitle("FEB number");
  HFrac.Draw("histp");
  drawHorizontalLine(&HMeanCoh);
  drawHorizontalLine(&HMeanCoh,MaxFrac,2);
  Canvas.Print(outpath+"/"+outfilename+"_Frac.gif");
  
}


TH1F* getNoiseHisto(TTree * T,TString Cut,TString title){
  TString HName=TString("HNoise")+title.Data();
  TH1F* H =new TH1F(HName.Data(),"",NHistBins,HistLow,HistHigh);
  H->GetXaxis()->SetTitle("sample0-pedestal");
  T->Draw(TString(H->GetXaxis()->GetTitle())+">>"+HName.Data(),Cut);
  return H;
}

TH1F* plotNoise(TTree * T,TString Cut,TString title){
  TH1F*H=getNoiseHisto(T,Cut,title);

  H->Draw("hist");

  TLatex text;
  text.SetTextSize(0.07);
  char mean[100];
  sprintf(mean,"#mu=%.1f",H->GetMean());
  char rms[100];
  sprintf(rms,"#sigma=%.1f",H->GetRMS());
  text.DrawLatexNDC(0.2,0.8,title);
  text.DrawLatexNDC(0.25,0.7,mean);
  text.DrawLatexNDC(0.25,0.6,rms);
  return H;
}


void drawVerticalLines(TH1F* H,int n,float ymin,float ymax){
  TLine line;
  line.SetLineWidth(1);
  line.SetLineColor(1);
  for(int b=1;b<H->GetNbinsX();b++){
    if((b-1)%n==0)
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





//  TChain * TUPLE[NDET][NSIDE][NFT];
  // for(int d=0;d<NDET;d++)
  //   for(int s=0;s<NSIDE;s++)
  //     for(int F=0;F<NFT;F++){
  // 	TUPLE[d][s][F]=new TChain(TString("tuple_")+(long)d+"_"+(long)s+"_"+(long)F);
  // 	TUPLE[d][s][F]->Add(infile);
  //     }
  //plotBarrelFEB(outpath,0,0,7);//Barrel A
  

void plotBarrelFEB(TString outpath,  int PosNeg, int firstFT,int lastFT){
  TString outfilename=TString("")+"BarrelFEB_Side"+(long)PosNeg;

  int NFT=lastFT-firstFT+1;
  TH1F HMean_Slot("HMean_Slot","",NFT*NBFEB,0,NFT*NBFEB);
  TH1F HRMS_Slot("HRMS_Slot","",NFT*NBFEB,0,NFT*NBFEB);


  for(int i = firstFT; i<=lastFT; i++){
    Canvas.Clear();
    Canvas.Divide(4,4);
    TH1F* H[NBFEB];
    for(int l = 0; l<NBFEB; l++){//14 FEB's per Barrel FT
      Canvas.cd(l+1);
      TString cut=TString("slot==")+(long)(l+1);
      H[l]=plotNoise(TUPLE[0][PosNeg][i],cut,TString("FT")+(long)i+",FEB"+(long)(l+1));
      HMean_Slot.SetBinContent(l+1+NBFEB*(i-firstFT),H[l]->GetMean());
      HRMS_Slot.SetBinContent(l+1+NBFEB*(i-firstFT),H[l]->GetRMS());
    }
    Canvas.Print(outpath+"/"+outfilename+"_FT"+(long)i+".gif");

    for(int l = 0; l<NBFEB; l++)
      delete H[l];
  }

  Canvas.Clear();
  HMean_Slot.GetYaxis()->SetTitle("Mean");
  HMean_Slot.GetYaxis()->SetRangeUser(MeanLow,MeanHigh);
  //HMean_Slot.GetXaxis()->SetNdivisions(0);  
  HMean_Slot.GetXaxis()->SetTitle("FEB number");
  HMean_Slot.Draw("histp");
  //drawVerticalLines(&HMean_FT,NBFEB,MeanLow,MeanHigh);
  //TLatex key;
  //key.SetTextColor(2);
  //key.DrawLatexNDC(0.2,0.05,"#minus Feedthrough");
  //key.SetTextColor(1);
  //key.DrawLatexNDC(0.6,0.05,"#bullet FEB");
  //drawLabels(&HMean_FT,NBFEB,firstFT,MeanLow);
  Canvas.Print(outpath+"/"+outfilename+"_FT"+(long)firstFT+"To"+(long)lastFT+"_Mean.gif");

  Canvas.Clear();
  HRMS_Slot.GetYaxis()->SetTitle("RMS");
  HRMS_Slot.GetYaxis()->SetRangeUser(RMSLow,RMSHigh);
  //HRMS_Slot.GetXaxis()->SetNdivisions(0);
  HRMS_Slot.GetXaxis()->SetTitle("FEB number");
  HRMS_Slot.Draw("histp");
  //drawVerticalLines(&HRMS_FT,NBFEB,RMSLow,RMSHigh);
  //key.SetTextColor(2);
  //key.DrawLatexNDC(0.2,0.05,"#minus Feedthrough");
  //key.SetTextColor(1);
  //key.DrawLatexNDC(0.6,0.05,"#bullet FEB");
  //drawLabels(&HRMS_FT,NBFEB,firstFT,RMSLow);
  Canvas.Print(outpath+"/"+outfilename+"_FT"+(long)firstFT+"To"+(long)lastFT+"_RMS.gif");
  

}
