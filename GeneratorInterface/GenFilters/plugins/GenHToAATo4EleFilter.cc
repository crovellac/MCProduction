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
  //std::cout << "Checking genParticles" << std::endl;
  for (reco::GenParticleCollection::const_iterator iGen = genParticles->begin(); iGen != genParticles->end(); ++iGen) {

    //std::cout << "PDG ID of iGen: " << iGen->pdgId() << std::endl;
    //std::cout << "Number of daughters: " << iGen->numberOfDaughters() << std::endl;
    /*   
    if (abs(iGen->pdgId()) == 35) {
	std::cout << "Particle is Higgs" << std::endl;
	std::cout << "Higgs has " << iGen->numberOfDaughters() << " daughters" << std::endl;
	if (iGen->numberOfDaughters() == 2) {
            std::cout << "PDG ID of daughter 0: " << iGen->daughter(0)->pdgId() << std::endl;
            std::cout << "PDG ID of daughter 1: " << iGen->daughter(1)->pdgId() << std::endl;


	    if (abs(iGen->daughter(0)->daughter(0)->pdgId()) == 11) {
                 std::cout << "Electron 0-0 pt: " << iGen->daughter(0)->daughter(0)->pt() << std::endl;
		 std::cout << "Electron 0-1 pt: " << iGen->daughter(0)->daughter(1)->pt() << std::endl;
	         std::cout << "Electron 1-0 pt: " << iGen->daughter(1)->daughter(0)->pt() << std::endl;
		 std::cout << "Electron 1-1 pt: " << iGen->daughter(1)->daughter(1)->pt() << std::endl;
		 std::cout << "ele_pt_cut: " << elePtCut_ << std::endl;

                 std::cout << "Electron 0-0 eta: " << iGen->daughter(0)->daughter(0)->eta() << std::endl;
                 std::cout << "Electron 0-1 eta: " << iGen->daughter(0)->daughter(1)->eta() << std::endl;
                 std::cout << "Electron 1-0 eta: " << iGen->daughter(1)->daughter(0)->eta() << std::endl;
                 std::cout << "Electron 1-1 eta: " << iGen->daughter(1)->daughter(1)->eta() << std::endl;
                 std::cout << "ele_eta_cut: " << eleEtaCut_ << std::endl;


		 float deltaeta = fabs(iGen->daughter(0)->daughter(0)->eta()-iGen->daughter(0)->daughter(1)->eta());
                 float deltaphi = fabs(iGen->daughter(0)->daughter(0)->phi()-iGen->daughter(0)->daughter(1)->phi());
                 float deltaR = sqrt(deltaeta*deltaeta+deltaphi*deltaphi);
                 std::cout << "dR between 0-0 and 0-1: " << deltaR << std::endl;
                 deltaeta = fabs(iGen->daughter(1)->daughter(0)->eta()-iGen->daughter(1)->daughter(1)->eta());
                 deltaphi = fabs(iGen->daughter(1)->daughter(0)->phi()-iGen->daughter(1)->daughter(1)->phi());
                 deltaR = sqrt(deltaeta*deltaeta+deltaphi*deltaphi);
                 std::cout << "dR between 1-0 and 1-1: " << deltaR << std::endl;
                 std::cout << "ele_dR_cut: " << eledRCut_ << std::endl;
                 std::cout << "****************************************" << std::endl;
	    }
	}
    }
    */
    //Check that iGen, the particle we're looking at, is the Higgs and has two daughter particles (making it the mother particle)
    if ( abs(iGen->pdgId()) != 35 || iGen->numberOfDaughters() != 2 ) continue;
    std::cout << "Point A" << std::endl;
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
    std::cout << "Added HToEleEleCandidate" << std::endl;
  }
  std::cout << "Did event pass selection?: " << (HToEleEleCandidate >= nHiggs_) << std::endl;
  std::cout << "num HToEleEleCandidates: " << HToEleEleCandidate << std::endl;
  return (HToEleEleCandidate >= nHiggs_);  //Return boolean whether event passes cut values
}

// Define module as a plug-in
DEFINE_FWK_MODULE(GenHToAATo4EleFilter);
