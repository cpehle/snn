#ifndef _SNNBASE_H
 #define _SNNBASE_H
/**
 * \file snnbase.h
 * General spiking neuron library base classes
 * \author Brett Graham
 * \date 12-04-07
 * \todo
 * - do I need the abstract destructor statements?
 * - add latex of activation functions
 * - find better way to read spike data (rather than cycling through previous activity
 * - sort out analog inputs (outside API, assign AnalogArray, run configure)
 * - sort out how to connect projections regardless of the underlying type
*/

#include <math.h>
#include <vector>
#include "cfgfile.h"

#define DSPIKESTRENGTH 0.05

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

/// class for analog inputs
/**
	This may work much better using a Template class
*/
enum DataType {
	FLOAT,
	DOUBLE,
	CHAR,
	UCHAR,
	INT
};

typedef unsigned char uchar;

class AnalogArray {
	public:
		void* data; // this can be pointed at anything
		int width;
		int height;
		int depth;
		std::string id;
		DataType type;
		void* get_value(int x) {
			switch (type) {
				case FLOAT : {
					float* tempFloat = (float*)data;
					return (void*)&tempFloat[x];
					break; }
				case DOUBLE : {
					double* tempDouble = (double*)data;
					return (void*)&tempDouble[x];
					break; }
				case CHAR : {
					char* tempChar = (char*)data;
					return (void*)&tempChar[x];
					break; }
				case UCHAR : {
					uchar* tempUchar = (uchar*)data;
					return (void*)&tempUchar[x];
					break; }
				case INT : {
					int* tempInt = (int*)data;
					return (void*)&tempInt[x];
					break; }
				default :
					return NULL;
			};
		};
		void* get_value(int x, int y) {
			get_value(x + y*width);
		};
		void* get_value(int x, int y, int z) {
			get_value(x + y*width + z*width*height);
		};
};

/// Abstract layer base class
/**
	An abstract base class for all layers (a sheet of neurons)
*/
class Layer {
	public:
		/// Constructor - setup default values for neuron model
		Layer() { id = ""; width = 0; height = 0; depth = 0; };
		/// layer name, may or may not use this
		std::string id;
		/// one-dimensional vector holding neurons
		std::vector<SNeuron*> neurons;
		/// how many neurons equal 1 y-step (in other words, layer width)
		int width;
		/// how many neurons equal 1 z-step (in other words, layer height)
		int height;
		/// used for 3d layers, else 1
		int depth;
		/// functions to return a specific neuron based on either a 1-d 2-d or 3-d call
		SNeuron* get_neuron(int x) { return neurons.at(x); };
		SNeuron* get_neuron(int x, int y) { return neurons.at(x+y*width); };
		SNeuron* get_neuron(int x, int y, int z) { return neurons.at(x+y*width+z*width*height); };
		void add_n_neurons(int n) { for (int i = 0; i < n; i++) { add_neuron(); }; };
		virtual void add_neuron() = 0;
		/// configuration of layer
		/**
			Configuration takes place through processing of single lines from a CfgFile class object
		*/
		virtual int configure(CfgLineItems line, bool verbose) = 0;
		void step() { for (int i = 0; i < neurons.size(); i++) { neurons[i]->step(); }; };
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

/// Projection - base class
/**
	A connection between two layers of neurons
*/
class Projection {
	public:
		Projection() { id=""; /*width=0; height=0; depth=0;*/ commonWeight = 1.0; };
		std::string id;
//		SynapseType synapseType;
		std::vector<Synapse*> synapses;
/*		int width;
		int height;
		int depth; */
		double commonWeight;
/*		Synapse* get_synapse(int x) { return synapses.at(x); };
		Synapse* get_synapse(int x, int y) { return synapses.at(x+y*width); };
		Synapse* get_synapse(int x, int y, int z) { return synapses.at(x+y*width+z*height); };
		int connect_topographic();
		int connect_divergent();
		int connect_convergent();
		int connect_full(); */
		virtual void add_synapse() = 0;
		virtual int configure(CfgLineItems line, bool verbose) = 0;
		void step() { for(int i=0; i < synapses.size(); i++){ synapses[i]->step(); }; };
};

#endif
