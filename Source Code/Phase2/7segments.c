                  //Function declaration
//---------------------------------------------------------------------------
void update_7_segment(void);
//--------------------------------------------------------------------------
       //Function Definition

//Update 7 Segment Readings
void update_7_segment(void){
    	int count = (int)dist_travelled - (int)old_dist;
        int i = 0;
        if(dist_travelled > old_dist){
            for(i=0; i < count; i++){
                    send_dist_to_segment();
            }
            old_dist = dist_travelled;
        }
}