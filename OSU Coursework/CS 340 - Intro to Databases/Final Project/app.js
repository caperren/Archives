var root_url = '/~perrenc/CS340_Final_Website/';

var get_members_url = 'db_interactions/get_members.php';
var get_positions_url = root_url + 'db_interactions/get_positions.php';
var get_teams_url = root_url + 'db_interactions/get_teams.php';
var get_skills_url = root_url + 'db_interactions/get_skills.php';

var add_new_mem_url = root_url + 'db_interactions/add_member.php';
var del_mem_url = root_url + 'db_interactions/del_member.php';
var assign_skill_mem_url = root_url + 'db_interactions/assign_skill.php';
var assign_team_mem_url = root_url + 'db_interactions/assign_team.php';

var add_new_skill_url = root_url + 'db_interactions/add_new_skill.php';
var add_new_team_url = root_url + 'db_interactions/add_new_team.php';
var add_new_position_url = root_url + 'db_interactions/add_new_position.php';

var find_with_skill_url = root_url + 'db_interactions/find_with_skill.php';
var find_with_team_url = root_url + 'db_interactions/find_with_team.php';
var find_with_position_url = root_url + 'db_interactions/find_with_position.php';

////////// Function to reset inputs //////////
function reset_inputs() {
    document.getElementById('mem_add_first_name').value = "";
    document.getElementById('mem_add_last_name').value = "";
    document.getElementById('mem_add_position_drop').selectedIndex = 0;
    document.getElementById('mem_add_superior_drop').selectedIndex = 0;
    document.getElementById('mem_add_email').value = "";
    document.getElementById('mem_add_phone').value = "";
    document.getElementById('mem_add_osuid').value = "";
    document.getElementById('mem_add_rfid').value = "";

    document.getElementById('mem_del_first_name').value = "";
    document.getElementById('mem_del_last_name').value = "";

    document.getElementById('assign_skill_first_name').value = "";
    document.getElementById('assign_skill_last_name').value = "";
    document.getElementById('assign_skill_position_drop').selectedIndex = 0;

    document.getElementById('add_team_first_name').value = "";
    document.getElementById('add_team_last_name').value = "";
    document.getElementById('add_team_position_drop').selectedIndex = 0;

    document.getElementById('skill_add_name').value = "";
    document.getElementById('skill_add_description').value = "";

    document.getElementById('team_add_name').value = "";
    document.getElementById('team_add_description').value = "";

    document.getElementById('position_add_name').value = "";
    document.getElementById('position_add_description').value = "";

    document.getElementById('show_with_skill_drop').selectedIndex = 0;
    document.getElementById('show_with_team_drop').selectedIndex = 0;
    document.getElementById('show_with_position_drop').selectedIndex = 0;

}

////////// Functions to show position search //////////
function show_with_position() {
    var position_name = document.getElementById('show_with_position_drop').value;

    var req = new XMLHttpRequest();
    req.open('POST', find_with_position_url, true);
    req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    req.addEventListener('load', on_show_with_position_response_received);
    req.send("name=" + position_name);
}

function on_show_with_position_response_received(response) {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    status_box.textContent = "";
    err_box.textContent = "";

    var header_data = ["First Name", "Last Name", "Position"];

    var req_status = response.srcElement.status;
    if(req_status >=200 && req_status < 400){

        var response_json = JSON.parse(response.srcElement.responseText);
        if(!response_json.is_error){
            var members = response_json.members;
            generate_table_and_show("Members Position Search", header_data, members);
            status_box.textContent = "Members position list shown successfully!"
        }else{
            err_box.textContent = "MEMBERS POSITION SHOW: Unknown error with request. Please consult logs.";
        }
    }else{
        err_box.textContent = "MEMBERS POSITION SHOW: Failed to contact server for request. Please verify webserver is up and files exist.";
    }
}

////////// Functions to show team search //////////
function show_with_team() {
    var team_name = document.getElementById('show_with_team_drop').value;

    var req = new XMLHttpRequest();
    req.open('POST', find_with_team_url, true);
    req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    req.addEventListener('load', on_show_with_team_response_received);
    req.send("name=" + team_name);
}

function on_show_with_team_response_received(response) {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    status_box.textContent = "";
    err_box.textContent = "";

    var header_data = ["First Name", "Last Name", "Team"];

    var req_status = response.srcElement.status;
    if(req_status >=200 && req_status < 400){

        var response_json = JSON.parse(response.srcElement.responseText);
        if(!response_json.is_error){
            var members = response_json.members;
            generate_table_and_show("Members Team Search", header_data, members);
            status_box.textContent = "Members team list shown successfully!"
        }else{
            err_box.textContent = "MEMBERS TEAM SHOW: Unknown error with request. Please consult logs.";
        }
    }else{
        err_box.textContent = "MEMBERS TEAM SHOW: Failed to contact server for request. Please verify webserver is up and files exist.";
    }
}

////////// Functions to show skill search //////////
function show_with_skill() {
    var skill_name = document.getElementById('show_with_skill_drop').value;

    var req = new XMLHttpRequest();
    req.open('POST', find_with_skill_url, true);
    req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    req.addEventListener('load', on_show_with_skill_response_received);
    req.send("name=" + skill_name);
}

function on_show_with_skill_response_received(response) {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    status_box.textContent = "";
    err_box.textContent = "";

    var header_data = ["First Name", "Last Name", "Skill"];

    var req_status = response.srcElement.status;
    if(req_status >=200 && req_status < 400){

        var response_json = JSON.parse(response.srcElement.responseText);
        if(!response_json.is_error){
            var members = response_json.members;
            generate_table_and_show("Members Skill Search", header_data, members);
            status_box.textContent = "Members skill list shown successfully!"
        }else{
            err_box.textContent = "MEMBERS SKILLS SHOW: Unknown error with request. Please consult logs.";
        }
    }else{
        err_box.textContent = "MEMBERS SKILLS SHOW: Failed to contact server for request. Please verify webserver is up and files exist.";
    }
}

////////// Functions to show positions //////////
function show_positions() {
    var req = new XMLHttpRequest();
    req.open('POST', get_positions_url, true);
    req.addEventListener('load', on_show_positions_response_received);
    req.send(null);
}

function on_show_positions_response_received(response) {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    status_box.textContent = "";
    err_box.textContent = "";

    var header_data = ["Name", "Description"];

    var req_status = response.srcElement.status;
    if(req_status >=200 && req_status < 400){

        var response_json = JSON.parse(response.srcElement.responseText);

        if(!response_json.is_error){
            var positions = response_json.positions;
            generate_table_and_show("Positions List", header_data, positions);
            status_box.textContent = "Positions list shown successfully!"
        }else{
            err_box.textContent = "POSITIONS SHOW: Unknown error with request. Please consult logs.";
        }
    }else{
        err_box.textContent = "POSITIONS SHOW: Failed to contact server for request. Please verify webserver is up and files exist.";
    }
}

////////// Functions to show teams //////////
function show_teams() {
    var req = new XMLHttpRequest();
    req.open('POST', get_teams_url, true);
    req.addEventListener('load', on_show_teams_response_received);
    req.send(null);
}

function on_show_teams_response_received(response) {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    status_box.textContent = "";
    err_box.textContent = "";

    var header_data = ["Name", "Description"];

    var req_status = response.srcElement.status;
    if(req_status >=200 && req_status < 400){

        var response_json = JSON.parse(response.srcElement.responseText);

        if(!response_json.is_error){
            var teams = response_json.teams;
            generate_table_and_show("Teams List", header_data, teams);
            status_box.textContent = "Teams list shown successfully!"
        }else{
            err_box.textContent = "TEAMS SHOW: Unknown error with request. Please consult logs.";
        }
    }else{
        err_box.textContent = "TEAMS SHOW: Failed to contact server for request. Please verify webserver is up and files exist.";
    }
}

////////// Functions to show skills //////////
function show_skills() {
    var req = new XMLHttpRequest();
    req.open('POST', get_skills_url, true);
    req.addEventListener('load', on_show_skills_response_received);
    req.send(null);
}

function on_show_skills_response_received(response) {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    status_box.textContent = "";
    err_box.textContent = "";

    var header_data = ["Name", "Description"];

    var req_status = response.srcElement.status;
    if(req_status >=200 && req_status < 400){

        var response_json = JSON.parse(response.srcElement.responseText);

        if(!response_json.is_error){
            var skills = response_json.skills;
            generate_table_and_show("Skills List", header_data, skills);
            status_box.textContent = "Skills list shown successfully!"
        }else{
            err_box.textContent = "SKILLS SHOW: Unknown error with request. Please consult logs.";
        }
    }else{
        err_box.textContent = "SKILLS SHOW: Failed to contact server for request. Please verify webserver is up and files exist.";
    }

}

////////// Functions to show members //////////
function show_members() {
    var req = new XMLHttpRequest();
    req.open('POST', get_members_url, true);
    req.addEventListener('load', on_show_members_response_received);
    req.send(null);
}

function on_show_members_response_received(response) {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    status_box.textContent = "";
    err_box.textContent = "";

    var header_data = ["First Name", "Last Name", "Position", "Superior", "Email", "Phone", "OSU ID #", "RFID #"];

    var req_status = response.srcElement.status;
    if(req_status >=200 && req_status < 400){

        var response_json = JSON.parse(response.srcElement.responseText);

        if(!response_json.is_error){
            var members = response_json.members;
            generate_table_and_show("Members List", header_data, members);
            status_box.textContent = "Members list shown successfully!"
        }else{
            err_box.textContent = "MEMBER SHOW: Unknown error with request. Please consult logs.";
        }
    }else{
        err_box.textContent = "MEMBER SHOW: Failed to contact server for request. Please verify webserver is up and files exist.";
    }

}

////////// Generate table and show //////////
function generate_table_and_show(title, headers, array_data) {

    var table_div = document.getElementById('main_output_content');
    table_div.innerHTML = "";

    var new_h2 = document.createElement("h2");
    new_h2.textContent = title;
    table_div.appendChild(new_h2);

    var table = document.createElement("table");
    var table_header = table.createTHead();

    var row = table_header.insertRow(0);


    for(var head_iter in headers){
        var cell = row.insertCell(head_iter);
        cell.textContent = headers[head_iter];
    }
    row.style.fontWeight = "bold";

    for(var row_iter in array_data){
        var cur_row = array_data[row_iter];

        row = table.insertRow(parseInt(row_iter) + 1);

        for(var col_iter in cur_row){
            cell = row.insertCell(col_iter);
            cell.textContent = array_data[row_iter][col_iter];
        }
    }

    table_div.appendChild(table);
}


////////// Add new position on_click_handler //////////
function on_add_new_position_clicked() {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    status_box.textContent = "";
    err_box.textContent = "";

    var name = document.getElementById('position_add_name').value;
    var description = document.getElementById('position_add_description').value;

    if(!name){
        status_box.textContent = "";
        err_box.textContent = "Name must be present!";
        return;
    }

    var param_string = "name=" + name;
    if (description) { param_string += "&description=" + description; }

    var req = new XMLHttpRequest();
    req.open('POST', add_new_position_url, true);
    req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    req.addEventListener('load', on_add_change_delete_response_received);
    req.send(param_string);

}

////////// Add new team on_click_handler //////////
function on_add_new_team_clicked() {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    status_box.textContent = "";
    err_box.textContent = "";

    var name = document.getElementById('team_add_name').value;
    var description = document.getElementById('team_add_description').value;

    if(!name){
        status_box.textContent = "";
        err_box.textContent = "Name must be present!";
        return;
    }

    var param_string = "name=" + name;
    if (description) { param_string += "&description=" + description; }

    var req = new XMLHttpRequest();
    req.open('POST', add_new_team_url, true);
    req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    req.addEventListener('load', on_add_change_delete_response_received);
    req.send(param_string);

}

////////// Add new skill on_click_handler //////////
function on_add_new_skill_clicked() {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    status_box.textContent = "";
    err_box.textContent = "";

    var name = document.getElementById('skill_add_name').value;
    var description = document.getElementById('skill_add_description').value;

    if(!name){
        status_box.textContent = "";
        err_box.textContent = "Name must be present!";
        return;
    }

    var param_string = "name=" + name;
    if (description) { param_string += "&description=" + description; }

    var req = new XMLHttpRequest();
    req.open('POST', add_new_skill_url, true);
    req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    req.addEventListener('load', on_add_change_delete_response_received);
    req.send(param_string);

}

////////// Assign team to member on_click_handler //////////
function on_assign_team_to_member_clicked() {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    status_box.textContent = "";
    err_box.textContent = "";

    var first = document.getElementById('add_team_first_name').value;
    var last = document.getElementById('add_team_last_name').value;
    var team = document.getElementById('add_team_position_drop').value;

    if(!first || !last){
        status_box.textContent = "";
        err_box.textContent = "First and last must be present!";
        return;
    }

    var param_string = "first_name=" + first + "&last_name=" + last + "&team=" + team;

    var req = new XMLHttpRequest();
    req.open('POST', assign_team_mem_url, true);
    req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    req.addEventListener('load', on_add_change_delete_response_received);
    req.send(param_string);

}

////////// Assign skill to member on_click_handler //////////
function on_assign_skill_to_member_clicked() {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    status_box.textContent = "";
    err_box.textContent = "";

    var first = document.getElementById('assign_skill_first_name').value;
    var last = document.getElementById('assign_skill_last_name').value;
    var skill = document.getElementById('assign_skill_position_drop').value;

    if(!first || !last){
        status_box.textContent = "";
        err_box.textContent = "First and last must be present!";
        return;
    }

    var param_string = "first_name=" + first + "&last_name=" + last + "&skill=" + skill;

    var req = new XMLHttpRequest();
    req.open('POST', assign_skill_mem_url, true);
    req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    req.addEventListener('load', on_add_change_delete_response_received);
    req.send(param_string);

}

////////// Delete member on_click_handler //////////
function on_delete_member_clicked() {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    status_box.textContent = "";
    err_box.textContent = "";

    var first = document.getElementById('mem_del_first_name').value;
    var last = document.getElementById('mem_del_last_name').value;

    if(!first || !last){
        status_box.textContent = "";
        err_box.textContent = "First and last must be present!";
        return;
    }

    var param_string = "first_name=" + first + "&last_name=" + last

    var req = new XMLHttpRequest();
    req.open('POST', del_mem_url, true);
    req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    req.addEventListener('load', on_add_change_delete_response_received);
    req.send(param_string);

}

////////// Add member on_click_handler //////////
function on_add_member_clicked() {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    status_box.textContent = "";
    err_box.textContent = "";

    var first = document.getElementById('mem_add_first_name').value;
    var last = document.getElementById('mem_add_last_name').value;
    var position = document.getElementById('mem_add_position_drop').value;
    var superior = document.getElementById('mem_add_superior_drop').value;
    var email = document.getElementById('mem_add_email').value;
    var phone = document.getElementById('mem_add_phone').value;
    var osuid = document.getElementById('mem_add_osuid').value;
    var rfid = document.getElementById('mem_add_rfid').value;

    if(!first || !last || !position || !email || !osuid){
        status_box.textContent = "";
        err_box.textContent = "First, last, position, email, and OSU ID must be present!";
        return;
    }

    var param_string = "first_name=" + first + "&last_name=" + last + "&position=" + position;

    if (superior != "--") { param_string += "&superior=" + superior; }
    param_string += "&email=" + email;
    if (phone) { param_string += "&phone=" + phone; }
    param_string += "&osu_num=" + osuid;
    if (rfid) { param_string += "&rfid_num=" + rfid; }

    var req = new XMLHttpRequest();
    req.open('POST', add_new_mem_url, true);
    req.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
    req.addEventListener('load', on_add_change_delete_response_received);
    req.send(param_string);

}

////////// Functions for updating club skill dropdowns //////////
function update_skill_dropdowns() {
    var req = new XMLHttpRequest();
    req.open('POST', get_skills_url, true);
    req.addEventListener('load', on_update_skills_response_received);
    req.send(null);
}

function on_update_skills_response_received(response) {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    //status_box.textContent = "";
    //err_box.textContent = "";

    var assign_skill = document.getElementById('assign_skill_position_drop');
    var skill_search = document.getElementById('show_with_skill_drop');

    var req_status = response.srcElement.status;
    if(req_status >=200 && req_status < 400){

        var response_json = JSON.parse(response.srcElement.responseText);

        if(!response_json.is_error){
            var skills = response_json.skills;

            assign_skill.innerHTML = "";
            populate_select_with_options(assign_skill, skills);

            skill_search.innerHTML = "";
            populate_select_with_options(skill_search, skills);
        }else {
            //err_box.textContent = "SKILLS UPDATE: Unknown error with request. Please consult logs.";
        }
    }else{
        //err_box.textContent = "SKILLS UPDATE: Failed to contact server for request. Please verify webserver is up and files exist.";
    }
}

////////// Functions for updating club team dropdowns //////////
function update_team_dropdowns() {
    var req = new XMLHttpRequest();
    req.open('POST', get_teams_url, true);
    req.addEventListener('load', on_update_teams_response_received);
    req.send(null);
}

function on_update_teams_response_received(response) {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    //status_box.textContent = "";
    //err_box.textContent = "";

    var add_to_team = document.getElementById('add_team_position_drop');
    var team_search = document.getElementById('show_with_team_drop');

    var req_status = response.srcElement.status;
    if(req_status >=200 && req_status < 400){

        var response_json = JSON.parse(response.srcElement.responseText);

        if(!response_json.is_error){
            var teams = response_json.teams;

            add_to_team.innerHTML = "";
            populate_select_with_options(add_to_team, teams);

            team_search.innerHTML = "";
            populate_select_with_options(team_search, teams);
        }else {
            //err_box.textContent = "TEAMS UPDATE: Unknown error with request. Please consult logs.";
        }
    }else{
        //err_box.textContent = "TEAMS UPDATE: Failed to contact server for request. Please verify webserver is up and files exist.";
    }
}

////////// Functions for updating club position dropdowns //////////
function update_position_dropdowns() {
    var req = new XMLHttpRequest();
    req.open('POST', get_positions_url, true);
    req.addEventListener('load', on_update_positions_response_received);
    req.send(null);
}

function on_update_positions_response_received(response) {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    //status_box.textContent = "";
    //err_box.textContent = "";

    var pos_add = document.getElementById('mem_add_position_drop');
    var sup_add = document.getElementById('mem_add_superior_drop');
    var pos_search = document.getElementById('show_with_position_drop');

    var req_status = response.srcElement.status;
    if(req_status >=200 && req_status < 400){

        var response_json = JSON.parse(response.srcElement.responseText);

        if(!response_json.is_error){
            var positions = response_json.positions;

            clear_dropdown(pos_add);
            populate_select_with_options(pos_add, positions);

            clear_dropdown(sup_add);
            populate_select_with_options(sup_add, [["--"]]);
            populate_select_with_options(sup_add, positions);

            clear_dropdown(pos_search);
            populate_select_with_options(pos_search, positions);
        }else {
            //err_box.textContent = "POSITIONS UPDATE: Unknown error with request. Please consult logs.";
        }
    }else{
        //err_box.textContent = "POSITIONS UPDATE: Failed to contact server for request. Please verify webserver is up and files exist.";
    }
}

////////// Generic use functions //////////
function populate_select_with_options(select_element, option_array){
    for(var option_iter in option_array){
        var option = document.createElement("option");
        option.textContent = option_array[option_iter][0];
        select_element.add(option);
    }
}

function on_add_change_delete_response_received(response) {
    var status_box = document.getElementById('header_status_bar');
    var err_box = document.getElementById('header_error_bar');

    var response_json = JSON.parse(response.srcElement.responseText);

    if(response.is_error){
        err_box.textContent = "Full failure. Please verify that the entry does not already exist, or check the backend code!"
    }else{
        if(response_json.affected_rows > 0){
            status_box.textContent = "Request completed successfully!"
        }else if (response_json.affected_rows == -1){
            err_box.textContent = "This entry already exists. Please make a unique entry!"
        }else{
            err_box.textContent = "No records exist, or a large error has occurred!"
        }
    }
    populate_page_and_show_members();

}

function clear_dropdown(dropdown) {
    while(dropdown.options.length > 0){
        dropdown.remove(0);
    }
}

////////// Load listener to populate page with data //////////
function populate_page_and_show_members(){
    update_skill_dropdowns();
    update_team_dropdowns();
    update_position_dropdowns();
    reset_inputs();
    show_members();
}

function assign_window_event_handlers() {
    document.getElementById('mem_add_submit').addEventListener('click', on_add_member_clicked);
    document.getElementById('mem_del_submit').addEventListener('click', on_delete_member_clicked);
    document.getElementById('assign_skill_submit').addEventListener('click', on_assign_skill_to_member_clicked);
    document.getElementById('add_team_submit').addEventListener('click', on_assign_team_to_member_clicked);
    document.getElementById('skill_add_submit').addEventListener('click', on_add_new_skill_clicked);
    document.getElementById('team_add_submit').addEventListener('click', on_add_new_team_clicked);
    document.getElementById('position_add_submit').addEventListener('click', on_add_new_position_clicked);

    document.getElementById('show_members_submit').addEventListener('click', show_members);
    document.getElementById('show_skills_submit').addEventListener('click', show_skills);
    document.getElementById('show_teams_submit').addEventListener('click', show_teams);
    document.getElementById('show_positions_submit').addEventListener('click', show_positions);

    document.getElementById('skill_search_submit').addEventListener('click', show_with_skill);
    document.getElementById('team_search_submit').addEventListener('click', show_with_team);
    document.getElementById('position_search_submit').addEventListener('click', show_with_position);
}

function setup_page_on_load() {
    populate_page_and_show_members();
    assign_window_event_handlers();
}

window.addEventListener("load", setup_page_on_load);