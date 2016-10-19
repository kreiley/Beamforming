/***********FixedBeamformer.h*************************************/
#ifndef __FIXEDBEAMFORMER_H__
#define __FIXEDBEAMFORMER_H__

extern void mic_array_init();
extern void calculate_delay(double angle);
extern double get_current_delay(int mic_number);
extern double get_x(int mic_number);
extern double get_y(int mic_number);

#endif
