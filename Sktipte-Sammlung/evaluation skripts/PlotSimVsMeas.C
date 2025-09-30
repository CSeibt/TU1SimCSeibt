
//#include "TMath.h"

void PlotSimVsMeas()
{
    TString inFileName1 = "SimulatedEfficiency.root";
    TFile* inFile1 = TFile::Open(inFileName1.Data(), "READ");
    TGraphErrors* ge1 = (TGraphErrors*) inFile1->Get("simulatedEfficiency");

    Double_t *ey = ge1->GetEY();
    Double_t *x_vals = ge1->GetX();
    TGraph *uncertainty = new TGraph(ge1->GetN(), x_vals, ey);

    TGraphErrors* ge2 = new TGraphErrors("Measurement_data.txt", "%lg %lg %lg %lg");
    ge2->SetMarkerStyle(23);
    ge2->SetMarkerSize(1);
    ge1->SetLineWidth(2);

    // leeres Residuen-Graphen erstellen
    TGraphErrors *res = new TGraphErrors();
    res->SetName("Residuum");

    // schleife über punkte des ersten graphen (simulation)

        for (int j=0; j < ge2->GetN(); j++){

            //Double_t x_1 = ge1->GetPointX(i);
            Double_t x_2 = ge2->GetPointX(j);

            Double_t y_1 = ge1->Eval(x_2);
            Double_t y_2 = ge2->GetPointY(j);

            Double_t y_1_fehler = uncertainty->Eval(x_2);
            Double_t y_2_fehler = ge2->GetErrorY(j);

            // if (abs(x_1-x_2)<0.5){

                // berechnen des Residuums
                Double_t residuum = (y_1-y_2)/y_2;

                Double_t res_fehler = sqrt(pow(1/y_2 * y_1_fehler, 2) + pow(-y_1/pow(y_2, 2) * y_2_fehler, 2));

                // einfügen in Plot
                cout << x_2 << "\t" << residuum << "\t" << y_1 << "\t" << y_2 << endl;
                res->AddPoint(x_2, residuum);
                res->SetPointError(j, 0, res_fehler);

            //}
        }

    //}


    TCanvas* d = new TCanvas;
    d->Divide(1, 2);

    // plotte effizient
    d->cd(1);
    TMultiGraph *mg = new TMultiGraph();
    mg->Add(ge1);
    mg->Add(ge2);
    mg->SetTitle("; Energy [MeV]; #varepsilon(E_{#gamma})");

    mg->GetXaxis()->SetRangeUser(0, 2);
    mg->GetYaxis()->SetTitleSize(0.08);
    mg->GetYaxis()->SetTitleOffset(0.4);
    mg->GetXaxis()->SetTitleSize(0.05);
    mg->GetXaxis()->SetTitleOffset(0.8);

    d->cd(1)->SetLogy();
    mg->GetXaxis()->SetLimits(0., 2.);
    ge2->SetLineColor(kRed);
    ge2->SetMarkerColor(kRed);
    ge2->SetMarkerSize(0.5);
    ge1->SetLineColor(kBlue);
    ge1->SetMarkerColor(kBlue);
    ge1->SetMarkerSize(05);
    mg->Draw("AP");

    // legend
    TLegend* leg = new TLegend(0.7, 0.7, 0.85, 0.8);
    leg->AddEntry(ge1, "Simulated", "PE");
    leg->AddEntry(ge2, "Measured", "PE");
    leg->Draw();

    // plotte Residuum
    d->cd(2);
    //ge2->Draw("AP");
    auto fa1 = new TF1("fa1","0",0,2);
    res->SetMarkerStyle(21);
    res->SetMarkerSize(0.5);
    //res->GetXaxis()->SetRangeUser(0, 2);
    res->SetTitle("; Energy [MeV]; Residual");
    res->GetXaxis()->SetTitleSize(0.05);
    res->GetXaxis()->SetTitleOffset(0.8);
    res->GetYaxis()->SetTitleSize(0.05);
    res->GetYaxis()->SetTitleOffset(0.8);

    res->GetXaxis()->SetLimits(0., 2.);
    res->Draw("AP");

    fa1->SetLineColor(kRed);
    fa1->Draw("samePL");

    //d->Print("Residuumplot.pdf");

    TString outFileName = "ResidualPlot.root";
    TFile* outFile = TFile::Open(outFileName.Data(), "UPDATE");
    TString graphName = "ResidualPlot";
    res->SetName(graphName.Data());
    outFile->cd();
    res->Write(graphName.Data(), TObject::kOverwrite);
    outFile->Save();
    outFile->Close();
}
