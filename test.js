const native = require('./index.js');

var test = new native();

test.on('loop', function(counter){
	console.log("loop " + counter);
});

test.start();

setTimeout(function(){
	test.stop()
}, 5000);