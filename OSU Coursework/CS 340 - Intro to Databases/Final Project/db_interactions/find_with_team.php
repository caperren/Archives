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

        SELECT om.first_name, om.last_name, ct.name
        FROM members om
        INNER JOIN member_teams_relations mtr ON om.id = mtr.member_id
        INNER JOIN club_teams ct ON ct.id = mtr.team_id
        WHERE ct.name = ?

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

        $db_query->bind_result($first_name, $last_name, $team);

        while($entry = $db_query->fetch()){
            $resp_arr['members'][] = array($first_name, $last_name, $team);
        }

        echo json_encode($resp_arr);
    }
}


