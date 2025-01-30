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

//Class declaration
class GenHToAATo4EleFilter : public edm::global::EDFilter<> {
public:
  explicit GenHToAATo4EleFilter(const edm::ParameterSet&);

private:
  bool filter(edm::StreamID, edm::Event&, const edm::EventSetup&) const override;

  //Member data
  const edm::EDGetTokenT<reco::GenParticleCollection> token_;
  const double elePtCut_, eleEtaCut_, nHiggs_, eledRCut_ ;
};

//Constructor
GenHToAATo4EleFilter::GenHToAATo4EleFilter(const edm::ParameterSet& params)
    : token_(consumes<reco::GenParticleCollection>(params.getParameter<edm::InputTag>("src"))),
      elePtCut_(params.getParameter<double>("elePtCut")),
      eleEtaCut_(params.getParameter<double>("eleEtaCut")),
      nHiggs_(params.getParameter<double>("nHiggs")),
      eledRCut_(params.getParameter<double>("eledRCut")) {}

bool GenHToAATo4EleFilter::filter(edm::StreamID, edm::Event& evt, const edm::EventSetup& params) const {
  using namespace std;
  using namespace edm;
  using namespace reco;

  //Read GenParticles Collection from Event
  edm::Handle<reco::GenParticleCollection> genParticles;
  evt.getByToken(token_, genParticles);

  //Loop over all generated particles in Event
  unsigned HToEleEleCandidate = 0;
  for (reco::GenParticleCollection::const_iterator iGen = genParticles->begin(); iGen != genParticles->end(); ++iGen) {
    //Check that iGen, the particle we're looking at, is the Higgs and has two daughter particles (making it the mother particle)
    if ( abs(iGen->pdgId()) != 35 || iGen->numberOfDaughters() != 2 ) continue;
    //Check that the two daughter particles are the pseudoscalar a
    if ( abs(iGen->daughter(0)->pdgId()) != 36 || abs(iGen->daughter(1)->pdgId()) != 36 ) continue;
    //Check that the pseudoscalars' daughter particles are electrons
    if ( abs(iGen->daughter(0)->daughter(0)->pdgId()) != 11 || abs(iGen->daughter(0)->daughter(1)->pdgId()) != 11 ) continue;
    if ( abs(iGen->daughter(1)->daughter(0)->pdgId()) != 11 || abs(iGen->daughter(1)->daughter(1)->pdgId()) != 11 ) continue;
    //Check that the electrons pass our pt cut
    if ( iGen->daughter(0)->daughter(0)->pt() < elePtCut_ && iGen->daughter(0)->daughter(1)->pt() < elePtCut_ ) continue;
    if ( iGen->daughter(1)->daughter(0)->pt() < elePtCut_ && iGen->daughter(1)->daughter(1)->pt() < elePtCut_ ) continue;
    //Check that the electrons pass our eta cut
    if ( iGen->daughter(0)->daughter(0)->eta() > eleEtaCut_ || iGen->daughter(0)->daughter(1)->eta() > eleEtaCut_ ) continue;
    if ( iGen->daughter(1)->daughter(0)->eta() > eleEtaCut_ || iGen->daughter(1)->daughter(1)->eta() > eleEtaCut_ ) continue;
    //Check that the first pseudoscalar's electrons pass our dR cut
    float deltaeta = fabs(iGen->daughter(0)->daughter(0)->eta()-iGen->daughter(0)->daughter(1)->eta());
    float deltaphi = fabs(iGen->daughter(0)->daughter(0)->phi()-iGen->daughter(0)->daughter(1)->phi());
    float deltaR = sqrt(deltaeta*deltaeta+deltaphi*deltaphi);
    if ( deltaR > eledRCut_ ) continue;
    //Check that the second pseudoscalar's electrons pass our dR cut
    deltaeta = fabs(iGen->daughter(1)->daughter(0)->eta()-iGen->daughter(1)->daughter(1)->eta());
    deltaphi = fabs(iGen->daughter(1)->daughter(0)->phi()-iGen->daughter(1)->daughter(1)->phi());
    deltaR = sqrt(deltaeta*deltaeta+deltaphi*deltaphi);
    if ( deltaR > eledRCut_ ) continue;
    ++HToEleEleCandidate;
  }
  return (HToEleEleCandidate >= nHiggs_);  //Return boolean whether event passes cut values
}

// Define module as a plug-in
DEFINE_FWK_MODULE(GenHToAATo4EleFilter);
