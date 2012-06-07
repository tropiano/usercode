#ifndef EfficiencyElectron_h
#define EfficiencyElectron_h

#include "TFile.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "TH1D.h"
#include "TList.h"
#include "TFile.h"
#include "MyPatAnalysis.h"
#include "TagAndProbeFillerElectron.h"

class EfficiencyElectron : public MyPatAnalysis{

	public :
		EfficiencyElectron();

		virtual ~EfficiencyElectron();

		virtual void begin(TFile* out, const edm::ParameterSet&);

		static MyPatAnalysis* create() {
			return new EfficiencyElectron();
		} 

		virtual void process(const fwlite::Event& iEvent);

		virtual void finalize(); 

		//Histograms

		TH1D *AccDenom_GenExclJetNumber, *AccDenom_GenInclJetNumber, *AccDenom_RecoExclJetNumber, *AccDenom_RecoInclJetNumber, *AccDenom_GenSingleElPt_Barrel, *AccDenom_GenSingleElPt_Endcap;

		TH1D *EffDenom_GenExclJetNumber, *EffDenom_RecoExclJetNumber, *EffDenom_RecoInclJetNumber, *EffDenom_GenInclJetNumber, *EffDenom_GenSingleElPt_Barrel, *EffDenom_GenSingleElPt_Endcap;

		TH1D *RecoInclJetEffWGA_1, *RecoInclJetEffWGA_12, *RecoInclJetEffWGA_123, *RecoInclJetEffWGA_1234, *RecoInclJetEffWGA_12345, *RecoInclJetEffWGA_123456, *RecoInclJetEffWGA_1234567;
		TH1D *RecoExclJetEffWGA_1, *RecoExclJetEffWGA_12, *RecoExclJetEffWGA_123, *RecoExclJetEffWGA_1234, *RecoExclJetEffWGA_12345, *RecoExclJetEffWGA_123456, *RecoExclJetEffWGA_1234567;

		TH1D *RecoInclJetEff_1, *RecoInclJetEff_12, *RecoInclJetEff_123, *RecoInclJetEff_1234, *RecoInclJetEff_12345, *RecoInclJetEff_123456, *RecoInclJetEff_1234567;
		TH1D *RecoExclJetEff_1, *RecoExclJetEff_12, *RecoExclJetEff_123, *RecoExclJetEff_1234, *RecoExclJetEff_12345, *RecoExclJetEff_123456, *RecoExclJetEff_1234567;

		TH1D *RecoSingleElPtEff_Barrel_1, *RecoSingleElPtEff_Barrel_12, *RecoSingleElPtEff_Barrel_123, *RecoSingleElPtEff_Barrel_1234, *RecoSingleElPtEff_Barrel_12345, *RecoSingleElPtEff_Barrel_123456, *RecoSingleElPtEff_Barrel_1234567;
		TH1D *RecoSingleElPtEff_Endcap_1, *RecoSingleElPtEff_Endcap_12, *RecoSingleElPtEff_Endcap_123, *RecoSingleElPtEff_Endcap_1234, *RecoSingleElPtEff_Endcap_12345, *RecoSingleElPtEff_Endcap_123456, *RecoSingleElPtEff_Endcap_1234567;


	private:

		std::string _sample, _selections, _JetType;

		TDirectory* _dir;

		TFile* _file;

		int _Acc, _Trg, _Qual, _Imp, _Iso, _ElID, _Conv;

		std::vector<TH1D*> _histoVector;

		std::string _RecoCutFlags[8];

		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> tag_cuts;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> tag_cuts_0;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> tag_cuts_1;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> tag_cuts_AllSel;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> tag_cuts_0_AllSel;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> tag_cuts_1_AllSel;

		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts_1;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts_12;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts_123;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts_1234;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts_12345;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts_123456;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts_1234567;

		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts0_1;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts0_12;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts0_123;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts0_1234;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts0_12345;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts0_123456;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts0_1234567;

		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts1_1;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts1_12;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts1_123;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts1_1234;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts1_12345;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts1_123456;
		std::vector<bool (*)(const reco::Candidate&, int run, double rho)> probe_cuts1_1234567;
 
		TagAndProbeFillerElectron* _TagProbe_Electron_Excl_12;
		TagAndProbeFillerElectron* _TagProbe_Electron_Excl_123;
		TagAndProbeFillerElectron* _TagProbe_Electron_Excl_1234;
		TagAndProbeFillerElectron* _TagProbe_Electron_Excl_12345;
		TagAndProbeFillerElectron* _TagProbe_Electron_Excl_123456;
		TagAndProbeFillerElectron* _TagProbe_Electron_Excl_1234567;
		TagAndProbeFillerElectron* _TagProbe_Electron_Excl_Global;
		TagAndProbeFillerElectron* _TagProbe_Electron_Excl_Double;

		TagAndProbeFillerElectron* _TagProbe_Electron0_Excl_12;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Excl_123;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Excl_1234;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Excl_12345;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Excl_123456;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Excl_1234567;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Excl_Global;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Excl_Double;

		TagAndProbeFillerElectron* _TagProbe_Electron1_Excl_12;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Excl_123;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Excl_1234;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Excl_12345;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Excl_123456;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Excl_1234567;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Excl_Global;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Excl_Double;

		TagAndProbeFillerElectron* _TagProbe_Electron_Incl_12;
		TagAndProbeFillerElectron* _TagProbe_Electron_Incl_123;
		TagAndProbeFillerElectron* _TagProbe_Electron_Incl_1234;
		TagAndProbeFillerElectron* _TagProbe_Electron_Incl_12345;
		TagAndProbeFillerElectron* _TagProbe_Electron_Incl_123456;
		TagAndProbeFillerElectron* _TagProbe_Electron_Incl_1234567;
		TagAndProbeFillerElectron* _TagProbe_Electron_Incl_Global;
		TagAndProbeFillerElectron* _TagProbe_Electron_Incl_Double;

		TagAndProbeFillerElectron* _TagProbe_Electron0_Incl_12;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Incl_123;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Incl_1234;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Incl_12345;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Incl_123456;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Incl_1234567;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Incl_Global;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Incl_Double;

		TagAndProbeFillerElectron* _TagProbe_Electron1_Incl_12;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Incl_123;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Incl_1234;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Incl_12345;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Incl_123456;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Incl_1234567;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Incl_Global;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Incl_Double;

		TagAndProbeFillerElectron* _TagProbe_Electron_Pt_Barrel_12;
		TagAndProbeFillerElectron* _TagProbe_Electron_Pt_Barrel_123;
		TagAndProbeFillerElectron* _TagProbe_Electron_Pt_Barrel_1234;
		TagAndProbeFillerElectron* _TagProbe_Electron_Pt_Barrel_12345;
		TagAndProbeFillerElectron* _TagProbe_Electron_Pt_Barrel_123456;
		TagAndProbeFillerElectron* _TagProbe_Electron_Pt_Barrel_1234567;
		TagAndProbeFillerElectron* _TagProbe_Electron_Pt_Barrel_Global;

		TagAndProbeFillerElectron* _TagProbe_Electron_Pt_Endcap_12;
		TagAndProbeFillerElectron* _TagProbe_Electron_Pt_Endcap_123;
		TagAndProbeFillerElectron* _TagProbe_Electron_Pt_Endcap_1234;
		TagAndProbeFillerElectron* _TagProbe_Electron_Pt_Endcap_12345;
		TagAndProbeFillerElectron* _TagProbe_Electron_Pt_Endcap_123456;
		TagAndProbeFillerElectron* _TagProbe_Electron_Pt_Endcap_1234567;
		TagAndProbeFillerElectron* _TagProbe_Electron_Pt_Endcap_Global;

		TagAndProbeFillerElectron* _TagProbe_Electron0_Pt_Barrel_12;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Pt_Barrel_123;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Pt_Barrel_1234;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Pt_Barrel_12345;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Pt_Barrel_123456;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Pt_Barrel_1234567;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Pt_Barrel_Global;
		
		TagAndProbeFillerElectron* _TagProbe_Electron0_Pt_Endcap_12;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Pt_Endcap_123;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Pt_Endcap_1234;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Pt_Endcap_12345;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Pt_Endcap_123456;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Pt_Endcap_1234567;
		TagAndProbeFillerElectron* _TagProbe_Electron0_Pt_Endcap_Global;
		
		TagAndProbeFillerElectron* _TagProbe_Electron1_Pt_Barrel_12;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Pt_Barrel_123;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Pt_Barrel_1234;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Pt_Barrel_12345;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Pt_Barrel_123456;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Pt_Barrel_1234567;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Pt_Barrel_Global;
		
		TagAndProbeFillerElectron* _TagProbe_Electron1_Pt_Endcap_12;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Pt_Endcap_123;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Pt_Endcap_1234;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Pt_Endcap_12345;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Pt_Endcap_123456;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Pt_Endcap_1234567;
		TagAndProbeFillerElectron* _TagProbe_Electron1_Pt_Endcap_Global;
		
		
		int _run;
		
		double _rho;
		
		bool _Norm;
		
		double _norm;
		
		int _entries, _EventsPerFile, _EventNumber, _ProcEvents;
		
		double _targetLumi;
		
		double _xsec;
		
		double _ptBinTP[11];
		double _ptBinTP0[11];
		double _ptBinTP1[11];

};

#endif

