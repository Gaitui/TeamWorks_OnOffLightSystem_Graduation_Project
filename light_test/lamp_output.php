<?php
  require_once("dbtools.inc.php");
  $link = create_connection();
  $id=$_POST["id"];
  $sql="SELECT * FROM light WHERE ID='$id'";
  $result = execute_sql($link,"tesp",$sql);
  $row = mysqli_fetch_assoc($result);
  $lamp=$row{"lamp"};
  $sec=$row{"time"};
  $file = fopen($row{"ID"}.".txt","w");
  fwrite($file,$lamp.PHP_EOL.$sec);
  fclose($file);
  mysqli_close($link);
  exit();
 ?>
