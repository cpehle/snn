#include "snn.h"
#include "cv.h"
#include "highgui.h"

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <unistd.h>

// TODO highpass filter input!!!!!!!!!!!

bool verbose = false;
char* inputFile = "out.avi";

struct CosyneOptions {
	bool verbose;
	char* inputFile;
	// TODO, timesteps is actually how many FRAMES until stop
	int timesteps;
	int fps;
	char* cfgFile;
};

void init_options(CosyneOptions* options) {
	options->verbose = false;
	options->inputFile = "out.avi";
	options->timesteps = 0;
	options->fps = 0;
	options->cfgFile = "cfg/net.cfg";
}

void parse_options(CosyneOptions* options, int argc, char** argv) {
	const char* optflags = "i:t:f:hv";
	//const char* optflags = "i:o:s:k:b:f:d:cvgh";
	int ch;
	// use getopt to parse the flags
	while(-1 != (ch = getopt(argc, argv, optflags))) {
		switch(ch) {
			// case values must match long_options
			case 'i': // infile
				options->inputFile = optarg;
				break;
			case 't': // timesteps
				options->timesteps = atoi(optarg);
				break;
			case 'v': // verbose
				options->verbose = true;
				break;
			case 'f': // cfgFile
				options->cfgFile = optarg;
				break;
			case '?': // help
			case 'h':
			case ':':
			default:  // unknown
				//std::cout << "error in command line options\n";
				std::cerr << "ERROR: parsing command line option\n";
//				print_usage(argc, argv);
				exit (-1);
		}
	}
}

int main(int argc, char* argv[]) {
	CosyneOptions* options = new CosyneOptions;
	init_options(options);
	parse_options(options, argc, argv);
	SNN snn;
	int uUnits;
	
//	// make a1 : set type HERE NOT in the cfg file
//	uUnits = 10;
//	if (snn.add_analog("a1",UCHAR,10)) std::cout << "error building analog a1\n";
////	std::cout << "making inputs\n";
//	uchar input[10];
//	for (int i=0; i<10; i++) {
//		input[i] = (uchar)(i);
//	}
//	snn.analogs[snn.get_analog_index("a1")]->data = (void*)input;
	
	// ---- OpenCV testing ----
	CvCapture* capture;
	capture = cvCaptureFromAVI(options->inputFile);
	if (!capture) {
		std::cerr << "could not open avi file\n";
		return -1;
	}
	if (options->verbose) std::cerr << "opened " << options->inputFile << "\n";
	IplImage* img = 0;
	if (!cvGrabFrame(capture)) {
		std::cerr << "capture failed\n";
		return -1;
	}
	img = cvRetrieveFrame(capture);
	CvSize size = cvGetSize(img);
	IplImage* grayImg = cvCreateImage(size,img->depth,1);
	cvCvtColor(img,grayImg,CV_RGB2GRAY);
	uchar* data = (uchar *)grayImg->imageData;
	// get fps
	cvQueryFrame(capture);
	options->fps = (int) cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	if (options->verbose) std::cerr << options->fps << " fps\n";
	// ---- end OpenCV ----
	
	uUnits = 19200;
	if (snn.add_analog("a1",UCHAR,160,120)) std::cout << "error building analog a1\n";
	snn.analogs[snn.get_analog_index("a1")]->data = (void*)data;

	snn.process_cfg_file(options->cfgFile, options->verbose);
	if (options->verbose) std::cerr << "processed " << options->cfgFile << "\n";
	bool framesLeft = true;
	int frame = 0;
	if (! options->fps) {
		std::cerr << "ERROR: fps could not be determined from file\n";
		return -1;
	}
//	std::cerr << options->timesteps << " timesteps\n";
//	std::cerr << options->inputFile << " inputFile\n";
	int stepsPerFrame = 1000/options->fps;
	while (framesLeft) {
//	for (int t=0; t< 10; t++) {
		//TODO calculate how many steps to take based on fps??
		if (options->verbose) std::cerr << "stepping network for frame " << frame << "\n";
		int timestep = stepsPerFrame * frame;
		for (int t=0; t < stepsPerFrame; t++) {
			snn.step();
		
	//		// print activity for l1
	//		for (int i=0; i<uUnits; i++) {
	//			std::cout << snn.layers[snn.get_layer_index("l1")]->neurons[i]->getAct() << " ";
	//		}
	//		std::cout << "\n";
			
			// print spiking for l1
			int act = 0;
			for (int i=0; i<uUnits; i++) {
				if (snn.layers[snn.get_layer_index("retina")]->neurons[i]->getSpiked()) ++act;
	//			std::cout << snn.layers[snn.get_layer_index("l1")]->neurons[i]->getSpiked() << " ";
			}
			std::cout << (timestep+t) << " ";
			std::cout << act << " ";
			act = 0;
			for (int i=0; i<uUnits; i++) {
				if (snn.layers[snn.get_layer_index("tectum")]->neurons[i]->getSpiked()) ++act;
			}
			std::cout << act << " ";
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
		} // end stepsPerFrame
//		std::cout << "\n";
//		std::cout << "\n";
//		std::cerr << frame << " frames " << options->timesteps << " timesteps " << options->inputFile << " inputFile\n";;
//		if (frame >= options->timesteps) std::cerr << "frame greater than timesteps\n";
		if ((options->timesteps != 0) && (frame >= options->timesteps)) {
//			std::cerr << "this should quit the program\n";
			framesLeft = false;
		}
		if (options->verbose) std::cerr << "grabbing next frame\n";
		if (!cvGrabFrame(capture)) {
			framesLeft = false;
		} else {
			img = cvRetrieveFrame(capture);
			cvCvtColor(img,grayImg,CV_RGB2GRAY);
			++frame;
		};
	}
	if (options->verbose) std::cerr << "closing program\n";	
	cvReleaseCapture( &capture );
	return 0;
}

