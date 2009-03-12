#include "Firenze/BosonUtilities/interface/AddFourMomentaTransversePlane.h"
#include "DataFormats/Candidate/interface/Candidate.h"

using namespace reco;
 
void AddFourMomentaTransversePlane::set( Candidate & c ) const {
  Candidate::LorentzVector p4( 0, 0, 0, 0 );
  Candidate::Charge charge = 0;
  Candidate::iterator b = c.begin(), e = c.end(); 
  //std::cout << "Adding..." << std::endl;
  for(  Candidate::iterator d = b; d != e; ++ d ) {
    Candidate::LorentzVector p4t (d->p4().x(), d->p4().y(), 0, d->p4().t()*sin(d->p4().theta()));
    //std::cout << "original four momentum " << d->p4() << " angle " << d->p4().theta() << " transverse momentum " << p4t << std::endl;  
    p4 += p4t;
    charge += d->charge();
  }
  //std::cout << "final transverse momentum " << p4 << "  mass " << p4.mass() << "  mT " << p4.mt() << "  theta " << p4.theta() << std::endl;
  c.setP4( p4 );
  c.setCharge( charge );
}

