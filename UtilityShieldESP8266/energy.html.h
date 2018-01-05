const char power_html[] =  R"=====(
<!-- header -->
<!-- menu -->
<div id="content">
<h2>@title</h2>
<form action="" method="post">
<table>

<tr><th></th><td><input type="submit"  value="Save"></td></tr>
</table>
</form>
</div>
<!-- footer -->
)=====";

void send_power_html()
{
  String html = power_html;
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
// 
    }
    WriteConfig();
  }

//115200 8N1
//9600 7E1

  html.replace( "<!-- header -->", html_header() );
  html.replace( "<!-- menu -->", html_menu() );
  html.replace( "<!-- footer -->", html_footer() );
  html.replace( "@title", "Power" ); 
//
  server.send ( 200, "text/html", html);
	Serial.println(__FUNCTION__); 
}

