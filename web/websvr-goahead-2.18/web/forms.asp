<html>
<!- Copyright (c) Go Ahead Software Inc., 2000-2000. All Rights Reserved. ->
<head>
<title>GoForm TEST</title>
<link rel="stylesheet" href="style/normal_ws.css" type="text/css">
<% language=javascript %>
</head>

<body>
<h1>GoForm Test</h1>
<form action=/goform/formTEST method=POST>

<table>
<tr>
	<td>wan_ip:</td><td><input type=text name=wan_ip size=50 value=""></td>
</tr>
<tr>
	<td>APN:</td><td><input type=text name=APN size=50 value=""></td>
</tr>
<tr>
	<td>lan_ip:</td><td><input type=text name=lan_ip size=50 value=""></td>
</tr>
<tr>
	<td>subip_start:</td><td><input type=text name=subip_start size=50 value=""></td>
</tr>
<tr>
	<td>subip_end:</td><td><input type=text name=subip_end size=50 value=""></td>
</tr>
<tr>
    <td></td>
      <td ALIGN="CENTER"> 
        <input type=submit name=submit value="QUERY"> <input type=submit name=submit value="SET"></td>
</tr>
</table>

</form>

</body>
</html>
