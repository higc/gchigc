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
   
$d_latitude = isset($_POST['d_latitude']) ? $_POST['d_latitude'] : '';
$d_longitude = isset($_POST['d_longitude']) ? $_POST['d_longitude'] : '';
	
$query = "insert into location(date, time, latitude, longitude) values ('1111-11-11', 'null', '$d_latitude', '$d_longitude')";
   
mysqli_query($conn,$query)
  or die("Invalid query: ".mysqli_error($conn)); 

mysqli_close($conn);
?>

<html>
<head>
	<title> Enter danger locations. </title>
</head>

<BODY>
<font size="4"> Insert done. </font>

</body>
</html>	