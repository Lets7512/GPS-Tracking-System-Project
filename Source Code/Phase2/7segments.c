                  //Function declaration
//---------------------------------------------------------------------------
void Systick_init(void);
void delay_ms(int i);
void delay_us(int i);
//---------------------------------------------------------------------------
void update_7_segment(void);
void send_dist_to_segment(void);
void pinA3_High_output(void);
void pinA3_Low_output(void);
//--------------------------------------------------------------------------
void light_led_after_100m(void);
void light_led_at_distance(void);
void red_led_output(void);
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
//-------------------------------------------------------------------------
//System Clock
void Systick_init(void){
        NVIC_ST_RELOAD_R = 16;  // for us delay
        //NVIC_ST_RELOAD_R = 16;  // for us delay
        NVIC_ST_CTRL_R = 5;
        NVIC_ST_CURRENT_R = 0;
}
//Delay in ms function to be used
void delay_ms(int i){
    int counter = 0;
    while(counter != i*1000){
        if(NVIC_ST_CTRL_R & 0x10000){
            counter++;
        }
    }
}
//Delay in us function to be used
void delay_us(int i){
    int counter = 0;
    while(counter != i){
        if(NVIC_ST_CTRL_R & 0x10000){
            counter++;
        }
    }
}
//--------------------------------------------------------------------------
//Send distance to 7 segments
void send_dist_to_segment(void){
    GPIO_PORTA_DATA_R |= 0x04; // Pin A2
    delay_us(40);
    GPIO_PORTA_DATA_R &=~0x04;
}
//7 Segments All Clear - Set Reset - D0 = High to clear 7 segments
void pinA3_High_output(void){
    GPIO_PORTA_DATA_R |= 0x08;
}
//Pin D0 = Low
void pinA3_Low_output(void){
    GPIO_PORTA_DATA_R &= ~0x0C;
}
//Turn on LED after 100m
void light_led_after_100m(void){
    if(dist_travelled >= 100){
        red_led_output(); //led red o/p function
    }
}

//LED Distance reached
void light_led_at_distance(void){
	double till_point;
	till_point = calculate_distance(current_lat,current_long,dst_lat,dst_long);
    	if(till_point<=15){
        	red_led_output(); //led red o/p function
    	}
}

//LEDs Reset Function and Turn on red LED
void red_led_output(void){
        GPIO_PORTF_DATA_R &= ~(0x0E);
        GPIO_PORTF_DATA_R |= 0x02;
}