#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "localization.h"

#define threshold 0.5
#define speed_of_sound 343.0
#define freq 16000

/*assumes a equalateral triangle microphone array because its easier*/

int buffer_size = 0;
double distance = 0;

struct microphone {
	double x;
	double y;
	double* buffer;
} microphone;


microphone * localization_init(double x, double y, double * buffer){
	microphone * mic = malloc(sizeof(microphone));
	mic->x = x;
	mic->y = y;
	mic->buffer = buffer;
	return mic;
}

/*tan((speed of sound * number of samples / freq)/ distance) = angle */ 

int find_source(microphone * m1, microphone * m2, microphone * m3){
	bool done1  = false;
	bool done2  = false;
	bool done3  = false;
	int first = 0;
	int second = 0;
	int third = 0;
	int num_samples1 = -1;
	int num_samples2 = -1;
	for(int i = 0; i < buffer_size; i++){
		if(!done1 && m1->buffer[i] >= threshold){
			done1 = true;
			if(!first){first = 1;}
			else if(!second){second = 1;}
			else {third = 1;}
		}
		if(!done2 && m2->buffer[i] >= threshold){
			done2 = true;
			if(!first){first = 2;}
			else if(!second){second = 2;}
			else {third = 2;}
		
		}
		if(!done3 && m3->buffer[i] >= threshold){
			done3 = true;
			if(!first){first = 3;}
			else if(!second){second = 3;}
			else {third = 3;}
		
		}
		if(first && !second){
			num_samples1++;
			num_samples2++;
		}
		if(second){
			num_samples2++;
		}
		if(third){break;}
	}
	if(num_samples1 < 0){return -1;}
	double reference_angle = atan(((speed_of_sound * num_samples1)/freq)/distance);
}

void set_buffer_size(int bf_size){
	buffer_size = bf_size;
}

void calc_distance(microphone m1, microphone m2){
	double a, b;
	a = m1->x - m2->x;
	b = m1->y - m2->y;	
	distance = sqrt(a*a + b*b);
}

