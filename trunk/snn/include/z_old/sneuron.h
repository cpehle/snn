#ifndef _SNEURON_H
 #define _SNEURON_H
/**
 * \file sneuron.h
 * General spiking neuron library
 * \author Brett Graham
 * \date 9-23-07
 * \todo
 * - find better way to store spike data for longer window of previous activity (> 32 ms)
 * - do I need the abstract destructor statements?
 * - add latex of activation functions
*/

#include <math.h>
#include <vector>

/// Enum holding possible neuron types
enum SNType {
	liafms,
	liafdtms,
	aeifms
};

/// Abstract spiking neuron model base class
/**
	An abstract base class for all spiking neuron model classes.
*/
class SNeuron {
	public:
		/// Abstract destructor (do I need this?)
		virtual ~SNeuron() {};
		/// General update function
		virtual void step () = 0;
		/// Get current activation
		double getAct () { return(dact); };
		
		/// Get current spiking value
		/**
			\return bool : signifies if neuron model spiked during the current timestep
		*/
		bool getSpiked () {
			if ((uspikeAct & 1) == 1) {
				return(true);
			} else {
				return(false);
			};
		};
		
		/// Get spiking activity pointer
		/**
			\return unsigned int* : returns pointer to unsigned in that contains a bit representation 
			of spiking activity over the previous 32 milliseconds
		*/
		unsigned int* getSpikeActPtr () { return(&uspikeAct); };
		
		/// Get input value pointer
		/**
			Returns pointer to input value, used for controlling connections and stimulating the neuron model
		*/
		double* getInputPtr () { return(&dinput); };
		
		// connection routines, moved to synapse.h in SSynapse class
/*		void addPost ( double* target ) {
			vpost.push_back(target);
		}
		void propogateSpike() {
			if (bspiked) {
				for(unsigned int i=0; i < vpost.size(); i++)	{
					*vpost[i] += SPIKESTRENGTH;
				}
			}
		} */
	protected:
		/// Stores current input value
		double dinput;
		/// Stores current activation
		double dact;
		/// Stores a bit-packed representation of the previous 32 milliseconds of spiking activity
		unsigned int uspikeAct;
		
		/// Update spiking history adding a spike at t=0
		void addSpike() {
			uspikeAct = ((uspikeAct << 1) | 1);
		};
		/// Update spiking history adding no spike at t=0
		void addNoSpike() {
			uspikeAct = (uspikeAct << 1);
		};
		// 32 bit unsigned int
//		std:: vector <double*> vpost;
};

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
		LiafDTMs (double threshDecay = 0.001, double threshStep = 0.1, double thresholdmV = 0.06, double R = 30000, double C = 0.000001) {
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


//

class AeifMs: public SNeuron {
	public:
		/// Constructor
		/**
			\param thresholdmV : firing threshiking;
			\param R : membrane resistance
			\param C : smembrane capacitance
			Calculates membrane time constant, tau = R*C*1000, and corresponding exp(-1/tau)
		*/
		AeifMs (bool bursting = false, double resetValue = 0.003, double thresholdmV = 0.02);
		void step ();
	protected:
		bool bburst;
		double dreset;
		double dthresh;
		double dw;
};
/*
class LiafRGC: public LiafMs {

// filtered input
}
*/
#endif
