<?php
include '../db_creds.php';
ini_set('display_errors', 'On');

$robo_db = new mysqli($db_host, $db_user, $db_pass, $db_name);
$resp_arr = array();

if($robo_db->connect_errno){
    $resp_arr['is_error'] = true;
    $resp_arr['error_reason'] = "Failed to connect to database!";
    echo json_encode($resp_arr);
    return;
}else{
    $db_query = $robo_db->prepare(<<<QUERY_STRING

        INSERT INTO members(first_name, last_name, position, superior, email, phone, osu_num, rfid_num)
        VALUES (
        ?,
        ?, 
        (SELECT id FROM club_positions cp WHERE cp.name = ?), 
        (SELECT id FROM club_positions cp WHERE cp.name = ?), 
        ?,
        ?,
        ?,
        ?
        )

QUERY_STRING
    );

    $db_query->bind_param("ssssssii", $_POST['first_name'], $_POST['last_name'], $_POST['position'],
        $_POST['superior'], $_POST['email'], $_POST['phone'], $_POST['osu_num'], $_POST['rfid_num']);

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
        $resp_arr['affected_rows'] = $db_query->affected_rows;
        echo json_encode($resp_arr);
    }
}


