var openweather_api_string = '&APPID=4f5bcff87da17d71fc4f89eb44c1a8da';
var openweather_url = 'http://api.openweathermap.org/data/2.5/weather?';

function on_openweather_submit_pressed(event){
    
    var search = document.getElementById("open_weather_input").value;
    
    if(isNaN(parseInt(search))){
        var payload = encodeURI("q=" + search);
    }else{
        var payload = encodeURI("zip=" + search + ",us") 
    }
    
    var req = new XMLHttpRequest();
    var full_url = openweather_url + payload + openweather_api_string;
    
    req.open('GET', full_url, true);
    req.addEventListener('load', on_openweather_response_received);
    req.send(null);
    
    console.log(full_url);
        
    event.preventDefault();
}

function on_openweather_response_received(response){
    var location = document.getElementById("location");
    var conditions = document.getElementById("current_conditions");
    var temp = document.getElementById("current_temp");
    var pressure = document.getElementById("current_pressure");
    var humidity = document.getElementById("current_humidity");
    var rainfall = document.getElementById("current_recent_rainfall");
    var details = document.getElementById("request_details");
    
    var text_box = document.getElementById("open_weather_input");
    text_box.value = "";
    
    var req_status = response.srcElement.status;
    console.log(response);
    
    if(req_status >=200 && req_status < 400){
        var response_json = JSON.parse(response.srcElement.responseText);
        
        if(response_json.cod == 404){
            location.textContent = "";
            conditions.textContent = "";
            temp.textContent = "";
            pressure.textContent = "";
            humidity.textContent = "";
            rainfall.textContent = "";
            details.textContent = "City Not Found!";   
        }else{
        
            location.textContent = response_json.name;
            conditions.textContent = response_json.weather[0].main;
            temp.textContent = (parseFloat(response_json.main.temp) - 273.15).toFixed(2).toString() + " °C";
            pressure.textContent = response_json.main.pressure + " hPa";
            humidity.textContent = response_json.main.humidity + "%";

            var rainfall_data = response_json.rain;
            if(typeof(rainfall_data) == 'undefined'){
                rainfall.textContent = "";
            }else{
              rainfall.textContent = response_json.rain['3h'].toFixed(1) + "mm in the last 3 hours";  
            }

            details.textContent = "City Found Sucessfully!";
        }
    }else{
        location.textContent = "";
        conditions.textContent = "";
        temp.textContent = "";
        pressure.textContent = "";
        humidity.textContent = "";
        rainfall.textContent = "";
        details.textContent = "Failed communications with Open Weather Server!";
    }
}

function on_httpbin_submit_pressed(event){
    var text_input = document.getElementById("httpbin_input").value;
    
    var req = new XMLHttpRequest();
    var payload = {'mydata': text_input};
    req.open('POST', "http://httpbin.org/post", true);
    req.setRequestHeader('Content-Type', 'application/json');
    req.addEventListener('load', on_httpbin_response_receieved);
    req.send(JSON.stringify(payload));
    
    event.preventDefault();
}

function on_httpbin_response_receieved(response){
    var response_span_json = document.getElementById("httpbin_reponse_json");
    var response_span_data = document.getElementById("httpbin_reponse");
    
    var parsed_json = JSON.parse(response.srcElement.responseText);
    
    response_span_json.textContent = parsed_json.data;
    response_span_data.textContent = parsed_json.json.mydata;
}

function setup_events(){
    document.getElementById("weather_submit_button").addEventListener("click", on_openweather_submit_pressed);
    document.getElementById("httpbin_submit_button").addEventListener("click", on_httpbin_submit_pressed);
}

document.addEventListener('DOMContentLoaded', setup_events);