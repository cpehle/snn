#include "snnaeif.h"
#include "atob.h"

/// default values from Brette 2006
double d_C = 281.0 ; //nF
double d_g_L = 30.0 ; //nS
double d_E_L = -0.0706 ; //V
double d_V_T = -0.0504 ; //V
double d_d_T = 0.002 ; //V
double d_t_w = 144 ; //nSec from 144000000 due to calc every ms
double d_a = 4.0 ; //nS
double d_b = 0.0805 ; //nA
double d_spike_threshold = 0.02;

/// assign neuron parameters
AeifMs::AeifMs (double thresholdmV, bool bursting, double resetValue) {
//AeifMs::AeifMs (bool bursting = false, double resetValue = 0.003, double thresholdmV = 0.02) {
	dact = 0.0;
	dinput = 0.0;
	uspikeAct = 0;
	bburst = bursting;
	dreset = resetValue;
	dthresh = thresholdmV;
	dw = 0.0;
}

///
void AeifMs::step () {
	/// attempted updating dw after vm+=, got funny results... still slightly off brette 2005
	dw += (d_a * (dact - d_E_L) - dw) / (d_t_w) ; 
	//w *= 2.0;
	dact += ( -d_g_L * ( dact - d_E_L ) + d_g_L * d_d_T * exp( (dact-d_V_T)/d_d_T) - dw + dinput ) / (d_C);
	//w += (d_a * (vm - d_E_L) - w) / (d_t_w) ;
	if ( dact >= dthresh ) {
		if (bburst) {
			dact = d_V_T + dreset;
		} else {
			dact = d_E_L;
		}
		dw += d_b;
		addSpike();
	}else{
		addNoSpike();
	};
	dinput = 0.0;
}

AeifMsLayer::AeifMsLayer() {
	commonThresh = 0.02;
	commonBursting = false;
	commonReset = 0.003;
	width = 0;
	height = 1;
	depth = 1;
}

void AeifMsLayer::add_neuron() {
	SNeuron* buff = new AeifMs();
	neurons.push_back(buff);
}

void AeifMsLayer::add_neuron(double thresholdmV, bool bursting, double resetValue) {
	SNeuron* buff = new AeifMs(thresholdmV, bursting, resetValue);
	neurons.push_back(buff);
}

/// Parse config file line
/**
	Valid lines are:
	- \b thresh : sets threshold (commonThresh)
	- \b bursting : set neurons as bursting (commonBursting)
	- \b reset : set reset value (commonReset)
	- \b width : sets layer width, this is the only necessary parameter
	- \b height : sets layer height, defaults to 1
	- \b depth : sets layer depth, defaults to 1
	- \b make : constructs the neurons using the common<X> parameters assigned above
	need to add error checking and allow for 'manual' creation of neurons
	\param line : vector of strings containing items of a configuration line
	\param verbose : set true if you want lots of output
	Returns 0 if all is OK, if anything else than error
*/
int AeifMsLayer::configure(CfgLineItems line, bool verbose) {
	if (line->at(0) == "thresh") {
		// check if neurons have already been created
		if (neurons.size() > 0) { // if so
			return 1; // error, trying to change common parameter after neurons were created
		} else {
			commonThresh = atof(line->at(1).c_str());
			if (verbose) std::cerr << "\tset commonThresh to " << commonThresh << "\n";
			return 0;
		}
	} else if (line->at(0) == "bursting") {
		if (neurons.size() > 0) {
			return 1;
		} else {
			commonBursting = atob(line->at(1).c_str());
			if (verbose) std::cerr << "\tset commonBursting to " << commonBursting << "\n";
			return 0;
		}
	} else if (line->at(0) == "reset") {
		if (neurons.size() > 0) {
			return 1;
		} else {
			commonReset = atof(line->at(1).c_str());
			if (verbose) std::cerr << "\tset commonReset to " << commonReset << "\n";
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
				add_neuron(commonThresh, commonBursting, commonReset);
			}
			if (verbose) std::cerr << "\tMade " << nNeurons << " neurons in layer " << id << "\n";
			return 0;
		}
	} else {
		return 1;
		//error unkown line
	}
}
