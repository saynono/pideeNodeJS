

const pidee = require('../build/Debug/pidee');

console.log( pidee.say() );

console.log("DIP Switch State => " + pidee.getDip() );


pidee.onDipSwitchChange( 
	function(){ console.log("onDipSwitchChange : " + arguments[0] + "     " + arguments[1] );}
);



for( var i=0;i<10;i++ ){
	console.log("DIP " + i + " => " + pidee.getDip(i) );
}

