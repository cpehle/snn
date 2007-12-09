#ifndef _SNN_H
 #define _SNN_H

#include "snnbase.h"
#include "snnliaf.h"
#include "snnliafdt.h"
#include "snnaeif.h"
#include "snnaconn.h"
#include "snnsconn.h"
#include "snnstdpconn.h"
#include "cfgfile.h"

/**
 * \file snn.h
 * General spiking neuron library
 * \author Brett Graham
 * \date 12-04-07
 * \todo
 * - figure out projection building procedure
 * - fix projection building procedure
 * - split up unit & projection files into subdirectories
 * - document how to add a new unit or projection
 * - add error checking
 * - setup general cfg for snn
 * - setup single line neuron creation
*/

/// SNN
/**
	Spiking neural network class
*/
class SNN {
	public:
		int process_cfg_file(std::string filename = "net.cfg", bool verbose = false);
		int add_layer(CfgLineItems cfgLine);
		int add_projection(CfgLineItems cfgLine);
		int get_layer_index(std::string id);
		int get_projection_index(std::string id);
		
		std::vector<Layer*> layers;
		std::vector<Projection*> projections;
		
		void run_network(int timeSteps = 1) { for (int i = 0; i < timeSteps; i++) {step();}; };
		void step();
	private:
};

#endif
