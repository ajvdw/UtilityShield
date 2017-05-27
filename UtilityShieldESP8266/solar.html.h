const char solar_html[] =  R"=====(
<!-- header -->
<!-- menu -->
<div id="content">
<h2>@title</h2>
<form action="" method="post">
<table>
<tr><th>Pulses/kWh:</th><td><input type="text" id="pulsesperkwh" name="pulsesperkwh" value="@pulsesperkwh" size="4" maxlength="4" ></tr>
<tr><th>Meter(kWh):</th><td><input type="checkbox" id="flg" name="flg" value="on" onclick="chk=document.getElementById('meterkwh');chk.disabled=chk.disabled?0:1;">&nbsp;
<input type="text" id="meterkwh" name="meterkwh" value="@meterkwh" size="9" disabled></td></tr>
<tr><th></th><td><input type="submit"  value="Save"></td></tr>
</table>
</form>
</div>
<!-- footer -->
)=====";

void send_solar_html()
{
  String html = solar_html;
  bool savepulsecount = false;
  
  if (server.args() > 0  )  // Save Settings
  {
    if( !AdminEnabled )
    {
      send_error_html();
      return;   
    }
    String temp = "";
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "flg"){ if( server.arg(i) == "on" ) savepulsecount = true; } 
      if (server.argName(i) == "pulsesperkwh") config.Pulsesperkwh =  server.arg(i).toInt();   
      if (server.argName(i) == "meterkwh" && savepulsecount )
      {
          config.SolarPulseCount =  (long) (server.arg(i).toFloat() * config.Pulsesperkwh); 
          lSolarPulseCounter = config.SolarPulseCount;
      }    
    }
    WriteConfig();
  }

  html.replace( "<!-- header -->", html_header() );
  html.replace( "<!-- menu -->", html_menu() );
  html.replace( "<!-- footer -->", html_footer() );
  html.replace( "@title", "Solar" ); 
  if( config.Pulsesperkwh > 0 )
    html.replace( "@meterkwh", kWhString() );
  else
    html.replace( "@meterkwh", (String)"0");
  html.replace( "@pulsesperkwh", (String)  config.Pulsesperkwh );
  server.send ( 200, "text/html", html);
	Serial.println(__FUNCTION__); 
}

