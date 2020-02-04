<!DOCTYPE  html>

<html>
<head>
<title>Publico222</title>
</head>
<body>

<div>
 <?php
$servername = "localhost";
$username = "becariosmaster";
$password = NULL;
$dbname = "becarios";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error){
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT * FROM becarios WHERE inet_ntoa(ip) = '" . $_SERVER['REMOTE_ADDR'] . "'";

//echo $sql;
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    // output data of each row
	while($row = $result->fetch_assoc()) 
	{
		echo "<p>Saludos: ". $row["nombre"] . "</p>";
		echo "<p>Tu ip: " . $_SERVER["REMOTE_ADDR"] ." </p>";
		echo "<table>";
		echo "<tr><th>Reto</th><th>Calif</th><th>PORT</th><th>PORT-PRUEBAS</th></tr>";
		echo "<tr><td>1</td><td>" . $row["r1"] . "</td><td>7791</td><td>5591</td>";
		echo "<tr><td>2</td><td>" . $row["r2"] . "</td><td>7792</td><td>5592</td>";
		echo "<tr><td>3</td><td>" . $row["r3"] . "</td><td>7793</td><td>5593</td>";
		echo "<tr><td>4</td><td>" . $row["r4"] . "</td><td>7794</td><td>5594</td>";
		echo "</table>";
//		echo "<tr><td>".$row["nombre"]."</td> ";
	}
} else {
    echo "0 results";
}

//echo "</table>";

//" <td>".$row["firstname"]." ".$row["lastname"]."</td></tr>";
//    echo "<table><tr><th>ID</th><th>Name</th></tr>";

$conn->close();
?> 




</div>

</body>
</html>
