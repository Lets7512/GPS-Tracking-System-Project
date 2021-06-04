                        //Include headers Section
#include "tm4c123gh6pm.h"
#include "math.h"
#include "string.h"
//---------------------------------------------------------------------------
                        //Global Variables to be used
double dist_travelled = 0;
double old_dist = 0;
char dist_buffer[16];												
												
								
//---------------------------------------------------------------------------
                        //Function declaration
//---------------------------------------------------------------------------
void portF(void);
void Systick_init(void);

void update_7_segment(void);
void send_dist_to_segment(void);
void pinA3_High_output(void);
void pinA3_Low_output(void);
void light_led_after_100m(void);
void red_led_output(void);

//--------------------------------------------------------------------------
                        // Main Function
int main(){
	
}




//--------------------------------------------------------------------------
                        //Function Definition

//PortF Intialize to use with Tiva Board LEDs
void portF(void){
        SYSCTL_RCGCGPIO_R |= 0x20;                      // enable clock to PORTF
        while((SYSCTL_PRGPIO_R & 0x20) == 0){}          // waiting for port F stabilization
        GPIO_PORTF_LOCK_R = 0x4C4F434B;                 // unlock port F
        GPIO_PORTF_CR_R |= 0x0E;                        // unlock port F
        GPIO_PORTF_DIR_R |= 0x0E;                       // output pins LEDs
        GPIO_PORTF_DEN_R |= 0x0E;                       // Digital Pins
        GPIO_PORTF_AMSEL_R &= ~0x0E;                    // Analog select ; clear 3 pins of LEDs of analog property
        GPIO_PORTF_AFSEL_R &= ~0x0E;                    // Alternative function or General Purpose Selection; here we selected GPIO
        GPIO_PORTF_PCTL_R &= ~0x0000FFF0;               // Definition of the alternative function of the pin; here we clear the leds pins
        GPIO_PORTF_DATA_R &= ~0x0E;                     // clear data from LEDs.
}

//Update 7 Segment Readings
void update_7_segment(void){   
	int count = (int)dist_travelled - (int)old_dist;
    	int i = 0;
    	if(dist_travelled > old_dist){
        	for(i=0; i < count; i++){
            		send_dist_to_segment();
        	}
        	old_dist = dist_travelled;
    	}
}
void send_dist_to_segment(void){
    GPIO_PORTA_DATA_R |= 0x04; // Pin A2
    delay_ms(10);
    GPIO_PORTA_DATA_R &=~0x04;
}

//7 Segments All Clear - Set Reset - D0 = High to clear 7 segments
void pinA3_High_output(void){
    GPIO_PORTA_DATA_R |= 0x0C;
}

//Pin D0 = Low
void pinA3_Low_output(void){
    GPIO_PORTA_DATA_R &= ~0x0C;
}

//System Clock
void Systick_init(void){
        NVIC_ST_RELOAD_R = (int)(15999999/1000000);  // for us delay
				//NVIC_ST_RELOAD_R = 16;  // for us delay
        NVIC_ST_CTRL_R = 5;
        NVIC_ST_CURRENT_R = 0;
}

//Turn on LED after 100m
void light_led_after_100m(void){
    if(dist_travelled >= 100){
        red_led_output(); //led red o/p function
    }
}

//LEDs Reset Function and Turn on red LED
void red_led_output(void){
        GPIO_PORTF_DATA_R &= ~(0x0E);
        GPIO_PORTF_DATA_R |= 0x02;
}