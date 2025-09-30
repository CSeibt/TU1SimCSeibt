#include "TROOT.h"
#include "TCanvas.h"
#include "TPad.h"
#include "TFile.h"
#include "TH1D.h"
#include "TF1.h"

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;


void Analyze_nuclide()
{
    const TString nuclides[] = {"57Co", "137Cs", "88Y", "65Zn", "60Co", "60Co", "88Y"};     // Nuclide Names for evaluation
    const double energies[] = {0.122, 0.661, 0.898, 1.116, 1.173, 1.332, 1.836}; // MeV     // Energies of Corresponding nulide to be evaluated
    const double probabilities[] = {0.856, 0.851, 0.937, 0.5004, 0.9985, 0.999826, 0.992};  // Emission Popabilities of corresponding gamma lines

    const TString plotName = "plot.pdf";

    TCanvas *c = new TCanvas;

    c->Print(plotName + "[", "pdf"); //open

    TGraphErrors *graph = new TGraphErrors;
    int iPoint = 0;


    for (const auto nuclide : nuclides)
    {
        const auto inFileName = TString::Format("run%s.root", nuclide.Data());      // Open root files
        TFile* inFile = TFile::Open(inFileName, "READ");
        if (inFile->IsZombie())
        {
            cerr << "Failed to open input file." << endl;
            return;
        }

        TH1D *hAll = static_cast<TH1D*>(inFile->Get("EdepAll"));
        TH1D *hVis = static_cast<TH1D*>(inFile->Get("Evis"));

        const double nSimulated = hAll->GetEntries();

        const double binWidth = hAll->GetBinWidth(1);

        TF1* ff = new TF1("fitFunction", "gausn + pol1(3)", energies[iPoint]-0.006, energies[iPoint]+0.006);
        ff->SetParameter(0, 1e-2*nSimulated*binWidth);
        ff->SetParLimits(0, 0, nSimulated*binWidth);

        ff->SetParameter(1, energies[iPoint]);
        ff->SetParLimits(1, energies[iPoint]-0.002, energies[iPoint]+0.002);

        ff->SetParameter(2, 0.001);
        ff->SetParLimits(2, 0, 0.003);

        ff->SetParameter(3, 0.000);
        ff->SetParameter(4, 0.000);

        hVis->Draw();
        hVis->GetXaxis()->SetRangeUser(energies[iPoint]-0.007, energies[iPoint]+0.007);

        hVis->Fit("fitFunction", "IRB");

        const double nFEP = hVis->GetFunction("fitFunction")->GetParameter(0) / binWidth;
        const double FEP = nFEP / nSimulated;
        const double Efficiency = (nFEP) / nSimulated / probabilities[iPoint];

        graph->SetPoint(iPoint, energies[iPoint], Efficiency);
        graph->SetPointError(iPoint, 0, Efficiency * 1.0/sqrt(nFEP));


        iPoint += 1;
    }

    c->Print(plotName + "]", "pdf"); // close

    graph->SetTitle("; Gamma Energy (MeV); Full Energy Peak Efficiency (for single gamma)");
    graph->SetLineColor(kBlue);
    graph->SetMarkerColor(kBlue);
    graph->Draw("AP");
    c->SetLogx(1);
    c->SetLogy(1);
    c->Print("efficiency.pdf");

    TString outFileName = "SimulatedEfficiency.root";
    TFile* outFile = TFile::Open(outFileName.Data(), "UPDATE");
    TString graphName = "SimulatedEfficiency";
    graph->SetName(graphName.Data());
    outFile->cd();
    graph->Write(graphName.Data(), TObject::kOverwrite);
    outFile->Save();
    outFile->Close();

}
