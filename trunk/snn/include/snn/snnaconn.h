#ifndef _SNNACONN_H
 #define _SNNACONN_H

#include "snnbase.h"

/// Analog to spiking neuron model synapse class
/**
	Connects an analog value (of type double) to a spiking neuron model
*/
class ASynapse: public Synapse {
	public:
		/// Constructor
		/**
			Connects a presynaptic analog input (double) and postsynaptic neuron model
		*/
		ASynapse (DataType inputType, void* pPre, double* PostInput, double weight = 1.0) {
//			std::cerr << "analog input " << *pPre << "\n";
			type = inputType;
			ppreAnalog = pPre;
			ppostInput = PostInput;
			dweight = weight;
		}
		/// Update function
		/**
			Multiplies analog input by weight value and adds result to the postsynaptic input
		*/
		void step () {
//			std::cerr << *ppreAnalog << "\n";
			switch (type) {
				case FLOAT : {
					float* tempFloat = (float*)ppreAnalog;
					*ppostInput += *tempFloat * dweight;
//					return (void*)&tempFloat[x];
					break; }
				case DOUBLE : {
					double* tempDouble = (double*)ppreAnalog;
					*ppostInput += *tempDouble * dweight;
//					return (void*)&tempDouble[x];
					break; }
				case CHAR : {
					char* tempChar = (char*)ppreAnalog;
					/** \todo
					 * - do I need this cast to an int?
					*/
					*ppostInput += (int)*tempChar * dweight;
//					return (void*)&tempChar[x];
					break; }
				case UCHAR : {
					uchar* tempUchar = (uchar*)ppreAnalog;
					/** \todo
					 * - do I need this cast to an int?
					*/
					*ppostInput += (int)*tempUchar * dweight;
//					return (void*)&tempUchar[x];
					break; }
				case INT : {
					int* tempInt = (int*)ppreAnalog;
					*ppostInput += *tempInt * dweight;
//					return (void*)&tempInt[x];
					break; }
				default :
					std::cerr << "ERROR: no sensible type found\n";
					//return NULL;
			};
		}
	protected:
		void* ppreAnalog;
		double* ppostInput;
		DataType type;
};

class AProjection: public Projection {
	public:
		AProjection(AnalogArray* Pre, Layer* Post);
		AnalogArray* pre;
		Layer* post;
		DataType commonType;
		void add_synapse();
		void add_synapse(DataType inputType, void* pPre, double* PostInput);
		void add_synapse(DataType inputType, void* pPre, double* PostInput, double weight);
		int connect_topographic();
		int connect_full();
		int configure(CfgLineItems line, bool verbose);
};

#endif
