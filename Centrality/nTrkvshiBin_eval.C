#include <iostream>
#include <TFile.h>
#include <TProfile.h>

void nTrkvshiBin_eval(TString inputname, Float_t val, Option_t* option="x")
{
  TString foption = option;
  foption.ToLower();
  TFile* inf = new TFile(Form("%s",inputname.Data()));
  TProfile* hhiHFvshiBin = (TProfile*)inf->Get("hhiHFvshiBin");
  
  if(foption=="y")
    {
      for(int i=0;i<hhiHFvshiBin->GetSize()-3;i++)
        {
          if(hhiHFvshiBin->GetBinContent(i+1)>=val && hhiHFvshiBin->GetBinContent(i+2)<val)
            {
              Float_t vallow = hhiHFvshiBin->GetBinContent(i+1);
              Float_t valhigh = hhiHFvshiBin->GetBinContent(i+2);
              Float_t xvallow = hhiHFvshiBin->GetBinCenter(i+1);
              Float_t xvalhigh = hhiHFvshiBin->GetBinCenter(i+2);
              Float_t xval = xvallow + (xvalhigh-xvallow)*(vallow-val)/(vallow-valhigh);
              std::cout<<"ETSum: "<<val<<"  ==>  Centrality: "<<Form("%.0f",xval)<<"%"<<std::endl;
              break;
            }
          if(i==hhiHFvshiBin->GetSize()-2) std::cout<<"error: no proper value found."<<std::endl;
        }
    }
  else if(foption=="x")
    {
      std::cout<<"Centrality: "<<val<<"%  ==>  ETSum: "<<hhiHFvshiBin->GetBinContent(hhiHFvshiBin->FindBin(val))<<std::endl;
    }
  else std::cout<<"error: invalid option."<<std::endl;

}

int main(int argc, char* argv[])
{
  if(argc==3)
    {
      nTrkvshiBin_eval(argv[1], atof(argv[2]));
      return 0;
    }
  else if(argc==4)
    {
      nTrkvshiBin_eval(argv[1], atof(argv[2]), argv[3]);
      return 0;      
    }
  return 1;
}
