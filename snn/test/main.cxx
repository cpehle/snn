#include "snn.h"

int main(int argc, char* argv[]) {
//	Layer* pLayer = new LiafMsLayer();
//	pLayer->add_neuron();
	SNN snn;
	bool verbose = true;
	snn.process_cfg_file("cfg/net.cfg", verbose);
	snn.step();
//	snn.run_network(100);
}


//// --------------------------- cfg file test
//#include "cfgfile.h"

//int process_cfg(std::string filename) {
//	CfgFile myCfg(filename);
//	CfgLineItems myItems = myCfg.get_data_ptr();
//	while (! myCfg.get_cfg_eof()) {
//		if ((myItems->size() == 2) && (myItems->at(0) == "import")) {
//			std::cout << "entering " << myItems->at(1) << "\n";
//			process_cfg(myItems->at(1));
//		} else {
//			for (unsigned int i = 0; i < myItems->size(); i++) {
////				if (myItems->at(i) == "connfile") {
////					printf("connfile found\n");
////				} else if (myItems->at(i) == "unitfile") {
////					printf("unitfile found\n");
////				}
//				std::cout << myItems->at(i) << " ";
//			}
//			std::cout << "\n";
//		}
//		myCfg.advance();
//	}
//}

//int main( int argc , char* argv[] ){
//	process_cfg("net.cfg");
//}

