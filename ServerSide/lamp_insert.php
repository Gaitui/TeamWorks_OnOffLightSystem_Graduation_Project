<?php
    require_once("dbtools.inc.php");
    $link = create_connection();
	for($i=0;$i<10;$i++)
	{
		$id=rand(1,20);
		$sec=rand(3,10);
		$lamp='';
		for($j=0;$j<64;$j++)
		{
			$tmp=$lamp.(rand(0,1)+'0');
			$lamp=$tmp;
		}
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
	}
	mysqli_close($link);
?>
<!DOCTYPE html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>tesp</title>
<meta name="keywords" content="" />
<meta name="description" content="" />
<link href="http://fonts.googleapis.com/css?family=Source+Sans+Pro:200,300,400,600,700,900" rel="stylesheet" />
<link href="default.css" rel="stylesheet" type="text/css" media="all" />
<link href="fonts.css" rel="stylesheet" type="text/css" media="all" />
</head>
<body>
<div id="header-wrapper">
	<div id="header" class="container">
		<div id="logo">
			<h1>tesp</h1>
		</div>
	</div>
</div>
<div id="portfolio" class="container">
	<div style="text-align:center;"><b><font size="5" color="black">
	歡迎加入&nbsp;</br></br>
	<center>
	<table width="200" border="1">
		<tr><td>ID</td><td>時間</td><td>狀態</td></tr>
		<?php
			if($result == true)
			{
				echo"YEEEEE";
			}
			else
			{
				echo"RRRRRR";
			}
		?>
	</table>
	</center>
	</font></b></div>
</body>
</html>
