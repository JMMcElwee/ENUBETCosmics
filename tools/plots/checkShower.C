/* ================= CHECKSHOWER ================= * 
 * Created: 05-04-2024                             * 
 * Author:  Jordan McElwee                         * 
 * Email: mcelwee@lp2ib.in2p3.fr                   * 
 *                                                 * 
 * Macro for plotting important shower             * 
 * distributions. This will check the events have  * 
 * been populated correctly.                       * 
 *                                                 * 
 * Changelog:                                      * 
 * =============================================== */


void checkShower(const char *corsikaFile){

  // Read in the Corsika File 
  TFile *corsInput = TFile::Open(corsikaFile);
  TTree *shower = nullptr;
  corsInput->GetObject("shower",shower);

  TCanvas c1("","");
  c1.cd();
  gStyle->SetOptStat(0);
  c1.SetRightMargin(0.04);
  c1.SetTopMargin(0.04);

  // Shower energy profile 
  TH1D *energyHist = new TH1D("energyHist",";Energy [GeV]; n_{hits}",40,0,500);
  c1.SetLogy();
  shower->Draw("E>>energyHist");
  energyHist->SetLineColor(kBlue);
  c1.SaveAs("shower_sum.pdf(");

  // Shower energy profile scaled to energy
  TH1D *energyScale = new TH1D("energyScale",";Energy [GeV]; n_{hits}*E^{2.7}",40,0,500);
  c1.SetLogy(0);
  shower->Draw("E>>energyScale", "pow(E,2.7)");
  energyScale->SetLineColor(kRed);
  c1.SaveAs("shower_sum.pdf");

  // Shower angular profile
  c1.SetLeftMargin(0.12);
  TH1D *thetaHist = new TH1D("thetaHist",";#theta [rad];n_{hits}", 100, 0, 1.25);
  shower->Draw("theta>>thetaHist");
  thetaHist->SetLineColor(kBlue);
  c1.SaveAs("shower_sum.pdf");

  // Shower angular profile (cos*cos)
  TH1D *cosTheta = new TH1D("cosTheta",";cos#theta * cos#theta; n_{hits}", 100, 0, 1);
  shower->Draw("cos(theta)*cos(theta)>>cosTheta");
  cosTheta->SetLineColor(kRed);
  c1.SaveAs("shower_sum.pdf)");
  
  exit(0);
}
