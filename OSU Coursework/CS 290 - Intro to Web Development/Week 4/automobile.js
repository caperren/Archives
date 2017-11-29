function Automobile( year, make, model, type ){
    this.year = year; //integer (ex. 2001, 1995)
    this.make = make; //string (ex. Honda, Ford)
    this.model = model; //string (ex. Accord, Focus)
    this.type = type; //string (ex. Pickup, SUV)
    this.logMe = function(print_type){
        if(print_type){
            console.log(this.year + " " + this.make + " " + this.model + " " + this.type);  
        }else{
            console.log(this.year + " " + this.make + " " + this.model);    
        }
        
    }
}

var automobiles = [ 
    new Automobile(1995, "Honda", "Accord", "Sedan"),
    new Automobile(1990, "Ford", "F-150", "Pickup"),
    new Automobile(2000, "GMC", "Tahoe", "SUV"),
    new Automobile(2010, "Toyota", "Tacoma", "Pickup"),
    new Automobile(2005, "Lotus", "Elise", "Roadster"),
    new Automobile(2008, "Subaru", "Outback", "Wagon")
    ];

//Implemented insertion sort algorithm.
//Reversed [j] and [j-1] from traditional algorithm to sort max first
function sortArr( comparator, array ){
    for(var i = 1 ; i < array.length ; i++){
        var j = i;
        while((j > 0) && (comparator(array[j], array[j-1]))){
            var temp = array[j];
            array[j] = array[j-1];
            array[j-1] = temp;
            j--;            
        }
    }
}


function yearComparator(auto1, auto2){
    if(auto1.year > auto2.year){
        return true;
    }else{
        return false;
    }
}


function makeComparator(auto1, auto2){
    if(auto1.make.toLowerCase() < auto2.make.toLowerCase()){
        return true;
    }else{
        return false;
    }
}

function typeComparator(auto1, auto2){
    var type_order = {'roadster': 4, 'pickup': 3, 'suv': 2, 'wagon': 1}
    var auto1_type = type_order[auto1.type.toLowerCase()];
    var auto2_type = type_order[auto2.type.toLowerCase()];
    
    if(typeof(auto1_type) == 'undefined'){ auto1_type = 0; }
    if(typeof(auto2_type) == 'undefined'){ auto2_type = 0; }
    
    if(auto1_type == auto2_type){
        return yearComparator(auto1, auto2);
    }else if(auto1_type > auto2_type){
        return true;
    }else{
        return false;
    }
}


function print_auto_array(input_array, print_type){
    for(index in input_array){
        input_array[index].logMe(print_type);
    }  
}

var print_type = true;

console.log("*****");
console.log("The cars sorted by year are:");
sortArr(yearComparator, automobiles);
print_auto_array(automobiles, print_type);

console.log("\nThe cars sorted by make are:");
sortArr(makeComparator, automobiles);
print_auto_array(automobiles, print_type);

console.log("\nThe cars sorted by type are:");
sortArr(typeComparator, automobiles);
print_auto_array(automobiles, print_type);
console.log("*****");