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

        SELECT
        om.first_name,
        om.last_name,
        (SELECT name FROM club_positions cp
        INNER JOIN members m ON m.position = cp.id
        WHERE m.first_name = om.first_name AND m.last_name = om.last_name),
        (SELECT name FROM club_positions cp
        INNER JOIN members m ON m.superior = cp.id
        WHERE m.first_name = om.first_name AND m.last_name = om.last_name),
        om.email,
        om.phone,
        om.osu_num,
        om.rfid_num
        FROM members om;

QUERY_STRING
    );

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

        $db_query->bind_result($first_name, $last_name, $position, $superior, $email, $phone, $osu_num, $rfid_num);

        while($entry = $db_query->fetch()){
            $resp_arr['members'][] = array($first_name, $last_name, $position, $superior, $email, $phone, $osu_num, $rfid_num);
        }

        echo json_encode($resp_arr);
    }
}


