#ifndef _SNN_H
 #define _SNN_H

#include "snn/snnbase.h"
#include "snn/snnliaf.h"
#include "snn/snnliafdt.h"
#include "snn/snnaeif.h"
#include "snn/snnaconn.h"
#include "snn/snnsconn.h"
#include "snn/snnstdpconn.h"
#include "cfgfile.h"

/**
 * \file snn.h
 * General spiking neuron library
 * \author Brett Graham
 * \date 12-04-07
 * \todo
 * - figure out projection building procedure
 * - fix projection building procedure
 * - document how to add a new unit or projection
 * - add error checking
 * - setup general cfg for snn
 * - setup single line neuron creation
 * - setup input and output
 * - document snn.configure
 * - AProjection - split up into separate data types?
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
		//int add_analog(CfgLineItems cfgLine);
		int add_analog( std::string id, DataType type, int width, int height=1, int depth=1);
		int get_layer_index(std::string id);
		int get_projection_index(std::string id);
		int get_analog_index(std::string id);
		
		std::vector<Layer*> layers;
		std::vector<Projection*> projections;
		std::vector<AnalogArray*> analogs;
		
		void run_network(int timeSteps = 1) { for (int i = 0; i < timeSteps; i++) {step();}; };
		void step();
	private:
};

#endif
