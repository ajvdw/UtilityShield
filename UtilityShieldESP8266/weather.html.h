const char weather_html[] = R"=====(
<!-- header -->
<!-- menu -->
<div id="content">
<h2>@title</h2>
<form action="" method="post">
<table>
<tr><th>Update:</th><td><input type="text" id="update" name="update" size="3"maxlength="6" value="@update"> minutes (0=disable)</td></tr>
<tr><th>Location:</th><td><input type="text" id="location" name="location" maxlength="128" value="@location"></td></tr>
<tr><th>API String:</th><td><input type="text" id="apistring" name="apistring" maxlength="32" value="@apistring"></td></tr>
<tr><th></th><td><input type="submit" value="Save"></td></tr>
</table>
</form>
</div>
<!-- footer -->
)=====";

void send_weather_html()
{
  String html = weather_html;
   
  if (server.args() > 0 )  // Save Settings
  {
    if( !AdminEnabled )
    {
      send_error_html();
      return;
    }  
    for ( uint8_t i = 0; i < server.args(); i++ ) 
    {
      if (server.argName(i) == "update") config.GetEvery =  server.arg(i).toInt(); 
      if (server.argName(i) == "location") config.Location = server.arg(i);
      if (server.argName(i) == "apistring") config.WUApiKey =  server.arg(i); 
    }
    WriteConfig();
  }
  html.replace( "<!-- header -->", html_header() );
  html.replace( "<!-- menu -->", html_menu() );   
  html.replace( "<!-- footer -->", html_footer() );  
  html.replace("@title", "Weather Underground");
  html.replace("@update", (String) config.GetEvery);
  html.replace("@location", (String) config.Location);
  html.replace("@apistring", (String) config.WUApiKey);

  server.send ( 200, "text/html", html ); 
  Serial.println(__FUNCTION__); 
}



