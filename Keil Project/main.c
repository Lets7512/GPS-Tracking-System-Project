                        //Include headers Section
#include "tm4c123gh6pm.h"
#include "math.h"
#include "string.h"
//---------------------------------------------------------------------------
                        //Global Variables to be used
double dist_travelled = 0;
double old_dist = 0;
char dist_buffer[16];	
double PI = 3.1415926535 ;
												
								
//---------------------------------------------------------------------------
                        //Function declaration
//---------------------------------------------------------------------------
void portF(void);
void portE(void);
void portD(void);
void portC(void);
void portB(void);
void portA(void);
//---------------------------------------------------------------------------
void Systick_init(void);
void delay_ms(int i);
void delay_us(int i);
//---------------------------------------------------------------------------
void update_7_segment(void);
void send_dist_to_segment(void);
void pinA3_High_output(void);
void pinA3_Low_output(void);
void light_led_after_100m(void);
void red_led_output(void);
double calculate_distance(double pre_lati, double pre_longi,double lati, double longi);
void update_dist_travelled(void);
void LCD_Init(void);
void LCD_Send_Command(unsigned char choice);
void LCD_Send_Data(unsigned char data);
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

//PortE Intialize
void portE(void){
        SYSCTL_RCGCGPIO_R |= 0x10;
        while((SYSCTL_PRGPIO_R & 0x10) == 0){}
        GPIO_PORTE_DEN_R = 0x01;
        GPIO_PORTE_DIR_R = 0x00;
        GPIO_PORTE_AFSEL_R = 0x00;
        GPIO_PORTE_AMSEL_R = 0x00;
        GPIO_PORTE_PCTL_R = 0x00000000;
        GPIO_PORTE_DATA_R = 0x00;
}

//PortD Intialize
void portD(void){
        SYSCTL_RCGCGPIO_R |= 0x08;
        while((SYSCTL_PRGPIO_R & 0x08) == 0){}
        GPIO_PORTD_LOCK_R = 0x4C4F434B;
        GPIO_PORTD_DEN_R |= 0x03;    // Digital Pin D0,D1
        GPIO_PORTD_DIR_R |= 0x03;    // Digital outputD0,D1
        GPIO_PORTD_AFSEL_R = 0x00000000;
        GPIO_PORTD_AMSEL_R = 0x00000000;
        GPIO_PORTD_PCTL_R = 0x00000000;
        GPIO_PORTD_DATA_R = 0x00000000;
}

//PortC Intialize
void portC(void){
        SYSCTL_RCGCGPIO_R |= 0x04;
        while((SYSCTL_PRGPIO_R & 0x04) == 0){}
        GPIO_PORTC_DEN_R = 0x00;
        GPIO_PORTC_DIR_R = 0x00;
        GPIO_PORTC_AFSEL_R = 0x00;
        GPIO_PORTC_AMSEL_R = 0x00;
        GPIO_PORTC_PCTL_R = 0x00000000;
        GPIO_PORTC_DATA_R = 0x00;
}

//PortB Intialize
void portB(void){
        SYSCTL_RCGCGPIO_R |= 0x02;
        while((SYSCTL_PRGPIO_R & 0x02) == 0){}
        GPIO_PORTB_DEN_R = 0xFF; //for LCD
        GPIO_PORTB_DIR_R = 0xFF;
        GPIO_PORTB_AFSEL_R = 0x00;
        GPIO_PORTB_AMSEL_R = 0x00;
        GPIO_PORTB_PCTL_R = 0x00000000;
        GPIO_PORTB_DATA_R = 0x00;
}

//PortA Intialize
void portA(void){
        SYSCTL_RCGCGPIO_R |= 0x01;
        while((SYSCTL_PRGPIO_R & 0x01) == 0){}
        GPIO_PORTA_DEN_R = 0xEC; //A5 , A6 , A7 , A3,A2
        GPIO_PORTA_DIR_R = 0xEC;
        GPIO_PORTA_AFSEL_R = 0x00;
        GPIO_PORTA_AMSEL_R = 0x00;
        GPIO_PORTA_PCTL_R = 0x00000000;
        GPIO_PORTA_DATA_R = 0x00;
}

//Distance calculate function this function is called haversine function 
// the error is up to 0.5% because it consider the earth as perfect sphere 
double calculate_distance(double pre_lati, double pre_longi,double lati, double longi){
        pre_lati = pre_lati*PI/180;
        pre_longi = pre_longi*PI/180;
        lati = lati*PI/180;
        longi = longi*PI/180;
        double delta_lati = lati - pre_lati;
        double delta_longi = longi - pre_longi;
        double R = 6371000; // radias in meters to get the result in meters
        double a = (pow(sin(delta_lati/2),2)) + (cos(pre_lati) * cos(lati) * pow(sin(delta_longi/2),2)) ;
        double c = 2 * asin(sqrt(a));
        double d = R * c;
        return d;
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
//Update Distance
void update_dist_travelled(void){
    dist_travelled = dist_travelled + 0.2;
    if (dist_travelled > 150){
        dist_travelled = 150;
    }
}
//System Clock
void Systick_init(void){
        NVIC_ST_RELOAD_R = (int)(15999999/1000000);  // for us delay
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
//Send distance to 7 segments
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
//LCD Initialize
void LCD_Init(void){
	LCD_Send_Command(WakeUp);
	delay_us(30);
	LCD_Send_Command(Set5x7FontSize);
	delay_us(30);
	LCD_Send_Command(x5x7_chars_8bit);
	delay_us(30);
	LCD_Send_Command(shift_cursor);
	delay_us(30);
	LCD_Send_Command(ClearDisp);
	delay_ms(2);
	LCD_Send_Command(DispOn);
	delay_ms(2);
}

//LCD Send Command
void LCD_Send_Command(unsigned char choice){
	GPIO_PORTA_DATA_R = 0; // Making RS and R/W with 0
	GPIO_PORTB_DATA_R = choice; // Sending Command
	GPIO_PORTA_DATA_R |= 0x80;  // Making the Latch From Low to High
	delay_us(40);
	GPIO_PORTA_DATA_R = 0;  // Making the Latch From Low to High
}

//LCD send data
void LCD_Send_Data(unsigned char data){
	GPIO_PORTA_DATA_R = 0x20; // Making RS 1 and R/W with 0
	GPIO_PORTB_DATA_R = data; // Sending Command
	GPIO_PORTA_DATA_R |= 0x80;  // Making the Latch From Low to High
	delay_us(40);
	GPIO_PORTA_DATA_R = 0;  // Making the Latch From Low to High
}
