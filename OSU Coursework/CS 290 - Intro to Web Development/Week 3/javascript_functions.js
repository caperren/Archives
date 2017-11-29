//////////////////////////////////////////////////////////////////////
// Call a function before it's declared
//////////////////////////////////////////////////////////////////////

//Function call and console logging BEFORE function decleration
console.log(simple_string_reverser("Here is some text to reverse!")); //Workss

function simple_string_reverser(input_word) {
    if(typeof(input_word) != 'string'){
        console.log("Error. Wrong input type.");
        return;
    }
    
    var new_simple_string = "";
    var input_length = input_word.length;
    for(var i = 0 ; i < input_word.length ; i++) {
        new_simple_string += input_word[input_length-1-i];
    }
    return new_simple_string;
}

//////////////////////////////////////////////////////////////////////
// Call a function assigned to a variable before it's declared
//////////////////////////////////////////////////////////////////////

 console.log(one_year_ago_date_string()); //Doesn't work

 var one_year_ago_date_string = function(){
    var temp_date = new Date();
    temp_date.setFullYear(temp_date.getFullYear()-1);
    return temp_date.toDateString();
}
console.log(one_year_ago_date_string()); //Works
