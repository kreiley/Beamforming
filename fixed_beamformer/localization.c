#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include "localization.h"

#define threshold 0.5
#define speed_of_sound 343.0
#define freq 16000

/*must be triangle microphone array because its easier*/

int buffer_size = 0;
double distance12 = 0;
double distance13 = 0;
double distance23 = 0;
double angle12 = 0;
double angle13 = 0;
double angle21 = 0;
double angle23 = 0;
double angle31 = 0;
double angle32 = 0;

microphone * build_mic(double x, double y, double * buffer){
	microphone * mic = malloc(sizeof(microphone));
	mic->x = x;
	mic->y = y;
	mic->buffer = buffer;
	return mic;
}

/*tan((speed of sound * number of samples / freq)/ distance) = angle */ 

double find_source(microphone * m1, microphone * m2, microphone * m3){
	bool done1  = false;
	bool done2  = false;
	bool done3  = false;
	int first = 0;
	int second = 0;
	int third = 0;
	int num_samples1 = -1;
	int num_samples2 = -1;
	for(int i = buffer_size - 1; i >= 0; i--){
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
	double delay = (speed_of_sound * num_samples1)/freq;
	if     (first == 1 && second == 2){return atan(delay/distance12) + angle12;}
	else if(first == 1 && second == 3){return atan(delay/distance13) + angle13;}
	else if(first == 2 && second == 1){return atan(delay/distance12) + angle21;}
	else if(first == 2 && second == 3){return atan(delay/distance23) + angle23;}
	else if(first == 3 && second == 1){return atan(delay/distance13) + angle31;}
	else if(first == 3 && second == 2){return atan(delay/distance23) + angle32;}
	return -1;
}

void set_buffer_size(int bf_size){
	buffer_size = bf_size;
}

double distance_calc(microphone * m1, microphone * m2){
	double a, b;
	a = m1->x - m2->x;
	b = m1->y - m2->y;	
	return sqrt(a*a + b*b);
}

double degree_calc(microphone * m1, microphone *m2){
	double x , y, m, theta, rho;
	x = m2->x - m1->x;
	y = m2->y - m1->y;
	if(x == 0 && y == 0){printf("error");return -7;}
	else if(x == 0){rho = M_PI/2;}
	else if(y == 0){rho = 0;}
	else {
		m = y / x;
		rho = atan(m);
	}
	if(x > 0 && y > 0){return rho;}
	else if (x < 0){return rho + M_PI/2;}
	else if (x > 0 && y < 0){return rho + 2*M_PI;}
}

void init_triangle(microphone * m1, microphone * m2, microphone * m3){
	angle12 = degree_calc(m1, m2);
	angle13 = degree_calc(m1, m3);
	angle21 = degree_calc(m2, m1);
	angle23 = degree_calc(m2, m3);
	angle31 = degree_calc(m3, m1);
	angle32 = degree_calc(m3, m2);
	distance12 = distance_calc(m1, m2);
	distance13 = distance_calc(m1, m3);
	distance23 = distance_calc(m2 ,m3); 
}

