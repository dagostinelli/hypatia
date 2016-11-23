$(document).ready(function(){
	$('.nav a,.footer a.up').click(function(e){
		$.scrollTo( this.hash || 0, 1500);
		e.preventDefault();
	});
});