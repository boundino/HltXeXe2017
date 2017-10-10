#include <iostream>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TProfile.h>
#include "../includes/xjjrootuti.h"
#include "../includes/xjjcuti.h"

const int NhiHF = 40000;
const float MAXhiHF = 4000;
const int NhiBin = 200;
void calibration(TString inputname, TString outputname)
{
  xjjroot::setgstyle();
  const int Ncent = 100;
  TFile* inf = new TFile(inputname.Data());
  TTree* ntHi = (TTree*)inf->Get("hiEvtAnalyzer/HiTree");
  ntHi->SetBranchStatus("*", 0);
  Float_t hiHF; xjjroot::setbranchaddress(ntHi, "hiHF", &hiHF);

  TH1D* hhiHF = new TH1D("hhiHF",";hiHF;Events", NhiHF, 0, MAXhiHF);
  for(int i=0;i<ntHi->GetEntries();i++)
    {
      if(i%1000==0) xjjc::progressbar(i, ntHi->GetEntries());
      ntHi->GetEntry(i);
      hhiHF->Fill(hiHF);
    }
  xjjc::progressbar_summary(ntHi->GetEntries());

  Float_t hiHFtotal = hhiHF->Integral();
  Float_t hiHFcurrent = 0;
  Float_t hiHFprevious = 0;
  Float_t hiBins[NhiBin+1];
  hiBins[NhiBin] = MAXhiHF;
  Int_t j=1;
  for(int i=NhiHF;i>0;i--)
    {
      hiHFprevious = hiHFcurrent;
      hiHFcurrent += hhiHF->GetBinContent(i);
      if((hiHFprevious/hiHFtotal)<(j*1.0/NhiBin) && (hiHFcurrent/hiHFtotal)>=(j*1.0/NhiBin))
        {
          // check if binning is fine enough
          if((hiHFcurrent/hiHFtotal)>=((j+1)*1.0/NhiBin)) {std::cout<<"fatal error: "<<j<<std::endl; return;}
          hiBins[NhiBin-j] = hhiHF->GetBinCenter(i)-hhiHF->GetBinWidth(i)/2.;
          j++;
        }
    }
  TH1D* hhiBinvshiHF = new TH1D("hhiHFvshiBin",";hiHF;hiBin", NhiBin, hiBins);
  for(int j=0;j<NhiBin;j++)
    {
      hhiBinvshiHF->SetBinContent(j+1, NhiBin-j-1);
    }

  TFile* outf = new TFile(Form("%s.root",outputname.Data()),"recreate");
  outf->cd();
  hhiBinvshiHF->Write();
  outf->Close();

}

int main(int argc, char* argv[])
{
  if(argc==3) 
    {
      calibration(argv[1], argv[2]);
      return 0;
    }
  return 1;
}
