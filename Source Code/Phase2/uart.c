//---------------------------------------------------------------------------
                        //Function declaration
//---------------------------------------------------------------------------
void UART0_Init(void);
//-------------------------------------------------------------------------------
				//UART Initialize
//-------------------------------------------------------------------------------
//UART0 Initialize
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