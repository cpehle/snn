#ifndef _SNNAEIF_H
 #define _SNNAEIF_H

/**
 * \file snnaeif.h
 * Adaptive exponential integrate and fire neuron model (from Brette 2006)
 * \author Brett Graham
 * \date 12-04-07
 * \todo
*/

#include "snnbase.h"

class AeifMs: public SNeuron {
	public:
		/// Constructor
		/**
			\param thresholdmV : double : set the spiking threshold
			\param bursting : bool : should this be a bursting neuron?
			\param resetValue : double : determines the 'burstiness' if the neuron is bursting
			Assigns parameters to internal values
		*/
		AeifMs (double thresholdmV = 0.02, bool bursting = false, double resetValue = 0.003);
		void step ();
	protected:
		bool bburst;
		double dreset;
		double dthresh;
		double dw;
};

class AeifMsLayer: public Layer {
	public:
		double commonThresh;
		double commonReset;
		bool commonBursting;
		AeifMsLayer();
		void add_neuron();
		void add_neuron(double thresholdmV, bool bursting, double resetValue);
		int configure(CfgLineItems line, bool verbose);
};

#endif
