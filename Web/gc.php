<?php
   include'melon/core.php';
   $data = getList('location');
?>

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
   
//select driveInfo   
$sql = "SELECT date, distance FROM driveInfo order by date desc limit 5";
$result = mysqli_query($conn,$sql)
  or die("Invalid query: ".mysqli_error($conn));  
$num_rows = mysqli_num_rows($result);  

if ($num_rows > 0) {  
	for($i=0;$row = mysqli_fetch_assoc($result);$i++){
		$date_d[$i] = $row['date'];
		$distance[$i] = $row['distance'];
	}
} 
else { 
  echo ("No rows returned");
}  

//select falldown
$sql = "SELECT date, count FROM falldown order by date desc limit 5";
$result = mysqli_query($conn,$sql)
  or die("Invalid query: ".mysqli_error($conn));  
$num_rows = mysqli_num_rows($result);  

if ($num_rows > 0) {  
	for($i=0;$row = mysqli_fetch_assoc($result);$i++){
		$date_f[$i] = $row['date'];
		$count[$i] = $row['count'];
	}
} 
else {
  echo ("No rows returned");
}


//select location
$sql = "SELECT latitude, longitude FROM location";
$result = mysqli_query($conn,$sql)
  or die("Invalid query: ".mysqli_error($conn));  
$num_rows = mysqli_num_rows($result);  

$x=array();

if ($num_rows > 0) {  
	for($i=0;$row = mysqli_fetch_assoc($result);$i++){
		$lati[$i] = $row['latitude'];
		$long[$i] = $row['longitude'];
		$x[$i]=$i;
	}
} 
else {
  echo ("No rows returned");
}



mysqli_close($conn);
?>


<html>
<head>
<script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>

    <script type="text/javascript">
   
     google.charts.load('current', {'packages':['bar']});
     google.charts.load('current', {'packages':['corechart']});
     google.charts.load('current', {'packages':['line']});

     
     google.charts.setOnLoadCallback(drawDistanceChart);
     google.charts.setOnLoadCallback(drawFallDownChart);
     google.charts.setOnLoadCallback(drawAnalysisChart);

    
    <!-- 주행거리 차트 -->

      function drawDistanceChart() {
        var data = google.visualization.arrayToDataTable([
          ['Day', 'Distance(m)',{ role: 'style' }],
		  <?php
			echo("['$date_d[4]', $distance[4], 'silver'],");
			echo("['$date_d[3]', $distance[3], 'silver'],");
			echo("['$date_d[2]', $distance[2], 'silver'],");
			echo("['$date_d[1]', $distance[1], 'silver'],");
			echo("['$date_d[0]', $distance[0], 'silver']");
		  ?>
        ]);

        var options = {
          chart: {
            title: 'Distacne Information',
            subtitle: 'Day, Distance(m): day1-day30',
          }
        };

        var chart = new google.charts.Bar(document.getElementById('columnchart_distance'));

        chart.draw(data, google.charts.Bar.convertOptions(options));
      }
     
     
 <!-- 넘어지는 횟수 차트 -->    
     function drawFallDownChart() {
        var data = google.visualization.arrayToDataTable([
          ['Day', 'FallDown',{ role: 'style' }],
		  <?php
			echo("['$date_f[4]', $count[4], 'color: #e5e4e2'],");
			echo("['$date_f[3]', $count[3], 'color: #e5e4e2'],");
			echo("['$date_f[2]', $count[2], 'color: #e5e4e2'],");
			echo("['$date_f[1]', $count[1], 'color: #e5e4e2'],");
			echo("['$date_f[0]', $count[0], 'color: #e5e4e2']");
		  ?>
        ]);

        var options = {
          chart: {
            title: 'The Num of Fall Down Information',
            subtitle: 'Day, Fall Down: day1-day30',
          }
        };

        var chart = new google.charts.Bar(document.getElementById('columnchart_FallDown'));

        chart.draw(data, google.charts.Bar.convertOptions(options));
      }
     
    
<!-- 분석 차트 -->    
function drawAnalysisChart() {

      var data = new google.visualization.DataTable();
      data.addColumn('number', 'Day');
      data.addColumn('number', 'Distance(km)');
      data.addColumn('number', 'FallDown');
   

      data.addRows([
	  <?php
			echo("[1, $distance[4], $count[4]],");
			echo("[2, $distance[3], $count[3]],");
			echo("[3, $distance[2], $count[2]],");
			echo("[4, $distance[1], $count[1]],");
			echo("[5, $distance[0], $count[0]],");
		  ?>
      ]);

      var options = {
        chart: {
          title: 'Analhysis',
          subtitle: ''
        },
        width: 900,
        height: 500
      };

      var chart = new google.charts.Line(document.getElementById('linechart_material'));
     chart.draw(data, google.charts.Line.convertOptions(options));
    }
   
   
    
    </script>

   

    </script>
   
   
   
   
   <title> GuardiCycle </title>
   <style type="text/css">
   h1{color:#22741C; font-size:24pt; text-align:center; font-family:arial,sans-serif}
   .menu{color:#22741C; font-size:12pt; text-align:center; font-family:arial,sans-serif; font-weight:bold}
   td{background:#B7F0B1}
   
   
   <!--본문 글에 대한 설정-->
   p{color:black; font-size:12pt; text-align:justify; font-family:arial,sans-serif}
   
   p.foot{color:white; font-size:9pt; text-align:center; font-family:arial,sans-serif; font-weight:bold}
   
   a:link,a:visited,a:active{color:white}
   
   </style>
</head>
<body>


<!-- 페이지 헤더 -->
<table width="100%" cellpadding="12" cellspacing="0" border="0">

<tr bgcolor="black">

   <td align="left">
   <td>
      <h1>GuardiCycle</h1>
     
   </td>
   
   <td align="right"></td>
   
</tr>
</table>


<!--메뉴-->
<table width="100%" bgcolor="white" cellpadding="4" cellspacing="4">

<tr>
   <td width="25%">
      
      <span class="menu">Home</span></td>
      <td width="25%">
         <nav>
      <ul>
     <!-- a는 하이퍼링크 생성-->
     <!-- href = Hyper Reference-->
        <li> <a href="#Distance">Distance</a></li>
      </ul>
    </nav>
      
      <!--<span class="menu">Distance</span></td>-->
      <td width="25%">
   <nav>
      <ul>
     
     <!--  a는 하이퍼링크 생성-->
     <!-- href = Hyper Reference-->
      
        <li> <a href="#Fall Down">Fall Down</a></li>
      </ul>
    </nav>
      <!--<span class="menu">Fall Down</span></td>-->
      <td width="25%">
      
         <nav>
      <ul>
     <!--  a는 하이퍼링크 생성-->
     <!-- href = Hyper Reference-->
       
        <li> <a href="#Analysis">Analysis</a></li>
      </ul>
    </nav>
   
      <!--<span class="menu">Analysis</span></td>-->
   
</tr>
</table>



<!--페이지 내용-->
<p>Welcome to GuardiCycle.</p>

<!--자전거 사진-->
<br><img src="https://image.flaticon.com/icons/svg/227/227347.svg" alt="자전거 사진" width="200" height="140">

<p>What is GuardiCycle? Guardicycle is a bicycle for children aged between 3 and 7. 
It measures how long, and how much they rode a bicycle.
It also measures where they have been during their riding. 
The main function for his bicycle is analyzing each child's riding pattern and educate if they're riding dangerously.</p>

<!--하위 컨텐츠에 대한 설명-->
<div id="sub-content">
  <article class="container">
 
 
    <h3>Distance and Speed</h3>
    <p>Today, Hyunah rode bicycle for 333 meters.</p>
    </br>
    </br>
    <h3>How much have they fall down?</h3>
    <p>Today, Hyunah fall down 3 times.</p>
    </br>
    </br>
    <h3>Where they have been?</h3>
    <p>Today, Hyunah went to Ewha Womans University, and DaeHyun-Dong.</p>
    </br>
    </br>
    <h3>Analysis</h3>
    <p>I will show bargraph here!</p>
    </br>
    </br>
 
    <div id="Distance">
 
      <h1></h1>
      </br>
      </br>
    </div>
  </article>
</div>
 
<h1 id="Distance">Distance</h1>
<p>Show me the distance!
<p>The graph below shows information about the distance your child travelled.
<div id="columnchart_distance" style="width: 800px; height: 500px;"></div>
<div style="padding: 30px 0px 0px 30px;"></div>
<!--공백 삽입-->
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>


<h1 id="Fall Down">Fall Down</h1>
<p>Show me the number of Fall Down!
<p>The graph below shows information that indicates how many times your child has fallen.
<div id="columnchart_FallDown" style="width: 800px; height: 500px;"></div>
<!--공백 삽입-->
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>


<h1 id="Location">Location</h1>

<div id="map"></div>

<br>
<font size="4"> 집의 위도와 경도를 입력하시오. </font>
<form name="form1" method="post" action="Hinsert.php">
	위도: <input type="text" name="latitude">
	경도: <input type="text" name="longitude">
	<input type="submit" value="확인">
</form>

<br>
<font size="4"> 위험장소의 위도와 경도를 입력하시오. </font>
<form name="form1" method="post" action="Dinsert.php">
	위도: <input type="text" name="d_latitude">
	경도: <input type="text" name="d_longitude">
	<input type="submit" value="확인">
</form>

 <div id="map"></div>

	<script src="https://maps.googleapis.com/maps/api/js?key=AIzaSyB9cnfdsG4SYMMAzqva1FeEdwDul3NhPhA&callback=initMap"
></script>
    <script>
      var map;

        map = new google.maps.Map(document.getElementById('map'), {
          center: {lat: <?=$lati[0]?>, lng:  <?=$long[0]?>},
          zoom: 15
        });


            <?php
		foreach ($x as $index){
             ?>
  var myLatLng = {lat:  <?=$lati[$index]?>, lng:   <?=$long[$index]?>};
		  var marker<?=$index?> = new google.maps.Marker({
				position: myLatLng,
				map: map,
				title: 'guardicycle Map!'
			  });

			 <?php
		  }	 
			 ?>

    </script>

<!--공백 삽입-->
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>
<BR>    <style>
      /* Always set the map height explicitly to define the size of the div
       * element that contains the map. */
      #map {
      width:70%;
        height: 70%;
      margin: auto;
      }
      /* Optional: Makes the sample page fill the window. */
      html, body {
        height: 100%;
        margin: 0;
        padding: 0;
      }
    </style>



<h1 id="Analysis">Analysis</h1>
<p>Show me the Analysis!
<p>The graph below shows information that indicates how many times your child has fallen and distance.
<div id="linechart_material" style="width: 900px; height: 500px"></div>



<!--페이지 푸터-->
<table width="100%" bgcolog="black" cellpadding="12" border="0">
<tr>
<td>
   <p class="foot"><&copy; Copyrights 2017 Guardians. All rights reserved.</p>
</td>
</tr>
</table>

</body>
</html>