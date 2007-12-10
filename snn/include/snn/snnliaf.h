#ifndef _SNNLIAF_H
 #define _SNNLIAF_H

#include "snnbase.h"

/**
 * \file snnliaf.h
 * Leaky integrate and fire neuron model
 * \author Brett Graham
 * \date 12-04-07
 * \todo
 * - add error checking to LiafMsLayer::configure
 * - add 'manual' neuron creation using single configuration lines
*/

/// Leaky integrate and fire spiking neuron models
/**
	Implementation of a leaky integrate and fire model neuron \n
	Static threshold, math stimplified to a step of 1 ms \n
*/
class LiafMs: public SNeuron {
	public:
		/// Constructor
		/**
			\param thresholdmV : firing threshold
			\param R : membrane resistance
			\param C : smembrane capacitance
			Calculates membrane time constant, tau = R*C*1000, and corresponding exp(-1/tau)
		*/
		LiafMs (double thresholdmV = 0.06, double R = 30000, double C = 0.000001) {
			// 
			devalue = exp(-1/(R * C * 1000));
			//Fevalue = 0.9672161;
			dthresh = thresholdmV;
			dact = 0.0;
			dinput = 0.0;
			uspikeAct = 0;
		}
		/// Update function
		/**
			Activation Function \n
			Simplified to: \n
		*/
		void step () {
			dact = dact * devalue + dinput;
			if (dact > dthresh) {
				addSpike();
				dact = 0.0;
			} else { 
				addNoSpike();
			}
			dinput = 0.0;
		}
	protected:
		double devalue;
		double dthresh;
};

/// Layer of LiafMs neurons
/**
 Class containing a vector of LiafMs model neurons and common parameters
 */
class LiafMsLayer: public Layer {
	public:
		double commonThresh;
		double commonR;
		double commonC;
		LiafMsLayer();
		void add_neuron();
		void add_neuron(double thresh, double mR, double mC);
		int configure(CfgLineItems line, bool verbose);
};

#endif
