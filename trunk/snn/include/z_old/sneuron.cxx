#include "sneuron.h"
// definitions of forward declarations

double d_C = 281.0 ; //nF
double d_g_L = 30.0 ; //nS
double d_E_L = -0.0706 ; //V
double d_V_T = -0.0504 ; //V
double d_d_T = 0.002 ; //V
double d_t_w = 144 ; //nSec from 144000000 due to calc every ms
double d_a = 4.0 ; //nS
double d_b = 0.0805 ; //nA
double d_spike_threshold = 0.02;

AeifMs::AeifMs (bool bursting, double resetValue, double thresholdmV) {
//AeifMs::AeifMs (bool bursting = false, double resetValue = 0.003, double thresholdmV = 0.02) {
	dact = 0.0;
	dinput = 0.0;
	uspikeAct = 0;
	bburst = bursting;
	dreset = resetValue;
	dthresh = thresholdmV;
	dw = 0.0;
}


void AeifMs::step () {
	dw += (d_a * (dact - d_E_L) - dw) / (d_t_w) ; // attempted after vm+=, got funny results... still slightly off brette 2005
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
