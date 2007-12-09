#ifndef _SNN_H
 #define _SNN_H
/**
 * \file snn.h
 * General spiking neural network library
 * \author Brett Graham
 * \date 9-23-07
 * \todo
 * - fix me
 * - make child layer classes? (parameters is where it messes up) YES
 * - have layers accept cfg files and parse them to find the necessary parameters
*/

#include "sneuron.h"
#include "synapse.h"
#include "cfgfile.h"
#include <string>
#include <vector>

/// Layer - base class
/**
	A layer of spiking neurons
*/
//template <class T>
//class SNNLayer {
//	public:
//		/// layer name, may or may not use this
//		std::string id;
//		/// holds neuron type - may change this to child classes of this basic layer class
//		SNType neuronType;
//		/// one-dimensional vector holding neurons
//		std::vector<SNeuron*> neurons;
//		/// how many neurons equal 1 y-step (in other words, layer width)
//		int yStep;
//		/// how many neurons equal 1 z-step (in other words, layer height)
//		int zStep;
//		SNeuron* get_neuron(int x) { return neurons.at(x); };
//		SNeuron* get_neuron(int x, int y) { return neurons.at(x+y*yStep); };
//		SNeuron* get_neuron(int x, int y, int z) { return neurons.at(x+y*yStep+z*zStep); };
//		void add_n_neurons(int n) { for (int i = 0; i < n; i++) { add_neuron(); }; };
//		virtual void add_neuron() = 0;
//		void step();
//};

class Layer {
	public:
		Layer();
		/// layer name, may or may not use this
		std::string id;
		/// holds neuron type - may change this to child classes of this basic layer class
//		SNType neuronType;
		/// one-dimensional vector holding neurons
		std::vector<SNeuron*> neurons;
		/// how many neurons equal 1 y-step (in other words, layer width)
		int yStep;
		/// how many neurons equal 1 z-step (in other words, layer height)
		int zStep;
		SNeuron* get_neuron(int x) { return neurons.at(x); };
		SNeuron* get_neuron(int x, int y) { return neurons.at(x+y*yStep); };
		SNeuron* get_neuron(int x, int y, int z) { return neurons.at(x+y*yStep+z*zStep); };
		void add_n_neurons(int n) { for (int i = 0; i < n; i++) { add_neuron(); }; };
		virtual void add_neuron() = 0;
		virtual int configure(CfgLineItems line) = 0;
		void step();
};

class LiafMsLayer: public Layer {
	public:
		double commonThresh;
		double commonR;
		double commonC;
		LiafMsLayer();
		void add_neuron();
		void add_neuron(double thresh, double mR, double mC);
		int configure(CfgLineItems line);
};

class LiafDTMsLayer: public Layer {
	public:
		void add_neuron();
		void add_neuron(double tDecay, double tStep, double thresh, double mR, double mC);
		int configure(CfgLineItems line);
};

class AeifMsLayer: public Layer {
	public:
		void add_neuron();
		void add_neuron(bool bursting, double resetValue, double thresholdmV);
		int configure(CfgLineItems line);
};

/// Projection - base class
/**
	A connection between two layers of neurons
*/
class Projection {
	public:
		Projection();
		std::string id;
		SynapseType synapseType;
		std::vector<Synapse*> synapses;
		int yStep;
		int zStep;
		double commonWeight;
		Synapse* get_synapse(int x) { return synapses.at(x); };
		Synapse* get_synapse(int x, int y) { return synapses.at(x+y*yStep); };
		Synapse* get_synapse(int x, int y, int z) { return synapses.at(x+y*yStep+z*zStep); };
		virtual void add_synapse() = 0;
		void step();
};

class AProjection: public Projection {
	public:
		void add_synapse();
		void add_synapse(double* pPre, double* PostInput);
		void add_synapse(double* pPre, double* PostInput, double weight);
};

class SProjection: public Projection {
	public:
		SProjection ();
		unsigned int commonDelay;
		void add_synapse();
		void add_synapse(unsigned int* PreSpiking, double* PostInput);
		void add_synapse(unsigned int* PreSpiking, double* PostInput, double weight);
		void add_synapse(unsigned int* PreSpiking, double* PostInput, double weight, unsigned int iDelay);
};

class STDPProjection: public Projection {
	public:
		void add_synapse();
		void add_synapse(unsigned int* PreSpiking, double* PostInput, unsigned int* PostSpiking);
		void add_synapse(unsigned int* PreSpiking, double* PostInput, unsigned int* PostSpiking, double weight);
};

/// SNN
/**
	Spiking neural network class
*/
class SNN {
	public:
		void add_layer(std::string layerId, SNType type, int nNeurons, int layerYStep, int layerZStep);
		void add_projection(std::string ProjectionId, SynapseType type, int nSynapses, int projectionYStep, int projectionZStep);
		
		std::vector<Layer*> layers;
		std::vector<Projection*> projections;
		
		void run_network(int timeSteps = 1) { for (int i = 0; i < timeSteps; i++) {step();}; };
		void step();
	private:
};

#endif
