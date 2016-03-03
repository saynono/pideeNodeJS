

const pidee = require('../build/Release/pidee');

console.log( pidee.say() );

console.log("DIP Switch State => " + pidee.getDip() );

for( var i=0;i<10;i++ ){
	console.log("DIP " + i + " => " + pidee.getDip(i) );
}

