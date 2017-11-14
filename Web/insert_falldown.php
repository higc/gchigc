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
$count = isset($_GET['count']) ? $_GET['count'] : '';

$query = "insert into falldown(date, count) values ('$date', '$count')";
   
mysqli_query($conn,$query)
  or die("Invalid query: ".mysqli_error($conn)); 

mysqli_close($conn);
?>


