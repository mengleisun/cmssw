#include "map"
#include "FWCore/Common/interface/TriggerNames.h"
#include "ggAnalysis/ggNtuplizer/interface/ggNtuplizer.h"

using namespace std;

// local variables: per-filter per-electron/muon/photon/jet arrays of matched trigger objects
// NOTE: number of elements in the arrays equals sizeof(Int_t)
vector<float> trgElePt[32], trgEleEta[32], trgElePhi[32];
vector<float> trgSinglePhoPt[32], trgSinglePhoEta[32], trgSinglePhoPhi[32];
vector<float> trgDoublePhoPt[32], trgDoublePhoEta[32], trgDoublePhoPhi[32];
vector<float> trgMuPt[32],  trgMuEta[32],  trgMuPhi[32];
vector<float> trgJetPt[32], trgJetEta[32], trgJetPhi[32];

vector<int> trgEleIndex_;
vector<int> trgSinglePhoIndex_;
vector<int> trgDoublePhoIndex_;
vector<int> trgMuIndex_;
vector<int> trgJetIndex_;
vector<float> trgElePt_;
vector<float> trgEleEta_;
vector<float> trgElePhi_;
vector<float> trgSinglePhoPt_;
vector<float> trgSinglePhoEta_;
vector<float> trgSinglePhoPhi_;
vector<float> trgDoublePhoPt_;
vector<float> trgDoublePhoEta_;
vector<float> trgDoublePhoPhi_;
vector<float> trgMuPt_;
vector<float> trgMuEta_;
vector<float> trgMuPhi_;
vector<float> trgJetPt_;
vector<float> trgJetEta_;
vector<float> trgJetPhi_;

void ggNtuplizer::branchesTriggers(TTree* tree) {

  tree->Branch("trgEleIndex",	    	&trgEleIndex_);         
  tree->Branch("trgSinglePhoIndex",	&trgSinglePhoIndex_);
  tree->Branch("trgDoublePhoIndex",  	&trgDoublePhoIndex_);
  tree->Branch("trgMuIndex",	    	&trgMuIndex_);
  tree->Branch("trgJetIndex",	    	&trgJetIndex_);
  tree->Branch("trgElePt",	    	&trgElePt_);
  tree->Branch("trgEleEta",	    	&trgEleEta_);
  tree->Branch("trgElePhi",	    	&trgElePhi_);
  tree->Branch("trgSinglePhoPt",    	&trgSinglePhoPt_);
  tree->Branch("trgSinglePhoEta",   	&trgSinglePhoEta_);
  tree->Branch("trgSinglePhoPhi",   	&trgSinglePhoPhi_);
  tree->Branch("trgDoublePhoPt",    	&trgDoublePhoPt_);
  tree->Branch("trgDoublePhoEta",   	&trgDoublePhoEta_);
  tree->Branch("trgDoublePhoPhi",   	&trgDoublePhoPhi_);
  tree->Branch("trgMuPt",	    	&trgMuPt_);
  tree->Branch("trgMuEta",	    	&trgMuEta_);
  tree->Branch("trgMuPhi",	    	&trgMuPhi_);
  tree->Branch("trgJetPt",	    	&trgJetPt_);
  tree->Branch("trgJetEta",	    	&trgJetEta_);
  tree->Branch("trgJetPhi",	    	&trgJetPhi_);                     
}


void ggNtuplizer::initTriggerFilters(const edm::Event &e) {
  // Fills the arrays above.

  // cleanup from previous execution
  for (size_t i = 0; i < 32; ++i) {
    trgElePt [i].clear();
    trgEleEta[i].clear();
    trgElePhi[i].clear();
    trgSinglePhoPt [i].clear();
    trgSinglePhoEta[i].clear();
    trgSinglePhoPhi[i].clear();
    trgDoublePhoPt [i].clear();
    trgDoublePhoEta[i].clear();
    trgDoublePhoPhi[i].clear();
    trgMuPt  [i].clear();
    trgMuEta [i].clear();
    trgMuPhi [i].clear();
    trgJetPt [i].clear();
    trgJetEta[i].clear();
    trgJetPhi[i].clear();
  }
  trgEleIndex_.clear();
  trgSinglePhoIndex_.clear();
  trgDoublePhoIndex_.clear();
  trgMuIndex_.clear();
  trgJetIndex_.clear();
  trgElePt_.clear();
  trgEleEta_.clear();
  trgElePhi_.clear();
  trgSinglePhoPt_.clear();
  trgSinglePhoEta_.clear();
  trgSinglePhoPhi_.clear();
  trgDoublePhoPt_.clear();
  trgDoublePhoEta_.clear();
  trgDoublePhoPhi_.clear();
  trgMuPt_.clear();
  trgMuEta_.clear();
  trgMuPhi_.clear();
  trgJetPt_.clear();
  trgJetEta_.clear();
  trgJetPhi_.clear();

  // filter => index (in trg*[] arrays) mappings
  static std::map<string,size_t> eleFilters;
  static std::map<string,size_t> phoSingleFilters;
  static std::map<string,size_t> phoDoubleFilters;
  static std::map<string,size_t> muFilters;
  static std::map<string,size_t> jetFilters;

  // one-time initialization
  if (eleFilters.size() == 0) {
    eleFilters["hltEle27WPTightGsfTrackIsoFilter"] = 0;
    eleFilters["hltEle25WP60SC4HcalIsoFilter"]     = 1;
    eleFilters["hltL1sL1SingleIsoEG20erORSingleIsoEG22erORSingleEG25"] = 2;
    eleFilters["hltSingleEle22WPLooseGsfTrackIsoFilter"] = 3;
    eleFilters["hltL1sL1SingleIsoEG20erORSingleIsoEG22erORSingleEG25"] = 4;
    eleFilters["hltSingleEle22WPTightGsfTrackIsoFilter"] = 5;
    eleFilters["hltL1sL1SingleEG20ORSingleIsoEG18erORSingleIsoEG20ORSingleIsoEG20erORSingleIsoEG22erORSingleEG25"] = 6;
    eleFilters["hltEle23WPLooseGsfTrackIsoFilter"] = 7;
    eleFilters["hltL1sL1SingleIsoEG20OrSingleEG20OrSingleEG25"] = 8;
    eleFilters["hltEle27noerWPLooseGsfTrackIsoFilter"] = 9;
    eleFilters["hltL1sL1SingleIsoEG22erOrSingleEG25"] = 10;
    eleFilters["hltEle27WPLooseGsfTrackIsoFilter"] = 11;
    eleFilters["hltL1sL1SingleIsoEG30erOrSingleEG40"] = 12;
    eleFilters["hltEle32WPTightGsfTrackIsoFilter"] = 13;

    //HLT_Mu17_Photon30_CaloIdL_L1ISO && HLT_Mu17_Photon35_CaloIdL_L1ISO_v2 
    muFilters["hltL1sL1Mu5IsoEG18"] = 0;
    muFilters["hltL2fL1sL1Mu5IsoEG18L1f5L2Filtered7"] = 1;
    muFilters["hltL3fL1sL1Mu5IsoEG18L1f5L2f7L3Filtered17"] = 2;
    muFilters["hltMu17Photon30CaloIdLL1ISOHEFilter"] = 3;
    muFilters["hltMu17Photon35CaloIdLL1ISOHEFilter"] = 4;
    //HLT_Mu23NoFiltersNoVtx_Photon23_CaloIdL_v2 && HLT_Mu28NoFiltersNoVtxDisplaced_Photon28_CaloIdL_v2
    muFilters["hltL1sL1Mu5EG20"] = 5;
    muFilters["hltL2fL1sMu5EG20L1f5L2NoVtxFiltered16"] = 6;
    //HLT_Mu23NoFiltersNoVtx_Photon23_CaloIdL_v2
    muFilters["hltL3fL1Mu5EG20L1f5L2NVf12L3NoFiltersNoVtxFiltered23"] = 7;
    muFilters["hltMu23NoFiltersNoVtxPhoton23CaloIdLHEFilter"] = 8;
    //HLT_Mu28NoFiltersNoVtxDisplaced_Photon28_CaloIdL_v2
    muFilters["hltL3fL1Mu5EG20L1f5L2NVf12L3NoFiltersNoVtxDisplacedFiltered28"] = 9;
    muFilters["hltMu28NoFiltersNoVtxDisplacedPhoton28CaloIdLHEFilter"] = 10;
    muFilters["hltL3fL1sMu5EG20L1f5L2NVf16L3NoFiltersNoVtxDisplacedFiltered33"] = 11;
    muFilters["hltMu33NoFiltersNoVtxDisplacedPhoton33CaloIdLHEFilter"] = 12;
    //HLT_Mu38NoFiltersNoVtx_Photon38_CaloIdL_v2
    muFilters["hltL3fL1sMu5EG20L1f5L2NVf16L3NoFiltersNoVtxFiltered38"] = 13;
    muFilters["hltMu38NoFiltersNoVtxPhoton38CaloIdLHEFilter"]= 14;
    //HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v2
    muFilters["hltL1sL1DoubleMu103p5ORDoubleMu125"] = 15;
    muFilters["hltL2pfL1sDoubleMu103p5L1f0L2PreFiltered0"] = 16;
    muFilters["hltL2fL1sDoubleMu103p5L1f0L2Filtered10OneMu"] = 17;
    muFilters["hltL3pfL1sDoubleMu103p5L1f0L2pf0L3PreFiltered8"] = 18;
    muFilters["hltL3fL1sDoubleMu103p5L1f0L2f10OneMuL3Filtered17"] = 19;
    muFilters["hltDiMuonGlb17Glb8RelTrkIsoFiltered0p4"] = 20;  //HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_v2
    muFilters["hltDiMuonGlb17Glb8RelTrkIsoFiltered0p4DzFiltered0p2"] = 21;
    muFilters["hltL2fL1sDoubleMu103p5L1f0OneMuL2Filtered10"] = 22;
    muFilters["hltL3fL1sDoubleMu103p5L1f0L2f10L3Filtered17"] = 23;
    muFilters["hltDiMuonGlbFiltered17TrkFiltered8"] = 24;
    muFilters["hltDiMuonGlb17Trk8RelTrkIsoFiltered0p4"] = 25;  //HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_v2
    muFilters["hltDiMuonGlb17Trk8RelTrkIsoFiltered0p4DzFiltered0p2"] = 26;
    muFilters["hltL3crIsoL1sMu20L1f0L2f10QL3f22QL3trkIsoFiltered0p09"] = 27;
    //HLT_IsoMu27_v2
    muFilters["hltL1sL1SingleMu25"] = 28; 
    muFilters["hltL2fL1sMu25L1f0L2Filtered10Q"] = 29;
    muFilters["hltL3fL1sMu25L1f0L2f10QL3Filtered27Q"] = 30;
    muFilters["hltL3crIsoL1sMu25L1f0L2f10QL3f27QL3trkIsoFiltered0p09"] = 31;
 

    phoSingleFilters["hltEG22HEFilter"]    = 0;
    phoSingleFilters["hltEG30HEFilter"]    = 1;
    phoSingleFilters["hltEG36HEFilter"]    = 2;
    phoSingleFilters["hltEG50HEFilter"]    = 3;
    phoSingleFilters["hltEG75HEFilter"]    = 4;
    phoSingleFilters["hltEG90HEFilter"]    = 5;
    phoSingleFilters["hltEG120HEFilter"]   = 6;
    phoSingleFilters["hltEG165HE10Filter"] = 7;
    phoSingleFilters["hltEG175HEFilter"]   = 8;
    phoSingleFilters["hltEG250erEtFilter"] = 9;
    phoSingleFilters["hltEG300erEtFilter"] = 10;
    phoSingleFilters["hltEG500HEFilter"]   = 11;
    phoSingleFilters["hltEG600HEFilter"]   = 12;

    //L1 seed for diphoton triggers                          
    phoDoubleFilters["hltSingleEGL1SingleEG40ORL1SingleEG25ORL1DoubleEG2210ORL1DoubleEG1510"]               = 0;
    //For path HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v      
    phoDoubleFilters["hltEG18Iso60CaloId15b35eHE10R9Id50b80eTrackIsoUnseededLastFilter"]                    = 1;
    //For path HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v                  
    phoDoubleFilters["hltEG18R9Id85b90eHE10R9Id50b80eR9pixSeedUnseededLastFilter"]                          = 2;
    phoDoubleFilters["hltEG18Iso60CaloId15b35eHE10R9Id50b80eTrackIsoUnseededpixSeedLastFilter"]             = 3;
    //For path HLT_Diphoton30_18_Solid_R9Id_AND_IsoCaloId_AND_HE_R9Id_Mass55_v               
    phoDoubleFilters["hltEG18Iso60CaloId15b35eHE10R9Id50b80eTrackIsoSolidUnseededLastFilter"]               = 4;
    phoDoubleFilters["hltEG30R9Id85b90eHE10R9Id50b80eR9IdLastFilter"]                                       = 5;
    phoDoubleFilters["hltEG30Iso60CaloId15b35eHE10R9Id50b80eEcalIsoFilter"]                                 = 6;
    //For path HLT_Diphoton30PV_18PV_R9Id_AND_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v 
    phoDoubleFilters["hltEG18PVR9Idb85e90HE10R9Id50b80eR9DoublePixelVetoUnseededLastFilter"]                = 7;
    phoDoubleFilters["hltEG18PVIso60CaloId15b35eHE10R9Id50b80eTrackIsoUnseededDoublePixelVetoLastFilter"]   = 8;
    phoDoubleFilters["hltEG30PVR9Idb85e90HE10R9Id50b80eR9IdLastFilter"]                                     = 9;
    phoDoubleFilters["hltEG30PVIso60CaloId15b35eHE10R9Id50b80eEcalIsoFilter"]                               = 10;
    //For path HLT_Diphoton30EB_18EB_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelVeto_Mass55_v
    phoDoubleFilters["hltEG18EBR9Idb85e90HE10R9Id50b80eR9DoublePixelVetoUnseededLastFilter"]                = 11;
    phoDoubleFilters["hltEG18EBIso60CaloId15b35eHE10R9Id50b80eTrackIsoUnseededDoublePixelVetoLastFilter"]   = 12;
    phoDoubleFilters["hltEG30EBR9Idb85e90HE10R9Id50b80eR9IdLastFilter"]                                     = 13;
    phoDoubleFilters["hltEG30EBIso60CaloId15b35eHE10R9Id50b80eEcalIsoFilter"]                               = 14;
    //For both HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v and HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_DoublePixelSeedMatch_Mass70_v 
    phoDoubleFilters["hltEG30LR9Id85b90eHE10R9Id50b80eR9IdLastFilter"]                                      = 15;
    phoDoubleFilters["hltEG30LIso60CaloId15b35eHE10R9Id50b80eEcalIsoLastFilter"]                            = 16;
    //For both HLT_Diphoton30_18_R9Id_OR_IsoCaloId_AND_HE_R9Id_Mass95_v and HLT_Diphoton30_18_Solid_R9Id_AND_IsoCaloId_AND_HE_R9Id_Mass55_v 
    phoDoubleFilters["hltEG18R9Id85b90eHE10R9Id50b80eR9UnseededLastFilter"]                                 = 17;
    //For path HLT_Photon26_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon16_AND_HE10_R9Id65_Eta2_Mass60_v      
    phoDoubleFilters["hltL1sL1SingleEG40ORL1SingleEG35ORL1DoubleEG2210ORL1DoubleEG1510"]                    = 18;
    phoDoubleFilters["hltEG16R9Id85HE10R9Id65R9IdUnseededLastFilter"]                                       = 19;
    phoDoubleFilters["hltEG16Iso50T80LCaloId24b40eHE10R9Id65TrackIsoUnseededLastFilter"]                    = 20;
    phoDoubleFilters["hltEG26Iso50T80LCaloId24b40eHE10R9Id65Eta2HcalIsoLastFilter"]                         = 21;
    phoDoubleFilters["hltEG26R9Id85HE10R9Id65R9IdEta2LastFilter"]                                           = 22;
    //For path HLT_Photon36_R9Id85_OR_CaloId24b40e_Iso50T80L_Photon22_AND_HE10_R9Id65_Eta2_Mass15_v    
    phoDoubleFilters["hltEG22R9Id85HE10R9Id65R9IdUnseededLastFilter"]                                       = 23;
    phoDoubleFilters["hltEG22Iso50T80LCaloId24b40eHE10R9Id65TrackIsoUnseededLastFilter"]                    = 24;
    phoDoubleFilters["hltEG36R9Id85HE10R9Id65R9IdEta2LastFilter"]                                           = 25;
    phoDoubleFilters["hltEG36Iso50T80LCaloId24b40eHE10R9Id65Eta2HcalIsoLastFilter"]                         = 26;


    jetFilters["hltSinglePFJet40"]  =  0;
    jetFilters["hltSinglePFJet60"]  =  1;
    jetFilters["hltSinglePFJet80"]  =  2;
    jetFilters["hltSinglePFJet140"] =  3;
    jetFilters["hltSinglePFJet200"] =  4;
    jetFilters["hltSinglePFJet260"] =  5;
    jetFilters["hltSinglePFJet320"] =  6;
    jetFilters["hltSinglePFJet400"] =  7;
    jetFilters["hltSinglePFJet450"] =  8;
    jetFilters["hltSinglePFJet500"] =  9;
  }
  
  // AOD vs miniAOD
  if (isAOD_) {
    edm::Handle<trigger::TriggerEvent> triggerHandle;
    e.getByToken(trgEventLabel_, triggerHandle);

    const trigger::TriggerObjectCollection& trgObjects = triggerHandle->getObjects();

    // loop over particular filters (and not over full HLTs)
    for (trigger::size_type iF = 0; iF != triggerHandle->sizeFilters(); ++iF) {
      // full filter name and its keys each corresponding to a matched (pt, eta, phi, ...) object
      string const&        label = triggerHandle->filterTag(iF).label();
      const trigger::Keys& keys  = triggerHandle->filterKeys(iF);

      std::map<string,size_t>::iterator idxEle = eleFilters.find(label);
      std::map<string,size_t>::iterator idxPhoSingle = phoSingleFilters.find(label);
      std::map<string,size_t>::iterator idxPhoDouble = phoDoubleFilters.find(label);
      std::map<string,size_t>::iterator idxMu  = muFilters.find(label);
      std::map<string,size_t>::iterator idxJet = jetFilters.find(label);

      // electron filters
      if (idxEle != eleFilters.end()) {
        size_t idx = idxEle->second;

        for (size_t iK = 0; iK < keys.size(); ++iK) {
          const trigger::TriggerObject& trgV = trgObjects.at(keys[iK]);
          trgElePt [idx].push_back(trgV.pt());
          trgEleEta[idx].push_back(trgV.eta());
          trgElePhi[idx].push_back(trgV.phi());
          trgEleIndex_.push_back(static_cast<int>(idx));
          trgElePt_.push_back(trgV.pt());
          trgEleEta_.push_back(trgV.eta());
          trgElePhi_.push_back(trgV.phi());
        }
      }

      // single photon filters
      if (idxPhoSingle != phoSingleFilters.end()) {
        size_t idx = idxPhoSingle->second;

        for (size_t iK = 0; iK < keys.size(); ++iK) {
          const trigger::TriggerObject& trgV = trgObjects.at(keys[iK]);
          trgSinglePhoPt [idx].push_back(trgV.pt());
          trgSinglePhoEta[idx].push_back(trgV.eta());
          trgSinglePhoPhi[idx].push_back(trgV.phi());
          trgSinglePhoIndex_.push_back(static_cast<int>(idx));
          trgSinglePhoPt_.push_back(trgV.pt());
          trgSinglePhoEta_.push_back(trgV.eta());
          trgSinglePhoPhi_.push_back(trgV.phi());
        }
      }

      // double photon filters
      if (idxPhoDouble != phoDoubleFilters.end()) {
        size_t idx = idxPhoDouble->second;

        for (size_t iK = 0; iK < keys.size(); ++iK) {
          const trigger::TriggerObject& trgV = trgObjects.at(keys[iK]);
          trgDoublePhoPt [idx].push_back(trgV.pt());
          trgDoublePhoEta[idx].push_back(trgV.eta());
          trgDoublePhoPhi[idx].push_back(trgV.phi());
          trgDoublePhoIndex_.push_back(static_cast<int>(idx));
          trgDoublePhoPt_.push_back(trgV.pt());
          trgDoublePhoEta_.push_back(trgV.eta());
          trgDoublePhoPhi_.push_back(trgV.phi());
        }
      }

      // muon filters
      if (idxMu != muFilters.end()) {
        size_t idx = idxMu->second;

        for (size_t iK = 0; iK < keys.size(); ++iK) {
          const trigger::TriggerObject& trgV = trgObjects.at(keys[iK]);
          trgMuPt [idx].push_back(trgV.pt());
          trgMuEta[idx].push_back(trgV.eta());
          trgMuPhi[idx].push_back(trgV.phi());
          trgMuIndex_.push_back(static_cast<int>(idx));
          trgMuPt_.push_back(trgV.pt());
          trgMuEta_.push_back(trgV.eta());
          trgMuPhi_.push_back(trgV.phi());
        }
      }

      // jet filters
      if (idxJet != jetFilters.end()) {
        size_t idx = idxJet->second;

        for (size_t iK = 0; iK < keys.size(); ++iK) {
          const trigger::TriggerObject& trgV = trgObjects.at(keys[iK]);
	  //cout<<"key : "<<label<<" "<<iK<<" "<<keys[iK]<<" "<<trgV.pt()<<" "<<trgV.eta()<<" "<<trgV.phi()<<endl;
          trgJetPt [idx].push_back(trgV.pt());
          trgJetEta[idx].push_back(trgV.eta());
          trgJetPhi[idx].push_back(trgV.phi());
          trgJetIndex_.push_back(static_cast<int>(idx));
          trgJetPt_.push_back(trgV.pt());
          trgJetEta_.push_back(trgV.eta());
          trgJetPhi_.push_back(trgV.phi());
        }
      }
    } // HLT filter loop

    return;
  } // if AOD

  //
  // miniAOD treatment
  //

  edm::Handle<pat::TriggerObjectStandAloneCollection> triggerHandleMiniAOD;
  e.getByToken(triggerObjectsLabel_, triggerHandleMiniAOD);

  edm::Handle<edm::TriggerResults> trgResultsHandle;
  e.getByToken(trgResultsLabel_, trgResultsHandle);

  const edm::TriggerNames &names = e.triggerNames(*trgResultsHandle);

  for (pat::TriggerObjectStandAlone obj : *triggerHandleMiniAOD) {
    obj.unpackPathNames(names);

    // loop over filters
    for (size_t iF = 0; iF < obj.filterLabels().size(); ++iF) {
      string label = obj.filterLabels()[iF];

      std::map<string,size_t>::iterator idxEle = eleFilters.find(label);
      std::map<string,size_t>::iterator idxPhoSingle = phoSingleFilters.find(label);
      std::map<string,size_t>::iterator idxPhoDouble = phoDoubleFilters.find(label);
      std::map<string,size_t>::iterator idxMu  = muFilters.find(label);
      std::map<string,size_t>::iterator idxJet = jetFilters.find(label);

      // electron filters
      if (idxEle != eleFilters.end()) {
        size_t idx = idxEle->second;
        trgElePt [idx].push_back(obj.pt());
        trgEleEta[idx].push_back(obj.eta());
        trgElePhi[idx].push_back(obj.phi());
        trgEleIndex_.push_back(static_cast<int>(idx));
        trgElePt_.push_back(obj.pt());
        trgEleEta_.push_back(obj.eta());
        trgElePhi_.push_back(obj.phi());
      }

      // single photon filters
      if (idxPhoSingle != phoSingleFilters.end()) {
        size_t idx = idxPhoSingle->second;
        trgSinglePhoPt [idx].push_back(obj.pt());
        trgSinglePhoEta[idx].push_back(obj.eta());
        trgSinglePhoPhi[idx].push_back(obj.phi());
        trgSinglePhoIndex_.push_back(static_cast<int>(idx));
        trgSinglePhoPt_.push_back(obj.pt());
        trgSinglePhoEta_.push_back(obj.eta());
        trgSinglePhoPhi_.push_back(obj.phi());
      }

      // double photon filters
      if (idxPhoDouble != phoDoubleFilters.end()) {
        size_t idx = idxPhoDouble->second;
        trgDoublePhoPt [idx].push_back(obj.pt());
        trgDoublePhoEta[idx].push_back(obj.eta());
        trgDoublePhoPhi[idx].push_back(obj.phi());
        trgDoublePhoIndex_.push_back(static_cast<int>(idx));
        trgDoublePhoPt_.push_back(obj.pt());
        trgDoublePhoEta_.push_back(obj.eta());
        trgDoublePhoPhi_.push_back(obj.phi());
      }

      // muon filters
      if (idxMu != muFilters.end()) {
        size_t idx = idxMu->second;
        trgMuPt [idx].push_back(obj.pt());
        trgMuEta[idx].push_back(obj.eta());
        trgMuPhi[idx].push_back(obj.phi());
        trgMuIndex_.push_back(static_cast<int>(idx));
        trgMuPt_.push_back(obj.pt());
        trgMuEta_.push_back(obj.eta());
        trgMuPhi_.push_back(obj.phi());
      }

      // jet filters
      if (idxJet != jetFilters.end()) {
        size_t idx = idxJet->second;
        trgJetPt [idx].push_back(obj.pt());
        trgJetEta[idx].push_back(obj.eta());
        trgJetPhi[idx].push_back(obj.phi());
        trgJetIndex_.push_back(static_cast<int>(idx));
        trgJetPt_.push_back(obj.pt());
        trgJetEta_.push_back(obj.eta());
        trgJetPhi_.push_back(obj.phi());
      }
    }
  }

}

Int_t ggNtuplizer::matchElectronTriggerFilters(double pt, double eta, double phi) {

  // bits in the return value correspond to decisions from filters defined above
  Int_t result = 0;

  for (size_t f = 0; f < 32; ++f)
    for (size_t v = 0; v < trgElePt[f].size(); ++v)
      if (fabs(pt - trgElePt[f][v])/trgElePt[f][v] < trgFilterDeltaPtCut_ &&
          deltaR(eta, phi, trgEleEta[f][v], trgElePhi[f][v]) < trgFilterDeltaRCut_) {
        result |= (1<<f);
        break;
      }

  return result;
}

Int_t ggNtuplizer::matchSinglePhotonTriggerFilters(double pt, double eta, double phi) {

  // bits in the return value correspond to decisions from filters defined above
  Int_t result = 0;

  for (size_t f = 0; f < 32; ++f)
    for (size_t v = 0; v < trgSinglePhoPt[f].size(); ++v)
      if (fabs(pt - trgSinglePhoPt[f][v])/trgSinglePhoPt[f][v] < trgFilterDeltaPtCut_ &&
          deltaR(eta, phi, trgSinglePhoEta[f][v], trgSinglePhoPhi[f][v]) < trgFilterDeltaRCut_) {
        result |= (1<<f);
        break;
      }

  return result;
}

Int_t ggNtuplizer::matchDoublePhotonTriggerFilters(double pt, double eta, double phi) {

  // bits in the return value correspond to decisions from filters defined above
  Int_t result = 0;

  for (size_t f = 0; f < 32; ++f)
    for (size_t v = 0; v < trgDoublePhoPt[f].size(); ++v)
      if (fabs(pt - trgDoublePhoPt[f][v])/trgDoublePhoPt[f][v] < trgFilterDeltaPtCut_ &&
          deltaR(eta, phi, trgDoublePhoEta[f][v], trgDoublePhoPhi[f][v]) < trgFilterDeltaRCut_) {
        result |= (1<<f);
        break;
      }

  return result;
}

Int_t ggNtuplizer::matchMuonTriggerFilters(double pt, double eta, double phi) {

  // bits in the return value correspond to decisions from filters defined above
  Int_t result = 0;

  for (size_t f = 0; f < 32; ++f)
    for (size_t v = 0; v < trgMuPt[f].size(); ++v)
      if (fabs(pt - trgMuPt[f][v])/trgMuPt[f][v] < trgFilterDeltaPtCut_ &&
          deltaR(eta, phi, trgMuEta[f][v], trgMuPhi[f][v]) < trgFilterDeltaRCut_) {
        result |= (1<<f);
        break;
      }

  return result;
}

Int_t ggNtuplizer::matchJetTriggerFilters(double pt, double eta, double phi) {

  // bits in the return value correspond to decisions from filters defined above
  Int_t result = 0;

  for (size_t f = 0; f < 32; ++f)
    for (size_t v = 0; v < trgJetPt[f].size(); ++v)
      if (fabs(pt - trgJetPt[f][v])/trgJetPt[f][v] < trgFilterDeltaPtCut_ &&
          deltaR(eta, phi, trgJetEta[f][v], trgJetPhi[f][v]) < trgFilterDeltaRCut_) {
        result |= (1<<f);
        break;
      }

  return result;
}

Double_t ggNtuplizer::deltaPhi(Double_t phi1, Double_t phi2) {

  Double_t dPhi = phi1 - phi2;
  if (dPhi > TMath::Pi()) dPhi -= 2.*TMath::Pi();
  if (dPhi < -TMath::Pi()) dPhi += 2.*TMath::Pi();

  return dPhi;
}

Double_t ggNtuplizer::deltaR(Double_t eta1, Double_t phi1, Double_t eta2, Double_t phi2) {

  Double_t dEta, dPhi ;
  dEta = eta1 - eta2;
  dPhi = deltaPhi(phi1, phi2);

  return sqrt(dEta*dEta+dPhi*dPhi);
}
