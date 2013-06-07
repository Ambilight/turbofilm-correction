var tfc = require ('../index');
var dict = require ('./small.dict');
var assert = require ('assert');
var test = [];

for (var i=0; i<dict.length; i++) {
	tfc.add (dict[i]);
	var j = Math.round(Math.random() * dict[i].length);
	var w = dict[i].slice(0,j)+String.fromCharCode(Math.round((Math.random()*25)+97))+dict[i].slice(j+1);
	var j2 = Math.round(Math.random() * dict[i].length);
	var w2 = dict[i].slice(0,j2)+String.fromCharCode(Math.round((Math.random()*25)+97))+dict[j2].slice(j2+1);
	test[test.length] = [dict[i], w];
	test[test.length] = [dict[i], w2];
};

var total = 0;
var result = [];
for (var i=0; i<test.length; i++) {
	var now = Date.now ();
	var query = tfc.query (test[i][0]);
	total+= Date.now()-now;
	result[result.length] = [test[i][0], test[i][1], query[0]];
}
console.log ("%s%s", total/test.length, 'ms');

for (var i=0; i<result.length; i++) {
	assert.equal (result[i][0], result[i][2]);
}