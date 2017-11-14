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
   
$date = isset($_GET['date']) ? $_GET['date'] : '';
$accum_time = isset($_GET['accum_time']) ? $_GET['accum_time'] : '';
$max_velocity = isset($_GET['max_velocity']) ? $_GET['max_velocity'] : '';
$distance = isset($_GET['distance']) ? $_GET['distance'] : '';

$query = "insert into driveInfo(date, accum_time, max_velocity, distance) values ('$date', '$accum_time', '$max_velocity', '$distance')";
   
mysqli_query($conn,$query)
  or die("Invalid query: ".mysqli_error($conn)); 

mysqli_close($conn);
?>


