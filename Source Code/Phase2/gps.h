#ifndef GPS_FUNCS_H
#define GPS_FUNCS_H

void update_time(void);

#endif

void update_time(void)
{
	
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