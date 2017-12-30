const char home_html[] = R"=====(
<!-- header -->
<!-- menu -->
<script type=text/javascript>
xmlhttp=new XMLHttpRequest();
function update_status()
{ 
  xmlhttp.onreadystatechange=function()
  {
    if(xmlhttp.readyState==4&&xmlhttp.status==200)
    {
       var arr=JSON.parse(xmlhttp.responseText);
       document.getElementById("solar").innerHTML=arr[0];
       document.getElementById("water").innerHTML=arr[1];
       document.getElementById("energy").innerHTML=arr[2];
       document.getElementById("datetime").innerHTML=arr[3];
       document.getElementById("downupload").innerHTML=arr[4];
       document.getElementById("weather").innerHTML=arr[5];
    }
  }
  xmlhttp.open("GET","status",true);xmlhttp.send();
}
self.setInterval("update_status()",300);
update_status();
</script>
<div id="content">
<h2>@title</h2>
<table>
<tr><th>Model:</th><td>@model</td></tr>
<tr><th>Firmware:</th><td>v@major.@minor</td></tr>
<tr><th>Date/Time:</th><td id=datetime></td></tr>
<tr><th>Solar:</th><td id=solar></td></tr>
<tr><th>Water:</th><td id=water></td></tr>
<tr><th>Energy:</th><td id=energy></td></tr>
<tr><th>Weather:</th><td id=weather></td></tr>
<tr><th>WU/PVO:</th><td id=downupload></td></tr>
</table>
</div>
<!-- footer -->
)=====";

void send_home_html()
{
  String html=home_html;
  Serial.println(__FUNCTION__); 

  html.replace( "<!-- header -->", html_header() );
  html.replace( "<!-- menu -->", html_menu() );  
  html.replace( "<!-- footer -->", html_footer() );  
  html.replace( "@title", "Status" );     
  html.replace( "@model", "Wemos D1 mini" );
  html.replace( "@major", String(VERSION_MAJOR) );
  html.replace( "@minor", String(VERSION_MINOR) );
  server.send ( 200, "text/html", html );

}

void send_status_json()
{
  String data="[";
  data += String("\"") + String(0.001*DailyWattHourSolar(),3) + " kWh (" + Watt() + " W)\",";
  data += String("\"") + (String)(DailyLiterWater()) + " L (" + String(LiterPerMinute(),1) + " L/min)\",";
  data += String("\"?\",");
  if( TimeValid )
    data += String("\"") + DateTimeString() + "\",";
  else
    data += String("\"?\",");
  data += String("\"") + GetResult + "/" + PostResult + "\",";
  
  data += String("\"") + weatherString() + "\",";
  data += String("\"") + RunningString() + "\"";
  data += String("]");
  server.send ( 200, "application/json", data );
  Serial.println(__FUNCTION__); 
}


