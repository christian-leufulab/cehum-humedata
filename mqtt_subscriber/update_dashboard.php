<?php
$servername = "localhost";
$username = "root";
$password = "Learsi.96";
$db = "mqtt";

// Create connection
$conn = new mysqli($servername, $username, $password, $db);

// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

$atm_pressure = 0;
$internal_pressure = 0;
$lat = 0;
$long = 0;

$sql = "SELECT * FROM logs ORDER BY id DESC LIMIT 1";

$result = $conn->query($sql);

if ($result->num_rows > 0) {
  $result = $result->fetch_assoc();
  $return = [
    'atm_pressure' => $result["atmospheric_pressure"],
    'internal_pressure' => $result["internal_pressure"],
    'lat' => $result["latitude"],
    'long' => $result["longitude"]

  ];
  echo json_encode($result);
} else {
  echo "0 results";
}


?>