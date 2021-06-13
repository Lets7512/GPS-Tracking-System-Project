



//Distance calculate function this function is called haversine function
// the error is up to 0.5% because it consider the earth as perfect sphere

double calculate_distance(double pre_lati, double pre_longi,double lati, double longi){
	double delta_lati;
	double delta_longi;
	double R;
	double a;
	double c;
	double d;
        pre_lati = pre_lati*PI/180;
        pre_longi = pre_longi*PI/180;
        lati = lati*PI/180;
        longi = longi*PI/180;
        delta_lati = lati - pre_lati;
        delta_longi = longi - pre_longi;
        R = 6372795; // distance computation for hypothetical sphere of radius 6372795 meters.
        a = (pow(sin(delta_lati/2),2)) + (cos(pre_lati) * cos(lati) * pow(sin(delta_longi/2),2)) ;
        c = 2 * asin(sqrt(a));
        d = R * c;

        return d;
}

//----------------------------------------------------------------------------

//Update Distance
void update_dist_travelled(void){
	double d;
	if(previous_lat==0 || previous_long==0){
		previous_lat = current_lat;
		previous_long = current_long;
	}

	if (current_lat ==0 || current_long ==0){
		return;
	}

	if (((previous_lat != current_lat) | (previous_long != current_long))&( (speed < 4)&(speed >0.4) & (fabs(normalized_speed) < 1.2))){
		d = calculate_distance(previous_lat,previous_long,current_lat,current_long);
    		dist_travelled += d;
		update_remaining_distance();
    		previous_lat = current_lat;
    		previous_long = current_long;
	}
}

//-------------------------------------------------------------------------
