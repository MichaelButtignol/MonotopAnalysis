// remove the leadingJetEtaPhiPt in the Reader.h and the babyTuple

#include "common.h"
#include "functions.h"
#include <iomanip>

// Sonic screwdriver

#include "sonicScrewdriver/interface/SonicScrewdriver.h" 
#include "sonicScrewdriver/interface/tables/TableDataMC.h" 
using namespace theDoctor;


// Global pointer used for the region/channel selectors
babyEvent* myEventPointer;

// #########################################################################""
// #########################################################################""

typedef struct
{
        float leadingJetPt;
        float leadingJetEta;
        float leadingJetPhi;
        float subleadingJetPt;
        float sub2leadingJetPt;
        float sub3leadingJetPt;
        float leadingBJetPt;
        float leadingBJetPhi;
        float selectedLeptonEta;
        float selectedLeptonPhi;
        float selectedLeptonPt;
        float leadingLeptonPt;
        float leadingLeptonPhi;
        float leadingLeptonEta;
        int   nJets;
        int   nSmearedJets;
        int   nBJets;
        int   nBTag;
        float LeptonBJetCorrelation;
        float LeptonJetCorrelation;
        float JetMETCorrelation;
        float METMuonCorrelation;
        float MET;
        float MTw;
        float MTw_plus_MET;
        int   hasOtherJets;
        int   hasExactlyOneRealMuon;
        int   hasExactlyOneMuon;
        int   selectedIsLeading;
        double Wmass;
        float SFtopPtReweighting;
} 
OnTheFlyVariables;

void computeOnTheFlyVariables(OnTheFlyVariables* onTheFlyVariables, babyEvent* myEvent)
{
        onTheFlyVariables->hasExactlyOneRealMuon    = 0;
     if ( myEvent->nMuons == 1 && myEvent->nElectrons == 0 &&  myEvent->selectedLeptonPDGId != 0 &&  myEvent->selectedLeptonPDGId != -9999)
     {
        onTheFlyVariables->hasExactlyOneRealMuon    = 1;
        onTheFlyVariables->selectedLeptonEta        = abs(myEvent->selectedLepton.Eta());
        onTheFlyVariables->selectedLeptonPt         = myEvent->selectedLepton.Pt();
        onTheFlyVariables->selectedLeptonPhi        = myEvent->selectedLepton.Phi();
    }

     onTheFlyVariables->hasExactlyOneMuon = 0;
     if ( myEvent->nMuonsWoIso == 1 && myEvent->nElectronsWoIso == 0 &&  myEvent->leadingLeptonPDGId != 0 &&  myEvent->leadingLeptonPDGId != -9999)
     {
        onTheFlyVariables->leadingLeptonPt                      = myEvent->leadingLepton.Pt();
        onTheFlyVariables->leadingLeptonEta                     = myEvent->leadingLepton.Eta();
        onTheFlyVariables->hasExactlyOneMuon                    = 1;
        double mW = 80.399;
        double t1 = myEvent->leadingLepton.Pz()*(myEvent->leadingLepton.Px()*myEvent->PhiCorrectedMET_JER.Px() + myEvent->leadingLepton.Py()*myEvent->PhiCorrectedMET_JER.Py() + mW*mW/2. );
        double det = myEvent->leadingLepton.Px()*myEvent->PhiCorrectedMET_JER.Px() + myEvent->leadingLepton.Py()*myEvent->PhiCorrectedMET_JER.Py() + mW*mW/2. - myEvent->PhiCorrectedMET_JER.Pt()*myEvent->PhiCorrectedMET_JER.Pt()*(myEvent->leadingLepton.E()*myEvent->leadingLepton.E() - myEvent->leadingLepton.Pz()*myEvent->leadingLepton.Pz());
        if (det < 0) det = 0;
        double t2 = myEvent->leadingLepton.E()*pow(det, 0.5);
        double denom = myEvent->leadingLepton.E()*myEvent->leadingLepton.E() - myEvent->leadingLepton.Pz()*myEvent->leadingLepton.Pz();
        double neutrinoPz = (t1 - t2)/denom;
        double neutrinoE  = pow( pow(myEvent->PhiCorrectedMET_JER.Px(),2) + pow(myEvent->PhiCorrectedMET_JER.Py(),2) + neutrinoPz*neutrinoPz, 0.5);

        TLorentzVector neutrino;
        neutrino.SetPxPyPzE(myEvent->PhiCorrectedMET_JER.Px(), myEvent->PhiCorrectedMET_JER.Py(), neutrinoPz, neutrinoE);

        TLorentzVector theWcand = neutrino + myEvent->leadingLepton;
        onTheFlyVariables->Wmass = theWcand.M();
     }

     onTheFlyVariables->nJets             = (int)myEvent->nJets;
     onTheFlyVariables->nSmearedJets      = (int)myEvent->smearedJets.size();
     onTheFlyVariables->leadingJetPt      = 0;
     onTheFlyVariables->leadingJetPhi     = 0;
     onTheFlyVariables->leadingJetEta     = 0;
     onTheFlyVariables->subleadingJetPt   = 0;
     onTheFlyVariables->sub2leadingJetPt  = 0;
     onTheFlyVariables->sub3leadingJetPt  = 0;
     onTheFlyVariables->leadingBJetPt     = 0;
     onTheFlyVariables->leadingBJetPhi    = 0;
     onTheFlyVariables->nBTag             = 0;

     for (int i=0;i<onTheFlyVariables->nSmearedJets;i++)
     {
             if (myEvent->smearedJets[i].Pt() > onTheFlyVariables->leadingJetPt)
             {
                     onTheFlyVariables->leadingJetPt  = myEvent->smearedJets[i].Pt();
                     onTheFlyVariables->leadingJetPhi = myEvent->smearedJets[i].Phi();
                     onTheFlyVariables->leadingJetEta = abs(myEvent->smearedJets[i].Eta());
             }
             if (myEvent->smearedJets_CSV[i] > 0.679)
             {
                onTheFlyVariables->nBTag++;
                if(myEvent->smearedJets[i].Pt() > onTheFlyVariables->leadingBJetPt)
                {
                     onTheFlyVariables->leadingBJetPt  = myEvent->smearedJets[i].Pt();
                     onTheFlyVariables->leadingBJetPhi = myEvent->smearedJets[i].Phi();
//                     onTheFlyVariables->leadingBJetEta = abs(myEvent->smearedJets[i].Eta());
                }
             }

     }

     if (onTheFlyVariables->nSmearedJets > 1) onTheFlyVariables->subleadingJetPt   = myEvent->smearedJets[1].Pt();
     if (onTheFlyVariables->nSmearedJets > 2) onTheFlyVariables->sub2leadingJetPt  = myEvent->smearedJets[2].Pt();
     if (onTheFlyVariables->nSmearedJets > 3) onTheFlyVariables->sub3leadingJetPt  = myEvent->smearedJets[3].Pt();
     onTheFlyVariables->nBJets            = onTheFlyVariables->nBTag;
     onTheFlyVariables->hasOtherJets = 0;
     if(onTheFlyVariables->nSmearedJets > 1) 
     {
        if (myEvent->smearedJets[1].Pt() > 40) 
        {
             onTheFlyVariables->hasOtherJets  = 2;
        }   
     }

     if(onTheFlyVariables->nSmearedJets == 0) 
     {
        onTheFlyVariables->leadingJetPt = -10;
     }

     if(onTheFlyVariables->hasExactlyOneMuon == 1)
     {
         onTheFlyVariables->METMuonCorrelation  = myEvent->PhiCorrectedMET_JER.Phi() - onTheFlyVariables->leadingLeptonPhi;
         onTheFlyVariables->MTw_plus_MET        = myEvent->MET_JER.Pt() + sqrt( 2.* myEvent->leadingLepton.Pt() * myEvent->MET_JER.Pt() *(1. - cos( myEvent->leadingLepton.Phi() - myEvent->PhiCorrectedMET_JER.Phi()) ));
         onTheFlyVariables->MET = myEvent->PhiCorrectedMET_JER.Pt();
         onTheFlyVariables->MTw = sqrt( 2.* myEvent->leadingLepton.Pt() * myEvent->PhiCorrectedMET_JER.Pt() *(1. - cos( myEvent->leadingLepton.Phi() - myEvent->PhiCorrectedMET_JER.Phi()) ));
 
     }


     if(onTheFlyVariables->nSmearedJets > 0 && onTheFlyVariables->hasExactlyOneRealMuon == 1)
     {
         onTheFlyVariables->JetMETCorrelation                    = myEvent->PhiCorrectedMET.Phi() - onTheFlyVariables->leadingJetPhi;
         onTheFlyVariables->LeptonJetCorrelation                 = myEvent->leadingLepton.Phi() - onTheFlyVariables->leadingJetPhi;
     }

     if(onTheFlyVariables->nBJets > 0 &&  onTheFlyVariables->hasExactlyOneRealMuon == 1)
     {
         onTheFlyVariables->LeptonBJetCorrelation                = myEvent->leadingLepton.Phi() - onTheFlyVariables->leadingBJetPhi;
     }

     onTheFlyVariables->selectedIsLeading = 0;
     if(myEvent->leadingLepton.Pt() == myEvent->selectedLepton.Pt()) onTheFlyVariables->selectedIsLeading = 1;

     onTheFlyVariables->SFtopPtReweighting = 1;
     for ( unsigned int ind = 0; ind < myEvent->genParticles.size(); ind++)
     {
        if(myEvent->genParticlesPDGId[ind] == 6)
        {
           onTheFlyVariables->SFtopPtReweighting = exp(0.159 - 0.00141* myEvent->genParticles[ind].Pt());
           break; 
        }
     }
}



// #############################
// ##   Regions definitions   ##
// #############################

vector<Cut> preSelection
{
//    Cut("hasExactlyOneMuon", '=', 1),
//    Cut("selectedIsLeading", '=', 1),
    Cut("leadingLeptonIsolation", '<', 0.12),
    Cut("leadingLeptonPt",        '>', 30),
//    Cut("nSmearedJets", '>' , 3),
//    Cut("nBJets", '=', 2),
//    Cut("MET", '>', 60 )
};

vector<Cut> signalRegion
{
    Cut("nSmearedJets",           '>', 0),
    Cut("nBJets",                 '=', 1),
    Cut("MTw",                    '>', 100),
    Cut("selectedIsLeading",      '=', 1),
    Cut("MET",                    '>', 100),
    Cut("hasExactlyOneMuon",      '=', 1),
    Cut("leadingLeptonIsolation", '<', 0.12),
    Cut("leadingLeptonPt",        '>', 30),
    Cut("leadingJetPt",           '>', 70),
    Cut("selectedLeptonPt",       '>', 30),
    Cut("hasOtherJets",           '<', 1)
};

vector<Cut> wjetsControlRegion
{
//    Cut("hasExactlyOneMuon",       '=',  1),
    Cut("leadingLeptonIsolation",  '<', 0.12),
    Cut("MTw",                     '>', 40),
    Cut("MTw",                     '<', 150),
    Cut("Mw",                      '>', 76),
    Cut("Mw",                      '<', 86),
    Cut("nSmearedJets",            '>', 0 ),
    Cut("nSmearedJets",            '<', 2 ),
    Cut("nBJets",                   '=', 0 ),
    Cut("MET", '>', 60 ),
};

vector<Cut> ttbarControlRegion
{
    Cut("nSmearedJets",            '>', 3 ),
    Cut("nBJets",                  '=', 2 ),
//    Cut("MTw",                     '>', 40),
    Cut("leadingLeptonIsolation",  '<', 0.12),
//    Cut("hasOtherJets",            '>', 1  ),
//    Cut("MET", '>', 60 ),
};

vector<Cut> qcdControlRegion
{
    Cut("nSmearedJets",               '>', 3  ),
    Cut("nBJets",                     '=', 2  ),
//    Cut("hasExactlyOneMuon", '=', 1),
    Cut("leadingLeptonIsolation",     '>', 0.6)
};


vector<Cut> qcdCheckScaleRegion
{
    Cut("nSmearedJets",           '>',  3),
    Cut("nBJets",                  '=', 2),
    Cut("MTw",                     '>', 40),
//    Cut("hasExactlyOneMuon",       '=',  1),
    Cut("leadingLeptonIsolation",  '<', 0.12),
};


vector<Cut> qcdScaleRegion
{
    Cut("nSmearedJets",          '>', 3),
    Cut("nBJets",                '=',  2),
    Cut("MTw",                   '<', 40),
//    Cut("hasExactlyOneMuon",     '=',  1),
    Cut("leadingLeptonIsolation",'<', 0.12),
};

// Channels
bool muChannel();

int main (int argc, char *argv[])
{

  printBoxedMessage("Starting plot generation");

  // ####################
  // ##   Init tools   ##
  // ####################
 
     // Set directly the lumi
     // Float_t lumi = 876.225;   //Run A 
     // Float_t lumi = 4412.0;   //Run B 
     // Float_t lumi = 7031.0;   //Run C 
     // Float_t lumi = 7248.0;   //Run D 
     // Float_t lumi = 5288.225;   //Run A + Run C 
     // Float_t lumi = 12319.225;   //Run A + Run B + Run C 
      Float_t lumi = 19567.225;   //Run A + Run B + Run C + Run D

     // Create a sonic Screwdriver
      SonicScrewdriver myScrewdriver;

     // Create a container for the event
     babyEvent myEvent;
     OnTheFlyVariables onTheFlyVariables;
     myEventPointer = &myEvent;

  // ##########################
  // ##   Create Variables   ##
  // ##########################


      myScrewdriver.AddVariable("selectedLeptonPt", "p_{T} (lepton)","GeV",20,0,300,&(onTheFlyVariables.selectedLeptonPt),"");
      myScrewdriver.AddVariable("selectedLeptonEta","#eta (lepton)","",15,0,3,    &(onTheFlyVariables.selectedLeptonEta),"");
      myScrewdriver.AddVariable("selectedLeptonIsolation","Isolation (selLepton)", "",32,-0.005,0.155,  &(myEvent.selectedLeptonIsolation), "");
      myScrewdriver.AddVariable("leadingLeptonPt", "p_{T} (leading lepton)","GeV",20,0,300,&(onTheFlyVariables.leadingLeptonPt),"");
      myScrewdriver.AddVariable("leadingLeptonIsolation","Isolation (leadLepton)", "", 50,-0.01,0.99,  &(myEvent.leadingLeptonIsolation), "");

      myScrewdriver.AddVariable("leadingJetPt","p_{T} (leading jet)","GeV",30,0,300,  &(onTheFlyVariables.leadingJetPt),"noUnderflowInFirstBin noOverflowInLastBin");
      myScrewdriver.AddVariable("leadingJetEta","#eta (leading jet)", "", 15,0,3,  &(onTheFlyVariables.leadingJetEta), "noUnderflowInFirstBin");
      myScrewdriver.AddVariable("leadingBJetPt","p_{T} (leading Bjet)","GeV",30,0,300,  &(onTheFlyVariables.leadingBJetPt),"noUnderflowInFirstBin noOverflowInLastBin");
      myScrewdriver.AddVariable("subleadingJetPt","p_{T} (subleading jet)","GeV",30,0,300,  &(onTheFlyVariables.subleadingJetPt),"noUnderflowInFirstBin noOverflowInLastBin");
      myScrewdriver.AddVariable("sub2leadingJetPt","p_{T} (sub2leading jet)","GeV",30,0,300,  &(onTheFlyVariables.sub2leadingJetPt),"noUnderflowInFirstBin noOverflowInLastBin");
      myScrewdriver.AddVariable("sub3leadingJetPt","p_{T} (sub3leading jet)","GeV",30,0,300,  &(onTheFlyVariables.sub3leadingJetPt),"noUnderflowInFirstBin noOverflowInLastBin");
      myScrewdriver.AddVariable("nSmearedJets","numberOfjets", "", 10,-0.5,9.5,  &(onTheFlyVariables.nSmearedJets), "");
      myScrewdriver.AddVariable("nBJets","numberOfBjets", "", 10,-0.5,9.5,  &(onTheFlyVariables.nBJets), "");
      myScrewdriver.AddVariable("hasOtherJets","Other Jets in Event", "",2,0,2, &(onTheFlyVariables.hasOtherJets), "");
      myScrewdriver.AddVariable("hasExactlyOneRealMuon","Events with exactly one real muon", "",2,0,2, &(onTheFlyVariables.hasExactlyOneRealMuon), "");
      myScrewdriver.AddVariable("hasExactlyOneMuon","Events with exactly one muon", "",2,0,2, &(onTheFlyVariables.hasExactlyOneMuon), "");
      myScrewdriver.AddVariable("selectedIsLeading","Events with exactly one real muon", "",2,0,2, &(onTheFlyVariables.selectedIsLeading), "");

      myScrewdriver.AddVariable("LeptonJetCorrelation","#Delta #phi (Lepton - LeadingJet)", "",25,-6.28,6.28,  &(onTheFlyVariables.LeptonJetCorrelation), "");
      myScrewdriver.AddVariable("LeptonBJetCorrelation","#Delta #phi (Lepton - BJet)", "",25,-6.28,6.28,  &(onTheFlyVariables.LeptonBJetCorrelation), "");
      myScrewdriver.AddVariable("JetMETcorrelation","#Delta #phi (MET - leading jet)", "",25,-6.28,6.28,  &(onTheFlyVariables.JetMETCorrelation), "");
      myScrewdriver.AddVariable("METMuoncorrelation","#Delta #phi (MET - selected lepton)", "",25,-6.28,6.28,  &(onTheFlyVariables.METMuonCorrelation), "");

      myScrewdriver.AddVariable("Mw",           "M^{W}",     "GeV", 35,0  ,350,  &(onTheFlyVariables.Wmass), "");
      myScrewdriver.AddVariable("MTw",          "M_{T}^{W}", "GeV", 25,0  ,250,  &(onTheFlyVariables.MTw), "");
      myScrewdriver.AddVariable("MTw_queue",    "M_{T}^{W}", "GeV", 25,100,600,  &(onTheFlyVariables.MTw), "noOverflowInLastBin");

      myScrewdriver.AddVariable("MET",          "Missing E_{T}",    "GeV",    30,0  ,300,    &(onTheFlyVariables.MET),""     );
      myScrewdriver.AddVariable("MET_queue",    "Missing E_{T}",    "GeV",    20,100,500,    &(onTheFlyVariables.MET),""     );
      
      myScrewdriver.AddVariable("MTw_plus_MET",     "M_{T}^{W} + Missing E_{T}",    "GeV",    20,0,1000,    &(onTheFlyVariables.MTw_plus_MET) );

      myScrewdriver.AddVariable("numberOfPrimaryVertices","N_{vtx}", "", 25,0,50,  &(myEvent.numberOfPrimaryVertices), "");


  // #########################################################
  // ##   Create ProcessClasses (and associated datasets)   ##
  // #########################################################

  // Backgrounds

      myScrewdriver.AddProcessClass("QCD",     "QCD",     "background", COLORPLOT_RED);
      myScrewdriver.AddDataset("QCD_Pt-20to30_MuEnrichedPt5","QCD",myEvent.numberOfInitialEvents,1865500.0);
      myScrewdriver.AddDataset("QCD_Pt-30to50_MuEnrichedPt5","QCD",myEvent.numberOfInitialEvents,806298.0);
      myScrewdriver.AddDataset("QCD_Pt-50to80_MuEnrichedPt5","QCD",myEvent.numberOfInitialEvents,176187.6);
      myScrewdriver.AddDataset("QCD_Pt-80to120_MuEnrichedPt5","QCD",myEvent.numberOfInitialEvents,40448.0);
      myScrewdriver.AddDataset("QCD_Pt-120to170_MuEnrichedPt5","QCD",myEvent.numberOfInitialEvents,7463.94);
      myScrewdriver.AddDataset("QCD_Pt-170to300_MuEnrichedPt5","QCD",myEvent.numberOfInitialEvents,2299.752);
      myScrewdriver.AddDataset("QCD_Pt-300to470_MuEnrichedPt5","QCD",myEvent.numberOfInitialEvents,151.8048);

      myScrewdriver.AddProcessClass("TTbar",     "t#bar{t}",     "background", kBlue);
      myScrewdriver.AddDataset("NTuple_53X_TTJetsMadgraphZ2","TTbar",myEvent.numberOfInitialEvents,245.8);
//      myScrewdriver.AddDataset("NTuple_53X_TT_CT10_TuneZ2star_8TeV-powheg-tauola","TTbar",myEvent.numberOfInitialEvents,122.9);

      myScrewdriver.AddProcessClass("SingleTop",     "SingleTop",     "background", kOrange-3);
      myScrewdriver.AddDataset("NTuple_53X_T_s-channel","SingleTop",myEvent.numberOfInitialEvents,3.79);
      myScrewdriver.AddDataset("NTuple_53X_T_t-channel","SingleTop",myEvent.numberOfInitialEvents,56.4);
      myScrewdriver.AddDataset("NTuple_53X_T_tW-channel","SingleTop",myEvent.numberOfInitialEvents,11.1);
      myScrewdriver.AddDataset("NTuple_53X_Tbar_t-channel","SingleTop",myEvent.numberOfInitialEvents,30.7);
      myScrewdriver.AddDataset("NTuple_53X_Tbar_tW-channel","SingleTop",myEvent.numberOfInitialEvents,11.1);

      myScrewdriver.AddProcessClass("Others",     "Others",     "background", kGreen-3);
//      myScrewdriver.AddDataset("NTuple_53X_TTWJets_8TeVmadgraph","Others",myEvent.numberOfInitialEvents,0.232);
//      myScrewdriver.AddDataset("NTuple_53X_TTZJets_8TeVmadgraph_v2","Others",myEvent.numberOfInitialEvents,0.208);
//      myScrewdriver.AddDataset("NTuple_53X_TZJetsTo3LNuB","Others",myEvent.numberOfInitialEvents,0.0096);
      myScrewdriver.AddDataset("NTuple_53X_WWJetsIncl","Others",myEvent.numberOfInitialEvents,5.817);
      myScrewdriver.AddDataset("NTuple_53X_WZJetsIncl","Others",myEvent.numberOfInitialEvents,1.189);
      myScrewdriver.AddDataset("NTuple_53X_ZZJetsIncl","Others",myEvent.numberOfInitialEvents,0.213);

      myScrewdriver.AddProcessClass("Zjets",     "Drell Yan",     "background", kOrange+3);
      myScrewdriver.AddDataset("NTuple_53X_DYJetsToLL_M-50","Zjets",myEvent.numberOfInitialEvents,3531.);
      myScrewdriver.AddDataset("NTuple_53X_DYJetsToLL_M-10To50","Zjets",myEvent.numberOfInitialEvents,860.5);

      myScrewdriver.AddProcessClass("Wjets",     "W+jets",     "background", kGreen+2);
      myScrewdriver.AddDataset("NTuple_53X_WJetsToLNu","Wjets",myEvent.numberOfInitialEvents,36864.3); //36864.3);

      // Signal(s)

      myScrewdriver.AddProcessClass("S1",     "S1 resonant",               "signal",  COLORPLOT_BLUE);
      myScrewdriver.AddDataset("S1_1000_100","S1",myEvent.numberOfInitialEvents,0.24209);

      // Data

      myScrewdriver.AddProcessClass("data" ,   "Data",    "data",       COLORPLOT_BLACK);
      myScrewdriver.AddDataset("NTuple_53X_SingleMuRun2012A","data",-1,-1);
      myScrewdriver.AddDataset("NTuple_53X_SingleMuRun2012B","data",-1,-1);
      myScrewdriver.AddDataset("NTuple_53X_SingleMuRun2012C","data",-1,-1);
      myScrewdriver.AddDataset("NTuple_53X_SingleMuRun2012D","data",-1,-1);

      // ##########################
      // ##    Create Regions    ##
      // ##########################

      myScrewdriver.AddRegion("preSelection","Iso(#mu) < 0.12",preSelection, "showCuts");
      myScrewdriver.AddRegion("ttbarControlRegion"   ,"nBJets = 2",ttbarControlRegion, "showCuts");
      myScrewdriver.AddRegion("wjetsControlRegion"   ,"nJets = 0", wjetsControlRegion, "showCuts");
      myScrewdriver.AddRegion("qcdControlRegion"   ,"Iso(#mu) > 0.6 ; nBJets = 2 "   ,qcdControlRegion, "showCuts");
      myScrewdriver.AddRegion("qcdScaleRegion"   ,"Iso(#mu) < 0.12 ; M_{T}^{W} < 40 GeV ; nBJets = 2"   ,qcdScaleRegion, "showCuts");
      myScrewdriver.AddRegion("qcdCheckScaleRegion"   ,"Iso(#mu) < 0.12 ; M_{T}^{W} > 40 GeV ; nBJets = 2"   ,qcdCheckScaleRegion, "showCuts");
      myScrewdriver.AddRegion("signalRegion","Signal region",signalRegion);

      // ##########################
      // ##   Create Channels    ##
      // ##########################

      myScrewdriver.AddChannel("muChannel",      "#mu-channel",  &muChannel      );

  // ########################################
  // ##       Create histograms and        ##
  // ##  schedule type of plots to produce ##
  // ########################################
  
     // Create histograms
     myScrewdriver.Create1DHistos();
     
     //myScrewdriver.Add2DHisto("selectedLeptonEta","selectedLeptonPt");
     //myScrewdriver.Add2DHisto("leadingJetEta","leadingJetPt");

     // Set options

     myScrewdriver.SetGlobalBoolOption  ("1DSuperimposed",    "includeSignal",                    true   );
     
     myScrewdriver.SetGlobalStringOption("1DStack",           "includeSignal",                    "stack");
     myScrewdriver.SetGlobalFloatOption ("1DStack",           "factorSignal",                     10.0    );

     myScrewdriver.SetGlobalBoolOption("DataMCRatio",       "splitUncertainties",                     true);
     myScrewdriver.SetGlobalStringOption("DataMCComparison",  "includeSignal",                       "yes");
     myScrewdriver.SetGlobalStringOption("DataMCComparison",  "ratioPosition",                    "bottom");
     myScrewdriver.SetGlobalFloatOption ("DataMCComparison",  "factorSignal",                      1.0    );
     
     //myScrewdriver.SetGlobalFloatOption("FigureOfMerit",     "backgroundSystematicUncertainty", 0.15);

     // Schedule plots
     
     myScrewdriver.SchedulePlots("1DSuperimposed");
     myScrewdriver.SchedulePlots("1DStack");
     myScrewdriver.SchedulePlots("1DDataMCComparison");
     myScrewdriver.SchedulePlots("1DFigureOfMerit","var=MTw_JER,cutType=keepHighValues");
     myScrewdriver.SchedulePlots("1DFigureOfMerit","var=MET,cutType=keepHighValues");
     myScrewdriver.SchedulePlots("1DFigureOfMerit","var=leadingJetPt,cutType=keepHighValues");
     myScrewdriver.SchedulePlots("1DFigureOfMerit","var=leadingLeptonPt,cutType=keepHighValues");

     //myScrewdriver.SchedulePlots("2D");
     // Config plots

     myScrewdriver.SetGlobalStringOption("Plot", "infoTopRight", "        CMS Internal");
     myScrewdriver.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV, L = 19.6 fb^{-1}");
     
     myScrewdriver.SetGlobalBoolOption("Plot", "exportPdf", true);
     myScrewdriver.SetGlobalBoolOption("Plot", "exportEps", false);
     myScrewdriver.SetGlobalBoolOption("Plot", "exportPng", false);

  // ########################################
  // ##        Run over the events         ##
  // ########################################
  
     vector<string> datasetsList;
     myScrewdriver.GetDatasetList(&datasetsList);

     TGraphAsymmErrors* ratio_eta_0to0p9;
     TGraphAsymmErrors* ratio_eta_0p9to1p2;
     TGraphAsymmErrors* ratio_eta_1p2to2p1;
     TFile* ftrigger = new TFile(string("SingleMuonTriggerEfficiencies_eta2p1_Run2012ABCD_v5trees.root").c_str());
     ftrigger->GetObject("IsoMu24_eta2p1_DATA_over_MC_TightID_IsodB_PT_ABSETA_Barrel_0to0p9_pt25-500_2012ABCD",ratio_eta_0to0p9);
     ftrigger->GetObject("IsoMu24_eta2p1_DATA_over_MC_TightID_IsodB_PT_ABSETA_Transition_0p9to1p2_pt25-500_2012ABCD",ratio_eta_0p9to1p2);
     ftrigger->GetObject("IsoMu24_eta2p1_DATA_over_MC_TightID_IsodB_PT_ABSETA_Endcaps_1p2to2p1_pt25-500_2012ABCD",ratio_eta_1p2to2p1);
   
     cout << "   > Reading datasets " << endl;
         
     myScrewdriver.SetLumi(lumi);

     for (unsigned int d = 0 ; d < datasetsList.size() ; d++)
     {
         // Get current dataset
         string currentDataset = datasetsList[d];
         string currentProcessClass = myScrewdriver.GetProcessClass(currentDataset); 
    
         // Open dataset tree
         TTree* theTree;
         TH1D*  theCutFlow;
         //TFile* f = new TFile((string("../MonotopBabyTuples/babyTuples/")+currentDataset+".root").c_str());
         TFile* f = new TFile((string("/opt/sbg/data/sbgse24/data1/cms/mbuttign/babyTuples_Monotop/BT_20_11_14/")+currentDataset+".root").c_str());
         //TFile* f = new TFile((string("../MonotopBabyTuples/babyTupleProduction/proofOutput/")+currentDataset+".root").c_str());
         f->GetObject("hCutFlow",theCutFlow);
         f->GetObject("babyTuple",theTree);

         intermediatePointers pointers;
         InitializeBranchesForReading(theTree,&myEvent,&pointers);
         ReadEvent(theTree,0,&pointers,&myEvent);
         myScrewdriver.SetTrueNumberOfEvents(currentDataset, myEvent.numberOfInitialEvents);

         // Reweight event to luminosity
         float lumiweight = myScrewdriver.GetDatasetLumiWeight(currentDataset);

         // Loop over the events
         int nEntries = theTree->GetEntries();
         int nEventsToRunOver;
         if (nEntries < 10000 || currentProcessClass == "signal")  nEventsToRunOver = nEntries;
         else                                                      nEventsToRunOver = nEntries/1000;

         //for (int i = 0 ; i < nEventsToRunOver ; i++)
         for (int i = 0 ; i < nEntries ; i++)
         {

             if (nEntries > 1000 && (i % (nEntries / 1000) == 0)) printProgressBar(i,nEntries,currentDataset);

             // Get the i-th entry
             ReadEvent(theTree,i,&pointers,&myEvent);
             // Compute variables on-the-fly
             computeOnTheFlyVariables(&onTheFlyVariables,&myEvent);

             if (onTheFlyVariables.hasExactlyOneMuon != 1)   continue;
//             if (onTheFlyVariables.hasExactlyOneRealMuon != 1)   continue;
             if (myEvent.leadingLeptonIsolation > 0.12)       continue;
             if (onTheFlyVariables.nSmearedJets < 4)         continue;
//             if (onTheFlyVariables.MET < 50)                  continue;
//             if (onTheFlyVariables.MTw < 80)                  continue;
             if (onTheFlyVariables.nBJets != 2)               continue;

             // Reweight event to PU
             float puweight      = myEvent.weightPileUp;
             float SFlepton = 1;

             if (myEvent.leadingLeptonIsolation < 0.12) SFlepton = myEvent.SF_lepton;
             else                                       SFlepton = 1;

             float topPtweight = 1;
             if (currentProcessClass == "TTbar") topPtweight = onTheFlyVariables.SFtopPtReweighting;
             double SFtrigger = 1;
             if (abs(onTheFlyVariables.leadingLeptonEta) < 0.9) SFtrigger = getSFtrigger(ratio_eta_0to0p9, onTheFlyVariables.leadingLeptonPt, onTheFlyVariables.leadingLeptonEta);
             else if (abs(onTheFlyVariables.leadingLeptonEta) > 0.9 && abs(onTheFlyVariables.leadingLeptonEta) < 1.2) SFtrigger = getSFtrigger(ratio_eta_0p9to1p2, onTheFlyVariables.leadingLeptonPt, onTheFlyVariables.leadingLeptonEta);
             else if (abs(onTheFlyVariables.leadingLeptonEta) > 1.2 && abs(onTheFlyVariables.leadingLeptonEta) < 2.1) SFtrigger = getSFtrigger(ratio_eta_1p2to2p1, onTheFlyVariables.leadingLeptonPt, onTheFlyVariables.leadingLeptonEta);

             // Global reweighting
             float weight;
             if (currentProcessClass == "data" || currentProcessClass == "signal") weight = 1;
             else                                                                  weight = lumiweight*puweight*SFlepton*SFtrigger*topPtweight;
//if (currentProcessClass == "TTbar") cout << "SFtrigger= " << SFtrigger << " | SFlepton= " << SFlepton << " | PUweight= " << puweight << " | Lumi weight= " << lumiweight << " | TopPtWeight= " << topPtweight << endl;            
            // Ignore QCD MC that should go in the preselection
            if (currentProcessClass == "QCD"  && myEvent.leadingLeptonIsolation < 0.6) continue;
            // Fill all the usual processes
            myScrewdriver.AutoFillProcessClass(currentProcessClass,weight);

             // To remplace the MC QCD with the QCD that really dominated the QCD Region (data driven method) 
            if(currentProcessClass == "data" && myEvent.leadingLeptonIsolation > 0.6)  
            {
                // This allows to fill the other regions
                myEvent.leadingLeptonIsolation  = - myEvent.leadingLeptonIsolation;
                myEvent.selectedLeptonIsolation  = - myEvent.selectedLeptonIsolation;

                // This fills the MC QCD with the "QCD-like" data
                myScrewdriver.AutoFillProcessClass("QCD",1);
            }
         }
         printProgressBar(nEntries,nEntries,currentDataset);
         cout << endl;
     }

     // ############################################
     // ##   Compute Cutflow table and apply SF   ##
     // ############################################

     vector<string> regionsForCutflow = { "preSelection", "qcdScaleRegion", "qcdControlRegion", "qcdCheckScaleRegion", "signalRegion"};
     //vector<string> regionsForCutflow = { "preSelection", "ttbarControlRegion","wjetsControlRegion","signalRegion"};
     TableDataMC cutFlowTable = TableDataMC(&myScrewdriver,regionsForCutflow,"muChannel", "signalRegion");
//     cutFlowTable.Print();

     // yield of QCD given by the DataDriven method
     Figure yieldQCDDataDriven = cutFlowTable.Get("qcdScaleRegion","QCD");

     // yields of MC and Data samples in the Scale Region
     Figure yieldOtherMCinSR   = cutFlowTable.Get("qcdScaleRegion","TTbar");
            yieldOtherMCinSR  += cutFlowTable.Get("qcdScaleRegion","SingleTop");
            yieldOtherMCinSR  += cutFlowTable.Get("qcdScaleRegion","Wjets");
            yieldOtherMCinSR  += cutFlowTable.Get("qcdScaleRegion","Zjets");
            yieldOtherMCinSR  += cutFlowTable.Get("qcdScaleRegion","Others");
     Figure yieldDatainSR      = cutFlowTable.Get("qcdScaleRegion","data");
     Figure SF_QCD            = (yieldDatainSR - yieldOtherMCinSR)/yieldQCDDataDriven;

     // apply the SF to the regions
     myScrewdriver.ApplyScaleFactor("QCD",     "qcdScaleRegion", "muChannel", SF_QCD);
     myScrewdriver.ApplyScaleFactor("QCD",     "ttbarControlRegion", "muChannel", SF_QCD);
     myScrewdriver.ApplyScaleFactor("QCD",     "wjetsControlRegion", "muChannel", SF_QCD);
     myScrewdriver.ApplyScaleFactor("QCD",     "preSelection", "muChannel", SF_QCD);
     myScrewdriver.ApplyScaleFactor("QCD",     "signalRegion", "muChannel", SF_QCD);
     myScrewdriver.ApplyScaleFactor("QCD",     "qcdCheckScaleRegion", "muChannel", SF_QCD);

//     cout << "SF QCD    = " << SF_QCD.Print()       << endl; 

     cutFlowTable = TableDataMC(&myScrewdriver,regionsForCutflow,"muChannel", "includeSignal");
     cutFlowTable.Print();

     cutFlowTable.Print("cutFlowTable.tab");
     cutFlowTable.PrintLatex("cutFlowTable.tex");


     // ###################################
     // ##   Make plots and write them   ##
     // ###################################

     cout << "   > Making and writing plots ... " << endl;

     system("mkdir -p ./plots/");
     myScrewdriver.MakePlots();
     myScrewdriver.WritePlots("./plots/");

     printBoxedMessage("Plot generation completed");


     return (0);

}


bool muChannel()
{
    return true;
}

