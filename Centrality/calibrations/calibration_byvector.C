#include <iostream>
#include <algorithm>
#include <TFile.h>
#include <TTree.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TProfile.h>
#include "../includes/xjjrootuti.h"
#include "../includes/xjjcuti.h"

const int NhiBin = 200;
void calibration(TString inputname, TString outputname)
{
  xjjroot::setgstyle();
  const int Ncent = 100;
  TFile* inf = new TFile(inputname.Data());
  TTree* ntHi = (TTree*)inf->Get("hiEvtAnalyzer/HiTree");
  ntHi->SetBranchStatus("*", 0);
  Float_t hiHF; xjjroot::setbranchaddress(ntHi, "hiHF", &hiHF);

  std::vector<Float_t> vhiHF;
  for(int i=0;i<ntHi->GetEntries();i++)
    {
      if(i%1000==0) xjjc::progressbar(i, ntHi->GetEntries());
      ntHi->GetEntry(i);
      vhiHF.push_back(hiHF);
    }
  xjjc::progressbar_summary(ntHi->GetEntries());

  std::sort(vhiHF.begin(),vhiHF.end());

  Float_t hiBins[NhiBin+1];
  hiBins[0] = 0;
  for(int j=1;j<NhiBin+1;j++)
    {
      hiBins[j] = vhiHF[int(j*vhiHF.size()/NhiBin)-1];
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
