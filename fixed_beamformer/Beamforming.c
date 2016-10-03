#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "delay.h"
#include "FixedBeamformer.h"
#include "delaytest.h"
#include "FixedBeamformer.c"

#define ANGLE_RESOLUTION 500    // Number of angle points to calculate

double freq = 16000; //frequency of data input
double speed_of_sound = 340.3; //speed of sound at sea level
int filterLength = 11; //Numer of FIR filter taps for fractional delay(shouled be odd)
int number_of_mics = 8;

void fractional_delay(double frac_delay);

int main(void)
{
	mic_array_init();
	double angle = 45.0;
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
		Delay_Init((freq * get_current_delay(i))/speed_of_sound,1, 1,1, i);
	}

	double *y= (double *) malloc(8*180*sizeof(double));
	double *y_delayed = (double *) malloc(8*180*sizeof(double));
	int x = 0;
	printf("reached");
		Delay_Init((freq * .5)/speed_of_sound, 1,1,1,1);
		
		for(double i = 0; i*M_PI/180 < 4*M_PI; i = i + 1){
			y[x] = sin(i*M_PI/180);
			//printf("%f ", y[x]);
			y_delayed[x] = Delay_task(y[x]);
			x+=1;
		}
		//printf("reached");

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
