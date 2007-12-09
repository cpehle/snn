#ifndef _SNNSCONN_H
 #define _SNNSCONN_H
 
#include "snnbase.h"

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

class SProjection: public Projection {
	public:
		SProjection ();
		unsigned int commonDelay;
		void add_synapse();
		void add_synapse(unsigned int* PreSpiking, double* PostInput);
		void add_synapse(unsigned int* PreSpiking, double* PostInput, double weight);
		void add_synapse(unsigned int* PreSpiking, double* PostInput, double weight, unsigned int iDelay);
		int configure(CfgLineItems line, bool verbose);
};


#endif
