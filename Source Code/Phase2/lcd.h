
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



#ifndef LCD_FUNCS_H
#define LCD_FUNCS_H

void LCD_Init(void);
void LCD_Send_Command(unsigned char choice);
void LCD_Send_Data(unsigned char data);
void LCD_Print(char str[],int row);

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
