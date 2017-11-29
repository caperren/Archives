<?php
include '../db_creds.php';
ini_set('display_errors', 'Off');

$robo_db = new mysqli($db_host, $db_user, $db_pass, $db_name);
$resp_arr = array();

if($robo_db->connect_errno){
    $resp_arr['is_error'] = true;
    $resp_arr['error_reason'] = "Failed to connect to database!";
    echo json_encode($resp_arr);
    return;
}else{
    $db_query = $robo_db->prepare(<<<QUERY_STRING

        SELECT m.first_name, m.last_name, cp.name 
        FROM club_positions cp
        INNER JOIN members m ON m.position = cp.id
        WHERE cp.name = ?;

QUERY_STRING
    );

    $db_query->bind_param("s", $_POST['name']);

    if(!$db_query){
        $resp_arr['is_error'] = true;
        $resp_arr['error_reason'] = "Parameter bind failed!";
        echo json_encode($resp_arr);
        return;
    }

    $db_query->execute();

    if(!$db_query){
        $resp_arr['is_error'] = true;
        $resp_arr['error_reason'] = "Database query failed!";
        $resp_arr['mysqli_errno'] = $db_query->errno;
        $resp_arr['mysqli_error'] = $db_query->error;
        echo json_encode($resp_arr);
        return;
    }else{
        $resp_arr['is_error'] = false;

        $db_query->bind_result($first_name, $last_name, $position);

        while($entry = $db_query->fetch()){
            $resp_arr['members'][] = array($first_name, $last_name, $position);
        }

        echo json_encode($resp_arr);
    }
}


