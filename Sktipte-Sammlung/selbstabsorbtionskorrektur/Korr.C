
//#include "TMath.h"

void Korr()
{
    TString inFileName1 = "point/SimulatedEfficiency-point.root";
    TFile* inFile1 = TFile::Open(inFileName1.Data(), "READ");
    TGraphErrors* ge1 = (TGraphErrors*) inFile1->Get("SimulatedEfficiency");

    TString inFileName2 = "volume/SimulatedEfficiency-volume.root";
    TFile* inFile2 = TFile::Open(inFileName2.Data(), "READ");
    TGraphErrors* ge2 = (TGraphErrors*) inFile2->Get("SimulatedEfficiency");

    // leeres Korrektur-Graphen erstellen
    TGraphErrors *korr = new TGraphErrors();
    korr->SetName("Korrekturfaktor");

        // schleife über punkte des 2. Graphen (experiment)
        for (int j=0; j < ge1->GetN(); j++){
            // check, ob punkte übereinstimmen

            //Double_t x_1 = ge1->GetPointX(i);
            Double_t x_2 = ge2->GetPointX(j);

            Double_t y_1 = ge1->GetPointY(j);
            Double_t y_2 = ge2->GetPointY(j);

            Double_t korrektur = y_2/y_1;

            // einfügen in Plot
            cout << x_2 << " \t" << korrektur << "\t" << endl;
            korr->AddPoint(x_2, korrektur);


        }


    // create canvas
    TCanvas* d = new TCanvas;
    d->Divide(1, 2);

    // plot efficiencies
    d->cd(1);
    TMultiGraph *mg = new TMultiGraph();
    mg->Add(ge1);
    mg->Add(ge2);
    mg->SetTitle("; Energy [MeV]; #varepsilon(E_{#gamma})");

    // axis formatting
    mg->GetXaxis()->SetRangeUser(0., 2.7);
    mg->GetXaxis()->SetLimits(0., 2.7);
    mg->GetXaxis()->SetTitleSize(0.05);
    mg->GetXaxis()->SetTitleOffset(0.8);

    mg->GetYaxis()->SetTitleSize(0.08);
    mg->GetYaxis()->SetTitleOffset(0.4);

    d->cd(1)->SetLogy();


    // point formatting
    ge2->SetLineColor(kRed);
    ge2->SetMarkerColor(kRed);
    ge2->SetMarkerSize(0.5);
    ge1->SetLineColor(kBlue);
    ge1->SetMarkerColor(kBlue);
    ge1->SetMarkerSize(0.5);
    mg->Draw("AP");

    // legend
    TLegend* leg = new TLegend(0.7, 0.7, 0.85, 0.8);
    leg->AddEntry(ge1, "5mm Fuellstand", "PE");
    leg->AddEntry(ge2, "3.5mm Fuellstand", "PE");
    leg->Draw();

    // plot Correction factors
    d->cd(2);
    korr->SetMarkerStyle(21);
    korr->SetMarkerSize(0.5);
    korr->SetTitle("; Energy [MeV]; K(E)");
    korr->GetXaxis()->SetTitleSize(0.05);
    korr->GetXaxis()->SetTitleOffset(0.8);
    korr->GetYaxis()->SetTitleSize(0.05);
    korr->GetYaxis()->SetTitleOffset(0.8);

    korr->GetXaxis()->SetLimits(0., 2.7);
    korr->Draw("AP");


    TString outFileName = "CorrectionFactors.root";
    TFile* outFile = TFile::Open(outFileName.Data(), "UPDATE");
    TString graphName = "CorrectionFactors";
    korr->SetName(graphName.Data());
    outFile->cd();
    korr->Write(graphName.Data(), TObject::kOverwrite);
    outFile->Save();
    outFile->Close();
}
