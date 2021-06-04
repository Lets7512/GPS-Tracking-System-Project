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
void Systick_init(void);

void update_7_segment(void);
void send_dist_to_segment(void);
void pinA3_High_output(void);
void pinA3_Low_output(void);

//--------------------------------------------------------------------------
                        // Main Function
int main(){
	
}




//--------------------------------------------------------------------------
                        //Function Definition
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
