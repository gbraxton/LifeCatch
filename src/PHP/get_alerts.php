<?php
       $con=mysqli_connect("georgebraxtoncom.ipagemysql.com", "XXXXXXXXXX", 'XXXXXXXXXX', "XXXXXXXX");
      // Check connection
       if (mysqli_connect_errno())
       {
            echo "Failed to connect to MySQL: " . mysqli_connect_error();
       }
      header('Content-Type: application/json');
      $postdata = $_POST['user'];
      mysqli_query('SET CHARACTER SET utf8');
      $result = mysqli_query($con,  "SELECT * FROM alerts WHERE user =  '{$postdata}'");
      $return_arr = array();
      while ( $row = $result->fetch_array(MYSQLI_ASSOC) ) {
                  // create rowArr
                  $rowArr = array(
                           alert_type => $row['alert_type'],
                           time => $row['time']
                     );

                    // store rowArr in $return_arr
                   $return_arr['lclog'][] = $rowArr;

         }

          // display json encode

          echo json_encode($return_arr);
?>
