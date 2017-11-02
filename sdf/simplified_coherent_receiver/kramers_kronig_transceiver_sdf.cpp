# include "netxpto.h"

# include "binary_source.h"
# include "m_qam_mapper.h"	
# include "m_pam_mapper.h"							// Modified for PAM
# include "discrete_to_continuous_time.h"
# include "pulse_shaper.h"
# include "local_oscillator.h"
# include "iq_modulator.h"
# include "sink.h"

int main() {

	// #####################################################################################################
	// ################################### System Input Parameters #########################################
	// #####################################################################################################

	BinarySourceMode sourceMode{ PseudoRandom };
	int patternLength{ 5 };
	string bitStream{ "0" };
	double bitPeriod{ 1.0 / 50e9 };
	//vector<t_iqValues> iqAmplitudes{ { { 0,0 },{ 1,0 },{ 2,0 },{ 3,0 } } };
	int numberOfBits{ 1000 };				                                        // For value of {-1}, it'll generate long bit sequence.
	int numberOfSamplesPerSymbol{ 16 };
	double rollOffFactor{ 0.3 };
	int impulseResponseTimeLength{ 16 };
	double setOpticalPower{ 1e-3 };

	// #####################################################################################################
	// ########################### Signals Declaration and Inicialization ##################################
	// #####################################################################################################

	Binary S1{ "S1.sgn" };										// Binary Sequence
	TimeDiscreteAmplitudeDiscreteReal S2{ "S2.sgn" };			// MPAM Mapping
	TimeContinuousAmplitudeDiscreteReal S3{ "S3.sgn" };			// Discrete to continious time
	TimeContinuousAmplitudeContinuousReal S4{ "S4.sgn" };		// Pulse Shapping filter
	BandpassSignal S5{ "S5.sgn" };								// Oscillator
	BandpassSignal S6{ "S6.sgn" };								// Mixer	
	BandpassSignal S7{ "S7.sgn" };								// Fork_s7
	BandpassSignal S8{ "S8.sgn" };								// Fork_s8
	BandpassSignal S9{ "S9.sgn" };								// Hilbert Transformer
	BandpassSignal S10{ "S10.sgn" };							// IQ Modulator

	// #####################################################################################################
	// ########################### Blocks Declaration and Inicialization ###################################
	// #####################################################################################################

	BinarySource B1{ vector<Signal*> {}, vector<Signal*> { &S1} };
	B1.setMode(sourceMode);
	B1.setPatternLength(patternLength);
	B1.setBitStream(bitStream);
	B1.setBitPeriod(bitPeriod);
	B1.setNumberOfBits(numberOfBits);

	MPamMapper B2{ vector<Signal*> { &S1 }, vector<Signal*> { &S2} };						// #1
	B2.setIqAmplitudes(iqAmplitudes);

	DiscreteToContinuousTime B3{ vector<Signal*> { &S2 }, vector<Signal*> { &S3 } };
	B3.setNumberOfSamplesPerSymbol(numberOfSamplesPerSymbol);

	PulseShaper B4{ vector<Signal*> { &S3}, vector<Signal*> { &S4 } };
	B4.setRollOffFactor(rollOffFactor);
	B4.setImpulseResponseTimeLength(impulseResponseTimeLength);
	B4.setSeeBeginningOfImpulseResponse(false);

	Oscillator B5{ vector<Signal*> {}, vector<Signal*> { &S5 } };							// #2
	B5.setFrequency(Frequency);
	B5.setAmplitude(Amplitude);
	B5.setPhase(Phase);
	//

	Mixer B6{ vector<Signal*> { &S4,&S5 }, vector<Signal*> { &S6 } };						// #3
	//

	Fork B78{ vector<Signal*> { &S6 }, vector<Signal*> { &S7,&S8 } };					    // #4
	//

	HilbertTranform B9{ vector<Signal*> { &S8 }, vector<Signal*> { &S9 } };				    // #5
	//

	IqModulator B10{ vector<Signal*> { &S7, &S9 }, vector<Signal*> { &S10 } };

	Sink B11{ vector<Signal*> { &S10 }, vector<Signal*> {} };
	B11.setNumberOfSamples(5000);
	B11.setDisplayNumberOfSamples(true);

	// #####################################################################################################
	// ########################### System Declaration and Inicialization ###################################
	// #####################################################################################################

	System MainSystem{ vector<Block*> { &B1, &B2, &B3, &B4, &B5, &B6, &B78, &B9, &B10, &B11 } };

	// #####################################################################################################
	// #################################### System Run #####################################################
	// #####################################################################################################

	MainSystem.run();

	return 0;

}