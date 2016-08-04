#include "TriggerEfficiencyAnalyzer.h"
#include <array>
using namespace std;
using namespace reco;
using namespace edm;

//An analyzer to get the hlt/reco turn on curves. wrt gen
TriggerEfficiencyAnalyzer::TriggerEfficiencyAnalyzer (const edm::ParameterSet &cfg) :
  recoMuonLabel_ (consumes<vector<reco::Muon>>(cfg.getParameter<edm::InputTag> ("recoMuonLabel"))),
  triggerSummaryLabel_ (consumes<trigger::TriggerEvent>(cfg.getParameter<edm::InputTag> ("triggerSummaryLabel"))),
  //Get the inputTag of the filter from the config filter
  filterTag_ (cfg.getParameter<edm::InputTag> ("filterTag")),
  histograms_ (cfg.getParameter<vector<edm::ParameterSet>>("histograms"))
 {
  TH1D::SetDefaultSumw2 ();  
  for(uint i =0; i < histograms_.size(); i++)
    {
      TH1D* tmpHist =  fs->make<TH1D>(TString(histograms_[i].getParameter<string>("variable")),TString(histograms_[i].getParameter<string>("variable")),int(histograms_[i].getParameter<int>("nBins")),histograms_[i].getParameter<double>("lBin"),histograms_[i].getParameter<double>("hBin"));     
      vanilaHistograms.push_back(tmpHist);  
      TH1D* tmpMatchedHist = fs->make<TH1D>(TString(histograms_[i].getParameter<string>("variable") + "_filterMatched"),TString(histograms_[i].getParameter<string>("variable") + "_filterMatched"),int(histograms_[i].getParameter<int>("nBins")),histograms_[i].getParameter<double>("lBin"),histograms_[i].getParameter<double>("hBin"));
      filterMatchedHistograms.push_back(tmpMatchedHist);  
      TH1D* tmpEffHist = fs->make<TH1D>(TString(histograms_[i].getParameter<string>("variable") + "_filterEff"),TString(histograms_[i].getParameter<string>("variable") + "_filterEff"),int(histograms_[i].getParameter<int>("nBins")),histograms_[i].getParameter<double>("lBin"),histograms_[i].getParameter<double>("hBin"));
      filterEffHistograms.push_back(tmpEffHist);  
    } 
 }

TriggerEfficiencyAnalyzer::~TriggerEfficiencyAnalyzer ()

{
}

void
TriggerEfficiencyAnalyzer::analyze(const edm::Event &event, const edm::EventSetup &setup)
{  
  //Collect the trigger filter objects.
  trigger::TriggerObjectCollection filterObjects = filterFinder(triggerSummaryLabel_, filterTag_, event);
  
  //Select reco muons to be considered, the denominator. 
  vector<reco::Muon> selectedRecoMuon = recoMuonSelector(recoMuonLabel_, event);
  
  //Collect reco muons that can be matched to the trigger filters.
  vector<reco::Muon> filterMatchedRecoMuon = recoMuonMatcher(filterObjects, selectedRecoMuon); 
 
  //Fill the histograms.
  for(uint j = 0; j < vanilaHistograms.size(); j++)
    {
      fillMuonHistogram(selectedRecoMuon, vanilaHistograms[j]);    
      fillMuonHistogram(filterMatchedRecoMuon, filterMatchedHistograms[j]);
      fillMuonHistogram(filterMatchedRecoMuon, filterEffHistograms[j]);
    }
  filterMatchedRecoMuon.clear();
  selectedRecoMuon.clear();
}

//find the filters
trigger::TriggerObjectCollection 
TriggerEfficiencyAnalyzer::filterFinder(edm::EDGetTokenT<trigger::TriggerEvent> triggerSummaryLabel, edm::InputTag filterTag, const edm::Event &event)
{
  event.getByToken(triggerSummaryLabel, triggerSummary);
  trigger::TriggerObjectCollection allTriggerObjects = triggerSummary->getObjects(); 
  //filterTag_ is the inputTag of the filter you want to match
  size_t filterIndex = (*triggerSummary).filterIndex(filterTag);
  trigger::TriggerObjectCollection filterObjects;
  if(filterIndex < (*triggerSummary).sizeFilters())
    { 
      const trigger::Keys &keysObjects = (*triggerSummary).filterKeys(filterIndex);
      for(size_t j = 0; j < keysObjects.size(); j++)
        {
          trigger::TriggerObject foundObject = (allTriggerObjects)[keysObjects[j]];
          filterObjects.push_back(foundObject);
        }
    }
  std::cout<<filterObjects.size()<<endl; 
  return filterObjects;
}

//DeltaR match
bool 
TriggerEfficiencyAnalyzer::match(double eta_1, double phi_1, double eta_2, double phi_2)
{
  bool matched = deltaR(eta_1,phi_1,eta_2,phi_2) < 0.5 ? true:false;
  return matched;
}
//Fill all the histograms.
void
TriggerEfficiencyAnalyzer::fillMuonHistogram(vector<reco::Muon> recoMuonSet, TH1D* histogram)
{
  for (unsigned int j = 0 ; j < recoMuonSet.size() ; j++)
    {
       vector<string> name = getStringSegments(string(histogram->GetName()), '_');
       double value = muonVariables(recoMuonSet[j], name[0]);
       histogram->Fill(value);
    }
}
//Map the variables to plot in the histograms to variables of reco muon 
double
TriggerEfficiencyAnalyzer::muonVariables(const reco::Muon muon, string variable)
{
  double value = 0;
  if(variable == "Pt") value = muon.pt(); 
  if(variable == "Eta") value = muon.eta(); 
  if(variable == "Phi") value = muon.phi(); 
  return value;
}
//Select muons to be considered, the denominator
vector<reco::Muon> 
TriggerEfficiencyAnalyzer::recoMuonSelector(edm::EDGetTokenT<vector<reco::Muon>> recoMuonTag, const edm::Event &event)
{
  vector<reco::Muon> selectedRecoMuon;
  edm::Handle<reco::MuonCollection> MuonCollection;
  if(event.getByToken(recoMuonTag, MuonCollection))
    {
      for (reco::MuonCollection::const_iterator recoMuon = MuonCollection ->begin(); recoMuon != MuonCollection->end(); recoMuon++) 
        {
          if(recoMuon->pt() > 10 && abs(recoMuon->eta()) < 2.1) 
            selectedRecoMuon.push_back(*recoMuon);
        }  
    }
  return selectedRecoMuon;
  selectedRecoMuon.clear();
}
//Find muons that can be matched to the filter, numerator
vector<reco::Muon>
TriggerEfficiencyAnalyzer::recoMuonMatcher(const trigger::TriggerObjectCollection Objects, vector<reco::Muon> recoMuonSet)
{
  vector<reco::Muon> matchedRecoMuon;
  if(recoMuonSet.size()) 
    {
      for (unsigned int j = 0 ; j < recoMuonSet.size() ; j++)
        {
          for(unsigned int i = 0 ; i < Objects.size() ; i++)
            {                 
              if(match(recoMuonSet[j].eta(),recoMuonSet[j].phi(),Objects[i].eta(),Objects[i].phi()))
                {
                  matchedRecoMuon.push_back(recoMuonSet[j]);
                  break;
                }
            }  
         }
     }
  return matchedRecoMuon;
  matchedRecoMuon.clear();
}

vector<string>
TriggerEfficiencyAnalyzer::getStringSegments(string input, char delim)
{
    vector<string> elems;
    std::stringstream ss(input);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

void
TriggerEfficiencyAnalyzer::endJob()
{
  //Get the efficiency histograms. Here is also the place to rebin the histograms if needed. Also one can use TEfficiency instead of histograms.
  for(uint m = 0; m < filterEffHistograms.size(); m++)
    {
      filterEffHistograms[m]->Sumw2();
      filterEffHistograms[m]->Divide(filterEffHistograms[m], vanilaHistograms[m],1,1,"B"); 
    }
}
DEFINE_FWK_MODULE(TriggerEfficiencyAnalyzer);
