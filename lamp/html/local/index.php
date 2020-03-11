<!DOCTYPE  html>

<html>
<head>
<title>PBSI CTF</title>
<style>
* {
  box-sizing: border-box;
}

/* Create two equal columns that floats next to each other */
.column {
  float: left;
  width: 50%;
  padding: 10px;
  height: 300px; /* Should be removed. Only for demonstration */
}

/* Clear floats after the columns */
.row:after {
  content: "";
  display: table;
  clear: both;
}
</style>

</head>
<body>

<div class="row">
<div class="column">
 <?php
$servername = "localhost";
$username = "becariosmaster";
$password = NULL;
$dbname = "becarios";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error){
    die("Connection failed");//. $conn->connect_error);
}

$sql = "SELECT t2.nombre as nombre FROM (SELECT ip FROM reto1 ORDER BY time DESC LIMIT 5) AS t1 INNER JOIN (SELECT * FROM becarios) as t2 ON t1.ip = t2.ip";
//$sql = "SELECT * FROM becarios WHERE inet_ntoa(ip) = '" . $_SERVER['REMOTE_ADDR'] . "'";
//echo $sql;
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    // output data of each row
	echo "<h1>Participaciones Reto 1</h1>";
	echo "<table>";
	echo "<tr><th>Nombre</th><tr>";

	while($row = $result->fetch_assoc()) 
	{
		echo "<tr><td>" . $row["nombre"] . "</td></td>";
	}
	echo "</table>";

} else {
    echo "<p>No hay participantes</p>";

}

$sql = "SELECT t2.nombre as nombre FROM (SELECT ip FROM reto2 ORDER BY time DESC LIMIT 5) AS t1 INNER JOIN (SELECT * FROM becarios) as t2 ON t1.ip = t2.ip";
//$sql = "SELECT * FROM becarios WHERE inet_ntoa(ip) = '" . $_SERVER['REMOTE_ADDR'] . "'";
//echo $sql;
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    // output data of each row
	echo "<h1>Participaciones Reto 2</h1>";
	echo "<table>";
	echo "<tr><th>Nombre</th><tr>";

	while($row = $result->fetch_assoc()) 
	{
		echo "<tr><td>" . $row["nombre"] . "</td></td>";
	}
	echo "</table>";

} else {
    echo "<p>No hay participantes</p>";

}

$sql = "SELECT t2.nombre as nombre FROM (SELECT ip FROM reto3 ORDER BY time DESC LIMIT 5) AS t1 INNER JOIN (SELECT * FROM becarios) as t2 ON t1.ip = t2.ip";
//$sql = "SELECT * FROM becarios WHERE inet_ntoa(ip) = '" . $_SERVER['REMOTE_ADDR'] . "'";
//echo $sql;
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    // output data of each row
	echo "<h1>Participaciones Reto 3</h1>";
	echo "<table>";
	echo "<tr><th>Nombre</th><tr>";

	while($row = $result->fetch_assoc()) 
	{
		echo "<tr><td>" . $row["nombre"] . "</td></td>";
	}
	echo "</table>";

} else {
    echo "<p>No hay participantes</p>";
}

$sql = "SELECT t2.nombre as nombre FROM (SELECT ip FROM reto4 ORDER BY time DESC LIMIT 5) AS t1 INNER JOIN (SELECT * FROM becarios) as t2 ON t1.ip = t2.ip";
//$sql = "SELECT * FROM becarios WHERE inet_ntoa(ip) = '" . $_SERVER['REMOTE_ADDR'] . "'";
//echo $sql;
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    // output data of each row
	echo "<h1>Participaciones Reto 4</h1>";
	echo "<table>";
	echo "<tr><th>Nombre</th><tr>";

	while($row = $result->fetch_assoc()) 
	{
		echo "<tr><td>" . $row["nombre"] . "</td></td>";
	}
	echo "</table>";

} else {
    echo "<p>No hay participantes</p>";
}


//echo "</table>";

//" <td>".$row["firstname"]." ".$row["lastname"]."</td></tr>";
//    echo "<table><tr><th>ID</th><th>Name</th></tr>";

$conn->close();
?> 
</div>



</div>

<script>
setTimeout(function(){ location.reload(); }, 1000);
</script>
</body>
</html>
