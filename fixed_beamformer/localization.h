/***********LOCALIZATION.h*************************************/
#ifndef __LOCALIZATION_H__
#define __LOCALIZATION_H__

typedef struct microphone {
	double x;
	double y;
	double * buffer;
} microphone;

extern microphone * build_mic(double x, double y);
extern double find_source(microphone * m1, microphone * m2, microphone * m3);
extern void set_buffer_size(int bf_size);
double distance_calc(microphone * m1, microphone * m2);
double degree_calc(microphone * m1, microphone * m2);
extern void init_triangle(microphone * m1, microphone * m2, microphone * m3); 
extern void set_buffer(microphone * m1, double * buffer);
void find_mic_3(microphone * m1, microphone * m2);
double calc_right(double delay, double distance, double angle);
double calc_left(double delay, double distance, double angle);
double to_degrees(double angle);
#endif
