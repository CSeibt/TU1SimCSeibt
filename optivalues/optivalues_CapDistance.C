#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <TCanvas.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TString.h>
#include <TStyle.h>

using namespace std;

void optivalues_CapDistance() {


  gStyle->SetOptFit();

  // --- Datensatz-Beschreibung ---
  struct Dataset {
    TString label; // z.B. "14cm"
    vector<TString> files;
  };


   vector<Dataset> datasets = {
    {"14cm", {
      "CapDistance_allenergies/absVollenergieansprechvermögen_14cm_exp.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_14cm_sim_5.2.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_14cm_sim_5.7.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_14cm_sim_6.0.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_14cm_sim_6.5.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_14cm_sim_7.0.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_14cm_sim_7.5.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_14cm_sim_8.0.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_14cm_sim_8.5.txt"
      
    }},
    {"0cm", {
      "CapDistance_allenergies/absVollenergieansprechvermögen_0cm_exp.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_0cm_sim_5.2.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_0cm_sim_5.7.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_0cm_sim_6.0.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_0cm_sim_6.5.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_0cm_sim_7.0.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_0cm_sim_7.5.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_0cm_sim_8.0.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_0cm_sim_8.5.txt",
    }}
  }; // */
  
   /*  vector<Dataset> datasets = {
    {"14cm", {
      "CapDistance_allenergies/absVollenergieansprechvermögen_14cm_exp_inklAm.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_14cm_sim_7.73.txt"
      
    }},
    {"0cm", {
      "CapDistance_allenergies/absVollenergieansprechvermögen_0cm_exp_inklAm.txt",
      "CapDistance_allenergies/absVollenergieansprechvermögen_0cm_sim_7.73.txt"

    }}
  }; // */
  
 /* vector<Dataset> datasets = {
    {"14cm", {
      "CapDistance_highenergies/absVollenergieansprechvermögen_14cm_exp.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_14cm_sim_5.2.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_14cm_sim_5.7.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_14cm_sim_6.0.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_14cm_sim_6.5.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_14cm_sim_7.0.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_14cm_sim_7.5.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_14cm_sim_8.0.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_14cm_sim_8.5.txt"
    }},
    {"0cm", {
      "CapDistance_highenergies/absVollenergieansprechvermögen_0cm_exp.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_0cm_sim_5.2.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_0cm_sim_5.7.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_0cm_sim_6.5.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_0cm_sim_6.0.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_0cm_sim_7.0.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_0cm_sim_7.5.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_0cm_sim_8.0.txt",
      "CapDistance_highenergies/absVollenergieansprechvermögen_0cm_sim_8.5.txt",
    }}
  }; // */

  const int colors[] = { kBlack, kRed, kBlue, kGreen+2, kMagenta+2, kOrange+7,
    kCyan+1, kSpring+5, kViolet+5, kAzure+1, kPink+7};

  // Speicher für abschließenden Fitvergleich
  vector<double> fit_x_14cm, fit_y_14cm, fit_err_14cm;
  vector<double> fit_x_0cm, fit_y_0cm, fit_err_0cm;

  for (const auto& ds : datasets) {
    const int num_infile = ds.files.size();

    std::vector<std::vector<double>> vec_eff(num_infile), vec_eff_stat(num_infile), vec_eff_sys(num_infile), vec_eff_err_comb(num_infile), vec_eff_weight(num_infile);
    std::vector<std::vector<double>> vec_energy(num_infile), vec_energy_err(num_infile);

    for (int i = 0; i < num_infile; ++i) {
        std::ifstream file(ds.files[i]);
        if (!file.is_open()) {
            std::cerr << "Fehler beim Öffnen von Datei: " << ds.files[i] << std::endl;
            return;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line.find_first_not_of(" \t\r\n") == std::string::npos) continue; // Skip empty lines

            std::istringstream iss(line);
            double eff, eff_stat, eff_sys, en, en_err;
            if (!(iss >> eff >> eff_stat >> eff_sys >> en >> en_err)) {
                std::cerr << "Warnung: Ungültige Zeile in Datei " << ds.files[i] << ": " << line << std::endl;
                continue;
            }

            vec_eff[i].push_back(eff);
            vec_eff_stat[i].push_back(eff_stat);
            vec_eff_sys[i].push_back(eff_sys);
            vec_energy[i].push_back(en);
            vec_energy_err[i].push_back(en_err);

            double err_comb = std::sqrt(eff_stat * eff_stat + eff_sys * eff_sys);
            vec_eff_err_comb[i].push_back(err_comb);
        }
        file.close();
    }
    
    
    // --- Hauptplot ---
    TString cname = "Efficiency_" + ds.label;
    TCanvas* c1 = new TCanvas(cname, cname, 1300, 600);
    gPad->SetGrid();
    gPad->SetLogy();

    TMultiGraph* mg = new TMultiGraph();
    TLegend* leg = new TLegend(0.74, 0.6, 0.9, 0.9);
    leg->SetTextSize(0.03);
   

    for (int i = 0; i < num_infile; ++i) {
      TGraphErrors* g = new TGraphErrors(vec_energy[i].size(),
                                         &vec_energy[i][0],
                                         &vec_eff[i][0],
                                         &vec_energy_err[i][0],
                                         &vec_eff_err_comb[i][0]);
      if(i==0)
      {
      g->SetMarkerStyle(21);
      g->SetMarkerSize(1.5);
      }
      else{
      g->SetMarkerStyle(20);
      g->SetMarkerSize(1.0);
      }
      g->SetMarkerColor(colors[i]);
      g->SetLineColor(colors[i]);
      mg->Add(g, "P");

      if (i == 0)
        leg->AddEntry(g, "Experiment", "lep");
      else {
        TString label = Form("Simulation %.1f mm",
            std::stof(ds.files[i](ds.files[i].Last('_') + 1, ds.files[i].Last('.') - ds.files[i].Last('_') - 1).Data()));
        leg->AddEntry(g, label, "lep");
      }
    }

    mg->SetTitle("Abs. full energy peak efficiency " + ds.label + ";Energy [keV];#epsilon [%]");
    //mg->GetYaxis()->SetRangeUser(4,36);
    mg->GetYaxis()->SetTitleOffset(0.8);
    mg->GetXaxis()->SetTitleOffset(1.2);
    mg->GetYaxis()->SetTitleSize(0.04);
    mg->GetXaxis()->SetTitleSize(0.04);
    mg->GetXaxis()->SetLabelSize(0.04);
    mg->GetYaxis()->SetLabelSize(0.04);
    mg->GetYaxis()->SetNdivisions(5);
    mg->GetXaxis()->SetNdivisions(5);
    mg->GetXaxis()->CenterTitle();
    mg->GetYaxis()->CenterTitle();
    mg->GetXaxis()->SetLabelOffset(0.007);
    mg->GetYaxis()->SetLabelOffset(0.006);
    mg->Draw("A");
    leg->Draw();

    // --- Residuals & Fitplot ---
    for (int i = 1; i < num_infile; ++i) {
      TString rname = "Residual_" + ds.label + Form("_%d", i);
      TCanvas* cres = new TCanvas(rname, rname, 1200, 300);
      cres->cd();
      gPad->SetGrid();

      TGraphErrors* gres = new TGraphErrors();
      for (size_t j = 0; j < vec_eff[0].size(); ++j) {
		  int n_points = vec_eff[0].size();  // Anzahl Datenpunkte
		std::vector<std::vector<double>> vec_eff_weight(n_points);
		std::vector<double> sum_weights(n_points, 0.0); 
      
        double A = vec_eff[0][j];
        double B = vec_eff[i][j];
        double sigma_A = vec_eff_err_comb[0][j];
        double sigma_B = vec_eff_err_comb[i][j];
        
        double resid = (A - B) / A * 100.0;
        
        double err = 100.0 * sqrt( pow(sigma_B / A, 2) + pow((B * sigma_A) / (A * A), 2) );
  
      
       

        //double resid = (vec_eff[0][j] - vec_eff[i][j]) / vec_eff[0][j] * 100.0;
       // double err = vec_eff_err_comb[0][j] / vec_eff[0][j] * 100.0;
        gres->SetPoint(j, vec_energy[0][j], resid);
        gres->SetPointError(j, 0.0, err);
        
       double weight = 1.0 / (err * err);
       vec_eff_weight[j].push_back(weight); 
       
       // cout<<vec_energy[0][j]<<" A "<<A <<" sigma_A "<<sigma_A <<" B "<<B <<" sigma_B "<<sigma_B <<" err "<<err << " w= "<< weight<<endl;
       cout<<"vec_energy[0][j] "<<vec_energy[0][j]<<" vec_eff[0][j] "<<vec_eff[0][j]<<" vec_eff_err_comb[0][j] "<<vec_eff_err_comb[0][j]<< " i 0  j "<<j << endl;
       cout<<"vec_energy[i][j] "<<vec_energy[i][j]<<" vec_eff[i][j] "<<vec_eff[i][j]<<" vec_eff_err_comb[i][j] "<<vec_eff_err_comb[i][j]<< " i "<<i <<" j "<<j << endl;
       cout<<"weight "<<weight<<endl;
      
      }

     
	  /*for (int f = 0; f < num_infile; ++f) {
		 double fehler_gewMittel = std::sqrt(1.0 / sum_weights[f]);
		 std::cout << "Datei " << f << ": " << fehler_gewMittel << std::endl;
	  }*/
	
      gres->SetMarkerStyle(20);
      gres->SetMarkerSize(1.1);
      gres->SetMarkerColor(colors[i]);
      gres->Draw("AP");

	  //gres->GetXaxis()->SetRangeUser(5,9);
      gres->GetXaxis()->SetTitle("Energie [keV]");
      gres->GetYaxis()->SetTitle("Residual [%]");
      gres->GetXaxis()->SetLimits(0, 2000);
      gres->GetYaxis()->CenterTitle();
      gres->GetXaxis()->CenterTitle();

      TF1* fconst = new TF1("fconst", "[0]", 0, 2000);
      fconst->SetLineColor(colors[i]);
      gres->Fit(fconst, "R");
      fconst->Draw("same");

      // --- Fitwert & Sim-Parameter extrahieren ---
      double fit_val = fconst->GetParameter(0);
      double fit_err = fconst->GetParError(0);
      TString simstr = ds.files[i](ds.files[i].Last('_') + 1, ds.files[i].Last('.') - ds.files[i].Last('_') - 1);
      double simval = atof(simstr.Data());

      if (ds.label == "14cm") {
        fit_x_14cm.push_back(simval);
        fit_y_14cm.push_back(fit_val);
        fit_err_14cm.push_back(fit_err);
      } else {
        fit_x_0cm.push_back(simval);
        fit_y_0cm.push_back(fit_val);
        fit_err_0cm.push_back(fit_err);
      }
    }
    
    
  }

  // --- Gesamtdarstellung Fitkonstanten ---
  TCanvas* cfit = new TCanvas("FitKonstantenGesamt", "Konstanter Fit vs. Simulationsabstand", 800, 500);
  gPad->SetGrid();

int n14 = fit_x_14cm.size();
std::vector<double> xerr14(n14, 0.1); // Vektor mit 0.1 für alle X-Fehler


  TGraphErrors* g14 = new TGraphErrors(fit_x_14cm.size(), &fit_x_14cm[0], &fit_y_14cm[0], nullptr , &fit_err_14cm[0]);
  TGraphErrors* g0 = new TGraphErrors(fit_x_0cm.size(), &fit_x_0cm[0], &fit_y_0cm[0], nullptr, &fit_err_0cm[0]);

  g14->SetMarkerStyle(21); g14->SetMarkerColor(kBlue+1); g14->SetLineColor(kBlue+1);
  g0->SetMarkerStyle(22); g0->SetMarkerColor(kRed+1); g0->SetLineColor(kRed+1);

  g0->SetTitle("Variation of Cap Distance;Cap Distance [mm];Residual [%]");
  g0->Draw("AP");
  g14->Draw("P SAME");

  TLegend* leg = new TLegend(0.12, 0.75, 0.35, 0.88);
  leg->AddEntry(g14, "14 cm", "lep");
  leg->AddEntry(g0, "0 cm", "lep");
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->Draw();
  
  //gStyle->SetOptStat(0);  // Statistikbox (Counts, Mean, RMS usw.) ausschalten
 // gStyle->SetOptFit(0);

  TF1* fit14 = new TF1("fit14", "[0]+[1]*x", 0, 10);
  TF1* fit0 = new TF1("fit0", "[0]+[1]*x", 0, 10);
  g14->Fit(fit14, "R");
  g0->Fit(fit0, "R");
  fit14->SetLineColor(kBlue+1); fit0->SetLineColor(kRed+1);
  fit14->Draw("same");
  fit0->Draw("same");
  
	  // Berechne x0 = -b/a
	double a14 = fit14->GetParameter(1);
	double b14 = fit14->GetParameter(0);
	double x0_14 = -b14 / a14;

	double a0 = fit0->GetParameter(1);
	double b0 = fit0->GetParameter(0);
	double x0_0 = -b0 / a0;

	cout << "x0 (14 cm): " << x0_14 << " mm" << endl;
	cout << "x0 (0 cm): " << x0_0 << " mm" << endl;
	
  TCanvas* cfit_fehler = new TCanvas("FitKonstantenGesamt fehler", "Konstanter Fit vs. Simulationsabstand", 800, 500);
  gPad->SetGrid();


  g14->SetMarkerStyle(21); g14->SetMarkerColor(kBlue+1); g14->SetLineColor(kBlue+1);
  g0->SetMarkerStyle(22); g0->SetMarkerColor(kRed+1); g0->SetLineColor(kRed+1);

  g14->SetTitle("Fitkonstante vs. Simulationsabstand;Simulationswert [mm];Fitkonstante [%]");
  g0->Draw("AP");
  g14->Draw("P SAME");
 // g14->Draw("AP");
 // g0->Draw("P SAME");

  TLegend* leg2 = new TLegend(0.12, 0.75, 0.35, 0.88);
  leg2->AddEntry(g14, "14 cm", "lep");
  leg2->AddEntry(g0, "0 cm", "lep");
  leg2->SetBorderSize(0);
  leg2->SetFillStyle(0);
  leg2->Draw();

  TF1* fit14_fehler = new TF1("fit14", "[0]+[1]*(x-[2])", 0, 10);
  fit14_fehler->FixParameter(2, x0_14);
  
  TF1* fit0_fehler = new TF1("fit0", "[0]+[1]*(x-[2])", 0, 10);
  fit0_fehler->FixParameter(2, x0_0);
    
  g14->Fit(fit14_fehler, "R");
  g0->Fit(fit0_fehler, "R");
  fit14->SetLineColor(kBlue+1); fit0->SetLineColor(kRed+1);
  fit14_fehler->Draw("same");
  fit0_fehler->Draw("same");

  double offset0 = fit0_fehler->GetParError(0);
  double offset14 = fit14_fehler->GetParError(0);
  
  	cout << "offset (14 cm): " << offset14 <<  endl;
	cout << "offset (0 cm): " << offset0 <<  endl;
	
	//____________________________________________________________________
	
	  // --- Plot der x0-Werte mit Fehlerbalken (offsets) ---
  TCanvas* cx0 = new TCanvas("cx0", "x0 Vergleich", 600, 500);

  // X-Werte (0 = 0cm, 1 = 14cm)
  double x_vals[2] = {1, 2};
  double y_vals[2] = {x0_0, x0_14};
  double x_errs[2] = {0.0, 0.0};  // keine x-Fehler
  double y_errs[2] = {offset0, offset14};

  TGraphErrors* gx0 = new TGraphErrors(2, x_vals, y_vals, x_errs, y_errs);
  gx0->SetTitle("x_{0}-Werte aus linearem Fit;x-Kanal;x_{0} [mm]");
  gx0->SetMarkerStyle(20);
  gx0->SetMarkerSize(1.2);
  gx0->SetMarkerColor(kBlack);
  gx0->SetLineColor(kBlack);

  gx0->GetXaxis()->SetTitle("Setup");
  gx0->GetYaxis()->SetTitle("Cap Distance [mm]");
  gx0->GetYaxis()->SetTitleOffset(1.4);
  gx0->GetXaxis()->SetTitleOffset(1.2);
  gx0->GetYaxis()->SetTitleSize(0.04);
  gx0->GetXaxis()->SetTitleSize(0.04);
  gx0->GetXaxis()->SetLabelSize(0.04);
  gx0->GetYaxis()->SetLabelSize(0.04);
//  gx0->GetYaxis()->SetNdivisions(1);
  gx0->GetXaxis()->SetNdivisions(2);
  gx0->GetXaxis()->CenterTitle();
  gx0->GetYaxis()->CenterTitle();
  gx0->GetXaxis()->SetLabelOffset(0.007);
  gx0->GetYaxis()->SetLabelOffset(0.006);
  gx0->Draw("APE");

  TF1* fit_final = new TF1("fit_final", "[0]", 0, 10);
  gx0->Fit(fit_final, "R");
  fit_final->Draw("same");
	
}
