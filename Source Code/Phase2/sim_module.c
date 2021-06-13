
void gsm_http_post(char http_post_request_info[]);
void gsm_send_uart(char command[]);
void gsm_config_gprs(void);
















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