#include "snnaconn.h"

AProjection::AProjection(AnalogArray* Pre, Layer* Post) {
	id="";
/*	width=0;
	height=0;
	depth=0; */
	commonWeight = 1.0;
	pre = Pre;
	post = Post;
	commonType = Pre->type;
}

void AProjection::add_synapse() {
}

void AProjection::add_synapse(DataType inputType, void* pPre, double* PostInput) {
	synapses.push_back(new ASynapse(inputType,pPre,PostInput));
}
void AProjection::add_synapse(DataType inputType, void* pPre, double* PostInput, double weight) {
	synapses.push_back(new ASynapse(inputType,pPre,PostInput,weight));
}

int AProjection::connect_topographic() {
	// check that layers are same size
	if ((pre->width == post->width) && (pre->height == post->height) && (pre->depth == post->depth)) {
		int count = 0;
		for (int z = 0; z < pre->depth; z++) {
			for (int y = 0; y < pre->height; y++) {
				for (int x = 0; x < pre->width; x++) {
					/// NEED TO FIX THIS
					add_synapse(commonType, pre->get_value(x,y,z),post->get_neuron(x,y,z)->getInputPtr(), commonWeight);
					count++;
				}
			}
		}
		return count;
	} else {
		// sizes do NOT agree
		std::cerr << "sizes don't agree when building " << this->id << "\n";
		return 0; // return number of synapses made
	}
}

int AProjection::connect_full() {
	return 1;
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
int AProjection::configure(CfgLineItems line, bool verbose) {
	if (line->at(0) == "weight") {
		if (synapses.size() > 0) {
			return 1;
		} else {
			commonWeight = atof(line->at(1).c_str());
			if (verbose) std::cerr << "\tset commonWeight to " << commonWeight << "\n";
			return 0;
		}
//	} else if (line->at(0) == "type") {
//		if (synapses.size() > 0) {
//			return 1;
//		} else {
//			// TODO make conversion from string to DATATYPE
//			commonType = atot(line->at(1).c_str());
//			if (verbose) std::cerr << "\tNOTDONE: set commonType to " << line->at(1).c_str() << "\n";
//			return 0;
//		}
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
	} else if (line->at(0) == "connect") {
		if (line->at(1) == "topographic") {
			int returnValue = connect_topographic();
			if (verbose) std::cerr << "\tconnected topographically, made " << returnValue << " synapses\n";
			if (returnValue > 0) {
				return 0;
			} else {
				return 1;
			}
		} else if (line->at(1) == "full") {
			int returnValue = connect_full();
			if (verbose) std::cerr << "\tconnected fully, made " << returnValue << " synapses\n";
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
