# corsika2root

This converter returns a ROOT file from the information output from Corsika. It takes the DATxxxxxx as an argument, and outputs the information in a more friendly way. 


## Requirements 

-- Corsika environment ($COAST_DIR set to corsika path)
-- ROOT (currently only tested on ROOT6) 
-- GCC


## Information available in Corsika

The ROOT output can be customised for the specific use. For our current purposes, only the basic information has been included. However, here is a full list of the information available in each header which can be added as required. 

### Run header

CREAL   GetRunID () const
CINT    GetDateStart () const
CREAL   GetVersion () const
CINT    GetNObservationLevels () const
CREAL   GetObservationHeight (int index) const
CREAL   GetSpectralSlope () const
CREAL   GetEMin () const
CREAL   GetEMax () const
CREAL   GetFlagEGS4 () const
CREAL   GetFlagNKG () const
CREAL   GetCutoffHadrons () const
CREAL   GetCutoffMuons () const
CREAL   GetCutoffElectrons () const
CREAL   GetCutoffPhotons () const
CREAL   GetSamplingPlanePointX () const
CREAL   GetSamplingPlanePointY () const
CREAL   GetSamplingPlanePointZ () const
CREAL   GetSamplingPlaneTheta () const
CREAL   GetSamplingPlanePhi () const
double  GetSamplingPlaneNormalX () const
double  GetSamplingPlaneNormalY () const
double  GetSamplingPlaneNormalZ () const
CREAL   GetRotationAngle () const
CINT    GetNumberOfShowers () const
CREAL   GetAtmosphereLayerBoundary (int index) const
CREAL   GetAtmosphereA (int index) const
CREAL   GetAtmosphereB (int index) const
CREAL   GetAtmosphereC (int index) const
CREAL   GetVerticalDepth (const CREAL heightAboveSeaLevel) const
CREAL   GetConstNFLAIN () const
CREAL   GetConstNFLDIF () const
CREAL   GetConstNFLPI0 () const
CREAL   GetConstNFLPIF () const
CREAL   GetConstNFLCHE () const
CREAL   GetConstNFRAGM () const

### Shower header

CINT    GetEventNumber () const
CREAL   GetParticleId () const
CREAL   GetEnergy () const
CREAL   GetStartingAltitude () const
CREAL   GetFirstTarget () const
CREAL   GetZFirst () const
CREAL   GetPx () const
CREAL   GetPy () const
CREAL   GetPz () const
CREAL   GetTheta () const
CREAL   GetPhi () const
CINT    GetNRandomSequences () const
CINT    GetSeed (int index) const
CINT    GetInitialCallsMod (int index) const
CINT    GetInitialCallsDiv (int index) const
CREAL   GetRunNumber () const
CINT    GetDateStart () const
CREAL   GetVersion () const
CINT    GetNObservationLevels () const
CREAL   GetObservationHeight (int index) const
CREAL   GetSpectralSlope () const
CREAL   GetEMin () const
CREAL   GetEMax () const
CREAL   GetCutoffHadrons () const
CREAL   GetCutoffMuons () const
CREAL   GetCutoffElectrons () const
CREAL   GetCutoffPhotons () const
CREAL   GetNFLAIN () const
CREAL   GetNFLDIF () const
CREAL   GetNFLPI0 () const
CREAL   GetNFLPIF () const
CREAL   GetNFLCHE () const
CREAL   GetNFRAGM () const
CREAL   GetBx () const
CREAL   GetBz () const
CREAL   GetFlagEGS4 () const
CREAL   GetFlagNKG () const
CREAL   GetHadronicLowEModell () const
CREAL   GetHadronicHighEModell () const
CREAL   GetFlagCherenkov () const
CREAL   GetFlagNeutrino () const
CREAL   GetFlagCurved () const
CINT    GetFlagComputer () const        1: IBM, 2: Transputer, 3: DEC/UNIX, 4: Mac, 5: VAX/VMS, 6: GNU/Linux
CREAL   GetThetaMin () const
CREAL   GetThetaMax () const
CREAL   GetPhiMin () const
CREAL   GetPhiMax () const
CREAL   GetCherenkovBunch () const
CREAL   GetCherenkovNumberX () const
CREAL   GetCherenkovNumberY () const
CREAL   GetCherenkovGridX () const
CREAL   GetCherenkovGridY () const
CREAL   GetCherenkovDetectorX () const
CREAL   GetCherenkovDetectorY () const
CREAL   GetCherenkovOutputFlag () const
CREAL   GetArrayRotation () const
CREAL   GetFlagExtraMuonInformation () const
CREAL   GetMultipleScatteringStep () const
CREAL   GetCherenkovBandwidthMin () const
CREAL   GetCherenkovBandwidthMax () const
CINT    GetNUsesOfEvent () const
CREAL   GetCherenkovCoreX (int index) const
CREAL   GetCherenkovCoreY (int index) const
CREAL   GetFlagSIBYLL () const
CREAL   GetFlagSIBYLLCross () const
CREAL   GetFlagQGSJET () const
CREAL   GetFlagQGSJETCross () const
CREAL   GetFlagDPMJET () const
CREAL   GetFlagDPMJETCross () const
CREAL   GetFlagVENUSCross () const
CREAL   GetFlagMuonMultiple () const
CREAL   GetNKGRadialRange () const
CREAL   GetEFractionThinningH () const
CREAL   GetEFractionThinningEM () const
CREAL   GetWMaxHadronic () const
CREAL   GetWMaxEM () const
CREAL   GetRMaxThinning () const
CREAL   GetInnerAngle () const
CREAL   GetOuterAngle () const
CREAL   GetTransitionEnergy () const
CREAL   GetSkimmingIncidence () const
CREAL   GetSkimmingAltitude () const
CREAL   GetStartingHeight () const

### Data Header

CREAL   GetWeight () const
virtual std::string GetParticleName () const
int     GetParticleID () const
int     GetParticleId () const
bool    IsParticle () const
bool    IsNucleus () const
bool    IsCherenkov () const
bool    IsMuonProductionInfo () const
bool    IsEmpty () const
ParticleType    GetType () const
virtual int     GetObservationLevel () const
virtual int     GetHadronicGeneration () const
virtual CREAL   GetPx () const
virtual CREAL   GetPy () const
virtual CREAL   GetPz () const
virtual CREAL   GetX () const
virtual CREAL   GetY () const
virtual CREAL   GetTime () const
double  GetMass () const        mass in GeV
int     GetPDGCode () const
double  GetKinEnergy () const   kin. energy in GeV
double  GetTheta () const       zenith angle in rad
double  GetPSquared () const    squared of momentum in GeV
double  GetE () const