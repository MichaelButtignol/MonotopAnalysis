#ifndef babyFormat
#define babyFormat

typedef struct 
{
    // ------------
    // General info
    // ------------

    Int_t run;                                  // Run number
    Int_t lumi;                                 // Lumiblock number
    Int_t event;                                // Event number

    // ------------
    // Trigger info
    // ------------
    
    Bool_t triggerMuon;                         // Single-Muon trigger has been fired
    Bool_t triggerElec;                         // Single-Elec trigger has been fired
    
    // ------------
    // Leptons info
    // ------------

    Short_t        numberOfLepton;              // Number of selected leptons
    TLorentzVector selectedLepton;              // p4 of the leading selected lepton
    Float_t        selectedLeptonEta;           // Eta of the leading selected lepton
    Short_t        selectedLeptonPDGId;         // pdgid of the leading selected lepton
    Short_t        nLooseMuons;                 // Number of selected loose muons
    Short_t        nMuons;                      // Number of selected muons
    Short_t        nElectrons;                  // Number of selected electrons
    Short_t        nLooseElectrons;             // Number of selected loose electrons
    Float_t        selectedLeptonIsolation;     // Isolation of the lepton after the cut 

    
    // ------------
    // Jets info
    // ------------

    Short_t                nJets;               // Number of selected jets
    Short_t                nBTag;               // Number of selected jets b-tagged
    vector<TLorentzVector> jets;                // p4 of the selected jets
    vector<Float_t>        jets_CSV_raw;        // CSV value of the selected jets, before reshaping 
    vector<Float_t>        jets_CSV_reshaped;   // CSV value of the selected jets, after reshaping

    // ------------
    // "High-level" /BDT variables
    // ------------

    Float_t MET;                                // Type-1 + phi-corrected PF MET
    Float_t MTw;                                // Transvers mass of the W boson
    Float_t leadingJetPt;                       // pT of the leading jet
    Float_t leadingJetEta;                      // Eta of the leading jet
    Float_t DeltaR_leadJet_lep;                 // DeltaR(lepton, leading jet)

    // ------------
    // Signal-specific infos
    // ------------

    Float_t mResonant;                          // Resonant mass
    Float_t mInvisible;                         // Invisible mass
 
    // ------------
    // Generator-level infos
    // ------------
   
    Int_t   numberOfGenLepton;                  // Number of MC leptons (e/mu/tau, leptons from tau decay are not taken into account)

    vector<TLorentzVector> genParticles;        // p4 of status=3 particles
    vector<Int_t>          genParticlesPDGId;   // PDG of status=3 particles
    vector<Int_t>          genParticlesMother;  // Mother index of the status=3 particles

    // ------------
    // Weights and infos related to weights
    // ------------

    Int_t   numberOfInitialEvents;              // Number of events for the given dataset or signal mass point, before any skimming/selection applied
    Float_t crossSection;                       // Cross section for the given dataset or signal mass point
    Int_t   numberOfTruePU;                     // Number of true pile-up (MC, used in weightPileup)
    Int_t   numberOfPrimaryVertices;            // Number of primary vertices, used to check pile-up data/MC aggreement

    Float_t weightCrossSection;                 // = (crossSection / numberOfInitialEvents) ; user should multiply this weight by the desired lumi
    Float_t weightPileUp;                       // Weight for PU-reweighting
    Float_t weightISRmodeling;                  // Weight for ISR modeling
    Float_t weightTopPt;                        // Weight for top Pt reweighting
    Float_t weightTriggerEfficiency;            // Weight for singleLepton trigger efficiency


        // CSV reshaping variations
        
    vector<Float_t> jets_CSV_reshapedUpBC;
    vector<Float_t> jets_CSV_reshapedDownBC;
    vector<Float_t> jets_CSV_reshapedUpLight;
    vector<Float_t> jets_CSV_reshapedDownLight;

        // Raw MET (used as a cross check for when applying MET filters after production)

    Float_t rawPFMET;                           // Raw MET from PF-based algorithm 
    
} babyEvent;

typedef struct
{

    TLorentzVector*         pointerToSelectedLepton;
    vector<TLorentzVector>* pointerToJets;
    vector<float>*          pointerToJets_CSV_raw;
    vector<float>*          pointerToJets_CSV_reshaped;
    vector<TLorentzVector>* pointerToGenParticles;
    vector<Int_t>*          pointerToGenParticlesPDGId;
    vector<Int_t>*          pointerToGenParticlesMother;
    vector<Float_t>*        pointerToJets_CSV_reshapedUpBC;
    vector<Float_t>*        pointerToJets_CSV_reshapedDownBC;
    vector<Float_t>*        pointerToJets_CSV_reshapedUpLight;
    vector<Float_t>*        pointerToJets_CSV_reshapedDownLight;

} intermediatePointers;

void ReadEvent(TTree* theTree, long int i, intermediatePointers* pointers, babyEvent* myEvent)
{
      theTree->GetEntry(i);
      myEvent->selectedLepton               = *(pointers->pointerToSelectedLepton);
      
      myEvent->jets                         = *(pointers->pointerToJets);
      myEvent->jets_CSV_raw                 = *(pointers->pointerToJets_CSV_raw);
      myEvent->jets_CSV_reshaped            = *(pointers->pointerToJets_CSV_reshaped);
      myEvent->genParticles                 = *(pointers->pointerToGenParticles);
      myEvent->genParticlesPDGId            = *(pointers->pointerToGenParticlesPDGId);
      myEvent->genParticlesMother           = *(pointers->pointerToGenParticlesMother);
}

void InitializeBranchesForReading(TTree* theTree, babyEvent* myEvent,intermediatePointers* pointers)
{
    theTree->SetBranchAddress("run",                                          &(myEvent->run));
    theTree->SetBranchAddress("lumi",                                         &(myEvent->lumi));
    theTree->SetBranchAddress("event",                                        &(myEvent->event));
    
    theTree->SetBranchAddress("triggerMuon",                                  &(myEvent->triggerMuon));
    theTree->SetBranchAddress("triggerElec",                                  &(myEvent->triggerElec));
    
    theTree->SetBranchAddress("numberOfLepton",                               &(myEvent->numberOfLepton));
    pointers->pointerToSelectedLepton = 0;
    theTree->SetBranchAddress("selectedLepton",                               &(pointers->pointerToSelectedLepton));
    theTree->SetBranchAddress("selectedLeptonPDGId",                          &(myEvent->selectedLeptonPDGId));
    theTree->SetBranchAddress("selectedLeptonIsolation",                      &(myEvent->selectedLeptonIsolation));
    
    theTree->SetBranchAddress("nJets",                                        &(myEvent->nJets));
    theTree->SetBranchAddress("nBTag",                                        &(myEvent->nBTag));
     
    pointers->pointerToJets = 0;
    theTree->SetBranchAddress("jets",                                         &(pointers->pointerToJets));
    pointers->pointerToJets_CSV_raw = 0;
    theTree->SetBranchAddress("jets_CSV_raw",                                 &(pointers->pointerToJets_CSV_raw));
    pointers->pointerToJets_CSV_reshaped = 0;
    theTree->SetBranchAddress("jets_CSV_reshaped",                            &(pointers->pointerToJets_CSV_reshaped));
    
    theTree->SetBranchAddress("MET",                                          &(myEvent->MET));
    theTree->SetBranchAddress("MTw",                                          &(myEvent->MTw));
    theTree->SetBranchAddress("leadingJetPt",                                 &(myEvent->leadingJetPt));
    theTree->SetBranchAddress("leadingJetEta",                                &(myEvent->leadingJetEta));
    theTree->SetBranchAddress("DeltaR_leadJet_lep",                           &(myEvent->DeltaR_leadJet_lep));
    
    theTree->SetBranchAddress("numberOfGenLepton",                            &(myEvent->numberOfGenLepton));

    pointers->pointerToGenParticles = 0;
    theTree->SetBranchAddress("genParticles",                                 &(pointers->pointerToGenParticles));
    pointers->pointerToGenParticlesPDGId = 0;
    theTree->SetBranchAddress("genParticlesPDGId",                            &(pointers->pointerToGenParticlesPDGId));
    pointers->pointerToGenParticlesMother = 0;
    theTree->SetBranchAddress("genParticlesMother",                           &(pointers->pointerToGenParticlesMother));

    theTree->SetBranchAddress("numberOfInitialEvents",                        &(myEvent->numberOfInitialEvents));
    theTree->SetBranchAddress("crossSection",                                 &(myEvent->crossSection));
    theTree->SetBranchAddress("numberOfTruePU",                               &(myEvent->numberOfTruePU));
    theTree->SetBranchAddress("numberOfPrimaryVertices",                      &(myEvent->numberOfPrimaryVertices));
    theTree->SetBranchAddress("weightCrossSection",                           &(myEvent->weightCrossSection));
    theTree->SetBranchAddress("weightPileUp",                                 &(myEvent->weightPileUp));

    /*
    pointers->pointerToJets_CSV_reshapedUpBC        = 0;
    theTree->SetBranchAddress("jets_CSV_reshapedUpBC",                        &(pointers->pointerToJets_CSV_reshapedUpBC));
    pointers->pointerToJets_CSV_reshapedDownBC      = 0;                                
    theTree->SetBranchAddress("jets_CSV_reshapedDownBC",                      &(pointers->pointerToJets_CSV_reshapedDownBC));
    pointers->pointerToJets_CSV_reshapedUpLight     = 0;                                
    theTree->SetBranchAddress("jets_CSV_reshapedUpLight",                     &(pointers->pointerToJets_CSV_reshapedUpLight));
    pointers->pointerToJets_CSV_reshapedDownLight   = 0;                                
    theTree->SetBranchAddress("jets_CSV_reshapedDownLight",                   &(pointers->pointerToJets_CSV_reshapedDownLight));
    */                                                                                    
    
    theTree->SetBranchAddress("rawPFMET",                                     &(myEvent->rawPFMET));

}
#endif

