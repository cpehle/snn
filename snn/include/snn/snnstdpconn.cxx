#include "snnstdpconn.h"

STDPProjection::STDPProjection() {
	id="";
/*	width=0;
	height=0;
	depth=0; */
	commonWeight = 1.0;
}

void STDPProjection::add_synapse() {
}
		
void STDPProjection::add_synapse(unsigned int* PreSpiking, double* PostInput, unsigned int* PostSpiking){
	synapses.push_back(new STDPSynapse(PreSpiking,PostInput,PostSpiking));
}

void STDPProjection::add_synapse(unsigned int* PreSpiking, double* PostInput, unsigned int* PostSpiking, double weight){
	synapses.push_back(new STDPSynapse(PreSpiking,PostInput,PostSpiking,weight));
}

/// Parse config file line
/**
	Valid lines are:
	- \b weight : sets weight (commonWeight)
	- \b width : sets layer width, this is the only necessary parameter
	- \b height : sets layer height, defaults to 1
	- \b depth : sets layer depth, defaults to 1
	- \b make : constructs the neurons using the common<X> parameters assigned above
	need to add error checking and allow for 'manual' creation of neurons
	\param line : vector of strings containing items of a configuration line
	\param verbose : set true if you want lots of output
	Returns 0 if all is OK, if anything else than error
*/
int STDPProjection::configure(CfgLineItems line, bool verbose) {
	if (line->at(0) == "weight") {
		if (synapses.size() > 0) {
			return 1;
		} else {
			commonWeight = atof(line->at(1).c_str());
			if (verbose) std::cerr << "\tset commonWeight to " << commonWeight << "\n";
			return 0;
		}
/*	} else if (line->at(0) == "width") {
		if (synapses.size() > 0) {
			return 1;
		} else {
			width = atoi(line->at(1).c_str());
			if (verbose) std::cerr << "\tset width to " << width << "\n";
			return 0;
		}
	} else if (line->at(0) == "height") {
		if (synapses.size() > 0) {
			return 1;
		} else {
			height = atoi(line->at(1).c_str());
			if (verbose) std::cerr << "\tset height to " << height << "\n";
			return 0;
		}
	} else if (line->at(0) == "depth") {
		if (synapses.size() > 0) {
			return 1;
		} else {
			depth = atoi(line->at(1).c_str());
			if (verbose) std::cerr << "\tset depth to " << depth << "\n";
			return 0;
		} */
	} else if (line->at(0) == "make") {
		// make neurons
		// check if dimensions have been set
/*		if (width == 0) {
			return 1;
		} else {*/
		/*
			int nNeurons = height*width*depth;
			for (int n = 0; n < nNeurons; n++) {
				add_neuron(commonThresh, commonR, commonC);
			}
			if (verbose) std::cerr << "\tMade " << nNeurons << " neurons in layer " << id << "\n";
			return 0;
		*/
//		}
	} else {
		return 1;
		//error unkown line
	}
//	std::cerr << "\t in LiafMsLayer ";
//	for (unsigned int i = 0; i < line->size(); i++) {
//		std::cerr << line->at(i) << " ";
//		//line->at(i) == "foo"
//	}
//	std::cerr << "\n";
	// if A.O.K. return 0, else something else
//	return 0;
}
