const volatile double PI_C= (double) 4.0*atan(1.0);

static double d2r (double d) {
  return (d / 180.0) * (PI_C);
}

double sind(double x) {

	if (!std::isfinite(x)) {
		return sin(x);
  	}
  	if (x < 0.0) {
		return -sind(-x);
  	}
  	int quo;
  	double x90 = remquo(fabs(x), 90.0, &quo);
  
	switch (quo % 4) {
    	case 0:
    	  // Use * 1.0 to avoid -0.0
      		return sin(d2r(x90)* 1.0);
    	case 1:
      		return cos(d2r(x90));
    	case 2:
      		return sin(d2r(-x90) * 1.0);
    	case 3:
      		return -cos(d2r(x90));
  	}
  return 0.0;
}

volatile double HarvesineD (double lat1, double lon1, double lat2, double lon2){
//	double EarthR=6378137.0;
	double EarthR=6371000.0;
	double Dlat=(lat2-lat1);
	double Dlon=(lon2-lon1);
	double a=sind(Dlat*0.5)*sind(Dlat*0.5);
	a=a+sind(90.0-lat2)*sind(90.0-lat1)*sind(Dlon)*sind(Dlon);
	double harvesine=2.0*atan2(sqrt(a), sqrt(1.0-a));
	return EarthR*harvesine; 
}

volatile double PitagorasD (double lat1, double lon1, double lat2, double lon2){
//	double EarthR=6378137.0;
	double EarthR=6371000.0;
	double Dlat=(lat2-lat1);
	double DlatRad=d2r(lat2-lat1);
	double Dlon=(lon2-lon1)*PI_C/180.0;
	double a=(double) Dlon*(sind(90.0-(lat1+0.5*Dlat)));
	return (double) EarthR*sqrt(a*a+DlatRad*DlatRad); 
}