/**
 * Created by Corwin Perren on 7/21/2017.
 */

function random_string(len) {
    var char_set = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789';
    var random_string = '';
    for (var i = 0; i < len; i++) {
        var random_position = Math.floor(Math.random() * char_set.length);
        random_string += char_set.substring(random_position, random_position+1);
    }
    return random_string;
}

function on_auth_button_clicked(){
    var SHA256 = new Hashes.SHA256();
    var rand_state = SHA256.hex(random_string(1024));
    window.location.replace('/lauth?state=' + rand_state);
}

function on_view_gaccount_button_clicked() {
    window.location.replace('/gaccount');
}

function assign_window_event_handlers() {
    document.getElementById("auth_button").addEventListener("click", on_auth_button_clicked)
    document.getElementById("view_gaccount_button").addEventListener("click", on_view_gaccount_button_clicked)
}

function setup_page_on_load() {
    assign_window_event_handlers();
}

window.addEventListener("load", setup_page_on_load);