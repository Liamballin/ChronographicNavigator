
//Adapted from javascript, original from: 

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
/* Latitude/longitude spherical geodesy tools                         (c) Chris Veness 2002-2017  */
/*                                                                                   MIT Licence  */
/* www.movable-type.co.uk/scripts/latlong.html                                                    */
/* www.movable-type.co.uk/scripts/geodesy/docs/module-latlon-spherical.html                       */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */

float distanceBetween(float lat1, float lon1, float lat2, float lon2){

  float r = 6371e3; //earth's radius in meters

  float la1 = radians(lat1);
  float la2 = radians(lat2);

  float deltaLat = radians(lat2 - lat1);
  float deltaLon = radians(lon2 - lon1);

  float a = sin(deltaLat/2) * sin(deltaLat/2) + cos(la1) * cos(lat2) * sin(deltaLon/2) * sin(deltaLon/2);
  
  float c = 2 * atan2(sqrt(a),sqrt(1-a));

  float d = r * c;

  return d;
  
}
