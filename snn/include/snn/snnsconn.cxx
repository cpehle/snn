#include "snnsconn.h"

SProjection::SProjection(Layer* PreLayer, Layer* PostLayer) {
	id="";
/*	width=0;
	height=0;
	depth=0; */
	commonWeight = 1.0;
	commonDelay = 0;
	pre = PreLayer;
	post = PostLayer;
}

void SProjection::add_synapse() {
}

void SProjection::add_synapse(unsigned int* PreSpiking, double* PostInput) {
	synapses.push_back(new SSynapse(PreSpiking,PostInput));
}

void SProjection::add_synapse(unsigned int* PreSpiking, double* PostInput, double weight) {
	synapses.push_back(new SSynapse(PreSpiking,PostInput,weight));
}

void SProjection::add_synapse(unsigned int* PreSpiking, double* PostInput, double weight, unsigned int iDelay) {
	synapses.push_back(new SSynapse(PreSpiking,PostInput,weight,iDelay));
}

int SProjection::connect_topographic() {
	// check that layers are same size
	if ((pre->width == post->width) && (pre->height == post->height) && (pre->depth == post->depth)) {
		int count = 0;
		for (int z = 0; z < pre->depth; z++) {
			for (int y = 0; y < pre->height; y++) {
				for (int x = 0; x < pre->width; x++) {
					//int index = x + y*pre->width + z*pre->height;
					add_synapse(pre->get_neuron(x,y,z)->getSpikeActPtr(),post->get_neuron(x,y,z)->getInputPtr(), commonWeight, commonDelay);
					count++;
				}
			}
		}
		return count;
	} else {
		// sizes do NOT agree
		return 0; // return number of synapses made
	}
}

int SProjection::connect_full() {
	// check that layers are same size
	int count = 0;
	for (int z = 0; z < pre->depth; z++) {
		for (int y = 0; y < pre->height; y++) {
			for (int x = 0; x < pre->width; x++) {
				for (int postZ = 0; postZ < post->depth; postZ++) {
					for (int postY = 0; postY < post->height; postY++) {
						for (int postX = 0; postX < post->width; postX++) {
							add_synapse(pre->get_neuron(x,y,z)->getSpikeActPtr(),post->get_neuron(postX,postY,postZ)->getInputPtr(), commonWeight, commonDelay);
							count++;
						}
					}
				}
			}
		}
	}
	return count; // return number of synapses made
}

/// Parse config file line
/**
	Valid lines are:
	- \b delay : sets delay (commonDelay)
	- \b weight : sets weight (commonWeight)
	- \b connect <type> : connects the pre and post layers with a connection type:
		- \b topographic : point-to-point
		- \b full : every neuron to every neuron
		- \b divergent <sigma> : max = sigma * 3 (not implemented)
		- \b convergent <sigma> : max = sigma * 3 (not implemented)

	need to add error checking and allow for 'manual' creation of neurons
	\param line : vector of strings containing items of a configuration line
	\param verbose : set true if you want lots of output
	Returns 0 if all is OK, if anything else than error
*/
int SProjection::configure(CfgLineItems line, bool verbose) {
	if (line->at(0) == "delay") {
		if (synapses.size() > 0) {
			return 1;
		} else {
			commonDelay = atoi(line->at(1).c_str());
			if (verbose) std::cout << "\tset commonDelay to " << commonDelay << "\n";
			return 0;
		}
	} else if (line->at(0) == "weight") {
		if (synapses.size() > 0) {
			return 1;
		} else {
			commonWeight = atof(line->at(1).c_str());
			if (verbose) std::cout << "\tset commonWeight to " << commonWeight << "\n";
			return 0;
		}
/*	} else if (line->at(0) == "width") {
		if (synapses.size() > 0) {
			return 1;
		} else {
			width = atoi(line->at(1).c_str());
			if (verbose) std::cout << "\tset width to " << width << "\n";
			return 0;
		}
	} else if (line->at(0) == "height") {
		if (synapses.size() > 0) {
			return 1;
		} else {
			height = atoi(line->at(1).c_str());
			if (verbose) std::cout << "\tset height to " << height << "\n";
			return 0;
		}
	} else if (line->at(0) == "depth") {
		if (synapses.size() > 0) {
			return 1;
		} else {
			depth = atoi(line->at(1).c_str());
			if (verbose) std::cout << "\tset depth to " << depth << "\n";
			return 0;
		} */
	} else if (line->at(0) == "connect") {
		if (line->at(1) == "topographic") {
			int returnValue = connect_topographic();
			if (verbose) std::cout << "\tconnected topographically, made " << returnValue << " synapses\n";
			if (returnValue > 0) {
				return 0;
			} else {
				return 1;
			}
		} else if (line->at(1) == "full") {
			int returnValue = connect_full();
			if (verbose) std::cout << "\tconnected fully, made " << returnValue << " synapses\n";
			if (returnValue > 0) {
				return 0;
			} else {
				return 1;
			}
		} else if (line->at(1) == "divergent") {
		} else if (line->at(1) == "convergent") {
		} else {
		}
		// make neurons
		// check if dimensions have been set
/*		if (width == 0) {
			return 1;
		} else { */
		/*
			int nNeurons = height*width*depth;
			for (int n = 0; n < nNeurons; n++) {
				add_neuron(commonThresh, commonR, commonC);
			}
			if (verbose) std::cout << "\tMade " << nNeurons << " neurons in layer " << id << "\n";
			return 0;
		*/
//		}
	} else {
		return 1;
		//error unkown line
	}
//	std::cout << "\t in LiafMsLayer ";
//	for (unsigned int i = 0; i < line->size(); i++) {
//		std::cout << line->at(i) << " ";
//		//line->at(i) == "foo"
//	}
//	std::cout << "\n";
	// if A.O.K. return 0, else something else
//	return 0;
}
