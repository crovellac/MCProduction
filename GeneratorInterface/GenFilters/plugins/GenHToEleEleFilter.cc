/*
Original Author:  Davide Di Croce
         Created:  Fev 2021
*/

//System include files
#include <memory>
#include <vector>

//User include files
#include "FWCore/Framework/interface/global/EDFilter.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/Math/interface/deltaR.h"

//Class declaration
class GenHToEleEleFilter : public edm::global::EDFilter<> {
public:
  explicit GenHToEleEleFilter(const edm::ParameterSet&);

private:
  bool filter(edm::StreamID, edm::Event&, const edm::EventSetup&) const override;

  //Member data
  const edm::EDGetTokenT<reco::GenParticleCollection> token_;
  const double ele1PtCut_, ele2PtCut_, eleEtaCut_, nHiggs_, eledRCut_ ;
};

//Constructor
GenHToEleEleFilter::GenHToEleEleFilter(const edm::ParameterSet& params)
    : token_(consumes<reco::GenParticleCollection>(params.getParameter<edm::InputTag>("src"))),
      ele1PtCut_(params.getParameter<double>("ele1PtCut")),
      ele2PtCut_(params.getParameter<double>("ele2PtCut")),
      eleEtaCut_(params.getParameter<double>("eleEtaCut")),
      nHiggs_(params.getParameter<double>("nHiggs")),
      eledRCut_(params.getParameter<double>("eledRCut")) {}

bool GenHToEleEleFilter::filter(edm::StreamID, edm::Event& evt, const edm::EventSetup& params) const {
  using namespace std;
  using namespace edm;
  using namespace reco;

  //Read GenParticles Collection from Event
  edm::Handle<reco::GenParticleCollection> genParticles;
  evt.getByToken(token_, genParticles);

  //Loop over GenParticles in Event
  unsigned HToEleEleCandidate = 0;
  for (reco::GenParticleCollection::const_iterator iGen = genParticles->begin(); iGen != genParticles->end(); ++iGen) {
    //Check if the particle we're looking at is a Higgs with two daughters
    if ( abs(iGen->pdgId()) != 9000036 || iGen->numberOfDaughters() != 2 ) continue;
    //Check if the two daughters are electrons
    if ( abs(iGen->daughter(0)->pdgId()) != 11 || abs(iGen->daughter(1)->pdgId()) != 11 ) continue;
    //Check if the daughters pass our cuts on pt, eta, and deltaR
    if (!( (iGen->daughter(0)->pt() > ele1PtCut_ && iGen->daughter(1)->pt() > ele2PtCut_) || (iGen->daughter(1)->pt() > ele1PtCut_ && iGen->daughter(0)->pt() > ele2PtCut_) )) continue;
    if ( iGen->daughter(0)->eta() > eleEtaCut_ || iGen->daughter(1)->eta() > eleEtaCut_ ) continue;
    float deltaR = reco::deltaR( iGen->daughter(0)->eta(), iGen->daughter(0)->phi(), iGen->daughter(1)->eta(), iGen->daughter(1)->phi());
    if ( deltaR > eledRCut_ ) continue;
    //If we've made it past all these checks, increment the number of H to Ele Ele candidates
    ++HToEleEleCandidate;
  }
  return (HToEleEleCandidate >= nHiggs_);  //Return boolean whether event passes cut values
}

// Define module as a plug-in
DEFINE_FWK_MODULE(GenHToEleEleFilter);
