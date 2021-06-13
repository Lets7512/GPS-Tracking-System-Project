void Process_GGA(char raw_data[]);
void parse_GGA(char GGA_data[]);
void update_GPS_vars(void);
void Process_GSA(char raw_data[]);
void parse_GSA(char GSA_data[]);
char GSV_data_parsed[20][30];
void Process_GSV(char raw_data[]);
void parse_GSV(char GSV_data[]);
void Process_GLL(char raw_data[]);
void parse_GLL(char GLL_data[]);
void Process_RMC(char raw_data[]);
void parse_RMC(char RMC_data[]);
//----------------------------------------------

enum RMC_index{tm_RMC, stat_RMC, lat_RMC, lat_dir_RMC, long_RMC,
long_dir_RMC, speed_RMC, track_angle_RMC, date_RMC, mag_var_RMC, checkS_RMC};

enum GGA_index{tm_GGA, lat_GGA, lat_dir_GGA, long_GGA, long_dir_GGA,
quality_GGA, nSvs_GGA, HDOP_GGA, height_GGA, M_GGA, geo_sep_GGA, M_geo_GGA,
age_GGA, ref_id_GGA,checkS_GGA};

enum GLL_index{lat_GLL, lat_dir_GLL, long_GLL, long_dir_GLL,
tm_GLL, valid_GLL, checkS_GLL};

enum GSV_index{total_messages_GSV, message_numb_GSV, total_SVs_GSV,
SV1_prn_GSV, SV1_elev_GSV, SV1_azim_GSV, SV1,snr_GSV, SV2_prn_GSV, SV2_elev_GSV, SV2_azim_GSV, SV2_snr_GSV,
SV3_prn_GSV, SV3_elev_GSV, SV3_azim_GSV, SV3_snr_GSV, SV4_prn_GSV, SV4_elev_GSV, SV4_azim_GSV, SV4_snr_GSV, checkS_GSV};

enum GSA_index{mode_1_GSA, mode_2_GSA, pnr_numb_GSA, PDOP_GSA, HDOP_GSA, VDOP_GSA, checkS_GSA};

//--------------------------------------------------------------------------

                        // Global enums
enum RMC_index rms_stat = stat_RMC;
enum GLL_index gll_valid = valid_GLL;
enum GGA_index gga_quality = quality_GGA;


enum RMC_index lat_rms = lat_RMC;
enum RMC_index long_rms = long_RMC;

enum GLL_index lat_gll = lat_GLL;
enum GLL_index long_gll = long_GLL;

enum GGA_index lat_gga = lat_GGA;
enum GGA_index long_gga = long_GGA;

enum RMC_index rmc_tm = tm_RMC;
enum GLL_index gll_tm = tm_GLL;
enum GGA_index gga_tm = tm_GGA;

enum RMC_index rmc_speed = speed_RMC;

//--------------------------------------------------------------------------
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
void Process_RMC(char raw_data[]){
	char tmp_data[6];

	if(strcmp(strncpy(tmp_data, raw_data, 6),"$GPRMC")==0){
		parse_RMC(raw_data);
	}
}
//RMC parse
void parse_RMC(char RMC_data[]){
	int len,i,row,col,size;
	char data[20][20]={'0'};
	len = strlen(RMC_data);
	row = 0;
	col =0;
	//UART0_WriteString("\nRMC is parsed\n");

	for(i=7;i<len;i++){
		if(RMC_data[i]==',' || RMC_data[i]=='*'){
			row++;
			col =0;
			continue;
		}
		data[row][col] = RMC_data[i];
		col++;
	}

	size = sizeof(data)/sizeof(data[0]);
	for (i = 0; i < size; i++){
        	strcpy(RMC_data_parsed[i], data[i]);
    }
}
//GLL Process Function
void Process_GLL(char raw_data[]){
	char tmp_data[6];

	if(strcmp(strncpy(tmp_data, raw_data, 6),"$GPGLL")==0){
		//UART0_WriteString("GLL Caughted");
		parse_GLL(raw_data);
	}
}
//GLL parse
void parse_GLL(char GLL_data[]){
	int len,i,row,col,size;
	char data[20][20]={'0'};
	len = strlen(GLL_data);
	row = 0;
	col =0;
	//UART0_WriteString("\nParsed GLL\n");
	//UART0_WriteString("\nGLL is parsed\n");

	for(i=7;i<len;i++){
		if(GLL_data[i]==',' || GLL_data[i]=='*'){
			row++;
			col =0;
			continue;
		}
		data[row][col] = GLL_data[i];
		col++;
	}

	size = sizeof(data)/sizeof(data[0]);
	for (i = 0; i < size; i++){
        strcpy(GLL_data_parsed[i], data[i]);
    }
}