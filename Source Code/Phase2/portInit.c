                      //Function declaration
//---------------------------------------------------------------------------
void portF(void);
void portE(void);
void portD(void);
void portC(void);
void portB(void);
void portA(void);
//--------------------------------------------------------------------------
                        //Function Definition

//PortF Initialize to use with Tiva Board LEDs
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

//PortA Initialize
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
