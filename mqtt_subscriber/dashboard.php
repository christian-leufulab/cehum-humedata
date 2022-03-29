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

$sql = "SELECT atmospheric_pressure, internal_pressure, latitude, longitude FROM logs ORDER BY id DESC LIMIT 1";

$result = $conn->query($sql);

if ($result->num_rows > 0) {
  $result = $result->fetch_assoc();
  $atm_pressure = $result["atmospheric_pressure"];
  $internal_pressure = $result["internal_pressure"];
  $lat = $result["latitude"];
  $long = $result["longitude"];
} else {
  echo "0 results";
}

header('Access-Control-Allow-Origin: *');
header("Access-Control-Allow-Headers: X-API-KEY, Origin, X-Requested-With, Content-Type, Accept, Access-Control-Request-Method");
header("Access-Control-Allow-Methods: GET, POST, OPTIONS, PUT, DELETE");
header("Allow: GET, POST, OPTIONS, PUT, DELETE");
$method = $_SERVER['REQUEST_METHOD'];
if($method == "OPTIONS") {
    die();
}

echo '
<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />

    <link rel="stylesheet" href="styles.css" />
    <link
      rel="stylesheet"
      href="http://cdn.leafletjs.com/leaflet-0.7.3/leaflet.css"
    />

    <script src="https://code.highcharts.com/highcharts.js"></script>
    <script src="https://code.highcharts.com/highcharts-more.js"></script>
    <script src="https://code.highcharts.com/modules/solid-gauge.js"></script>
    <script src="https://code.highcharts.com/modules/exporting.js"></script>
    <script src="https://code.highcharts.com/modules/export-data.js"></script>
    <script src="https://code.highcharts.com/modules/accessibility.js"></script>

    <title>Document</title>
  </head>
  <body>
    <figure class="highcharts-figure">
      <div id="container-atm-pressure" class="chart-container"></div>
      <div id="container-rpm" class="chart-container"></div>

      <div id="map" style="width: 100%; height: 400px"></div>
    </figure>

    <script src="http://cdn.leafletjs.com/leaflet-0.7.3/leaflet.js"></script>
    <script>
      let mapOptions = {
        center: [-39.8139, -73.2458],
        zoom: 10,
      };

      let map = new L.map("map", mapOptions);

      let layer = new L.TileLayer(
        "http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png"
      );
      map.addLayer(layer);

      let marker = new L.Marker(['. $lat . ', ' . $long . ']);
      marker.addTo(map);
    </script>

    <script>
    let atm_pres = '. $atm_pressure .';
    let int_pres = '. $internal_pressure .';
    </script>

    <script src="gauge.js"></script>
  </body>
</html>
';
?> 


