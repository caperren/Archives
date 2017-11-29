//Base code copied with permission from https://github.com/wolfordj/CS290-Server-Side-Examples
var express = require('express');
var app = express();
var handlebars = require('express-handlebars').create({defaultLayout:'main'});
var bodyParser = require('body-parser');

app.use(bodyParser.urlencoded({ extended: false }));
app.use(bodyParser.json());

app.engine('handlebars', handlebars.engine);
app.set('view engine', 'handlebars');
app.set('port', 3000);

function get_data_array(data){
	params = [];
	
	for(par in data){
		params.push({'name':par, 'value':data[par]});
	}
	
	return params;
}

function generateContext_GET(req){
	var dynamic_page_contents = {};
	dynamic_page_contents.StatusHeader = req.method + " Request Received";
	dynamic_page_contents.query_data = get_data_array(req.query);
	
	return dynamic_page_contents;
}

function generateContext_POST(req){
	var dynamic_page_contents = {};
	dynamic_page_contents.StatusHeader = req.method + " Request Received";
	
	dynamic_page_contents.query_data = get_data_array(req.query);	
	dynamic_page_contents.body_data = get_data_array(req.body);
	
	return dynamic_page_contents;
}

function my_get(req,res){
  res.render('week7_GET', generateContext_GET(req));
}

app.get('/', my_get);

app.post('/', function(req,res){
  res.render('week7_POST', generateContext_POST(req));
});

app.use(function(req,res){
  res.status(404);
  res.render('404');
});

app.use(function(err, req, res, next){
  console.error(err.stack);
  res.type('plain/text');
  res.status(500);
  res.render('500');
});

function on_listen(){
  console.log('Express started on http://localhost:' + app.get('port') + '; press Ctrl-C to terminate.');
}

app.listen(app.get('port'), on_listen);

