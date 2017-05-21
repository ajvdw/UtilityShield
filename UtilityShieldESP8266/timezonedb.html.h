const char tzdb_html[] PROGMEM = R"=====(
<!-- header -->
<!-- menu -->
<div id="content">
<h2>@title</h2>
<form action="" method="post">
<table>
<tr><th>Timezone Server:</th><td><input type="text" id="tzdbserver" name="tzdbserver" maxlength="128" value="@tzdbserver"></td></tr>
<tr><th>Latitude:</th><td><input type="text" id="lat" name="lat" value="@lat"></td></tr>
<tr><th>Longitude:</th><td><input type="text" id="lng" name="lng" value="@lng"></td></tr>
<tr><th>API String:</th><td><input type="text" id="tzdbapikey" name="tzdbapikey" maxlength="40" value="@tzdbapikey"></td></tr>
<tr><th></th><td><input type="submit" value="Save"></td></tr>
</table>
</form>
</div>
<!-- footer -->
)=====";

void send_tzdb_html()
{
	String html = tzdb_html;
	 
	if (server.args() > 0 )  // Save Settings
	{
    if( !AdminEnabled ){
      send_error_html();
      return;
    }
		for ( uint8_t i = 0; i < server.args(); i++ ) {
			if (server.argName(i) == "tzdbserver") config.TZdbServerName = urldecode( server.arg(i)); 
			if (server.argName(i) == "lat") config.Latitude =  (long)1E6*server.arg(i).toFloat(); 
      if (server.argName(i) == "lng") config.Longitude =  (long)1E6*server.arg(i).toFloat(); 
      if (server.argName(i) == "tzdbapikey") config.TZdbApiKey = urldecode( server.arg(i));  
		}
    timestamp = GetTimestamp();
    config.timestamp=timestamp;
		WriteConfig();

	}
  html.replace( "<!-- header -->", html_header() );
  html.replace( "<!-- menu -->", html_menu() );
  html.replace( "<!-- footer -->", html_footer() );
  html.replace("@title", "Time");
  html.replace("@tzdbserver", (String) config.TZdbServerName);
  html.replace("@lat", (String) ((float)config.Latitude/1E6));
  html.replace("@lng", (String) ((float)config.Longitude/1E6));
  html.replace("@tzdbapikey", (String) config.TZdbApiKey);

	server.send ( 200, "text/html", html ); 
	Serial.println(__FUNCTION__); 
}


