#ifndef _SYNAPSE_H
 #define _SYNAPSE_H
/**
 * \file synapse.h
 * General synapse library
 * \author Brett Graham
 * \date 9-23-07
 * \todo
 * - find better way to store spike data for longer window of previous activity (> 32 ms)
 * - find better way to read spike data (rather than cycling through previous activity
 * - find way to register neurons (or synapses) that spiked recently (this timestep) to simplify learning
*/

#include <math.h>
#include <vector>

#define DSPIKESTRENGTH 0.05

/// Enum holding possible synapse types
enum SynapseType {
	ssynapse,
	asynapse,
	stdpsynapse
};

/// Abstract synapse base class
/**
	An abstract base class for all synapses
*/
class Synapse { 
	public:
		/// A virtual destructor (do I need this?)
		virtual ~Synapse() {};
		/// A virtual update function
		/**
		Updates synapse object (timestep = 1 ms)
		*/
		virtual void step () = 0;
		/// Retrieves the synapse weight
		double getWeight() { return(dweight); };
	protected: 
		double dweight;
};

/// Basic spiking neuron synapse class
/**
	Translates spikes from a presynaptic neuron model to changes in the input of the post synaptic neuron model
*/
class SSynapse: public Synapse {
	public:
		/// A virtual destructor (do I need this?)
		virtual ~SSynapse() {};
		/// Contructor
		/**
			Connects two spiking neuron models
		*/
//		SSynapse (unsigned int* PreSpiking, double* PostInput, double weight = 1.0, double spikeStrength = DSPIKESTRENGTH) {
		SSynapse (unsigned int* PreSpiking, double* PostInput, double weight = 1.0, unsigned int iDelay = 0, double spikeStrength = DSPIKESTRENGTH) {
			ppreSpiking = PreSpiking;
			ppostInput = PostInput;
			dweight = weight;
			dspikeStrength = spikeStrength;
			delay = (1 << (iDelay));
		}
		/// Update function
		virtual void step () {
			/// check if presynaptic model neuron spiked this timestep
			if (((*ppreSpiking) & delay) == delay) {
//			if (((*ppreSpiking) & 1) == 1) {
				/// if presynaptic spike then add postsynaptic input
				/**
				*ppostInput += dspikeStrength * dweight;
				*/
				*ppostInput += dspikeStrength * dweight;
			};
		}
	protected:
		unsigned int* ppreSpiking;
		unsigned int delay;
		double* ppostInput;
		double dspikeStrength;
};

/// Analog to spiking neuron model synapse class
/**
	Connects an analog value (of type double) to a spiking neuron model
*/
class ASynapse: public Synapse {
	public:
		/// Constructor
		/**
			Connects a presynaptic analog input (double) and postsynaptic neuron model
		*/
		ASynapse (double* pPre, double* PostInput, double weight = 1.0) {
			ppreAnalog = pPre;
			ppostInput = PostInput;
			dweight = weight;
		}
		/// Update function
		/**
			Multiplies analog input by weight value and adds result to the postsynaptic input
		*/
		void step () {
			*ppostInput += *ppreAnalog * dweight;
		}
	protected:
		double* ppreAnalog;
		double* ppostInput;
};

/// Learning synapse model (using STDP)
/**
	Connects two spiking neuron models with a plastic synapse.
*/
class STDPSynapse: public Synapse {
	public:
		/// Constructor
		/**
			Connects two spiking neuron models.
		*/
		STDPSynapse (unsigned int* PreSpiking, double* PostInput, unsigned int* PostSpiking, double weight = 1.0, double spikeStrength = DSPIKESTRENGTH) {
			ppreSpiking = PreSpiking;
			ppostSpiking = PostSpiking;
			ppostInput = PostInput;
			dweight = weight;
			dspikeStrength = spikeStrength;
		}
		/// Update function
		void step () {
			if (((*ppreSpiking) & 1) == 1) {
				*ppostInput += dspikeStrength * dweight;
			};
		}
		/// Apply STDP learning to synapse
		/**
			Requires a learning rate (double) and learning window (double array, passed as pointer).
			There should be a better way to do this.
			Looks for either pre or postsynaptic activity at this timestep (t=0).
			If either is found, STDP learning is applied (if pre proceeds post -> LTP, if post preceeds pre -> LTD).
			At the moment this is limited to a +- 32 ms window due to spike history storage in a single unsigned int.
		*/
		void learn (double learningRate, double* learningWindow) {
			int preTime = -1;
			int postTime = -1;
			for (int i = 0; i < 32; i++) {
				if ( (*ppreSpiking & (1 << i)) != 0 ) {
					preTime = i;
					break;
				};
			};
			for (int i = 0; i < 32; i++) {
				if ( (*ppostSpiking & (1 << i)) != 0 ) {
					postTime = i;
					break;
				};
			};
			if (preTime == 0) {
				dweight -= (learningWindow[postTime]) * learningRate;
			};
			if (postTime == 0) {
				dweight += (learningWindow[preTime]) * learningRate;
			};
		}
	protected:
		double dspikeStrength;
		unsigned int* ppreSpiking;
		unsigned int* ppostSpiking;
		double* ppostInput;
};
#endif
