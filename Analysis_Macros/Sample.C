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

/////////////////////////////////// Track Length /////////////////////////////////////////
TH1D *hTrackLength = new TH1D("hTrackLength", "Track Length", 100, 0, 100);


////////////////////////////////// Track dQ/dX Induction //////////////////////////////////
TH1D *hTrackdQdXInduction = new TH1D("hTrackdQdXInduction", "dQ/dX Induction", 1250, 0, 2500);

////////////////////////////////// Track dQ/dX Collection //////////////////////////////////
TH1D *hTrackdQdXCollection = new TH1D("hTrackdQdXCollection", "dQ/dX Collection", 1250, 0, 2500);

///////////////////////////////// Hit Charge vs Wire Induction //////////////////////////////
TH2D *hHitChargeVSWireInduction = new TH2D("hHitChargeVSWireInduction", "Hit Charge vs Wire Number(Induction)", 240, 0, 240, 1250, 0, 2500);

///////////////////////////////// Hit Charge vs Wire Collection //////////////////////////////
TH2D *hHitChargeVSWireCollection = new TH2D("hHitChargeVSWireCollection","Hit Charge vs Wire Number (Collection)", 240, 0, 240, 1250, 0, 2500);


void Sample::Loop()
{
if (fChain == 0) return;
Long64_t nentries = fChain->GetEntriesFast();

// ====================================
//           Global Variables
// ====================================
int nTotalEvents = 0;

Long64_t nbytes = 0, nb = 0;


// =====================================
// 	   Variables for cuts
// =====================================

float MinimumTrackLength = 10.0;

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
   
   
   // ### Filling basic track variables ###
   hTrackLength->Fill(trklength[nTPCtrk]);
   
   // ###############################################################
   // ### Looping over the trajectory points for the prelim-track ###
   // ###############################################################
   for(int ntrjpts = 0; ntrjpts < nTrajPoint[nTPCtrk]; ntrjpts++)
      {
      
      hTrackXZ->Fill(trjPt_Z[nTPCtrk][ntrjpts], trjPt_X[nTPCtrk][ntrjpts]);
      hTrackYZ->Fill(trjPt_Z[nTPCtrk][ntrjpts], trjPt_Y[nTPCtrk][ntrjpts]);
      
      }//<----End ntrjpts

   // #######################################################
   // ### Looping over points associated with calorimetry ###
   // #######################################################
   for(int nspts = 0; nspts < ntrkhits[nTPCtrk]; nspts++)
      {
      // The dQ/dX variable and dE/dX variableis defined with three numbers
      // 1) Track Index
      // 2) Plane Index: 0 = Induction, 1 = Collection
      // 3) Spacepoint Index
      // e.g. trkdqdx[nTPCtrk][0][nspts] for Induction plane hits 
      // and  trkdqdx[nTPCtrk][1][nspts] for Collection plane hits
      hTrackdQdXInduction->Fill(trkdqdx[nTPCtrk][0][nspts]);
      hTrackdQdXCollection->Fill(trkdqdx[nTPCtrk][1][nspts]);
      
     
      
      }//<---End nspts
   
   
   // ################################################
   // ### Only looking at hits on wires for events ###
   // ###     above a minimum track length         ###
   // ################################################
   if(trklength[nTPCtrk] > MinimumTrackLength)
      {
      
      // #######################################
      // ### Loop over all hits in the event ###
      // #######################################
      for(int ihit = 0; ihit < nhits; ihit++)
         {
	 
	 // ### Induction hits are on plane 0 ###
	 if(hit_plane[ihit] == 0)
	    {
	    hHitChargeVSWireInduction->Fill(hit_wire[ihit], hit_charge[ihit]);
	    
	    }//<---end induction hits
	 
	 // ### Collection hits are on plane 1 ###
	 if(hit_plane[ihit] == 1)
	    {
	    hHitChargeVSWireCollection->Fill(hit_wire[ihit], hit_charge[ihit]);
	    
	    }//<---end induction hits
	 
	 }//<---end ihit
      
      
      }//<---end looking at events with long enought tracks
   
   
   }//<----end nTPCtrk


}//<----End jentry loop


// ====================================================
// ======  Make histogram file for data sample  ======= 
TFile myfile("./Histos.root","RECREATE");
hTrackXZ->Write();
hTrackYZ->Write();
hTrackdQdXInduction->Write();
hTrackdQdXCollection->Write();
hTrackLength->Write();
hHitChargeVSWireCollection->Write();
hHitChargeVSWireInduction->Write();


}
