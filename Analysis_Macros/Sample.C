#define Sample_cxx
#include "Sample.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <fstream>
//#include <TVector3.h>

/////////////////////////////////// Track X Z Position ///////////////////////////////////
TH2D *hTrackXZ = new TH2D("hTrackXZ", "Track X-Z", 200, -5, 95, 320, -5, 155);

/////////////////////////////////// Track Y Z Position ///////////////////////////////////
TH2D *hTrackYZ = new TH2D("hTrackYZ", "Track Y-Z", 200, -5, 95, 120, -30, 30);

void Sample::Loop()
{
if (fChain == 0) return;
Long64_t nentries = fChain->GetEntriesFast();

// ====================================
//           Global Variables
// ====================================
int nTotalEvents = 0;

Long64_t nbytes = 0, nb = 0;

// ############################
// ### Loop over all events ###
// ############################

for (Long64_t jentry=0; jentry<nentries;jentry++) 
{
Long64_t ientry = LoadTree(jentry);
if (ientry < 0) break;
nb = fChain->GetEntry(jentry);   nbytes += nb;
// #############################
// ### Counting Total Events ###
// #############################
nTotalEvents++;
   
// === Outputting every nEvents to the screen ===
if(nTotalEvents % 500 == 0){std::cout<<"Event = "<<nTotalEvents<<std::endl;}


// ####################################
// ### Loop over all the TPC Tracks ###
// ####################################
for(int nTPCtrk = 0; nTPCtrk < ntracks_reco; nTPCtrk++)
   {
   
   // ###############################################################
   // ### Looping over the trajectory points for the prelim-track ###
   // ###############################################################
   for(int ntrjpts = 0; ntrjpts < nTrajPoint[nTPCtrk]; ntrjpts++)
      {
      
      hTrackXZ->Fill(trjPt_Z[nTPCtrk][ntrjpts], trjPt_X[nTPCtrk][ntrjpts]);
      hTrackYZ->Fill(trjPt_Z[nTPCtrk][ntrjpts], trjPt_Y[nTPCtrk][ntrjpts]);
      
      }//<----End ntrjpts

   
   
   
   
   
   }//<----end nTPCtrk


}//<----End jentry loop

hTrackXZ->Draw();
hTrackYZ->Draw();
// ====================================================
// ======  Make histogram file for data sample  ======= 
TFile myfile("./AerogelHistos.root","RECREATE");
hTrackXZ->Write();
hTrackYZ->Write();

}
