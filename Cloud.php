<html>
<body>

<?php

$dbname = '0vrp4Gz4RM';
$dbuser = '0vrp4Gz4RM';
$dbpass = 'I2EQa4FkfT';
$dbhost = 'remotemysql.com:3306';

$connect = @mysqli_connect($dbhost,$dbuser,$dbpass,$dbname);

if(!$connect){
        echo "Error: " . mysqli_connect_error();
        exit();
}

echo "Connection Success!<br><br>";
$garden_id;
$humidity;
$temperature;
$water_level;
$soil_moist;
$water_pump;

$garden_id = $_POST["garden_id"];
$humidity = $_POST["humidity"];
$temperature = $_POST["temperature"];
$water_level = $_POST["water_level"];
$soil_moist = $_POST["soil_moist"];
$water_pump = $_POST["water_pump"];



$query = "INSERT INTO record(garden_id, humidity, temperature, water_level, soil_moist, water_pump) VALUES ('$garden_id', '$humidity', '$temperature', '$water_level', '$soil_moist', '$water_pump')";
$result = mysqli_query($connect,$query);

echo "Insertion Success!<br>";

?>
</body>
</html>

