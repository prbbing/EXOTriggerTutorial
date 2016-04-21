//#ifndef TriggerAnalyze_TriggerEfficiencyAnalyzer_h
//#define TriggerAnalyze_TriggerEfficiencyAnalyzer_h

#include <memory>
#include <map>
#include <string>
#include <vector>
#include <sstream>

#include <fstream>
#include <iostream>
#include <algorithm>
#include <regex>
#include "TH1.h"
#include "TH2.h"
#include "TH1D.h"
#include "TH2D.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"

#include "DataFormats/Common/interface/TriggerResults.h"

#include "DataFormats/HLTReco/interface/TriggerEvent.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/PatCandidates/interface/TriggerObjectStandAlone.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidate.h"
#include "DataFormats/RecoCandidate/interface/RecoChargedCandidateFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

using namespace std;

class TriggerEfficiencyAnalyzer : public edm::EDAnalyzer
{
    public:
      void analyze (const edm::Event &, const edm::EventSetup &);
      bool match(double eta_1, double phi_1, double eta_2, double phi_2);
      vector<reco::Muon> recoMuonSelector(edm::EDGetTokenT<vector<reco::Muon>> recoMuonTag, const edm::Event &event);
      vector<reco::Muon> recoMuonMatcher(const trigger::TriggerObjectCollection Objects, vector<reco::Muon> recoMuonSet);
      void fillMuonHistogram(vector<reco::Muon> recoMuonSet, TH1D* histogram);
      vector<string> getStringSegments(string input, char delim);
      double muonVariables(const reco::Muon muon, string variable);
      TriggerEfficiencyAnalyzer (const edm::ParameterSet &);
      ~TriggerEfficiencyAnalyzer();
      virtual void endJob() override; 

      edm::Service<TFileService> fs;
    private:
      edm::EDGetTokenT<vector<reco::Muon>> recoMuonLabel_;
      edm::EDGetTokenT<edm::TriggerResults> triggerBitsLabel_;
      edm::EDGetTokenT<trigger::TriggerEvent> triggerSummaryLabel_;
      edm::InputTag filterTag_;
      edm::Handle<edm::TriggerResults> triggerBits; 
      edm::Handle<trigger::TriggerEvent> triggerSummary; 
      vector<edm::ParameterSet> histograms_;
      vector<TH1D*> vanilaHistograms;
      vector<TH1D*> filterMatchedHistograms;
      vector<TH1D*> filterEffHistograms;
};  

//#endif
