#ifndef LCD_FUNCS_H
#define LCD_FUNCS_H

void LCD_Init(void);
void LCD_Send_Command(unsigned char choice);

#endif


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