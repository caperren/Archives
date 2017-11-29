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

        INSERT INTO member_teams_relations(member_id, team_id)
        VALUES (
        (SELECT id FROM members m WHERE m.first_name = ? AND m.last_name = ?),
        (SELECT id from club_teams ct WHERE ct.name = ?)
        )

QUERY_STRING
    );

    $db_query->bind_param("sss", $_POST['first_name'], $_POST['last_name'], $_POST['team']);

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


