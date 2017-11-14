<?php
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "guardicycle";

// Connect to mysql server
$conn = mysqli_connect($servername,$username,$password ) 
  or die("Connection failed! ".mysqli_error($conn));

// Select database
mysqli_select_db($conn, $dbname) 
  or die ("Database selection failed! ".mysqli_error($conn)); 
   
$latitude = isset($_POST['latitude']) ? $_POST['latitude'] : '';
$longitude = isset($_POST['longitude']) ? $_POST['longitude'] : '';
	
$query = "insert into location(date, time, latitude, longitude) values ('0000-00-00', 'null', '$latitude', '$longitude')";
   
mysqli_query($conn,$query)
  or die("Invalid query: ".mysqli_error($conn)); 

mysqli_close($conn);
?>

<html>
<head>
	<title> Enter latitude, longitude of your home. </title>
</head>

<BODY>
<font size="4"> Insert done. </font>

</body>
</html>	