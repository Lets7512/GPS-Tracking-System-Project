                  //Function declaration
//---------------------------------------------------------------------------
void update_7_segment(void);
void send_dist_to_segment(void);
void pinA3_High_output(void);
void pinA3_Low_output(void);
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
//--------------------------------------------------------------------------
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