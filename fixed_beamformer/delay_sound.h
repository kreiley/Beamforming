#ifndef __DELAY_SOUND_H__
#define __DELAY_SOUND_H__

#define BUFFER_SIZE 64000

typedef struct delay {
	int mic;
	double *read_pointer;
	double *write_pointer;
	double num_samples_to_delay;
	double fractional_delay;
	double delay_blend_param;
	double delay_tap_mix_volume;
	double feedback_volume;
	double buffer[BUFFER_SIZE];
} delay;


extern delay delay_init(double num_samples_to_delay, double fractional_delay, double fb_v, double dtm_v,double blend_param, int mic);
void delay_set_delay(delay d, double num_samples_to_delay, double fractional_delay);
void delay_set_feedback_volume(delay d, double fb_v);
void delay_set_delay_tap_mix_volume(delay d, double dtm_v);
void delay_set_mic(delay d, int mic);
void delay_set_blend_param(delay d, double delay_blend_param);
extern double delay_out(delay d, double in);


#endif
