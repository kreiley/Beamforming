/***********LOCALIZATION.h*************************************/
#ifndef __LOCALIZATION_H__
#define __LOCALIZATION_H__

typedef struct microphone {
	double x;
	double y;
	double * buffer;
} microphone;

extern microphone * build_mic(double x, double y, double * buffer);
extern double find_source(microphone * m1, microphone * m2, microphone * m3);
extern void set_buffer_size(int bf_size);
double distance_calc(microphone * m1, microphone * m2);
double degree_calc(microphone * m1, microphone * m2);
extern void init_triangle(microphone * m1, microphone * m2, microphone * m3); 

#endif
