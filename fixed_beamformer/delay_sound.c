#include "delay_sound.h"
#include "math.h"
#include "FixedBeamformer.h"
#include <stdlib.h>

#define BUFFER_SIZE 64000

extern delay * delay_init(double num_samples_to_delay, double fractional_delay, double fb_v, double dtm_v, double delay_blend_param, int mic) {
	delay *d;
	if(((d =  malloc(sizeof(delay))) == NULL)){return NULL;}
	else{
		d->buffer = malloc(sizeof (double) * BUFFER_SIZE);
		if(d->buffer == NULL){return NULL;}
		else{
			for(int i = 0; i < BUFFER_SIZE ; i++){
				d->buffer[i] = 0;
			}
		}
	}
	delay_set_delay(d, num_samples_to_delay, fractional_delay);
	delay_set_feedback_volume(d, fb_v);
	delay_set_delay_tap_mix_volume(d, dtm_v);
	delay_set_blend_param(d, delay_blend_param);
	delay_set_mic(d, mic);
	d->write_pointer = &d->buffer[BUFFER_SIZE - 1];
	return d;
}

void delay_set_delay(delay *d, double num_samples_to_delay, double fractional_delay){
	d->num_samples_to_delay = num_samples_to_delay;
	d->fractional_delay = fractional_delay;
}

void delay_set_feedback_volume(delay *d,double fb_v){
	d->feedback_volume = fb_v;
}

void delay_set_delay_tap_mix_volume(delay *d,double dtm_v){
	d->delay_tap_mix_volume = dtm_v;
}

void delay_set_mic(delay *d,int mic){
	d->mic = mic;	
}

void delay_set_blend_param(delay *d, double delay_blend_param){
	d->delay_blend_param = delay_blend_param;
}

void delete_delay(delay *d){
	if(d != NULL){
		free(d->buffer);
		free(d);
	}
}

extern double delay_out(delay *d, double in) {
	double out;
	double * y_previous;
	double * y;
	double x;
	double x_estimation_of_delay;
	
	d->read_pointer = d->write_pointer - (int)d->num_samples_to_delay;

	if(d->read_pointer < d->buffer){
		d->read_pointer = d->read_pointer + BUFFER_SIZE - 1;
	}

	y_previous == d->read_pointer -1;
	y = d->read_pointer;

	if(y_previous < d->buffer){
		y_previous = y_previous + BUFFER_SIZE;	
	}
	
	x_estimation_of_delay = (*(y_previous) - *(y))*d->fractional_delay + *(y);

	x = in + x_estimation_of_delay*d->feedback_volume;

	*d->write_pointer = x;
	
	out = x*d->delay_blend_param + x_estimation_of_delay*d->delay_tap_mix_volume;
	
	d->write_pointer = d->write_pointer + 1;

	if((d->write_pointer - &d->buffer[0]) > BUFFER_SIZE - 1) {
		d->write_pointer = &d->buffer[0];
	}
	
	return out;
}
