<?php
$con=mysqli_connect("georgebraxtoncom.ipagemysql.com", "XXXXXXXXXXXXX", "XXXXXXXXXXXX", "XXXXXXXXXXXXXX");
// Check connection
if (mysqli_connect_errno())
  {
  echo "Failed to connect to MySQL: " . mysqli_connect_error();
  }

$sql="INSERT INTO alerts (user, alert_type)
VALUES
('$_POST[user]', '$_POST[alert_type]')";

if (!mysqli_query($con,$sql))
  {
  die('Error: ' . mysqli_error($con));
  }
echo "1 record added";

mysqli_close($con);
?>
