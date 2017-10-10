#include <iostream>
#include <TFile.h>
#include <TH1D.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <TProfile.h>
#include "../includes/xjjrootuti.h"

std::map<TString, Color_t> colors = {std::pair<TString, Color_t>("XeXe",kRed+2),
                                     std::pair<TString, Color_t>("PbPb",kAzure+2)};
std::map<TString, TString> energy = {std::pair<TString, TString>("XeXe","5.44"),
                                     std::pair<TString, TString>("PbPb","5.02")};
void nTrkvshiBin_plothist(TString outputname, std::vector<TString> collsyst)
{
  xjjroot::setgstyle();
  std::vector<TFile*> inf(collsyst.size());
  std::vector<TProfile*> ahnTrkvshiBin(collsyst.size());
  std::vector<TH1D*> ahnTrk(collsyst.size());

  for(int i=0;i<collsyst.size();i++)
    {
      inf.at(i) = new TFile(Form("%s_%s.root",outputname.Data(),collsyst.at(i).Data()));
      ahnTrkvshiBin.at(i) = (TProfile*)inf.at(i)->Get("hnTrkvshiBin");
      ahnTrkvshiBin.at(i)->SetName(Form("hnTrkvshiBin_%s",collsyst.at(i).Data()));
      ahnTrk.at(i) = (TH1D*)inf.at(i)->Get("hnTrk");
      ahnTrk.at(i)->SetName(Form("hnTrk_%s",collsyst.at(i).Data()));
      xjjroot::setthgrstyle(ahnTrkvshiBin.at(i), colors[collsyst.at(i)], 20, 1.1, colors[collsyst.at(i)], 1, 1);
      xjjroot::setthgrstyle(ahnTrk.at(i), colors[collsyst.at(i)], 20, 1.1, colors[collsyst.at(i)], 1, 1);
    }

  TH2F* hemptyhnTrkvshiBin = new TH2F("hemptyhnTrkvshiBin",";Centrality (%);<nTrk>", 100, 0, 100, 100, 0, 8000);
  xjjroot::sethempty(hemptyhnTrkvshiBin, 0, 0.3);
  TH2F* hemptyzoom = new TH2F("hemptyzoom","",10, 80, 100, 100, 0, 40);
  xjjroot::sethempty(hemptyzoom);

  TCanvas* chnTrkvshiBin = new TCanvas("chnTrkvshiBin","",600,600);
  hemptyhnTrkvshiBin->Draw();
  xjjroot::drawCMS();
  for(int i=0;i<collsyst.size();i++)
    ahnTrkvshiBin.at(i)->Draw("pesame");
  chnTrkvshiBin->cd();
  TPad* pzoom = new TPad("pzoom","",0.5, 0.6, 0.957, 0.90);
  pzoom->SetFillColor(0);
  pzoom->SetBorderMode(0);
  pzoom->SetBorderSize(2);
  pzoom->SetRightMargin(0);
  pzoom->SetTopMargin(0);
  pzoom->Draw();
  pzoom->cd();
  hemptyzoom->Draw();
  for(int i=0;i<collsyst.size();i++)
    ahnTrkvshiBin.at(i)->Draw("pesame");
  chnTrkvshiBin->cd();
  xjjroot::drawline(0, 8000, 100, 8000, kBlack, 1, 1);
  xjjroot::drawline(100, 0, 100, 8000, kBlack, 1, 1);
  TLegend* leg = new TLegend(0.58, 0.60-0.06*collsyst.size(), 0.90, 0.60);
  xjjroot::setleg(leg, 0.045);
  for(int i=0;i<collsyst.size();i++)
    leg->AddEntry(ahnTrkvshiBin.at(i), Form("%s %s TeV",collsyst.at(i).Data(),energy[collsyst.at(i)].Data()), "p");
  leg->Draw();
  chnTrkvshiBin->SaveAs("plots/chnTrkvshiBin.pdf");

  for(int i=0;i<collsyst.size();i++)
    {
      TCanvas* chnTrkvshiBinseparate = new TCanvas("chnTrkvshiBinseparate","",600,600);
      hemptyhnTrkvshiBin->Draw();
      xjjroot::drawCMS(true, collsyst.at(i), energy[collsyst.at(i)]);
      ahnTrkvshiBin.at(i)->Draw("pesame");
      chnTrkvshiBinseparate->cd();
      TPad* pzoomseparate = new TPad("pzoomseparate","",0.5, 0.6, 0.957, 0.90);
      pzoomseparate->SetFillColor(0);
      pzoomseparate->SetBorderMode(0);
      pzoomseparate->SetBorderSize(2);
      pzoomseparate->SetRightMargin(0);
      pzoomseparate->SetTopMargin(0);
      pzoomseparate->Draw();
      pzoomseparate->cd();
      hemptyzoom->Draw();
      ahnTrkvshiBin.at(i)->Draw("pesame");
      chnTrkvshiBinseparate->cd();
      xjjroot::drawline(0, 8000, 100, 8000, kBlack, 1, 1);
      xjjroot::drawline(100, 0, 100, 8000, kBlack, 1, 1);
      chnTrkvshiBinseparate->SaveAs(Form("plots/chnTrkvshiBin_%s.pdf",collsyst.at(i).Data()));
      delete pzoomseparate;
      delete chnTrkvshiBinseparate;
    }

  TH2F* hemptyhnTrk = new TH2F("hemptyhnTrk",";nTrk;Probability",100, 0, 10000, 10, 1.e-5, 1);
  xjjroot::sethempty(hemptyhnTrk, 0, 0.3);
  hemptyhnTrk->GetXaxis()->SetNdivisions(505);
  TCanvas* chnTrk = new TCanvas("chnTrk","",600,600);
  chnTrk->SetLogy();
  hemptyhnTrk->Draw();
  xjjroot::drawCMS();
  for(int i=0;i<collsyst.size();i++)
    ahnTrk.at(i)->Draw("pesame");
  TLegend* leghnTrk = new TLegend(0.58, 0.80-0.06*collsyst.size(), 0.90, 0.80);
  xjjroot::setleg(leghnTrk, 0.045);
  for(int i=0;i<collsyst.size();i++)
    leghnTrk->AddEntry(ahnTrk.at(i), Form("%s %s TeV",collsyst.at(i).Data(),energy[collsyst.at(i)].Data()), "p");
  leghnTrk->Draw();
  chnTrk->SaveAs("plots/chnTrk.pdf");

  for(int i=0;i<collsyst.size();i++)
    {
      TCanvas* chnTrkseparate = new TCanvas("chnTrkseparate","",600,600);
      chnTrkseparate->SetLogy();
      hemptyhnTrk->Draw();
      xjjroot::drawCMS(true, collsyst.at(i), energy[collsyst.at(i)]);
      ahnTrk.at(i)->Draw("pesame");
      chnTrkseparate->SaveAs(Form("plots/chnTrk_%s.pdf",collsyst.at(i).Data()));
      delete chnTrkseparate;
    }
}

int main(int argc, char* argv[])
{
  if(argc>2)
    {
      std::vector<TString> collsyst;
      for(int p=2;p<argc;p++) collsyst.push_back(argv[p]);
      nTrkvshiBin_plothist(argv[1], collsyst);
      return 0;
    }
  return 1;
}
