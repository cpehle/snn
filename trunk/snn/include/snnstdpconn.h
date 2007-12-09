#ifndef _SNNSTDPCONN_H
 #define _SNNSTDPCONN_H

#include "snnbase.h"

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

class STDPProjection: public Projection {
	public:
		void add_synapse();
		void add_synapse(unsigned int* PreSpiking, double* PostInput, unsigned int* PostSpiking);
		void add_synapse(unsigned int* PreSpiking, double* PostInput, unsigned int* PostSpiking, double weight);
		int configure(CfgLineItems line, bool verbose);
};

#endif
