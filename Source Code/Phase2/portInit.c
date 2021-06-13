                      //Function declaration
//---------------------------------------------------------------------------
void portF(void);
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
