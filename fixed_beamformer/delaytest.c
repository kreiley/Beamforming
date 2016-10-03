#include "delay.h"
#include "math.h"
#include "FixedBeamformer.h"

void delaytest(void);
double get_delay(void);

/*****USAGE EXAMPLE****************************************/
void delaytest(void) {
    double xin;
    double yout;
    Delay_Init(get_delay(),0.7,0.7,1,1);

    while(1) { 
        //if (new_sample_flag()) {
            /*When there's new sample at your ADC or CODEC input*/
            /*Read the sample*/
          //  xin = read_sample();
            /*Apply the Delay_task function to the sample*/
            //yout = Delay_task(xin);

            /*Send the output value to your ADC or codec output*/
            //write_output(yout);
       // }
						
    }
}

double get_delay(){
	return 85.6;
}
