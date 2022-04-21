<?php

// http://YOUR-DOMAIN.com/datareceive.php?mac=000000000000&devicename=ExampleDevice&fwver=v1.0

 $var1 = $_GET['mac'];
 $var2 = $_GET['devicename'];
 $var3 = $_GET['fwver'];
 $fileContent = "[". date("Y-m-d H:i:s") ."] MAC: ".$var1.", Device Name: ".$var2.", Firmware Version: " .$var3. "\n";
 $fileStatus = file_put_contents('datalog.txt',$fileContent,FILE_APPEND);
 if($fileStatus != false) 
 {
  echo "SUCCESS";
 }
 else
 {
  echo "ERROR"; 
 }
?> 