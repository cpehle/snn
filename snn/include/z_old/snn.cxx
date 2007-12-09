#include "snn.h"

Layer::Layer() {
	id = "";
	yStep = 0;
	zStep = 0;
}

void Layer::step() {
	for (int i = 0; i < neurons.size(); i++) {
		neurons[i]->step();
	}
}
//==========Liaf==============
LiafMsLayer::LiafMsLayer() {
	// set defaults
	commonThresh = 0.06;
	commonR = 30000;
	commonC = 0.000001;
}

void LiafMsLayer::add_neuron() {
//	SNeuron* buff = new LiafMs(threshold,R,C);
	SNeuron* buff = new LiafMs();
	neurons.push_back(buff);
}

void LiafMsLayer::add_neuron(double thresh, double mR, double mC) {
	SNeuron* buff = new LiafMs(thresh,mR,mC);
	neurons.push_back(buff);
}

/// Parse config file line
int LiafMsLayer::configure(CfgLineItems line) {
	for (unsigned int i = 0; i < line->size(); i++) {
		//line->at(i) == "foo"
	}
	// if A.O.K. return 0, else something else
	return 0;
}
//==========LiafDT=============
void LiafDTMsLayer::add_neuron() {
//	SNeuron* buff = new LiafDTMs(threshDecay, threshStep, threshold, R, C);
	SNeuron* buff = new LiafDTMs();
	neurons.push_back(buff);
}

void LiafDTMsLayer::add_neuron(double tDecay, double tStep, double thresh, double mR, double mC) {
	SNeuron* buff = new LiafDTMs(tDecay, tStep, thresh, mR, mC);
	neurons.push_back(buff);
}

/// Parse config file line
int LiafDTMsLayer::configure(CfgLineItems line) {
	for (unsigned int i = 0; i < line->size(); i++) {
		//line->at(i) == "foo"
	}
	// if A.O.K. return 0, else something else
	return 0;
}
//==========Aeif==============
void AeifMsLayer::add_neuron() {
	SNeuron* buff = new AeifMs();
	neurons.push_back(buff);
}

void AeifMsLayer::add_neuron(bool bursting, double resetValue, double thresholdmV) {
	SNeuron* buff = new AeifMs(bursting, resetValue, thresholdmV);
	neurons.push_back(buff);
}

/// Parse config file line
int AeifMsLayer::configure(CfgLineItems line) {
	for (unsigned int i = 0; i < line->size(); i++) {
		//line->at(i) == "foo"
	}
	// if A.O.K. return 0, else something else
	return 0;
}
// =========================================
Projection::Projection() {
	commonWeight = 1.0;
	yStep = 0;
	zStep = 0;
	id = "undefined";
}

void Projection::step() {
	for (int i = 0; i < synapses.size(); i++) {
		synapses[i]->step();
	}
}

void AProjection::add_synapse() {
	std::cerr << "attempting to create Bad connection\n";
}
void AProjection::add_synapse(double*pPre, double* PostInput) {
	Synapse* buff = new ASynapse(pPre,PostInput,commonWeight);
	synapses.push_back(buff);
}

void AProjection::add_synapse(double*pPre, double* PostInput, double weight) {
	Synapse* buff = new ASynapse(pPre,PostInput,weight);
	synapses.push_back(buff);
}

SProjection::SProjection() {
	/** \todo
	 * - do I need to call the base constructor here?
	*/
	commonDelay = 0;
}
void SProjection::add_synapse() {
	std::cerr << "attempting to create Bad connection\n";
}

void SProjection::add_synapse(unsigned int* PreSpiking, double* PostInput) {
	Synapse* buff = new SSynapse(PreSpiking, PostInput, commonWeight);
	synapses.push_back(buff);
}

void SProjection::add_synapse(unsigned int* PreSpiking, double* PostInput, double weight) {
	Synapse* buff = new SSynapse(PreSpiking, PostInput, weight);
	synapses.push_back(buff);
}

void SProjection::add_synapse(unsigned int* PreSpiking, double* PostInput, double weight, unsigned int iDelay) {
	Synapse* buff = new SSynapse(PreSpiking, PostInput, weight, iDelay);
	synapses.push_back(buff);
}

void STDPProjection::add_synapse() {
	std::cerr << "attempting to create Bad connection\n";
}

void STDPProjection::add_synapse(unsigned int* PreSpiking, double* PostInput, unsigned int* PostSpiking) {
	Synapse* buff = new STDPSynapse(PreSpiking, PostInput, PostSpiking, commonWeight);
	synapses.push_back(buff);
}

void STDPProjection::add_synapse(unsigned int* PreSpiking, double* PostInput, unsigned int* PostSpiking, double weight) {
	Synapse* buff = new STDPSynapse(PreSpiking, PostInput, PostSpiking, weight);
	synapses.push_back(buff);
}

// ==================================

void SNN::step () {
	for (int i = 0; i < layers.size(); i++) {
		layers[i]->step();
	}
	for (int i = 0; i < projections.size(); i++) {
		projections[i]->step();
	}
	std::cout<< "stepped\n";
}

void SNN::add_layer(std::string layerId, SNType type, int nNeurons, int layerYStep, int layerZStep) {
	Layer* buff = new LiafMsLayer;
	buff->id = layerId;
//	buff->neuronType = type;
//	switch (type) {
//		case LiafMs:
//			for (int n = 0; n < nNeurons; n++) {
//				
//			break;
//		case LiafDTMs:
//			break;
//		case AeifMs
//			break;
//	}
	// make neurons
	buff->yStep = layerYStep;
	buff->zStep = layerZStep;
	layers.push_back(buff);
}

void SNN::add_projection(std::string projectionId, SynapseType type, int nSynapses, int projectionYStep, int projectionZStep) {
	Projection* buff = new SProjection;
	buff->id = projectionId;
	buff->synapseType = type;
	// make nSynapses
	buff->yStep = projectionYStep;
	buff->zStep = projectionZStep;
	projections.push_back(buff);
}
