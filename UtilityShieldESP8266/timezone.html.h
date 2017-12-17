const char tzdb_html[] = R"=====(
<!-- header -->
<!-- menu -->
<div id="content">
<h2>@title</h2>
<form action="" method="post">
<table>
<tr><th>Server:</th><td><input type="text" id="timeserver" name="timeserver" maxlength="128" value="@timeserver"></td></tr>
<tr><th>Start DST:</th><td><select name="startweek">@startweek;</select><select name="startday">@startday</select>&nbsp;of&nbsp;<select name="startmonth">@startmonth</select>&nbsp;at&nbsp;<select name="starthour">@starthour</select>hrs,&nbsp;UTC&nbsp;offset&nbsp;<select name="startoffset">@startoffset</select>:<select name="startminute">@startminute</select></td></tr>
<tr><th>End DST:</th><td><select name="endweek">@endweek;</select><select name="endday">@endday</select>&nbsp;of&nbsp;<select name="endmonth">@endmonth</select>&nbsp;at&nbsp;<select name="endhour">@endhour</select>hrs,&nbsp;UTC&nbsp;offset&nbsp;<select name="endoffset">@endoffset</select>:<select name="endminute">@endminute</select></td></tr>
<tr><th></th><td><input type="submit" value="Save"></td></tr>
</table>
</form>
</div>
<!-- footer -->
)=====";

void send_tzdb_html()
{
	String html = tzdb_html;
  String months[]={ "Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec" };
  String days[]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
  String weeks[]={"Last","First","Second","Third","Fourth"};
  String options;
	 
	if (server.args() > 0 )  // Save Settings
	{
    if( !AdminEnabled ){
      send_error_html();
      return;
    }
    for ( uint8_t i = 0; i < server.args(); i++ ) {
      if (server.argName(i) == "startweek") config.startweek=server.arg(i).toInt();
      if (server.argName(i) == "startday") config.startday=server.arg(i).toInt();
      if (server.argName(i) == "startmonth") config.startmonth=server.arg(i).toInt();
      if (server.argName(i) == "starthour") config.starthour=server.arg(i).toInt();
      if (server.argName(i) == "startminute") config.startminute=server.arg(i).toInt();
      if (server.argName(i) == "startoffset") config.startoffset=server.arg(i).toInt();
      if (server.argName(i) == "endweek") config.endweek=server.arg(i).toInt();
      if (server.argName(i) == "endday") config.endday=server.arg(i).toInt();
      if (server.argName(i) == "endmonth") config.endmonth=server.arg(i).toInt();
      if (server.argName(i) == "endhour") config.endhour=server.arg(i).toInt();
      if (server.argName(i) == "endminute") config.endminute=server.arg(i).toInt();
      if (server.argName(i) == "endoffset") config.endoffset=server.arg(i).toInt();   
      if (server.argName(i) == "timeserver") config.TimeServerName = urldecode( server.arg(i)); 
   
		}
    //timestamp = GetTimestamp();
    config.timestamp=timestamp;
		WriteConfig();
    TimeValid = false;
	}
  html.replace( "<!-- header -->", html_header() );
  html.replace( "<!-- menu -->", html_menu() );
  html.replace( "<!-- footer -->", html_footer() );

  html.replace("@title", "Timezone");
  html.replace("@timeserver", (String) config.TimeServerName);

  options=""; for( int i=0; i<5; i++ ){ options += (String)"<option " + (String)((config.startweek==i)?"selected":"") + (String)" value='" + (String)i + (String)"'>" + weeks[i] + (String)"</option>"; }
  html.replace("@startweek", options );
  options=""; for( int i=0; i<12; i++ ){ options += (String)"<option " + (String)((config.startmonth==i)?"selected":"") + (String)" value='" + (String)i +(String)"'>" + months[i] + (String)"</option>"; }
  html.replace("@startmonth", options );  
  options=""; for( int i=0; i<7; i++ ){ options += (String)"<option " + (String)((config.startday==i)?"selected":"") + (String)" value='" + (String)i +(String)"'>" + days[i] + (String)"</option>"; }
  html.replace("@startday", options );  
  options=""; for( int i=0; i<24; i++ ){ options += (String)"<option " + (String)((config.starthour==i)?"selected":"") + (String)" value='" + (String)i +(String)"'>" + (String)i + (String)":00</option>"; }
  html.replace("@starthour", options );  
  options=""; for( int i=0; i<25; i++ ){ options += (String)"<option " + (String)((config.startoffset==i)?"selected":"") + (String)" value='" + (String)i +(String)"'>" + (String)(i-12) + (String)"</option>"; }
  html.replace("@startoffset", options );  
  options=""; for( int i=0; i<60; i+=15 ){ options += (String)"<option " + (String)((config.startminute==i)?"selected":"") + (String)" value='" + (String)i +"'>" + ((i)?(String)i:(String)"00") + (String)"</option>"; }
  html.replace("@startminute", options );  
  options=""; for( int i=0; i<5; i++ ){ options += (String)"<option " + (String)((config.endweek==i)?"selected":"") + (String)" value='" + (String)i +(String)"'>" + weeks[i] + (String)"</option>"; }
  html.replace("@endweek", options );
  options=""; for( int i=0; i<12; i++ ){ options += (String)"<option " + (String)((config.endmonth==i)?"selected":"") + (String)" value='" + (String)i +(String)"'>" + months[i] + (String)"</option>"; }
  html.replace("@endmonth", options );  
  options=""; for( int i=0; i<7; i++ ){ options += (String)"<option " + (String)((config.endday==i)?"selected":"") + (String)" value='" + (String)i +(String)"'>" + days[i] + (String)"</option>"; }
  html.replace("@endday", options );  
  options=""; for( int i=0; i<24; i++ ){ options += (String)"<option " + (String)((config.endhour==i)?"selected":"") + (String)" value='" + (String)i +(String)"'>" + (String)i + (String)":00</option>"; }
  html.replace("@endhour", options );  
  options=""; for( int i=0; i<25; i++ ){ options += (String)"<option " + (String)((config.endoffset==i)?"selected":"") + (String)" value='" + (String)i +(String)"'>" + (String)(i-12) + (String)"</option>"; }
  html.replace("@endoffset", options );  
  options=""; for( int i=0; i<60; i+=15 ){ options += (String)"<option " + (String)((config.endminute==i)?"selected":"") + (String)" value='" + (String)i +(String)"'>" + ((i)?(String)i:(String)"00") + (String)"</option>"; }
  html.replace("@endminute", options );   
   
	server.send ( 200, "text/html", html ); 
	Serial.println(__FUNCTION__); 
}



