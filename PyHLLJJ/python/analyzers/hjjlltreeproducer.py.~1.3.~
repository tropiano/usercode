from CMGTools.RootTools.analyzers.TreeAnalyzer import TreeAnalyzer
import math

class hjjlltreeproducer( TreeAnalyzer ):
    '''Tree producer for the HZ, H->tt  analysis.'''

    def declareVariables(self):

        def var( varName ):
            self.tree.addVar('float', varName)

        def genparticleVars( pName ):
            var('g_{pName}Mass'.format(pName=pName))
            var('g_{pName}Pt'.format(pName=pName))
            var('g_{pName}Energy'.format(pName=pName))
            var('g_{pName}Eta'.format(pName=pName))
            var('g_{pName}Phi'.format(pName=pName))

        def jetVars( pName ):
            var('{pName}Mass'.format(pName=pName))
            var('{pName}Pt'.format(pName=pName))
            var('{pName}Energy'.format(pName=pName))
            var('{pName}Eta'.format(pName=pName))
            var('{pName}Phi'.format(pName=pName))
            var('{pName}Ntrk'.format(pName=pName))
            var('{pName}ChFraction'.format(pName=pName))
            var('{pName}PFraction'.format(pName=pName))
            var('{pName}EFraction'.format(pName=pName))
            var('{pName}NHFraction'.format(pName=pName))
            var('{pName}btag'.format(pName=pName))
        
        def particleVars( pName ):
            var('{pName}Mass'.format(pName=pName))
            var('{pName}Pt'.format(pName=pName))
            var('{pName}Energy'.format(pName=pName))
            var('{pName}Eta'.format(pName=pName))
            var('{pName}Phi'.format(pName=pName))
            var('{pName}Charge'.format(pName=pName))
        
            
        jetVars('j1rec')
        jetVars('j2rec')
        jetVars('j3rec')
        jetVars('j4rec')

        particleVars('mu1rec')
        particleVars('mu2rec')
        particleVars('e1rec')
        particleVars('e2rec')

        particleVars('hmumujj')
        particleVars('hmumu')
        particleVars('hjj')

        particleVars('heejj')
        particleVars('hee')

        particleVars('trueZhad')

        particleVars('hmumujjrefit')

        var('step')
        var('njets')

        var('deltaeta')
        var('cosdeltaphi')
        var('cosdeltaphigen')
        var('mjj')
        var('dimuonmass')
        var('dielectronmass')
        var('dimuonTrigger')
        var('dielectronTrigger')
        var('minDeltaPhiLJ')
        
        self.tree.book()


    def process(self, iEvent, event):

        def fill( varName, value ):
            setattr( self.tree.s, varName, value )

        def fJetVars( pName, particle ):
            fill('{pName}Mass'.format(pName=pName), particle.mass() )
            fill('{pName}Pt'.format(pName=pName), particle.pt() )
            fill('{pName}Energy'.format(pName=pName), particle.energy() )
            fill('{pName}Ntrk'.format(pName=pName), particle.component(1).number() + particle.component(2).number() + particle.component(3).number())
            fill('{pName}ChFraction'.format(pName=pName), particle.component(1).fraction() + particle.component(2).fraction() + particle.component(3).fraction())
            fill('{pName}EFraction'.format(pName=pName), particle.component(2).fraction() )
            fill('{pName}PFraction'.format(pName=pName), particle.component(4).fraction() )
            fill('{pName}NHFraction'.format(pName=pName), particle.component(5).fraction() )
            fill('{pName}Eta'.format(pName=pName), particle.eta() )
            fill('{pName}btag'.format(pName=pName), particle.btag(7))
#
        def fParticleVars( pName, particle ):
            fill('{pName}Mass'.format(pName=pName), particle.mass() )
            fill('{pName}Pt'.format(pName=pName), particle.pt() )
            fill('{pName}Energy'.format(pName=pName), particle.energy() )
            fill('{pName}Eta'.format(pName=pName), particle.eta() )
            fill('{pName}Phi'.format(pName=pName), particle.phi() )
            fill('{pName}Charge'.format(pName=pName), particle.charge() )

        def fgenParticleVars( pName, particle ):
            fill('g_{pName}Mass'.format(pName=pName), particle.mass() )
            fill('g_{pName}Pt'.format(pName=pName), particle.pt() )
            fill('g_{pName}Energy'.format(pName=pName), particle.energy() )
            fill('g_{pName}Eta'.format(pName=pName), particle.eta() )
#
        subevent = getattr( event, self.cfg_ana.anaName )
        fill('step',subevent.step)
        if len(subevent.alljets)>0:
          fJetVars('j1rec', subevent.alljets[0])
        if len(subevent.alljets)>1:
          fJetVars('j2rec', subevent.alljets[1])
        if len(subevent.alljets)>2:
          fJetVars('j3rec', subevent.alljets[2])
        if len(subevent.alljets)>3:  
          fJetVars('j4rec', subevent.alljets[3])  
        fill('cosdeltaphigen',math.cos(subevent.genVBFdeltaPhi))
        fill('njets',len(subevent.alljets))
        fill('dimuonTrigger', subevent.dimuonTrigger)
        fill('dielectronTrigger', subevent.dielectronTrigger)

        if (len(subevent.truezhad)):
          fParticleVars('trueZhad', subevent.truezhad[0])

        if len(subevent.leadingelectrons) > 0:
          fParticleVars('e1rec', subevent.leadingelectrons[0])
        if len(subevent.leadingelectrons) > 1:
          fParticleVars('e2rec', subevent.leadingelectrons[1])

        if len(subevent.leadingmuons) > 0:
          fParticleVars('mu1rec', subevent.leadingmuons[0])
        if len(subevent.leadingmuons) > 1:
          fParticleVars('mu2rec', subevent.leadingmuons[1])   
       
        if (subevent.step >= 2):
          fill('deltaeta', subevent.deltaeta)
          fill('mjj', subevent.mjj)
          if subevent.deltaphi > -99:
            fill('cosdeltaphi', math.cos(subevent.deltaphi))
        if (subevent.step >= 3):
          fill('dimuonmass', subevent.dimuonmass)
          fill('dielectronmass', subevent.dielectronmass)

        if len(subevent.hmumujj) > 0:
          fParticleVars('hmumujj', subevent.hmumujj[0])
          fParticleVars('hmumu', subevent.mumu[0])
          fParticleVars('hjj', subevent.jj[0])


        if len(subevent.heejj) > 0:
          fParticleVars('heejj', subevent.heejj[0])
          fParticleVars('hee', subevent.ee[0])
          fParticleVars('hjj', subevent.jj[0])  
        
        if len(subevent.hbest) > 0:
          fParticleVars('hmumujjrefit', subevent.hbest[0])
          fill('minDeltaPhiLJ', subevent.deltaPhiLJ[0])
        
          

        self.tree.fill()
