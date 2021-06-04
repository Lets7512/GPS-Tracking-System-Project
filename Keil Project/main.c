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
//System Clock
void Systick_init(void){
        NVIC_ST_RELOAD_R = (int)(15999999/1000000);  // for us delay
				//NVIC_ST_RELOAD_R = 16;  // for us delay
        NVIC_ST_CTRL_R = 5;
        NVIC_ST_CURRENT_R = 0;
}
