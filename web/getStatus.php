<?php
$servername = "localhost"; //fill these in in a sec
$username;
$password;
$dbname = "if0_34508132_StatusTracker";
$tablename = "Status";

if (isset($_GET['id'])) {
  $id = $_GET['id'];
} else {
  echo "Error: No ID specified";
  exit();
}

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
  die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT status FROM $tablename WHERE id = $id";

if (mysqli_query($conn, $sql)) {
    echo "Success";
  } else {
    echo "Error updating record: " . mysqli_error($conn);
  }

?>