#ifndef _SNNLIAFDT_H
 #define _SNNLIAFDT_H

#include "snnbase.h"

/**
 * \file snnliaf.h
 * Leaky integrate and fire neuron model widht a dynamic threshold
 * \author Brett Graham
 * \date 12-04-07
 * \todo
 * - write LiafDTMsLayer::configure
*/

/// Leaky Integrate and Fire Model Neuron with a dynamic threshold
/**
	Implementation of leaky integrate and fire model neuron with a dynamic threshold \n
	Simplified to run at a timestep of 1 ms
*/
class LiafDTMs: public SNeuron {
	public:
		/// Constructor
		/**
			\param threshDecay : decay rate of dynamic threshold
			\param threshStep : value added to decay after spike
			\param thresholdmV : baseline static threshold
			\param R : membrane resistance
			\param C  : membrane capacitance
			Calculate membrane time constant, tau = R*C*1000, and corresponding exp(-1/tau)
		*/
		LiafDTMs (double thresholdmV = 0.06, double threshDecay = 0.001, double threshStep = 0.1, double R = 30000, double C = 0.000001) {
			devalue = exp(-1/(R * C * 1000));
			dthresh = thresholdmV;
			dthreshDecay = threshDecay;
			ddynamicThresh = 0.0;
			dthreshStep = threshStep;
			dact = 0.0;
			dinput = 0.0;
			uspikeAct = 0;
		}
		///  Update function
		/**
			Activation function \n
		*/
		void step () {
			dact = dact * devalue + dinput;
			if (dact > (dthresh+ddynamicThresh)) {
				addSpike();
				dact = 0.0;
				ddynamicThresh += dthreshStep;
			} else {
				ddynamicThresh -= dthreshDecay;
				addNoSpike();
			}
			dinput = 0.0;
		}
	protected:
		double devalue;
		double dthresh;
		double dthreshDecay;
		double ddynamicThresh;
		double dthreshStep;
};

/// layer of LiafDTMs
/**
	contains a vector of LiafDTMs and common parameters
*/
class LiafDTMsLayer: public Layer {
	public:
		double commonThresh;
		double commonThreshDecay;
		double commonThreshStep;
		double commonR;
		double commonC;
		LiafDTMsLayer();
		void add_neuron();
		void add_neuron(double thresh, double tDecay, double tStep, double mR, double mC);
		int configure(CfgLineItems line, bool verbose);
};

#endif
