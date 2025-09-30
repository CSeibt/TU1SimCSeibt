
//#include "TMath.h"

void FitEfficiency()
{
    TGraphErrors* ge2 = new TGraphErrors("Measurement_data.txt", "%lg %lg %lg %lg");

    TCanvas* d = new TCanvas;

    ge2->GetXaxis()->SetRangeUser(0.08, 2.1);
    d->cd(1)->SetLogy();
    d->cd(1)->SetLogx();
    ge2->SetLineWidth(0);
    ge2->SetMarkerColor(kGreen);
    ge2->SetMarkerStyle(8);
    ge2->SetMarkerColor(kTeal+3);
    ge2->SetMarkerSize(0.7);
    ge2->SetTitle("Efficiency Curve at 3cm with full TCS-Correction; Energy [MeV]; Efficiency");
    ge2->Draw("AP");

    // legend
    TLegend* leg = new TLegend(0.7, 0.7, 0.85, 0.8);


    auto fa1 = new TF1("fa1","0",0,2);
    TF1* f1 = new TF1("f1", "TMath::Power([1]*x, [2])", 0.1, 2);
    f1->SetParameter(1, 0.54);
    //f1->SetParLimits(1, 0.3, 0.8);

    f1->SetParameter(0, 3.7);
    //f1->SetParLimits(0, 2, 5);



    ge2->Fit("f1", "IRB");

    f1->SetLineColor(kRed);

    fa1->SetLineColor(kRed);
    fa1->Draw("samePL");

    leg->AddEntry(ge2, "measured points", "P");
    leg->AddEntry(f1, "fitted curve", "L");
    leg->Draw();

    //d->Print("EfficiencyCurve.pdf");

    TString outFileName = "Efficiency-Curves.root";
    TFile* outFile = TFile::Open(outFileName.Data(), "UPDATE");
    TString graphName = "Efficiency";
    ge2->SetName(graphName.Data());
    outFile->cd();
    ge2->Write(graphName.Data(), TObject::kOverwrite);
    outFile->Save();
    outFile->Close();
}
