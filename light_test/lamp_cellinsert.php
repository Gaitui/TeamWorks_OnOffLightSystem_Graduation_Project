<?php
    require_once("dbtools.inc.php");
    $link = create_connection();
	$id=$_POST["id"];
	$sec=$_POST["time"];
	$lamp=$_POST["light"];
	$sql="SELECT * FROM light WHERE ID='$id'";
	$result = execute_sql($link, "tesp", $sql);
	if(mysqli_num_rows($result)!=0)
	{
		$sql="UPDATE light SET lamp='$lamp', time='$sec' WHERE ID='$id'";
	}
	else
	{
		$sql="INSERT INTO light (ID,lamp,time) VALUES ('$id','$lamp','$sec')";
	}
	$result = execute_sql($link,"tesp",$sql);
	mysqli_close($link);
?>