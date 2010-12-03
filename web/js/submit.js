/*****************************************

	submit.js
	WOMEN ART REVOLUTION
	
	Ted Hayes / 2010
	
*****************************************/

var tagData = [
	"movie",
	"film",
	"experimental",
	"video",
	"feminist",
	"modern",
	"postmodern",
	"art",
	"documentary",
	"video art"
];

var youtubePattern = /^(http:\/\/|www\.|http:\/\/www\.)?youtube.com\/watch\?(?=.*v=\w+)(?:\S+)?$/;
// www.youtube.com/watch?v=PIyQ-f1_bPs

/**** UTILITIES *****************/

function trace(what){
	$('#trace').append(what+"<br />");
}

/**** EVENT HANDLERS ************/

function selectTag(e, ui){
	trace(ui.item.value);
}

function urlChange(){
	var urlVal = $('#url').val();
	if(youtubePattern.test(urlVal)){
		//$('#url-check').show();
		$('#url-check').removeClass('ui-icon-alert').addClass('ui-icon-circle-check');
	} else {
		//$('#url-check').hide();
		$('#url-check').removeClass('ui-icon-circle-check').addClass('ui-icon-alert');
	}
}

function readmoreClick(){
	//trace("readmoreClick: "+this);
	$(this).parent().children('.theme-description').toggle();
}

function submitClick(){
	// check validation
	// submit data to backend
}

/**** INIT **********************/

$(function(){
	$('#tags-input').autoclear();
	$('#tags-input').autocomplete({
		source: tagData,
		select: selectTag
	});
	//trace("enterTag: "+enterTag);
	//$('#tags-input').result(enterTag);
	
	// Fields
	$('.autoclear').autoclear();
	$('input').addClass("ui-corner-all");
	$('#url').keyup(urlChange);
	$('#submit-button').click(submitClick);
	
	// misc
	//$('#url-check').hide();
	
	// theme logic
	$('.theme-description').hide();
	$('.theme-readmore').click(readmoreClick);
});