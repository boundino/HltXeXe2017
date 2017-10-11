#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TProfile.h>
#include "../includes/xjjrootuti.h"
#include "../includes/xjjcuti.h"

void nTrkvshiBin_savehist(TString inputname, TString outputname, TString collsyst)
{
  TFile* inf = new TFile(inputname.Data());
  TTree* ntHi = (TTree*)inf->Get("hiEvtAnalyzer/HiTree");
  ntHi->SetBranchStatus("*", 0);
  TTree* ntTrk = (TTree*)inf->Get("anaTrack/trackTree");
  ntTrk->SetBranchStatus("*", 0);
  Int_t hiBin; xjjroot::setbranchaddress(ntHi, "hiBin", &hiBin);
  Float_t hiHF;  xjjroot::setbranchaddress(ntHi, "hiHF", &hiHF);
  Int_t nTrk;  xjjroot::setbranchaddress(ntTrk, "nTrk", &nTrk);
  TFile* infcal;
  TH1D* hhiHFvshiBinThreshold;
  if(collsyst=="XeXe")
    {
      infcal = new TFile("rootfiles/calibration.root");
      hhiHFvshiBinThreshold = (TH1D*)infcal->Get("hhiHFvshiBin");
      hhiHFvshiBinThreshold->SetName("hhiHFvshiBinThreshold");
    }

  TProfile* hnTrkvshiBin = new TProfile("hnTrkvshiBin", ";Centrality (%);<nTrk>", 50, 0, 100, 0, 10000);
  TProfile* hhiHFvshiBin = new TProfile("hhiHFvshiBin", ";Centrality (%);<hiHF>", 50, 0, 100, 0, 10000);
  TH1D* hnTrk = new TH1D("hnTrk",";nTrk;Events", 100, 0, 10000);

  for(int i=0;i<ntHi->GetEntries();i++)
    {
      if(i%1000==0) xjjc::progressbar(i, ntHi->GetEntries());
      ntHi->GetEntry(i);
      ntTrk->GetEntry(i);
      hnTrk->Fill(nTrk);
      Float_t fillhiBin = (collsyst=="XeXe")?(hhiHFvshiBinThreshold->GetBinContent(hhiHFvshiBinThreshold->GetXaxis()->FindBin(hiHF))):hiBin;
      hnTrkvshiBin->Fill(fillhiBin/2., nTrk);
      hhiHFvshiBin->Fill(fillhiBin/2., hiHF);
    }
  xjjc::progressbar_summary(ntHi->GetEntries());
  hnTrk->Sumw2();
  hnTrk->Scale(1./hnTrk->Integral());

  TFile* outf = new TFile(Form("%s_%s.root",outputname.Data(),collsyst.Data()),"recreate");
  outf->cd();
  hnTrkvshiBin->Write();
  hhiHFvshiBin->Write();
  hnTrk->Write();
  outf->Close();

}

int main(int argc, char* argv[])
{
  if(argc==4) 
    {
      nTrkvshiBin_savehist(argv[1], argv[2], argv[3]);
      return 0;
    }
  return 1;
}
