#ifndef _SNNACONN_H
 #define _SNNACONN_H

#include "snnbase.h"

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

class AProjection: public Projection {
	public:
		AProjection();
		void add_synapse();
		void add_synapse(double* pPre, double* PostInput);
		void add_synapse(double* pPre, double* PostInput, double weight);
		int configure(CfgLineItems line, bool verbose);
};

#endif
