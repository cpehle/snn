#include "snn.h"

int SNN::add_layer(CfgLineItems cfgLine) {
	// error checking
	if (cfgLine->at(3) == "liaf") {
		layers.push_back(new LiafMsLayer);
		layers.at(layers.size()-1)->id = cfgLine->at(2);
		return 0;
	} else if (cfgLine->at(3) == "liafdt") {
		layers.push_back(new LiafDTMsLayer);
		layers.at(layers.size()-1)->id = cfgLine->at(2);
		return 0;
	} else if (cfgLine->at(3) == "aeif") {
		layers.push_back(new AeifMsLayer);
		layers.at(layers.size()-1)->id = cfgLine->at(2);
		return 0;
	} else {
		// error, no sensible type found
		return 1;
	}
}

int SNN::add_projection(CfgLineItems cfgLine) {
	// if type = aconn, look for analog input (4) and layer (5)
	// if not, look for layer (4) and layer (5)
	if (cfgLine->at(3) == "ssynapse") {
		// get layer : &layers.at(get_layer_index(cfgLine->at(4)))
		// get layer : &layers.at(get_layer_index(cfgLine->at(5)))
		if (cfgLine->size() == 6) {
			projections.push_back(new SProjection(
				layers.at(get_layer_index(cfgLine->at(4))),
				layers.at(get_layer_index(cfgLine->at(5)))
				));
			projections.at(projections.size()-1)->id = cfgLine->at(2);
			return 0;
		} else {
			return 1;
		}
	} else if (cfgLine->at(3) == "asynapse") {
		projections.push_back(new AProjection(
			analogs.at(get_analog_index(cfgLine->at(4))),
			layers.at(get_layer_index(cfgLine->at(5)))
			));
		projections.at(projections.size()-1)->id = cfgLine->at(2);
		return 0;
	} else if (cfgLine->at(3) == "stdpsynapse") {
		projections.push_back(new STDPProjection);
		projections.at(projections.size()-1)->id = cfgLine->at(2);
		return 0;
	} else {
		return 1;
	}
}

//int SNN::add_analog(CfgLineItems cfgLine) {
//	if (cfgLine->size() == 3) {
//		analogs.push_back(new AnalogArray);
//		AnalogArray * temp = analogs.at(projections.size()-1);
//		temp->id = cfgLine->at(1);
//		if (cfgLine->at(2) == "int") {
//			temp->type = INT;
//			return 0;
//		} else if (cfgLine->at(2) == "double") {
//			temp->type = DOUBLE;
//			return 0;
//		} else if (cfgLine->at(2) == "uchar") {
//			temp->type = UCHAR;
//			return 0;
//		} else if (cfgLine->at(2) == "char") {
//			temp->type = CHAR;
//			return 0;
//		} else if (cfgLine->at(2) == "float") {
//			temp->type = FLOAT;
//			return 0;
//		} else {
//			return 1;
//		}
//	} else {
//		return 1;
//	}
//}

int SNN::add_analog( std::string id, DataType type, int width, int height, int depth) {
	analogs.push_back(new AnalogArray);
	AnalogArray * temp = analogs.at(analogs.size()-1);
	temp->id = id;
	temp->type = type;
	temp->width = width;
	temp->height = height;
	temp->depth = depth;
	return 0;
}

/// Process configuration file
/**
 return 0 if all is well
 any other return means error!
 Document this!!
 */
int SNN::process_cfg_file(std::string filename, bool verbose) {
	CfgFile myCfg(filename);
	CfgLineItems myItems = myCfg.get_data_ptr();
	while (! myCfg.get_cfg_eof()) {
		if ((myItems->size() == 2) && (myItems->at(0) == "import")) {
			if (verbose) std::cout << "entering " << myItems->at(1) << "\n";
			this->process_cfg_file(myItems->at(1),verbose);
		} else if ((myItems->at(0) == "start")) { // in a start statement
			// each layer definition starts with 'start layer <id> <type>' and ends with 'end layer <id> <type>'
			if (myItems->at(1) == "layer") {
				// make layer of id = myItems->at(2) and type = myItems->at(3)
				if (add_layer(myItems)) return 1;
				int layerIndex = layers.size() - 1;
				if (verbose) std::cout << "making layer " << myItems->at(2) << " of type " << myItems->at(3) << " at index " << layerIndex << "\n";
				// keep feeding lines until "end layer <id> <type>"
				myCfg.advance();
				while ((myItems->at(0) != "end") && (! myCfg.get_cfg_eof())) {
					if (layers.at(layerIndex)->configure(myItems, verbose)) return 1; // return 1 on error
					myCfg.advance();
				}
				// check if id & type agree?
			} else if (myItems->at(1) == "projection") {
				// make projection of id = myItems->at(2) and type = myItems->at(3)
				if (verbose) std::cout << "adding projection\n";
				if (add_projection(myItems)) return 1;
				int projectionIndex = projections.size() -1;
				if (verbose) std::cout << "making projection " << myItems->at(2) << " of type " << myItems->at(3) << " at index " << projectionIndex << "\n";
				// keep feeding lines until "end projection <id> <type>"
				myCfg.advance();
				while ((myItems->at(0) != "end") && (! myCfg.get_cfg_eof())) {
					if (projections.at(projectionIndex)->configure(myItems, verbose)) return 1; // return 1 on error
					myCfg.advance();
				}
			}
			// --testing--
//			for (unsigned int i = 0; i < myItems->size(); i++) {
//				std::cout << myItems->at(i) << " ";
//			}
//			std::cout << "\n";
//		} else if ((myItems->at(0) == "analog")) {
//			//if (add_analog(myItems)) return 1;
//			// analog <id> <type>
		} else if ((myItems->at(0) == "timesteps")) {
		}
		myCfg.advance();
	}
}

int SNN::get_layer_index(std::string id) {
	for (int i=0; i < layers.size(); i++) {
		if (layers[i]->id == id) return i;
	}
}

int SNN::get_projection_index(std::string id) {
	for (int i=0; i < projections.size(); i++) {
		if (projections[i]->id == id) return i;
	}
}

int SNN::get_analog_index(std::string id) {
	for (int i=0; i < analogs.size(); i++) {
		if (analogs[i]->id == id) return i;
	}
}

void SNN::step () {
	// inputs
	for (int i = 0; i < layers.size(); i++) {
		layers[i]->step();
	}
//	std::cout << "layers stepped\n";
	for (int i = 0; i < projections.size(); i++) {
//		std::cout << "stepping " << projections[i]->id << "\n";
		projections[i]->step();
	}
	// outputs
//	std::cout<< "stepped\n";
}
