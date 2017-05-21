const char home_html[] PROGMEM = R"=====(
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
       document.getElementById("energy").innerHTML=arr[0];
       document.getElementById("power").innerHTML=arr[1];
       document.getElementById("datetime").innerHTML=arr[2];
       document.getElementById("upload").innerHTML=arr[3];
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
<tr><th>Firmware:</th><td>@firmware</td></tr>
<tr><th>Date/Time:</th><td id=datetime></td></tr>
<tr><th>Energy:</th><td id=energy></td></tr>
<tr><th>Power:</th><td id=power></td></tr>
<tr><th>Upload:</th><td id=upload></td></tr>
</table>
</div>
<!-- footer -->
)=====";

void send_home_html()
{
  String html=home_html;
  html.replace( "<!-- header -->", html_header() );
  html.replace( "<!-- menu -->", html_menu() );  
  html.replace( "<!-- footer -->", html_footer() );  
  html.replace( "@title", "Status" );     
  html.replace( "@model", "NodeMCU v3" );
  html.replace( "@firmware", "v0.1" );
  server.send ( 200, "text/html", html );
  Serial.println(__FUNCTION__); 
}

void send_status_json()
{
  String data="[";
  data += "\"" + kWhString() + " kWh\",";
  data += "\"" + WattString() + " Watt\",";
  data += "\"" + DateTimeString() + "\",";
  data += "\"" + PostResult + " ("+CountDownString()+")\",";
  data += "\"" + RunningString() + "\"";
  data += "]";
  server.send ( 200, "application/json", data );
  Serial.println(__FUNCTION__); 
}

