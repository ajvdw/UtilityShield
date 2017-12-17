const char water_html[] =  R"=====(
<!-- header -->
<!-- menu -->
<div id="content">
<h2>@title</h2>
<form action="" method="post">
<table>
<tr><th>Pulses/m3:</th><td><input type="text" id="pulsesperm3" name="pulsesperm3" value="@pulsesperm3" size="4" maxlength="4" ></tr>
<tr><th>Meter(m3):</th><td><input type="checkbox" id="flg" name="flg" value="on" onclick="chk=document.getElementById('meterm3');chk.disabled=chk.disabled?0:1;">&nbsp;<input type="text" id="meterm3" name="meterm3" value="@meterm3" size="9" disabled></td></tr>
<tr><th></th><td><input type="submit"  value="Save"></td></tr>
</table>
</form>
</div>
<!-- footer -->
)=====";

void send_water_html()
{
  String html = water_html;
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
      if (server.argName(i) == "pulsesperm3") config.Pulsesperm3 =  server.arg(i).toInt();   
      if (server.argName(i) == "meterm3" && savepulsecount )
      {
          config.WaterPulseCount =  (long) (server.arg(i).toFloat() * config.Pulsesperm3); 
          lWaterPulseCounter = config.WaterPulseCount;
          WaterPulseCountStart = lWaterPulseCounter;
      }    
    }
    WriteConfig();
  }

  html.replace( "<!-- header -->", html_header() );
  html.replace( "<!-- menu -->", html_menu() );
  html.replace( "<!-- footer -->", html_footer() );
  html.replace( "@title", "Water" ); 
  if( config.Pulsesperkwh > 0 )
    html.replace( "@meterm3", m3String() );
  else
    html.replace( "@meterm3", (String)"0");
  html.replace( "@pulsesperm3", (String)  config.Pulsesperm3 );
  server.send ( 200, "text/html", html);
	Serial.println(__FUNCTION__); 
}

