/***********DELAY.h*************************************/
#ifndef __DELAY_H__
#define __DELAY_H__

typedef struct microphone {
	double x;
	double y;
	double * buffer;
} microphone;

microphone * build_mic(double x, double y, double * buffer);
double find_source(microphone * m1, microphone * m2, microphone * m3);
void set_buffer_size(int bf_size);
double distance_calc(microphone * m1, microphone * m2);
double degree_calc(microphone * m1, microphone * m2);
void init_triangle(microphone * m1, microphone * m2, microphone * m3); 

#endif
