var current_col = 0;
var current_selected_cell;

function up_event_handler(current_event){
    var prev_row_sib = current_selected_cell.parentElement.previousElementSibling;

    if((typeof(prev_row_sib) == 'object') && (prev_row_sib != null)){ 
        handle_new_cell_selection(prev_row_sib.children[current_col]);
    }
}

function down_event_handler(current_event){
    var next_row_sib = current_selected_cell.parentElement.nextElementSibling;
    
    if((typeof(next_row_sib) == 'object') && (next_row_sib != null)){ 
        handle_new_cell_selection(next_row_sib.children[current_col]);
    }
}

function left_event_handler(current_event){
    var prev_sib = current_selected_cell.previousSibling;
    
    if((typeof(prev_sib) == 'object') && (prev_sib != null)){
        current_col--;
        handle_new_cell_selection(prev_sib);
    } 
}

function right_event_handler(current_event){
    var next_sib = current_selected_cell.nextSibling;
    
    if((typeof(next_sib) == 'object') && (next_sib != null)){
        current_col++;
        handle_new_cell_selection(next_sib);
    }  
}

function mark_event_handler(current_event){
  current_selected_cell.style.backgroundColor = 'yellow';
}

function handle_new_cell_selection(new_cell){
    current_selected_cell.style.border = '1px solid black';
    current_selected_cell = new_cell;
    current_selected_cell.style.border = '3px solid black';
}


function add_page_table(){
    var page_body = document.body;
    
    var new_table = document.createElement("table");
    new_table.setAttribute('id', 'my_table_id');
    new_table.style.border = '1px solid black';
    new_table.style.borderCollapse = 'collapse';
    new_table.style.marginLeft = 'auto';
    new_table.style.marginRight = 'auto';
    
    var new_thead = new_table.createTHead();
    var new_tbody = new_table.createTBody();
    var head_tr = new_thead.insertRow(0);
    
    for(var i = 0 ; i < 4 ; i++){
        var new_header_cell = head_tr.insertCell(i);
        new_header_cell.style.border = '1px solid black';
        new_header_cell.textContent = 'Header ' + (i+1);
    }
    
    for(var i = 0 ; i < 3 ; i++){
        var curr_row = new_tbody.insertRow(i);
        for(var j = 0 ; j < 4 ; j++){
            var curr_cell = curr_row.insertCell(j);
            curr_cell.style.border = '1px solid black';
            curr_cell.textContent = (j+1).toString() + ", " + (i+1).toString();
        }
    }
    
    new_table.appendChild(new_thead);
    new_table.appendChild(new_tbody);
    page_body.appendChild(new_table);
}

function add_page_buttons(){
    var page_body = document.body;
    
    var new_table = document.createElement("table");
    new_table.style.marginTop = '20px';
    new_table.style.marginLeft = 'auto';
    new_table.style.marginRight = 'auto';
    var new_tbody = new_table.createTBody();
    
    //Make row 0
    var curr_row = new_tbody.insertRow(0);
    
    //Make row 0 with up arrow
    curr_row.insertCell(0);
    var curr_cell = curr_row.insertCell(1);
    var curr_button = document.createElement('Button');
    curr_button.innerHTML = '&uarr;';
    curr_button.addEventListener("click", up_event_handler);
    curr_cell.appendChild(curr_button);
    curr_row.insertCell(2); //Final cell for 3x3
    
    //Make row 1
    var curr_row = new_tbody.insertRow(1);
    
    //Create right, Mark, and Left Buttons
    var curr_cell = curr_row.insertCell(0);
    var curr_button = document.createElement('Button');
    curr_button.innerHTML = '&rarr;';
    curr_button.addEventListener("click", right_event_handler);
    curr_cell.appendChild(curr_button);
    
    var curr_cell = curr_row.insertCell(0);
    var curr_button = document.createElement('Button');
    curr_button.textContent = 'Mark Cell';
    curr_button.addEventListener("click", mark_event_handler);
    curr_cell.appendChild(curr_button);
    
    var curr_cell = curr_row.insertCell(0);
    var curr_button = document.createElement('Button');
    curr_button.innerHTML = '&larr;';
    curr_button.addEventListener("click", left_event_handler);
    curr_cell.appendChild(curr_button);
    
    //Make row 2
    var curr_row = new_tbody.insertRow(2);
    
    //Add down arrow
    curr_row.insertCell(0);
    var curr_cell = curr_row.insertCell(1);
    var curr_button = document.createElement('Button');
    curr_button.innerHTML = '&darr;';
    curr_button.addEventListener("click", down_event_handler);
    curr_cell.appendChild(curr_button);
    curr_row.insertCell(2); //Final cell for 3x3
    
    
    //Center all buttons in table
    var all_td = new_table.getElementsByTagName('td');
    for(var i = 0 ; i < all_td.length ; i++) {
        all_td[i].style.textAlign = 'center';
    }
    
    //Add nested element and add to page
    new_table.appendChild(new_tbody);
    page_body.appendChild(new_table);
}

function select_cell_1_1(){
    var main_table = document.getElementById('my_table_id');
    current_selected_cell = main_table.children[1].children[0].firstElementChild;
    current_selected_cell.style.border = '3px solid black';
}

add_page_table();
add_page_buttons();
select_cell_1_1();


