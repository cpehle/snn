#include "snnliafdt.h"
// definitions of forward declarations

LiafDTMsLayer::LiafDTMsLayer() {
	commonThresh = 0.06;
	commonThreshDecay = 0.001;
	commonThreshStep = 0.1;
	commonR = 30000;
	commonC = 0.000001;
	width = 0;
	height = 1;
	depth = 1;
}

void LiafDTMsLayer::add_neuron() {
//	SNeuron* buff = new LiafDTMs(threshDecay, threshStep, threshold, R, C);
	SNeuron* buff = new LiafDTMs();
	neurons.push_back(buff);
}

void LiafDTMsLayer::add_neuron(double thresh, double tDecay, double tStep, double mR, double mC) {
	SNeuron* buff = new LiafDTMs(thresh, tDecay, tStep, mR, mC);
	neurons.push_back(buff);
}

/// Parse config file line
/**
	Valid lines are:
	- \b thresh : sets threshold (commonThresh)
	- \b tdecay : sets threshold decay (commonThreshDecay)
	- \b tstep : sets threshold step (commonThreshStep)
	- \b r : sets membrane resistance (commonR)
	- \b c : sets membrane capacitance (commonC)
	- \b width : sets layer width, this is the only necessary parameter
	- \b height : sets layer height, defaults to 1
	- \b depth : sets layer depth, defaults to 1
	- \b make : constructs the neurons using the common<X> parameters assigned above
	need to add error checking and allow for 'manual' creation of neurons
	\param line : vector of strings containing items of a configuration line
	\param verbose : set true if you want lots of output
	Returns 0 if all is OK, if anything else than error
*/
int LiafDTMsLayer::configure(CfgLineItems line, bool verbose) {
	if (line->at(0) == "thresh") {
		// check if neurons have already been created
		if (neurons.size() > 0) { // if so
			return 1; // error, trying to change common parameter after neurons were created
		} else {
			commonThresh = atof(line->at(1).c_str());
			if (verbose) std::cerr << "\tset commonThresh to " << commonThresh << "\n";
			return 0;
		}
	} else if (line->at(0) == "tdecay") {
		if (neurons.size() > 0) {
			return 1;
		} else {
			commonThreshDecay = atof(line->at(1).c_str());
			if (verbose) std::cerr << "\tset commonThreshDecay to " << commonThreshDecay << "\n";
			return 0;
		}
	} else if (line->at(0) == "tstep") {
		if (neurons.size() > 0) {
			return 1;
		} else {
			commonThreshStep = atof(line->at(1).c_str());
			if (verbose) std::cerr << "\tset commonThreshStep to " << commonThreshStep << "\n";
			return 0;
		}
	} else if (line->at(0) == "r") {
		if (neurons.size() > 0) {
			return 1;
		} else {
			commonR = atof(line->at(1).c_str());
			if (verbose) std::cerr << "\tset commonR to " << commonR << "\n";
			return 0;
		}
	} else if (line->at(0) == "c") {
		if (neurons.size() > 0) {
			return 1;
		} else {
			commonC = atof(line->at(1).c_str());
			if (verbose) std::cerr << "\tset commonC to " << commonC << "\n";
			return 0;
		}
	} else if (line->at(0) == "width") {
		if (neurons.size() > 0) {
			return 1;
		} else {
			width = atoi(line->at(1).c_str());
			if (verbose) std::cerr << "\tset width to " << width << "\n";
			return 0;
		}
	} else if (line->at(0) == "height") {
		if (neurons.size() > 0) {
			return 1;
		} else {
			height = atoi(line->at(1).c_str());
			if (verbose) std::cerr << "\tset height to " << height << "\n";
			return 0;
		}
	} else if (line->at(0) == "depth") {
		if (neurons.size() > 0) {
			return 1;
		} else {
			depth = atoi(line->at(1).c_str());
			if (verbose) std::cerr << "\tset depth to " << depth << "\n";
			return 0;
		}
	} else if (line->at(0) == "make") {
		// make neurons
		// check if dimensions have been set
		if (width == 0) {
			return 1;
		} else {
			int nNeurons = height*width*depth;
			for (int n = 0; n < nNeurons; n++) {
				add_neuron(commonThresh, commonThreshDecay, commonThreshStep, commonR, commonC);
			}
			if (verbose) std::cerr << "\tMade " << nNeurons << " neurons in layer " << id << "\n";
			return 0;
		}
	} else {
		return 1;
		//error unkown line
	}
}
