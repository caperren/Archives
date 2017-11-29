<?php
include '../db_creds.php';
ini_set('display_errors', 'Off');

$robo_db = new mysqli($db_host, $db_user, $db_pass, $db_name);
$resp_arr = array();
$affected_rows_total = 0;

if($robo_db->connect_errno){
    $resp_arr['is_error'] = true;
    $resp_arr['error_reason'] = "Failed to connect to database!";
    echo json_encode($resp_arr);
    return;
}else{
    ########## DELETION FROM MEMBER SKILLS RELATIONS ##########
    $db_query = $robo_db->prepare(<<<QUERY_STRING

        DELETE FROM member_skills_relations 
        WHERE member_id IN (
            SELECT id FROM members m 
            WHERE m.first_name = ? AND m.last_name = ?
        ) 

QUERY_STRING
    );

    $db_query->bind_param("ss", $_POST['first_name'], $_POST['last_name']);

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
        $affected_rows_total += $db_query->affected_rows;
    }

    ########## DELETION FROM MEMBER TEAMS RELATIONS ##########
    $db_query = $robo_db->prepare(<<<QUERY_STRING

        DELETE FROM member_teams_relations 
        WHERE member_id IN (
            SELECT id FROM members m 
            WHERE m.first_name = ? AND m.last_name = ?
        ) 

QUERY_STRING
    );

    $db_query->bind_param("ss", $_POST['first_name'], $_POST['last_name']);

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
        $affected_rows_total += $db_query->affected_rows;
    }
    ########## DELETION FROM MEMBERS ##########
    $db_query = $robo_db->prepare(<<<QUERY_STRING

        DELETE FROM members
        WHERE first_name = ? AND last_name = ?

QUERY_STRING
    );

    $db_query->bind_param("ss", $_POST['first_name'], $_POST['last_name']);

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
        $affected_rows_total += $db_query->affected_rows;
        $resp_arr['affected_rows'] = $affected_rows_total;
        echo json_encode($resp_arr);
    }
}