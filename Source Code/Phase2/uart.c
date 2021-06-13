//---------------------------------------------------------------------------
                        //Function declaration
//---------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------
				//UART Initialize
//-------------------------------------------------------------------------------
//UART0 Initialize -------- To test data recieved on PC
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