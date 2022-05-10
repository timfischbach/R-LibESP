<?php

// http://dl.timfischbach.com/datareceive.php?mac=000000000000&devicename=TestProduct&fwver=v1.0&status=OK

 $var1 = $_GET['mac'];
 $var2 = $_GET['devicename'];
 $var3 = $_GET['fwver'];
 $var4 = $_GET['status'];
 $fileContent = "[". date("Y-m-d H:i:s") ."] MAC: ".$var1.", Device Name: ".$var2.", Firmware Version: " .$var3. ", Status: " .$var4. "\n";
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