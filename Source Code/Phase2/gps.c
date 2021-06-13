void Process_GGA(char raw_data[]);
void parse_GGA(char GGA_data[]);
void update_GPS_vars(void);
void Process_GSA(char raw_data[]);
void parse_GSA(char GSA_data[]);
char GSV_data_parsed[20][30];
void Process_GSV(char raw_data[]);
void parse_GSV(char GSV_data[]);

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
	
}

//GSA Process Function
void Process_GSA(char raw_data[]){
	char tmp_data[6];

	if(strcmp(strncpy(tmp_data, raw_data, 6),"$GPGSA")==0){
		parse_GSA(raw_data);
	}
}

//GSA parse
void parse_GSA(char GSA_data[]){
	int len,i,row,col,size;
	char data[20][20]={'0'};
	len = strlen(GSA_data);
	row = 0;
	col =0;

	for(i=7;i<len;i++){
		if(GSA_data[i]==',' || GSA_data[i]=='*'){
			row++;
			col =0;
			continue;
		}
		data[row][col] = GSA_data[i];
		col++;
	}

	size = sizeof(data)/sizeof(data[0]);
	for (i = 0; i < size; i++){
        strcpy(GSA_data_parsed[i], data[i]);
    }
}

//GSV Process Function
void Process_GSV(char raw_data[]){
	char tmp_data[6];

	if(strcmp(strncpy(tmp_data, raw_data, 6),"$GPGSV")==0){
		parse_GSV(raw_data);
	}
}
//GSV parse
void parse_GSV(char GSV_data[])
{
	int len,i,row,col,size;
	char data[20][30]={'0'};
	len = strlen(GSV_data);
	row = 0;
	col =0;

	for(i=7;i<len;i++){
		if(GSV_data[i]==',' || GSV_data[i]=='*'){
			row++;
			col =0;
			continue;
		}
		data[row][col] = GSV_data[i];
		col++;
	}

	size = sizeof(data)/sizeof(data[0]);
	for (i = 0; i < size; i++){
        	strcpy(GSV_data_parsed[i], data[i]);
    	}
}

