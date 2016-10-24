#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "delay_sound.h"
#include "FixedBeamformer.c"
#include "FixedBeamformer.h"
#include "localization.h"
#include <stdbool.h>
#include "Beamforming.h"

#define ANGLE_RESOLUTION 500    // Number of angle points to calculate
#define number_of_mics 8
#define feedback_volume .4
#define mix_volume .35
#define blend_parameter .4
#define buffer_size 100
#define BUFFER_SIZE 64000

double freq = 16000; //frequency of data input
double speed_of_sound = 340.3; //speed of sound at sea level
int filterLength = 11; //Numer of FIR filter taps for fractional delay(shouled be odd)
double angle = 0;
double volume = .75;
bool turn_off = false;
char mode = 'M';
microphone * m1;
microphone * m2;
microphone * m3;

void run(double * in);
void delays_init();
void localization_init();
void change_delay();
double locate(double * buffer1, double * buffer2, double * buffer3, int buf_size);
void sin_test();
void print_delay_info();

int main(int argc, char* argv[]){
	if(argc == 2){angle = atof(argv[1]);}
	if(argc == 3){angle = atof(argv[1]); volume = atof(argv[2]);}
	if(argc == 4){angle = atof(argv[1]); volume = atof(argv[2]); mode = argv[3][0];}
	mic_array_init();
	delays_init();
	print_delay_info();
	localization_init();
	sin_test();
	//run(in);
}

void run(double * in){
	double sum = 0;
	double output = 0;
	int buf = 0;
	double * buffer1 = malloc(sizeof(double *) * buffer_size);
	double * buffer2 = malloc(sizeof(double *) * buffer_size);
	double * buffer3 = malloc(sizeof(double *) * buffer_size);
	while(!turn_off){
		double sum;
		sum = delay_out(delays[0], in[0]);	
		sum+= delay_out(delays[1], in[1]);	
		sum+= delay_out(delays[2], in[2]);	
		sum+= delay_out(delays[3], in[3]);
		sum+= delay_out(delays[4], in[4]);
		sum+= delay_out(delays[5], in[5]);
		sum+= delay_out(delays[6], in[6]);
		sum+= delay_out(delays[7], in[7]);
		output = sum/number_of_mics;
		buf++;
		buffer1[buf] = in[4];
		buffer2[buf] = in[6];
		buffer3[buf] = in[7];
		if(buf == buffer_size){
			angle = locate(buffer1, buffer2, buffer3, buffer_size);
			change_delay();
			buf = 0;
		}
	}
}

void delays_init(){
	calculate_delay(angle);
	for(int i =0; i < number_of_mics; i++){
		double num_of_samples_to_delay = (freq * get_current_delay(i))/speed_of_sound;
		double fractional = fmod(num_of_samples_to_delay,1);	
		delays[i] = delay_init(num_of_samples_to_delay,fractional, feedback_volume, mix_volume,blend_parameter, i);
	}
}

void localization_init(){
	m1 = build_mic(get_x(4), get_y(4));
	m2 = build_mic(get_x(6), get_y(6));
	m3 = build_mic(get_x(7), get_y(7));
	init_triangle(m1,m2,m3);
	set_buffer_size(buffer_size);

}

void change_delay(){
	calculate_delay(angle);
	for(int i = 0; i < number_of_mics; i++){
		double num_of_samples_to_delay = (freq * get_current_delay(i))/speed_of_sound;
		double fractional = fmod(num_of_samples_to_delay, 1);
		delay_set_delay(delays[i], num_of_samples_to_delay, fractional);
	}
}

double locate(double * buffer1, double * buffer2, double * buffer3, int buf_size){
	set_buffer(m1, buffer1);
	set_buffer(m2, buffer2);
	set_buffer(m3, buffer3);
	set_buffer_size(buf_size);
	return find_source(m1,m2,m3);
}

void print_delay_info(){
	for(int i =0; i<number_of_mics; i++){
		double time_of_delay = get_current_delay(i)/speed_of_sound;
		double num_of_samples_to_delay = (freq * get_current_delay(i))/speed_of_sound;
		printf("\n\nMicrophone %d:\n",i);
		printf("Distance of mic in meters from furthest mic from Audio Source:%f\n",get_current_delay(i));
		printf("Time of delay in seconds: %f\n", time_of_delay);
		printf("Number of Samples to delay: %f \n\n",num_of_samples_to_delay);
	}
}

void sin_test(){
	double * y_mic[number_of_mics];
	double * y = malloc(8*180*sizeof(double));
	for (int g = 0; g < number_of_mics; g++){
		double *y_delayed = malloc(8*180*sizeof(double));
		int x = 0;
		for(double i = 0; i*M_PI/180 < 4*M_PI; i = i + 4){
			y[x] = sin(i*M_PI/180);
			y_delayed[x] = delay_out(delays[g], y[x]);
			x+=1;

		}
		y_mic[g] = y_delayed;

	}
	printf("\n\n:::::::ANGLE = %f::::::::::\n\n", locate(y_mic[4], y_mic[6], y_mic[7],buffer_size));
	printf("\n---------INPUT-------\n\n");
	for(int i = 0; i < 180; i++){
		printf("%f\n",y[i]);
	}
	for(int mic = 0; mic < number_of_mics; mic++){
		printf("\n---------MICROPHONE %d----------\n\n",mic);	
		for(int i = 0; i < 180; i++){
			printf("%f\n",y_mic[mic][i]);
		}
	}
}
