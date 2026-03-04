#include <memory>
#include <numeric>
#include "GeneratorInterface/Core/interface/GeneratorFilter.h"
#include "GeneratorInterface/ExternalDecays/interface/ExternalDecayDriver.h"

#include "GeneratorInterface/Pythia8Interface/interface/Py8GunBase.h"

namespace gen {

  class Py8PtGunV3 : public Py8GunBase {
  public:
    Py8PtGunV3(edm::ParameterSet const&);
    ~Py8PtGunV3() override {}

    bool generatePartonsAndHadronize() override;
    const char* classname() const override;

  private:
    // PtGun particle(s) characteristics
    double fMinEta;
    double fMaxEta;
    double fMinPt;
    double fMaxPt;
    double fMinMass;
    double fMaxMass;
    bool fAddAntiParticle;
    bool fUnbiasing;
  };


  int sum(std::vector <int> dist) {
    return std::accumulate(dist.begin(), dist.end(), 0);
  }

  double max_element(std::vector <double> dist) {
      double max = 0;
      int s = dist.size();
      for (int i = 0; i < s; i++) {
          double el = dist[i];
          if (max < el){max = el;}
      }
      return max;
  }

  std::vector <double> get_inverse_pdf(std::vector <int> dist) {
    std::vector <double> invpdf(dist.size());
    double sum_hist = sum(dist);
    int s = dist.size();
    for (int i = 0; i < s; i++) {
        if (dist[i] != 0 ) {
            invpdf[i] = sum_hist / dist[i];
            //std::cout << "Bin " << i << " -> " << invpdf[i] << std::endl;
        }
        else {invpdf[i] = 1;}
    }
    double max_invpdf = max_element(invpdf);
    for (int i = 0; i < s; i++) {
        invpdf[i] = invpdf[i] / max_invpdf;
    }
    return invpdf;
}

  double lookup_mass_invpdf(double mgen, std::vector <double> m_bins, std::vector <double> m_invpdf) {
    int im = 0;
    int s1 = m_bins.size();
    int s2 = m_invpdf.size();
    for (int ib = 0; ib < s1; ib++) {
        im = ib;
        if (ib + 1 >  s2 - 1) { break; }
        if (mgen <= m_bins[ib]) { break; }
    }
    return m_invpdf[im];
  }  

  double lookup_pt_invpdf(double pTgen, std::vector <int> pT_bins, std::vector <double> pT_invpdf) {
    int ipt = 0;
    int s1 = pT_bins.size();
    int s2 = pT_invpdf.size();
    for (int ib = 0; ib < s1; ib++) {
        ipt = ib;
        if (ib + 1 >  s2 - 1) { break; }
        if (pTgen <= pT_bins[ib]) { break; }
    }
    return pT_invpdf[ipt];
  }

  double lookup_invpdf(double Mgen, std::vector <double> M_bins, double pTgen, std::vector <int> pT_bins, std::vector <double> invpdf) {
    unsigned int ibin = 0;
    unsigned int m1  = M_bins.size();
    unsigned int pt1 = pT_bins.size();
    unsigned int inv = invpdf.size();
    bool found_mass = false;
    bool found_end  = false;
    for (unsigned int ibx = 0; ibx < m1; ibx++) {
        if (found_mass || found_end) { break; }
        for (unsigned int iby = 0; iby < pt1; iby++) {
            ibin = (ibx*pt1)+ iby;
            if ( ((ibx*pt1) + iby + 1) >  (inv - 1) ) {
                found_end = true;
                break;
            }
            if ( (Mgen  <= M_bins[ibx]) && (pTgen <= pT_bins[iby]) ) {
                found_mass = true;
                break;
            }
        }
    }
    return invpdf[ibin];
  } 

  double get_rand_el(std::vector <int> dist) {
    int randomIndex = rand() % dist.size();
      return dist[randomIndex];
  }


  std::vector <int> pT_bins   = {45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160};
  std::vector <double> m_bins = {0.08, 0.15000000000000002, 0.22000000000000003, 0.29000000000000004, 0.36000000000000004, 0.43000000000000005, 0.5, 0.5700000000000001, 0.6400000000000001, 0.7100000000000001, 0.78, 0.8500000000000001, 0.9200000000000002, 0.9900000000000001, 1.06, 1.1300000000000001, 1.2000000000000002};

  std::vector <int> occ = {
//  45,     50,     55,     60,     65,     70,     75,     80,     85,     90,     95,     100,     105,     110,     115,     120,     125,     130,     135,     140,     145,     150,     155,     160
36, 32, 32, 30, 26, 26, 42, 30, 30, 28, 34, 34, 28, 36, 36, 46, 46, 46, 34, 32, 32, 26, 46, 46, // -> 0.08
48, 38, 38, 38, 44, 44, 48, 38, 38, 44, 52, 52, 34, 42, 42, 36, 46, 46, 46, 36, 36, 28, 40, 40, // -> 0.15000000000000002
58, 44, 44, 58, 30, 30, 32, 56, 56, 40, 58, 58, 28, 30, 30, 58, 44, 44, 34, 40, 40, 52, 38, 38, // -> 0.22000000000000003
52, 48, 48, 54, 40, 40, 50, 32, 32, 50, 36, 36, 52, 46, 46, 34, 44, 44, 32, 28, 28, 32, 28, 28, // -> 0.29000000000000004
66, 72, 72, 60, 56, 56, 68, 44, 44, 50, 54, 54, 38, 36, 36, 26, 46, 46, 42, 34, 34, 44, 38, 38, // -> 0.36000000000000004
88, 74, 74, 52, 52, 52, 62, 42, 42, 42, 60, 60, 36, 52, 52, 42, 44, 44, 40, 32, 32, 44, 26, 26, // -> 0.43000000000000005
86, 82, 82, 80, 70, 70, 58, 52, 52, 62, 50, 50, 46, 36, 36, 54, 52, 52, 50, 44, 44, 52, 40, 40, // -> 0.5
96, 96, 96, 76, 68, 68, 64, 36, 36, 42, 56, 56, 24, 62, 62, 64, 44, 44, 60, 34, 34, 32, 44, 44, // -> 0.5700000000000001
108, 100, 100, 116, 80, 80, 68, 56, 56, 80, 76, 76, 44, 54, 54, 54, 56, 56, 40, 70, 70, 40, 48, 48, // -> 0.6400000000000001
134, 108, 108, 118, 76, 76, 66, 70, 70, 82, 52, 52, 58, 62, 62, 68, 44, 44, 58, 34, 34, 56, 64, 64, // -> 0.7100000000000001
142, 108, 108, 90, 106, 106, 92, 108, 108, 74, 74, 74, 64, 52, 52, 72, 56, 56, 42, 46, 46, 58, 34, 34, // -> 0.78
188, 128, 128, 134, 124, 124, 122, 72, 72, 76, 80, 80, 46, 62, 62, 62, 34, 34, 62, 62, 62, 68, 38, 38, // -> 0.8500000000000001
152, 126, 126, 180, 156, 156, 112, 102, 102, 78, 96, 96, 82, 88, 88, 42, 58, 58, 58, 54, 54, 66, 68, 68, // -> 0.9200000000000002
146, 174, 174, 148, 100, 100, 156, 110, 110, 106, 104, 104, 82, 90, 90, 84, 62, 62, 70, 60, 60, 56, 58, 58, // -> 0.9900000000000001
128, 158, 158, 160, 136, 136, 146, 116, 116, 92, 80, 80, 62, 112, 112, 96, 74, 74, 72, 60, 60, 68, 48, 48, // -> 1.06
190, 184, 184, 146, 108, 108, 138, 142, 142, 116, 98, 98, 94, 80, 80, 68, 80, 80, 48, 62, 62, 56, 60, 60, // -> 1.1300000000000001
152, 158, 158, 156, 148, 148, 150, 134, 134, 126, 110, 110, 104, 92, 92, 84, 80, 80, 86, 72, 72, 64, 80, 80 // -> 1.2000000000000002
  };


  // implementation
  //
  Py8PtGunV3::Py8PtGunV3(edm::ParameterSet const& ps) : Py8GunBase(ps) {
    // ParameterSet defpset ;
    edm::ParameterSet pgun_params = ps.getParameter<edm::ParameterSet>("PGunParameters");  // , defpset ) ;
    fMinEta = pgun_params.getParameter<double>("MinEta");                                  // ,-2.2);
    fMaxEta = pgun_params.getParameter<double>("MaxEta");                                  // , 2.2);
    fMinPt = pgun_params.getParameter<double>("MinPt");                                    // ,  0.);
    fMaxPt = pgun_params.getParameter<double>("MaxPt");                                    // ,  0.);
    fMinMass = pgun_params.getParameter<double>("MinMass");                                // ,  0.);
    fMaxMass = pgun_params.getParameter<double>("MaxMass");                                // ,  0.);
    fAddAntiParticle = pgun_params.getParameter<bool>("AddAntiParticle");                  //, false) ;
    fUnbiasing = pgun_params.getParameter<bool>("Unbiasing");                             //, false) ;
  }

  bool Py8PtGunV3::generatePartonsAndHadronize() {
    fMasterGen->event.reset();
    std::vector <double> invpdf = get_inverse_pdf(occ);

    int NTotParticles = fPartIDs.size();
    if (fAddAntiParticle)
      NTotParticles *= 2;

    // energy below is dummy, it is not used
    (fMasterGen->event).append(990, -11, 0, 0, 2, 1 + NTotParticles, 0, 0, 0., 0., 0., 15000., 15000.);

    for (size_t i = 0; i < fPartIDs.size(); i++) {
      int particleID = fPartIDs[i];  // this is PDG - need to convert to Py8 ???

      double phi = (fMaxPhi - fMinPhi) * randomEngine().flat() + fMinPhi;
      double eta = (fMaxEta - fMinEta) * randomEngine().flat() + fMinEta;
      double the = 2. * atan(exp(-eta));

      double pt = (fMaxPt - fMinPt) * randomEngine().flat() + fMinPt;

      //double mass = (fMasterGen->particleData).m0(particleID);
      double mass = (fMaxMass-fMinMass) * randomEngine().flat() + fMinMass;

      double rand_sampler = rand() / double(RAND_MAX);
      double weight       = lookup_invpdf(mass, m_bins, pt, pT_bins, invpdf);
      if (fUnbiasing) {
        while ( rand_sampler > weight ) {
           rand_sampler = rand() / double(RAND_MAX);
           pt           = (fMaxPt-fMinPt) * randomEngine().flat() + fMinPt;
           mass         = (fMaxMass-fMinMass) * randomEngine().flat() + fMinMass;
           weight       = lookup_invpdf(mass, m_bins, pt, pT_bins, invpdf);
        }
      }

      double pp = pt / sin(the);  // sqrt( ee*ee - mass*mass );
      double ee = sqrt(pp * pp + mass * mass);

      double px = pt * cos(phi);
      double py = pt * sin(phi);
      double pz = pp * cos(the);

      if (!((fMasterGen->particleData).isParticle(particleID))) {
        particleID = std::abs(particleID);
      }
      if (1 <= std::abs(particleID) && std::abs(particleID) <= 6)  // quarks
        (fMasterGen->event).append(particleID, 23, 1, 0, 0, 0, 101, 0, px, py, pz, ee, mass);
      else if (std::abs(particleID) == 21)  // gluons
        (fMasterGen->event).append(21, 23, 1, 0, 0, 0, 101, 102, px, py, pz, ee, mass);
      // other
      else {
        (fMasterGen->event).append(particleID, 1, 1, 0, 0, 0, 0, 0, px, py, pz, ee, mass);
        int eventSize = (fMasterGen->event).size() - 1;
        // -log(flat) = exponential distribution
        double tauTmp = -(fMasterGen->event)[eventSize].tau0() * log(randomEngine().flat());
        (fMasterGen->event)[eventSize].tau(tauTmp);
      }

      // Here also need to add anti-particle (if any)
      // otherwise just add a 2nd particle of the same type
      // (for example, gamma)
      //
      if (fAddAntiParticle) {
        if (1 <= std::abs(particleID) && std::abs(particleID) <= 6) {  // quarks
          (fMasterGen->event).append(-particleID, 23, 1, 0, 0, 0, 0, 101, -px, -py, -pz, ee, mass);
        } else if (std::abs(particleID) == 21) {  // gluons
          (fMasterGen->event).append(21, 23, 1, 0, 0, 0, 102, 101, -px, -py, -pz, ee, mass);
        } else {
          if ((fMasterGen->particleData).isParticle(-particleID)) {
            (fMasterGen->event).append(-particleID, 1, 1, 0, 0, 0, 0, 0, -px, -py, -pz, ee, mass);
          } else {
            (fMasterGen->event).append(particleID, 1, 1, 0, 0, 0, 0, 0, -px, -py, -pz, ee, mass);
          }
          int eventSize = (fMasterGen->event).size() - 1;
          // -log(flat) = exponential distribution
          double tauTmp = -(fMasterGen->event)[eventSize].tau0() * log(randomEngine().flat());
          (fMasterGen->event)[eventSize].tau(tauTmp);
        }
      }
    }

    if (!fMasterGen->next())
      return false;
    evtGenDecay();

    event() = std::make_unique<HepMC::GenEvent>();
    return toHepMC.fill_next_event(fMasterGen->event, event().get());
  }

  const char* Py8PtGunV3::classname() const { return "Py8PtGunV3"; }

  typedef edm::GeneratorFilter<gen::Py8PtGunV3, gen::ExternalDecayDriver> Pythia8PtGunV3;

}  // namespace gen

using gen::Pythia8PtGunV3;
DEFINE_FWK_MODULE(Pythia8PtGunV3);
