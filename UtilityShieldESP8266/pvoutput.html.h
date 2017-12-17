const char pvoutput_html[] = R"=====(
<!-- header -->
<!-- menu -->
<div id="content">
<h2>@title</h2>
<form action="" method="post">
<table>
<tr><th>Update:</th><td><input type="text" id="update" name="update" size="3"maxlength="6" value="@update"> minutes (0=disable)</td></tr>
<tr><th>System ID:</th><td><input type="text" id="systemid" name="systemid" maxlength="6" value="@systemid"></td></tr>
<tr><th>API String:</th><td><input type="text" id="apistring" name="apistring" size="40" maxlength="64" value="@apistring"></td></tr>
<tr><th></th><td><input type="submit" value="Save"></td></tr>
</table>
</form>
</div>
<!-- footer -->
)=====";

void send_pvoutput_html()
{
	String html = pvoutput_html;
	 
	if (server.args() > 0 )  // Save Settings
	{
    if( !AdminEnabled )
    {
      send_error_html();
      return;
    }  
		for ( uint8_t i = 0; i < server.args(); i++ ) 
		{
			if (server.argName(i) == "update") config.PostEvery =  server.arg(i).toInt(); 
      if (server.argName(i) == "systemid") config.SystemId =  server.arg(i).toInt(); 
      if (server.argName(i) == "apistring") config.PVoutputApiKey = urldecode( server.arg(i));  
		}
		WriteConfig();
	}
  html.replace( "<!-- header -->", html_header() );
  html.replace( "<!-- menu -->", html_menu() );   
  html.replace( "<!-- footer -->", html_footer() );  
  html.replace("@title", "PVOutput");
  html.replace("@update", (String) config.PostEvery);
  html.replace("@systemid", (String) config.SystemId);
  html.replace("@apistring", (String) config.PVoutputApiKey);

	server.send ( 200, "text/html", html ); 
	Serial.println(__FUNCTION__); 
}



