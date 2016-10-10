#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "delay_sound.h"
#include "FixedBeamformer.h"
#include "FixedBeamformer.c"

#define ANGLE_RESOLUTION 500    // Number of angle points to calculate
#define number_of_mics 8

double freq = 16000; //frequency of data input
double speed_of_sound = 340.3; //speed of sound at sea level
int filterLength = 11; //Numer of FIR filter taps for fractional delay(shouled be odd)

delay *delays[number_of_mics];

void fractional_delay(double frac_delay);

int main(int argc, char* argv[])
{
	double angle;
	double volume;
	if(argc == 1){angle = atof(argv[0]);}
	if(argc == 2){angle = atof(argv[0]); volume = atof(argv[1]);}
	mic_array_init();
	calculate_delay(angle);
	for(int i =0; i<number_of_mics; i++){
		double time_of_delay = get_current_delay(i)/speed_of_sound;
		double num_of_samples_to_delay = (freq * get_current_delay(i))/speed_of_sound;
	//	fractional_delay(fmod(num_of_samples_to_delay, 1));
		printf("\n\nMicrophone %d:\n",i);
		printf("Distance of mic in meters from furthest mic from Audio Source:%f\n",get_current_delay(i));
		printf("Time of delay in seconds: %f\n", time_of_delay);
		printf("Number of Samples to delay: %f \n\n",num_of_samples_to_delay);
	}
	for(int i =0; i < number_of_mics; i++){
		double num_of_samples_to_delay = (freq * get_current_delay(i))/speed_of_sound;
		double fractional = fmod(num_of_samples_to_delay,1);	
		delays[i] = delay_init((freq * get_current_delay(i))/speed_of_sound,fractional, 1, 1,1, i);
	}

	double *y= (double *) malloc(8*180*sizeof(double));
	double *y_delayed = (double *) malloc(8*180*sizeof(double));
	int x = 0;
	printf("\nreached\n");
	double samples = (freq * .5)/speed_of_sound;
	double samples_frac = fmod(samples, 1);
		
	delay *dd = delay_init(samples,samples_frac, 1,.1,.1,1);
		
	for(double i = 0; i*M_PI/180 < 4*M_PI; i = i + 1){
		y[x] = sin(i*M_PI/180);
		//printf("%f ", y[x]);
		y_delayed[x] = delay_out(dd, y[x]);
		x+=1;
	}
	printf("\nreached\n");

	for(int i = 0; i < 4*180; i++){
		printf("%f\n",y[i]);
	}

	printf("\n\n---------DELAYED----------\n\n");	
	
	for(int i = 0; i < 4*180; i++){
		printf("%f\n",y_delayed[i]);
	}
}

void fractional_delay(double frac_delay){
	int filterLength = 11;             // Number of FIR filter taps (should be odd)
	int centreTap = filterLength / 2;  // Position of centre FIR tap

	for (int t=0 ; t<filterLength ; t++)
	{
	   // Calculated shifted x position
	   double x = t - frac_delay;

	   // Calculate sinc function value
	   double sinc = sin(M_PI * (x-centreTap)) / (M_PI * (x-centreTap));

	   // Calculate (Hamming) windowing function value
	   double window = 0.54 - 0.46 * cos(2.0 * M_PI * (x+0.5) / filterLength);

	   // Calculate tap weight
	   double tapWeight = window * sinc;

	   // Output information
	   printf("%3d % f % f % f\n", t, sinc, window, tapWeight);
	}
}


