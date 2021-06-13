                        //Include headers Section
#include "tm4c123gh6pm.h"
#include "math.h"
#include "stdint.h"
#include "string.h"
#include "stdio.h"
#include "stdlib.h"

//---------------------------------------------------------------------------
                        //Defines for LCD
#define ClearDisp 0x01
#define WakeUp 0x30
#define x5x7_chars_8bit 0x38
#define x5x7_chars_4bit 0x28
#define DispOn 0x0C // No Blinking and No Cursor
#define shift_cursor 0x06
#define first_row 0x80
#define second_row 0xC0
#define Set5x7FontSize 0x20
//---------------------------------------------------------------------------
                        //Global Variables to be used
double dist_travelled = 0;
double old_dist = 0;
char dist_buffer[16];
double PI = 3.1415926535 ;
double current_lat=0;
double current_long=0;
double previous_lat=0;
double previous_long=0;
double speed=0;
double dst_lat = 30.037226628381287;
double dst_long = 31.239754653238634;
char location_buffer[128]; // buffer to format the output variable in
char location_buffer1[128]; // buffer to format the output variable in
char time_buffer[128]; // buffer to format the output variable in
char date_buffer[128]; // buffer to format the output variable in
char quality_buffer[128]; // buffer to format the output variable in
char http_date_buffer[256];
char gps_raw[100];
char lat_long_hist[5];
char raw_gps_data_cache[10][100];
char GSV_data_parsed[20][30];
char GSA_data_parsed[20][20];
char RMC_data_parsed[20][20];
char GLL_data_parsed[20][20];
char GGA_data_parsed[20][20];
int time_hours,time_minutes,time_seconds;
double remaining_dst;
int reached = 0;
double speed_history[5];
double coords_history[5];
double normalized_speed,mean_speed;
int speed_hist_counter=0;
int speed_hist_norm_counter=0;
//String data_to_send = "";
//-------------------------------------------------------------------------------------------------------------------
char apn[21] = "internet.vodafone.net";                    //APN
char apn_u[1] = "";                     //APN-Username
char apn_p[1] = "";                     //APN-Password
char url[36] = "en558zika04rpsq.m.pipedream.net/gps";
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
//--------------------------------------------------------------------------
void light_led_after_100m(void);
void light_led_at_distance(void);
void red_led_output(void);
double calculate_distance(double pre_lati, double pre_longi,double lati, double longi);
void update_dist_travelled(void);
//--------------------------------------------------------------------------
void LCD_Init(void);
void LCD_Send_Command(unsigned char choice);
void LCD_Send_Data(unsigned char data);
void LCD_Print(char str[],int row);
//--------------------------------------------------------------------------
void UART0_Init(void);
unsigned char UART0_Available(void);
unsigned char UART0_Read(void);
void UART0_Write(unsigned char data);
void UART0_ReadString(char *str, char startCh , char stopCh);
void UART0_WriteString(char *str);
void UART3_Init(void);
unsigned char UART3_Available(void);
unsigned char UART3_Read(void);
void UART3_Write(unsigned char data);
void UART3_ReadString(char *str, char startCh ,char stopCh ,int shift);
void UART3_readString(char *str, char startCh , char stopCh);
void UART3_WriteString(char *str);
//--------------------------------------------------------------------------
void Process_GSV(char raw_data[]);
void Process_GSA(char raw_data[]);
void Process_GLL(char raw_data[]);
void Process_GGA(char raw_data[]);
void Process_RMC(char raw_data[]);
char *subString(char *someString, int begin, int end);
int checksum(char raw_data[]);
void parse_GSV(char GSV_data[]);
void parse_GSA(char GSA_data[]);
void parse_RMC(char RMC_data[]);
void parse_GLL(char GLL_data[]);
void parse_GGA(char GGA_data[]);
double convert_lat_to_decimal_degree(char coord_arr[]);
double convert_long_to_decimal_degree(char coord_arr[]);
void update_time(void);
void update_GPS_vars(void);
void update_remaining_distance(void);
double mean_of_array(double var_arr[]);
double normalize(double var,double var_arr[]);
void gsm_http_post(char http_post_request_info[]);
void gsm_send_uart(char command[]);
void gsm_config_gprs(void);
//--------------------------------------------------------------------------

enum RMC_index{tm_RMC, stat_RMC, lat_RMC, lat_dir_RMC, long_RMC,
long_dir_RMC, speed_RMC, track_angle_RMC, date_RMC, mag_var_RMC, checkS_RMC};

enum GGA_index{tm_GGA, lat_GGA, lat_dir_GGA, long_GGA, long_dir_GGA,
quality_GGA, nSvs_GGA, HDOP_GGA, height_GGA, M_GGA, geo_sep_GGA, M_geo_GGA,
age_GGA, ref_id_GGA,checkS_GGA};

enum GLL_index{lat_GLL, lat_dir_GLL, long_GLL, long_dir_GLL,
tm_GLL, valid_GLL, checkS_GLL};

enum GSV_index{total_messages_GSV, message_numb_GSV, total_SVs_GSV,
SV1_prn_GSV, SV1_elev_GSV, SV1_azim_GSV, SV1,snr_GSV, SV2_prn_GSV, SV2_elev_GSV, SV2_azim_GSV, SV2_snr_GSV,
SV3_prn_GSV, SV3_elev_GSV, SV3_azim_GSV, SV3_snr_GSV, SV4_prn_GSV, SV4_elev_GSV, SV4_azim_GSV, SV4_snr_GSV, checkS_GSV};

enum GSA_index{mode_1_GSA, mode_2_GSA, pnr_numb_GSA, PDOP_GSA, HDOP_GSA, VDOP_GSA, checkS_GSA};
//--------------------------------------------------------------------------

                        // Global enums
enum RMC_index rms_stat = stat_RMC;
enum GLL_index gll_valid = valid_GLL;
enum GGA_index gga_quality = quality_GGA;


enum RMC_index lat_rms = lat_RMC;
enum RMC_index long_rms = long_RMC;

enum GLL_index lat_gll = lat_GLL;
enum GLL_index long_gll = long_GLL;

enum GGA_index lat_gga = lat_GGA;
enum GGA_index long_gga = long_GGA;

enum RMC_index rmc_tm = tm_RMC;
enum GLL_index gll_tm = tm_GLL;
enum GGA_index gga_tm = tm_GGA;

enum RMC_index rmc_speed = speed_RMC;

//--------------------------------------------------------------------------
                        // Main Function
int main(){

	int counter,i =0;
    	Systick_init();
    	delay_ms(1000);
    	portF(); // Initialize PortF for the LEDs
    	portB(); // for data pins
    	portA(); // A5,A6,A7 as RS , R/W , E, reset and clock segement
    	pinA3_High_output();
    	delay_ms(50);
    	pinA3_Low_output();
    	LCD_Init();
	LCD_Send_Command(first_row);
    	UART0_Init();
    	UART3_Init();
	sprintf(location_buffer,"GPS Tracking Sys");
	sprintf(location_buffer1,"ASU Students G20");
	LCD_Print(location_buffer,0);
	LCD_Print(location_buffer1,1);
	gsm_config_gprs();
	delay_ms(2000);

    	while(1){

		UART3_ReadString(gps_raw,'$','*',4);
		delay_ms(100);
		Process_RMC(gps_raw);
		UART0_WriteString(gps_raw);
		update_time();
		update_GPS_vars();
		update_dist_travelled();
		update_remaining_distance();
		normalized_speed = normalize(speed,speed_history);
		mean_speed = mean_of_array(speed_history);
		sprintf(location_buffer,"wlk:%d untl:%d",(int)dist_travelled,(int)remaining_dst);
				
		if (remaining_dst < 20 && remaining_dst > 0.001){
			sprintf(location_buffer,"You Reached Dst");
		}

		LCD_Print("                 ",0);
		LCD_Print(location_buffer,0);
				
		sprintf(location_buffer1,"NS:%.1lf S:%.1lf",fabs(normalized_speed),speed);
			
		LCD_Print("                 ",1);
		LCD_Print(location_buffer1,1);
		update_7_segment();
		light_led_at_distance();

		if(time_seconds % 30){
			snprintf(http_date_buffer, sizeof(http_date_buffer),"{\"sent_info\":\"%.6lf,%.6lf\"}",current_lat,current_long);
			gsm_http_post(http_date_buffer);
		}
	}
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

//---------------------------------------------------------------------------------------

//Distance calculate function this function is called haversine function
// the error is up to 0.5% because it consider the earth as perfect sphere

double calculate_distance(double pre_lati, double pre_longi,double lati, double longi){
	double delta_lati;
	double delta_longi;
	double R;
	double a;
	double c;
	double d;
        pre_lati = pre_lati*PI/180;
        pre_longi = pre_longi*PI/180;
        lati = lati*PI/180;
        longi = longi*PI/180;
        delta_lati = lati - pre_lati;
        delta_longi = longi - pre_longi;
        R = 6372795; // distance computation for hypothetical sphere of radius 6372795 meters.
        a = (pow(sin(delta_lati/2),2)) + (cos(pre_lati) * cos(lati) * pow(sin(delta_longi/2),2)) ;
        c = 2 * asin(sqrt(a));
        d = R * c;

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
	double d;
	if(previous_lat==0 || previous_long==0){
		previous_lat = current_lat;
		previous_long = current_long;
	}

	if (current_lat ==0 || current_long ==0){
		return;
	}

	if (((previous_lat != current_lat) | (previous_long != current_long))&( (speed < 4)&(speed >0.4) & (fabs(normalized_speed) < 1.2))){
		d = calculate_distance(previous_lat,previous_long,current_lat,current_long);
    		dist_travelled += d;
		update_remaining_distance();
    		previous_lat = current_lat;
    		previous_long = current_long;
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

//---------------------------------------------------------------------------

//Send distance to 7 segments
void send_dist_to_segment(void){
    GPIO_PORTA_DATA_R |= 0x04; // Pin A2
    delay_us(40);
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

//---------------------------------------------------------------------------------

//LCD Initialize
void LCD_Init(void){
    LCD_Send_Command(WakeUp);
    delay_us(40);
		LCD_Send_Command(Set5x7FontSize);
    delay_us(40);
    LCD_Send_Command(x5x7_chars_8bit);
    delay_us(40);
    LCD_Send_Command(shift_cursor);
    delay_us(40);
    LCD_Send_Command(ClearDisp);
    delay_ms(2);
    LCD_Send_Command(DispOn);
    delay_ms(2);
		LCD_Send_Command(Set5x7FontSize);
    delay_us(40);
    LCD_Send_Command(x5x7_chars_8bit);
    delay_us(40);
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

//LCD Print Function
void LCD_Print(char str[],int row){
    int i;
    if (row == 0){
    LCD_Send_Command(first_row);
    }
    else{
        LCD_Send_Command(second_row);
    }
    delay_us(10);
    for (i = 0; i < str[i];i++){
        LCD_Send_Command(shift_cursor);
        delay_us(40);
            LCD_Send_Data(str[i]);
        delay_us(40);
    }
}

//-------------------------------------------------------------------------------

void UART0_Init(void){
        SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;

        UART0_CTL_R &= ~UART_CTL_UARTEN;
        UART0_IBRD_R = 104 ; //104
        UART0_FBRD_R = 11; //11
        UART0_CC_R = 0;
        UART0_LCRH_R = (UART_LCRH_WLEN_8 | UART_LCRH_FEN);
        UART0_CTL_R |= (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);

        GPIO_PORTA_AFSEL_R |= 0x03;
        GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R &~0xFF) | (GPIO_PCTL_PA1_U0TX | GPIO_PCTL_PA0_U0RX);
        GPIO_PORTA_DEN_R |= 0x03;
        GPIO_PORTA_AMSEL_R &= ~0x03;
}

//UART0 Serial available function
unsigned char UART0_Available(void){
    return ((UART0_FR_R & UART_FR_RXFE) == UART_FR_RXFE) ? 0 : 1;
}

//UART0 Serial Read Function -- Read
unsigned char UART0_Read(void){
        while(UART0_Available() != 1);
        return (unsigned char)(UART0_DR_R & 0xFF);
}

//UART0 Serial Write Function -- Transmit
void UART0_Write(unsigned char data){
        while((UART0_FR_R & UART_FR_TXFF) != 0);
        UART0_DR_R = data;
}

//UART0 Write string
void UART0_WriteString(char *str){                              //Write String
  while(*str){
    UART0_Write(*str);
    str++;
  }
}

//UART0 Read String
void UART0_ReadString(char *str, char startCh , char stopCh){      //Read String
    char c = UART0_Read();
    if (c == startCh){
    	while(c && c != stopCh){
        *str = c;
        str++;
        c = UART0_Read();
    	}
    }
    *str = 0x00;
}
//-------------------------------------------------------------------------

//UART3 -> Port C Initialize Function -- Rx -> PC6 Recieve from GPS  &  Tx -> PC7 trasnmit to SIM Module

void UART3_Init(void){
        SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R3;
        SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R2;

        UART3_CTL_R &= ~UART_CTL_UARTEN;
        UART3_IBRD_R = 104; //104
        UART3_FBRD_R = 11; //11
        UART3_CC_R = 0;
        UART3_LCRH_R = (0x3<<5);
        UART3_CTL_R |= (UART_CTL_RXE | UART_CTL_TXE | UART_CTL_UARTEN);

        GPIO_PORTC_AFSEL_R |= 0xC0;
        GPIO_PORTC_PCTL_R = (GPIO_PORTC_PCTL_R &~0xFF000000) | (GPIO_PCTL_PC7_U3TX | GPIO_PCTL_PC6_U3RX);
        GPIO_PORTC_DEN_R |= 0xC0;
        GPIO_PORTA_AMSEL_R &= ~0xC0;
}


//UART3 Serial available function
unsigned char UART3_Available(void){
    return ((UART3_FR_R & UART_FR_RXFE) == UART_FR_RXFE) ? 0 : 1;
}

//UART3 Serial Read Function -- Read
unsigned char UART3_Read(void){
        while(UART3_Available() != 1);
        return (unsigned char)(UART3_DR_R & 0xFF);
}

//UART3 Serial Write Function -- Transmit
void UART3_Write(unsigned char data){
        while((UART3_FR_R & UART_FR_TXFF) != 0);
        UART3_DR_R = data;
}

//UART3 Write String
void UART3_WriteString(char *str){                              //Write String
	while(*str){
      		UART3_Write(*str);
    		str++;
  	}
}

//UART3 Read String
void UART3_ReadString(char *str, char startCh ,char stopCh ,int shift){      //Read String
	int i;
	int cond = 0;
	char c = UART3_Read();
		
    	if (c == startCh){
		i=-2;
    		while(c && i < shift){
        		*str = c;
        		str++;
        		c = UART3_Read();

			if (c == stopCh){
				cond = 1;
			}

			if (cond){
				i++;
			}
    		}
    	}
    	*str = 0x00;
}

//UART3 Read String
void UART3_readString(char *str, char startCh , char stopCh){      //Read String
    char c = UART3_Read();
    if (c == startCh){
    	while(c && c != stopCh){
        	*str = c;
        	str++;
        	c = UART3_Read();
    	}
    }
    *str = 0x00;
}
//-------------------------------------------------------------------------

//GLL Process Function
void Process_GLL(char raw_data[]){
	char tmp_data[6];

	if(strcmp(strncpy(tmp_data, raw_data, 6),"$GPGLL")==0){
		//UART0_WriteString("GLL Caughted");
		parse_GLL(raw_data);
	}
}

//GGA Process Function
void Process_GGA(char raw_data[]){
	char tmp_data[6];

	if(strcmp(strncpy(tmp_data, raw_data, 6),"$GPGGA")==0){
		parse_GGA(raw_data);
	}
}

//GSV Process Function
void Process_GSV(char raw_data[]){
	char tmp_data[6];

	if(strcmp(strncpy(tmp_data, raw_data, 6),"$GPGSV")==0){
		parse_GSV(raw_data);
	}
}

//GSA Process Function
void Process_GSA(char raw_data[]){
	char tmp_data[6];

	if(strcmp(strncpy(tmp_data, raw_data, 6),"$GPGSA")==0){
		parse_GSA(raw_data);
	}
}

//RMC Process Function
void Process_RMC(char raw_data[]){
	char tmp_data[6];

	if(strcmp(strncpy(tmp_data, raw_data, 6),"$GPRMC")==0){
		parse_RMC(raw_data);
	}
}


char *subString(char *someString, int begin, int end) {
   	char *sub = malloc(sizeof(char)*(end-begin)+1);
	strncpy(sub, someString + begin, end - begin);
   	sub[end] = '\0';
   	return sub;
}

int checksum(char raw_data[]){
	int check,i,len;
	check = 0;
	len = strlen(raw_data)-3;
  	// iterate over the string, XOR each byte with the total sum:
  	for ( i= 1; i < len; i++) {
    		check = (check ^ (int)raw_data[i]);
  	} 
  	// return the result
  	return check;
}

//------------------------------------------------------------------------------
				//Parser Functions

//GSA parse
void parse_GSA(char GSA_data[]){
	int len,i,row,col,size;
	char data[20][20]={'0'};
	len = strlen(GSA_data);
	row = 0;
	col =0;

	for(i=7;i<len;i++){
		if(GSA_data[i]==',' || GSA_data[i]=='*'){
			row++;
			col =0;
			continue;
		}
		data[row][col] = GSA_data[i];
		col++;
	}

	size = sizeof(data)/sizeof(data[0]);
	for (i = 0; i < size; i++){
        strcpy(GSA_data_parsed[i], data[i]);
    }
}

//GSV parse
void parse_GSV(char GSV_data[])
{
	int len,i,row,col,size;
	char data[20][30]={'0'};
	len = strlen(GSV_data);
	row = 0;
	col =0;

	for(i=7;i<len;i++){
		if(GSV_data[i]==',' || GSV_data[i]=='*'){
			row++;
			col =0;
			continue;
		}
		data[row][col] = GSV_data[i];
		col++;
	}

	size = sizeof(data)/sizeof(data[0]);
	for (i = 0; i < size; i++){
        	strcpy(GSV_data_parsed[i], data[i]);
    	}
}

//RMC parse
void parse_RMC(char RMC_data[]){
	int len,i,row,col,size;
	char data[20][20]={'0'};
	len = strlen(RMC_data);
	row = 0;
	col =0;
	//UART0_WriteString("\nRMC is parsed\n");

	for(i=7;i<len;i++){
		if(RMC_data[i]==',' || RMC_data[i]=='*'){
			row++;
			col =0;
			continue;
		}
		data[row][col] = RMC_data[i];
		col++;
	}

	size = sizeof(data)/sizeof(data[0]);
	for (i = 0; i < size; i++){
        	strcpy(RMC_data_parsed[i], data[i]);
    }
}

//GLL parse
void parse_GLL(char GLL_data[]){
	int len,i,row,col,size;
	char data[20][20]={'0'};
	len = strlen(GLL_data);
	row = 0;
	col =0;
	//UART0_WriteString("\nParsed GLL\n");
	//UART0_WriteString("\nGLL is parsed\n");

	for(i=7;i<len;i++){
		if(GLL_data[i]==',' || GLL_data[i]=='*'){
			row++;
			col =0;
			continue;
		}
		data[row][col] = GLL_data[i];
		col++;
	}

	size = sizeof(data)/sizeof(data[0]);
	for (i = 0; i < size; i++){
        strcpy(GLL_data_parsed[i], data[i]);
    }
}

//GGA parse
void parse_GGA(char GGA_data[]){
	int len,i,row,col,size;
	char data[20][20]={'0'};
	len = strlen(GGA_data);
	row = 0;
	col =0;
	for(i=7;i<len;i++)
	{
		if(GGA_data[i]==',' || GGA_data[i]=='*')
		{
			row++;
			col =0;
			continue;
		}
		data[row][col] = GGA_data[i];
		col++;
	}
	size = sizeof(data)/sizeof(data[0]);
	for (i = 0; i < size; i++){
        strcpy(GGA_data_parsed[i], data[i]);
    	}
}

//-----------------------------------------------------------------------------------------
				//convert to decimal degrees

double convert_lat_to_decimal_degree(char coord_arr[]){
	char degree_arr[2];
	char minutes_arr[8];
	double degree,minutes,decimal_Degrees;
	strncpy(degree_arr, coord_arr,2);
	strncpy(minutes_arr, coord_arr+2,8);
	degree = atof(degree_arr);
	minutes = atof(minutes_arr);
	decimal_Degrees = degree + (minutes/60);
	return decimal_Degrees;
}

double convert_long_to_decimal_degree(char coord_arr[]){
	char degree_arr[3];
	char minutes_arr[8];
	double degree,minutes,decimal_Degrees;
	strncpy(degree_arr, coord_arr,3);
	strncpy(minutes_arr, coord_arr+3,8);
	degree = atof(degree_arr);
	minutes = atof(minutes_arr);
	decimal_Degrees = degree + (minutes/60);
	return decimal_Degrees;
}

//-----------------------------------------------------------------------
//update time function
void update_time(void){
	
	char time_arr[10];
	char tm_h[2];
	char tm_m[2];
	char tm_s[5];
	int rmc_utc_time,gll_utc_time,gga_utc_time;
	//UART0_WriteString("time udpate started");
	rmc_utc_time = atoi(RMC_data_parsed[rmc_tm]);
	//gll_utc_time = atoi(GLL_data_parsed[rmc_tm]);
	//gga_utc_time = atoi(GGA_data_parsed[rmc_tm]);
	//sprintf(time_buffer,"%d:%d:%d",gll_utc_time,gga_utc_time,rmc_utc_time);
	//LCD_Print(time_buffer,0);
		strcpy(time_arr,RMC_data_parsed[rmc_tm]);
	/*
	else if(gll_utc_time >= rmc_utc_time && rmc_utc_time >= gga_utc_time)
	{
		//UART0_WriteString("GLL time Caughted");
		strcpy(time_arr,GLL_data_parsed[rmc_tm]);
	}
	else if(gga_utc_time >= rmc_utc_time && rmc_utc_time >= gll_utc_time)
	{
		strcpy(time_arr,GGA_data_parsed[rmc_tm]);
	}*/
	
	strncpy(tm_h, time_arr,2);
	strncpy(tm_m, time_arr+2,2);
	strncpy(tm_s, time_arr+4,5);
	time_hours = atoi(tm_h);
	time_minutes = atoi(tm_m);
	time_seconds = atof(tm_s);
}


void update_GPS_vars(void){
	//int speed_hist_size= sizeof(speed_history)/sizeof(speed_history[0];
	if(strcmp(RMC_data_parsed[rms_stat],"A")==0){
		current_lat = convert_lat_to_decimal_degree(RMC_data_parsed[lat_rms]);
		current_long = convert_long_to_decimal_degree(RMC_data_parsed[long_rms]);
		speed = atof(RMC_data_parsed[rmc_speed]);

		if (speed !=0){
		speed_history[speed_hist_counter]=speed;
		speed_hist_counter++;
		speed_hist_norm_counter++;
		}

		if (speed_hist_counter >5){
			speed_hist_counter = 0;
		}

		if(speed_hist_norm_counter > 5){
			speed_hist_norm_counter=10;
		}
		return;
	}

	/*
	else if(strcmp(GLL_data_parsed[gll_valid],"A")==0){
		current_lat = convert_lat_to_decimal_degree(GLL_data_parsed[lat_gll]);
		current_long = convert_long_to_decimal_degree(GLL_data_parsed[long_gll]);
		return;
	}

	else if(atoi(GGA_data_parsed[gga_quality])==1 || atoi(GGA_data_parsed[gga_quality])==2 || atoi(GGA_data_parsed[gga_quality])==3){
		current_lat = convert_lat_to_decimal_degree(GGA_data_parsed[lat_gga]);
		current_long = convert_long_to_decimal_degree(GGA_data_parsed[long_gga]);
		return;
	}*/
	
}

//Remainig Distance
void update_remaining_distance(void){
	if (current_lat>0 && current_long>0){
	remaining_dst = calculate_distance(current_lat,current_long,dst_lat,dst_long);
	}
}

//---------------------------------------------------------------------------------
		//Functions to calculate mean to use to decrease error

double mean_of_array(double var_arr[]){
	int size= speed_hist_norm_counter;
	int i;
	double mean;
	for(i=0;i<size;i++){
		mean+=var_arr[i];
	}
	mean = mean / size;
	return mean;
}


double normalize(double var,double var_arr[]){
	double norm_var,mean,variance,diff;
	int min,max,i;
	int size= speed_hist_norm_counter;
	for(i=0;i<size;i++){
		mean+=var_arr[i];
	}
	mean = mean / size;
	for(i=0;i<size;i++){
		diff = var_arr[i]-mean;
		variance += pow(diff,2);
	}

	variance=variance/size;
	norm_var = (var - mean) / sqrt(variance);
	return norm_var;
}

//--------------------------------------------------------------------------------------
				//Bonus Part

void gsm_http_post(char http_post_request_info[]){
	char temp_arr[52];
	strcpy(temp_arr,"AT+HTTPPARA=URL,");
	strcat(temp_arr,url);
  	gsm_send_uart("AT+SAPBR=1,1");
  	gsm_send_uart("AT+SAPBR=2,1");
  	gsm_send_uart("AT+HTTPINIT");
  	gsm_send_uart("AT+HTTPPARA=CID,1");
  	gsm_send_uart(temp_arr);
  	gsm_send_uart("AT+HTTPPARA=CONTENT,application/json");
  	gsm_send_uart("AT+HTTPDATA=192,5000");
  	gsm_send_uart(http_post_request_info);
  	gsm_send_uart("AT+HTTPACTION=1");
  	gsm_send_uart("AT+HTTPREAD");
  	gsm_send_uart("AT+HTTPTERM");
  	gsm_send_uart("AT+SAPBR=0,1");
}

void gsm_config_gprs(void){
	char temp_arr[38];
	strcpy(temp_arr,"AT+SAPBR=3,1,APN,");
  	gsm_send_uart("AT+SAPBR=3,1,Contype,GPRS");
	strcat(temp_arr,apn);
  	gsm_send_uart(temp_arr);
  	if (strcmp(apn_u, "")!=0){
    		gsm_send_uart("AT+SAPBR=3,1,USER,");
  	}
  	if (strcmp(apn_p, "")!=0){
    		gsm_send_uart("AT+SAPBR=3,1,PWD,");
  	}
}

void gsm_send_uart(char command[]) {
	//int cmd_size = sizeof(command)/sizeof(command[0]);
	//int send_size = 9+cmd_size;
	char temp_arr[128];
	strcpy(temp_arr,"Send ->: ");
	strcat(temp_arr,command);
  	UART0_WriteString(temp_arr);
   	UART0_WriteString(command);
	UART0_Write('\n');
	delay_ms(500);
  	UART0_Write('\n');
}