<?php
header('Content-type: text/html');

echo <<<DONEUPPERSCRIPTS

<html>
    <head>
        <title>LifeCatch Realtime Monitoring</title>
        <style>
            .lrow {border-bottom: 1px solid white;}
            .lcell {width: 300px;}
        </style>
        <script src="http://ajax.googleapis.com/ajax/libs/jquery/1.11.0/jquery.min.js"  type="text/javascript"></script>
        <script type="text/javascript">
            var row_count = 0;
            $(document).ready(function(){
                $.ajax({
                    url: "get_alerts.php",
                    type: "POST",
                    dataType: 'json',
                    data: { user:  '{$_POST["userkey"]}'  },
                    success: function(data){
                        for( var i = 0; i < data.lclog.length; i++){
                            var row_data = "";
                            row_data +="<tr class='lrow'><td class='lcell'>" + data.lclog[i].alert_type + "</td><td>" + data.lclog[i].time + "</td></tr>";
                            $("#log").append(row_data);
                        }
                    },
                    error:function(){
                        $("#log").html('There was an error updating the settings');
                        $("#log").addClass('msg_error');
                        $("#log").fadeIn(1500);
                     }   
                }); 
            });
        </script>
    </head>
    <body style="background-color:#0066CC; color:#000080;">
        <h1 style="align: center">LifeCatch Realtime Monitor</h1>
        <h2 style="color: #000080;">Recent Alerts</h2>
        <div style="background-color: #000000; color: #ffffff; width: 600px; height: 480px;">
             <table class="lrow" style="width: 600px;" border="0">
                   <tbody>
                        <tr>
                                        <td class="lcell">Alert Type</td>
                                        <td>Date/Time</td>
                       </tr>
                  </tbody>
             </table>
             <table id="log" border="0">
                       <tbody >
                       </tbody>
            </table>
        </div>
   </body>
</html>

DONEUPPERSCRIPTS;
?>