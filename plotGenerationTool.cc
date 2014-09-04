#include "common.h"

// Sonic screwdriver

#include "../sonicScrewdriver/interface/SonicScrewdriver.h" 
using namespace theDoctor;


// Global pointer used for the region/channel selectors
babyEvent* myEventPointer;

// #########################################################################""
// #########################################################################""

typedef struct
{
        float selectedLeptonEta;
        float selectedLeptonPt;
        int   nJets;
        int   nBJets;
} 
OnTheFlyVariables;

void computeOnTheFlyVariables(OnTheFlyVariables* onTheFlyVariables, babyEvent* myEvent)
{
        onTheFlyVariables->selectedLeptonEta = myEvent->selectedLepton.Eta();
        onTheFlyVariables->selectedLeptonPt  = myEvent->selectedLepton.Pt();
        onTheFlyVariables->nJets             = (int)myEvent->nJets;
        onTheFlyVariables->nBJets            = (int)myEvent->nBTag;
}



// Regions
bool preSelection();
vector<Cut> signalRegion
{
    Cut("MET",     '>', 50),
    Cut("selectedLeptonPt",'>', 50)
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
     // Float_t lumi = 19000;   //test 

     // Create a sonic Screwdriver
      SonicScrewdriver myScrewdriver;

     // Create a container for the event
     babyEvent myEvent;
     OnTheFlyVariables onTheFlyVariables;
     myEventPointer = &myEvent;

  // ##########################
  // ##   Create Variables   ##
  // ##########################

      myScrewdriver.AddVariable("selectedLeptonPt", "p_{T} (lepton)","GeV",40,0,200,&(onTheFlyVariables.selectedLeptonPt),"");
      myScrewdriver.AddVariable("MET",           "Missing E_{T}",    "GeV",    50,0,100,    &(myEvent.MET),            ""     );
      myScrewdriver.AddVariable("selectedLeptonEta","#eta (lepton)","",30,-3,3,    &(onTheFlyVariables.selectedLeptonEta),"");
      myScrewdriver.AddVariable("leadingJetPt","p_{T} (leading jet)","GeV",40,0,200,  &(myEvent.leadingJetPt),"");
      myScrewdriver.AddVariable("leadingJetEta","#eta (leading jet)", "", 30,-3,3,  &(myEvent.leadingJetEta), "");
      myScrewdriver.AddVariable("numberOfPrimaryVertices","N_{vtx}", "", 51,0,50,  &(myEvent.numberOfPrimaryVertices), "");
      myScrewdriver.AddVariable("nJets","numberOfjets", "", 10,-0.5,9.5,  &(onTheFlyVariables.nJets), "");
      myScrewdriver.AddVariable("nBJets","numberOfBjets", "", 10,-0.5,9.5,  &(onTheFlyVariables.nBJets), "");
      myScrewdriver.AddVariable("MTw","M_{T}^{W}", "", 125,0,250,  &(myEvent.MTw), "");
      myScrewdriver.AddVariable("selectedLeptonIsolation","Isolation (selLepton)", "", 150,-0.01,2.99,  &(myEvent.selectedLeptonIsolation), "");

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

//      myScrewdriver.AddProcessClass("SingleTop",     "SingleTop",     "background", kOrange-3);
//      myScrewdriver.AddDataset("NTuple_53X_T_s-channel","SingleTop",myEvent.numberOfInitialEvents,3.79);
//      myScrewdriver.AddDataset("NTuple_53X_T_t-channel","SingleTop",myEvent.numberOfInitialEvents,56.4);
//      myScrewdriver.AddDataset("NTuple_53X_T_tW-channel","SingleTop",myEvent.numberOfInitialEvents,11.1);
//      myScrewdriver.AddDataset("NTuple_53X_Tbar_t-channel","SingleTop",myEvent.numberOfInitialEvents,30.7);
//      myScrewdriver.AddDataset("NTuple_53X_Tbar_tW-channel","SingleTop",myEvent.numberOfInitialEvents,11.1);

// Change that because of the trigger path in the NTuples! 
//      myScrewdriver.AddProcessClass("Others",     "Others",     "background", kGreen-3);
//      myScrewdriver.AddDataset("NTuple_53X_TTWJets_8TeVmadgraph","Others",myEvent.numberOfInitialEvents,0.232);
//      myScrewdriver.AddDataset("NTuple_53X_TTZJets_8TeVmadgraph_v2","Others",myEvent.numberOfInitialEvents,0.208);
//      myScrewdriver.AddDataset("NTuple_53X_TZJetsTo3LNuB","Others",myEvent.numberOfInitialEvents,0.0096);
//      myScrewdriver.AddDataset("NTuple_53X_WWJetsIncl","Others",myEvent.numberOfInitialEvents,5.817);
//      myScrewdriver.AddDataset("NTuple_53X_WZJetsIncl","Others",myEvent.numberOfInitialEvents,1.189);
//      myScrewdriver.AddDataset("NTuple_53X_ZZJetsIncl","Others",myEvent.numberOfInitialEvents,0.213);

      myScrewdriver.AddProcessClass("Zjets",     "Drell Yan",     "background", kOrange+3);
      myScrewdriver.AddDataset("NTuple_53X_DYJetsToLL_M-50","Zjets",myEvent.numberOfInitialEvents,3531.);
      myScrewdriver.AddDataset("NTuple_53X_DYJetsToLL_M-10To50","Zjets",myEvent.numberOfInitialEvents,2580.);

      myScrewdriver.AddProcessClass("Wjets",     "W+jets",     "background", kGreen+2);
      myScrewdriver.AddDataset("NTuple_53X_WJetsToLNu","Wjets",myEvent.numberOfInitialEvents,37509); //36864.3);

      // Signal(s)

      //myScrewdriver.AddProcessClass("S1",     "S1 resonant",               "signal",  COLORPLOT_BLUE, "no1DPlots");
      //myScrewdriver.AddDataset("S1_mres500p0_mchi100p0","S1",myEvent.numberOfInitialEvents,0.01);

      // Data

      myScrewdriver.AddProcessClass("data" ,   "Data",    "data",       COLORPLOT_BLACK);
      myScrewdriver.AddDataset("NTuple_53X_SingleMuRun2012A","data",-1,-1);
      myScrewdriver.AddDataset("NTuple_53X_SingleMuRun2012B","data",-1,-1);
      myScrewdriver.AddDataset("NTuple_53X_SingleMuRun2012C","data",-1,-1);
      myScrewdriver.AddDataset("NTuple_53X_SingleMuRun2012D","data",-1,-1);

      // ##########################
      // ##    Create Regions    ##
      // ##########################

      myScrewdriver.AddRegion("preSelection","Pre-selection",&preSelection);
      //myScrewdriver.AddRegion("signalRegion","Signal region","preSelection",signalRegion,"showCuts");

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
//     myScrewdriver.SetGlobalFloatOption ("1DStack",           "factorSignal",                     2.0    );

     myScrewdriver.SetGlobalStringOption("DataMCComparison",  "includeSignal",                    "no");
     myScrewdriver.SetGlobalStringOption("DataMCComparison",  "ratioPosition",                    "bottom");
//     myScrewdriver.SetGlobalFloatOption ("DataMCComparison",  "factorSignal",                     1.0    );
     
//     myScrewdriver.SetGlobalFloatOption ("FigureOfMerit",     "backgroundSystematicUncertainty",  0.15   );

     // Schedule plots
     
     myScrewdriver.SchedulePlots("1DSuperimposed");
     myScrewdriver.SchedulePlots("1DStack");
     myScrewdriver.SchedulePlots("1DDataMCComparison");
     //myScrewdriver.SchedulePlots("2D");
     // Config plots

     myScrewdriver.SetGlobalStringOption("Plot", "infoTopRight", "CMS Internal");
     myScrewdriver.SetGlobalStringOption("Plot", "infoTopLeft",  "#sqrt{s} = 8 TeV, L = 19.5 fb^{-1}");
     
     myScrewdriver.SetGlobalBoolOption("Plot", "exportPdf", true);
     myScrewdriver.SetGlobalBoolOption("Plot", "exportEps", false);
     myScrewdriver.SetGlobalBoolOption("Plot", "exportPng", false);

  // ########################################
  // ##        Run over the events         ##
  // ########################################
  
     vector<string> datasetsList;
     myScrewdriver.GetDatasetList(&datasetsList);

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
         //TFile* f = new TFile((string("../babyTuples/")+currentDataset+".root").c_str());
         //TFile* f = new TFile((string("../babyTuples/BTwoJetsPresel/")+currentDataset+".root").c_str());
         TFile* f = new TFile((string("../babyTupleProduction/proofOutput/")+currentDataset+".root").c_str());
         f->GetObject("hCutFlow",theCutFlow);
         f->GetObject("babyTuple",theTree);
cout << endl;
cout << "Dataset = " << currentDataset << endl;
         intermediatePointers pointers;
         InitializeBranchesForReading(theTree,&myEvent,&pointers);

         ReadEvent(theTree,0,&pointers,&myEvent);
         myScrewdriver.SetTrueNumberOfEvents(currentDataset, myEvent.numberOfInitialEvents);

         // Reweight event to luminosity
         float lumiweight = myScrewdriver.GetDatasetLumiWeight(currentDataset);


//cout << "Initial number of events = " << myEvent.numberOfInitialEvents << endl;
//cout << "Luminosity               = " << lumi                          << endl;

         // Loop over the events
         int nEntries = theTree->GetEntries();
cout << "Number of expected events for dataset: " << currentDataset << " is: " << lumiweight*nEntries << endl; 
cout << "Number of expected events for dataset: " << currentDataset << " is: " << lumi*myScrewdriver.GetDatasetCrossSection(currentDataset)*nEntries/myEvent.numberOfInitialEvents << endl; 
cout << "nEvents= " << nEntries << endl;
cout << "Lumiweight for dataset: " << currentDataset << " is: " << lumi*myScrewdriver.GetDatasetCrossSection(currentDataset)/myEvent.numberOfInitialEvents << endl; 
         for (int i = 0 ; i < nEntries ; i++)
         {

             if (i % (nEntries / 10) == 0) printProgressBar(i,nEntries,currentDataset);

             // Get the i-th entry
             ReadEvent(theTree,i,&pointers,&myEvent);

             // Compute variables on-the-fly
             computeOnTheFlyVariables(&onTheFlyVariables,&myEvent);

             // Reweight event to PU
             float puweight = myEvent.weightPileUp;
//cout << "PuWeight                 = " << puweight   << endl;
//cout << "LumiWeight               = " << lumiweight << endl;

             // Global reweighting
            // float weight = lumiweight;
            float weight = lumiweight*puweight;
//if(myScrewdriver.GetProcessClassType(currentProcessClass) == "data") cout << "WeightData = " << weight << endl;  
             // Fill all the variables
             myScrewdriver.AutoFillProcessClass(currentProcessClass,weight);

         }
         printProgressBar(nEntries,nEntries,currentDataset);
         cout << endl;
     }

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

// ##########################################
// ##   Regions and channels definitions   ##
// ##########################################

bool preSelection()
{
//    if (myEventPointer->MET < 30) return false;
//    if (myEventPointer->jets.size() != 1) return false;
//    if (myEventPointer->nBTag != 1) return false;
    return true;
}


bool muChannel()
{
    return true;
}

