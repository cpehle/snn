#include "snn.h"
//#include "cv.h"
//#include "highgui.h"

int main(int argc, char* argv[]) {
/*
	AnalogArray myAA;
	myAA.data = (void*)(new float [3]); // assign values to myAA.data
	myAA.type = FLOAT;
	float* temp = (float*)myAA.data;
	temp[0] = 0.0;
	temp[1] = 1.0;
	temp[2] = 2.0;
	
	float* newTemp = (float*)myAA.data;
	std::cout << *((float*)(myAA.get_value(0)))
		<< " " << *((float*)(myAA.get_value(1)))
		<< " " << *((float*)(myAA.get_value(2)))
		<< "\n";
//	std::cout << newTemp[0] << " " << newTemp[1] << " " << newTemp[2] << "\n"; 
*/

	SNN snn;
	int uUnits;
	// make a1 : set type HERE NOT in the cfg file
	uUnits = 10;
	if (snn.add_analog("a1",UCHAR,10)) std::cout << "error building analog a1\n";
//	std::cout << "making inputs\n";
	uchar input[10];
	for (int i=0; i<10; i++) {
		input[i] = (uchar)(i);
	}
	snn.analogs[snn.get_analog_index("a1")]->data = (void*)input;
	
//	// ---- OpenCV testing ----
//	CvCapture* capture;
//	capture = cvCaptureFromAVI("out.avi");
//	if (!capture) return -1;
//	IplImage* img = 0;
//	if (!cvGrabFrame(capture)) {
//		std::cerr << "capture failed\n";
//		return -1;
//	}
//	img = cvRetrieveFrame(capture);
//	CvSize size = cvGetSize(img);
//	IplImage* grayImg = cvCreateImage(size,img->depth,1);
//	cvCvtColor(img,grayImg,CV_RGB2GRAY);
//	uchar* data = (uchar *)grayImg->imageData;
//	// ---- end OpenCV ----
//	
//	uUnits = 19200;
//	if (snn.add_analog("a1",UCHAR,uUnits)) std::cout << "error building analog a1\n";
//	snn.analogs[snn.get_analog_index("a1")]->data = (void*)data;

	bool verbose = true;
//	std::cout << "configuring network\n";
	snn.process_cfg_file("cfg/smallTest.cfg");
//	std::cout << "running network\n";
	for (int t=0; t< 10; t++) {
		// set input
//		for (int i=1; i<2; i++) {
//			input[i] += 0.01;
//		}
//		std::cout << "stepping\n";
		// step
		snn.step();
//		std::cout << "reading output\n";
		// read output
//		for (int i=0; i<10; i++) {
//			std::cout << *((double*)(snn.analogs[snn.get_analog_index("a1")]->get_value(i))) << " ";
//		}
//		std::cout << "\n";
		
		// print activity for l1
		for (int i=0; i<uUnits; i++) {
			std::cout << snn.layers[snn.get_layer_index("l1")]->neurons[i]->getAct() << " ";
		}
		std::cout << "\n";
		
		// print spiking for l1
		for (int i=0; i<uUnits; i++) {
			std::cout << snn.layers[snn.get_layer_index("l1")]->neurons[i]->getSpiked() << " ";
		}
		std::cout << "\n";
		
//		// print act for l2
//		for (int i=0; i<uUnits; i++) {
//			std::cout << snn.layers[snn.get_layer_index("l2")]->neurons[i]->getAct() << " ";
//		}
//		std::cout << "\n";
//		// print spiking for l2
//		for (int i=0; i<uUnits; i++) {
//			std::cout << snn.layers[snn.get_layer_index("l2")]->neurons[i]->getSpiked() << " ";
//		}
//		std::cout << "\n";
		std::cout << "\n";
	}
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

