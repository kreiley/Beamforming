#include <stdio.h>
#include <math.h>
#include "delay.h"
#include "delaytest.h"

#define num_mics 8

typedef struct mic_array{
    	double x;
	double y;
	double delay_for_E;
	double delay_for_NE;
	double delay_for_N;
	double delay_for_NW;
	double delay_for_W;
	double delay_for_SW;
	double delay_for_S;
	double delay_for_SE;
	double current_delay;
} mic_array;

static mic_array mics[num_mics];
/*
double x_positions = {};
double y_positions = {};
*/
//void calculate_delay(double angle);
//void mic_array_init();
//double get_current_delay(int mic_number);

void calculate_delay(double angle){
	printf("\nAngle is : %f\n",angle);
	if(fmod(angle,45) != 0){
		double m = tan((angle*M_PI)/180);
		printf("\nSlope is : %f\n",m);
		double perp_m;
		perp_m = -1/m;
		printf("\nPerp Slope is : %f\n",perp_m);
		for(int i = 0; i < num_mics; i++){
			double b = mics[i].y - perp_m * mics[i].x ;
			printf("\nb =  %f , b/2 =%f\n m + perp_m = %f",b, b/2, m+perp_m);
			double x = b/(perp_m + m);
			double y = m * x;
			double c = sqrt(pow(x,2) + pow(y,2));
			printf("\nb = %f, x = %f, y=%f,c=%f",b,x,y,c);
			if(y < 0){c = c * -1;}
			mics[i].current_delay = c;		
		}
	}
	else if (fmod(angle, 360) == 0){
		for(int i = 0; i < num_mics; i++){
			mics[i].current_delay = mics[i].x;
		}
	}
	else if (fmod(angle, 180) == 0){
		for(int i = 0; i < num_mics; i++){
			mics[i].current_delay = -mics[i].x;
		}
	}
	else if (fmod(angle, 270) == 0){
		for(int i = 0; i < num_mics; i++){
			mics[i].current_delay = -mics[i].y;
		}
	}
	else if (fmod(angle, 90) == 0){
		for(int i = 0; i < num_mics; i++){
			mics[i].current_delay = mics[i].y;
		}
	}	
	else if (fmod(angle, 135) == 0){
		for(int i = 0; i < num_mics; i++){
			double b = mics[i].y - mics[i].x ;
			double x;
			if(mics[i].y - mics[i].x == 0) {x = 0; b =0;}
			else{x = -b/2;}
			double y = -1 * x;
			double c = sqrt(2)* x;
			printf("\nb = %f, x = %f, y=%f,c=%f",b,x,y,c);
			if(y < 0){c = c * -1;}
			mics[i].current_delay = c;	
		}
	}
	else if (fmod(angle, 45) == 0){
		for(int i = 0; i < num_mics; i++){
			double b = mics[i].y + mics[i].x ;
			double x;
			if(mics[i].y + mics[i].x == 0) {x = 0; b =0;}
			else{x = b/2;}
			double y = x;
			double c = sqrt(2)* x;
			printf("\nb = %f, x = %f, y=%f,c=%f",b,x,y,c);
			if(y < 0){c = c * -1;}
			mics[i].current_delay = c;	
		}
	}
	double temp = mics[0].current_delay;
	for(int i =1; i< num_mics; i++){
		if(mics[i].current_delay < temp){
			temp = mics[i].current_delay;
		}
	}
	for(int i =0; i < num_mics; i++){
		mics[i].current_delay = mics[i].current_delay - temp;
	}
}

void mic_array_init(){
	printf("\n Microphone Positions:\n");
	for(int i = 0; i < num_mics-1; i++){
		double theta = i * M_PI/4;
		double a =.01;
		double b =.8;
		double shift = -.5;
		mics[i].x = a*cos(theta - shift) * exp(b*(theta-shift)); 
		mics[i].y = a*sin(theta - shift) * exp(b*(theta - shift));
	}
	mics[num_mics-1].x = 0.489641;
	mics[num_mics-1].y = 0.08055;
	printf("\nx =\n");
	for(int i = 0; i < num_mics; i++){printf("%f\n",mics[i].x);}
	printf("\ny =\n");
	for(int i = 0; i < num_mics; i++){printf("%f\n",mics[i].y);}
}

double get_current_delay(int mic_number){
	return mics[mic_number].current_delay;
}

double get_x(int mic_number){
	return mics[mic_number].x;
}

double get_y(int mic_number){
	return mics[mic_number].y;
}
