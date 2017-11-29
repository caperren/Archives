function deepEqual(a, b){
    if(((typeof(a) === 'object') && (a !== null)) && 
       ((typeof(b) === 'object') && (b !== null))){
        
        var size_a = 0;
        var size_b = 0;
        
        for(prop in a){ size_a++; }
        
        for(prop in b){ size_b++; }
        
        if(size_a == size_b){
            for(prop in a){
                if(!b.hasOwnProperty(prop)){
                    return false;
                }
            }
            
            for(prop in a){
                if(!deepEqual(a[prop], b[prop])){
                    return false;
                }
            }
            
        }else{
            return false;
        }
        
    }else{
        return (a === b);
    }
    
    return true;
}

var obj = {here: {is: "an"}, object: 2};
console.log(deepEqual(obj, obj));

console.log(deepEqual(obj, {here: 1, asd: 2}));

console.log(deepEqual(obj, {here: {is: "an"}, object: 2}));

//console.log(deepEqual(1, 2));

//console.log(deepEqual(2, 2));