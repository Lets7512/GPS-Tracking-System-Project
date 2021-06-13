void Process_GGA(char raw_data[]);
void parse_GGA(char GGA_data[]);
void update_GPS_vars(void);

----------------------------------------------

//GGA Process Function
void Process_GGA(char raw_data[]){
	char tmp_data[6];

	if(strcmp(strncpy(tmp_data, raw_data, 6),"$GPGGA")==0){
		parse_GGA(raw_data);
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
	 for (i = 0; i < size; i++)
    {
        strcpy(GGA_data_parsed[i], data[i]);
    }
}
void update_GPS_vars(void)
{
	//int speed_hist_size= sizeof(speed_history)/sizeof(speed_history[0];
	if(strcmp(RMC_data_parsed[rms_stat],"A")==0)
	{
		current_lat = convert_lat_to_decimal_degree(RMC_data_parsed[lat_rms]);
		current_long = convert_long_to_decimal_degree(RMC_data_parsed[long_rms]);
		speed = atof(RMC_data_parsed[rmc_speed]);
		if (speed !=0)
		{
		speed_history[speed_hist_counter]=speed;
		speed_hist_counter++;
			speed_hist_norm_counter++;
		}
		if (speed_hist_counter >5)
		{
			speed_hist_counter = 0;
		}
		if(speed_hist_norm_counter > 5)
		{
			speed_hist_norm_counter=10;
		}
		return;
	}
	/*
	else if(strcmp(GLL_data_parsed[gll_valid],"A")==0)
	{
		current_lat = convert_lat_to_decimal_degree(GLL_data_parsed[lat_gll]);
		current_long = convert_long_to_decimal_degree(GLL_data_parsed[long_gll]);
		return;
	}
	else if(atoi(GGA_data_parsed[gga_quality])==1 || atoi(GGA_data_parsed[gga_quality])==2 || atoi(GGA_data_parsed[gga_quality])==3)
	{
		current_lat = convert_lat_to_decimal_degree(GGA_data_parsed[lat_gga]);
		current_long = convert_long_to_decimal_degree(GGA_data_parsed[long_gga]);
		return;
	}*/
	
}
