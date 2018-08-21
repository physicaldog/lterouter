//初始化加载
$(function(){
	checkRadio();
	cleanLog();
	//setInterval('getPingLog()',1000);
	
});


function start_sshd(arg) {
	$.ajax({
		type: "get",
		url: "/goform/start_sshd",
		async: true,
		data:{
			arg:arg,
		},
		success: function(data) {
			alert(data);
		}
	});
}

function stop_sshd() {231
	$.ajax({
		type: "get",
		url: "/gofrom/stop_sshd",
		async: true,
		success: function(data) {
			alert(data);
		}
	});
}

function checkRadio() {
	$(":radio").click(function() {
		var id = $(this).attr("id");
		if(id == "start_sshd") {
			start_sshd("start");
		} else {
			start_sshd("stop");
		}
	});
}

function cleanLog(){
	$.ajax({
		type:"get",
		url:"/goform/cleanLog",
		async:true,
		
		data:{
			/*pingAddr:$('#pingAddr').val(),
			pingCount:$('#pingCount').val(),*/
		},
		success:function(data){
			//$('#logText').text(data);
		}
	});
}

function getPingLog(){
	$.ajax({
		type:"get",
		url:"/goform/getPingLog",
		async:true,
		
		data:{
			/*pingAddr:$('#pingAddr').val(),
			pingCount:$('#pingCount').val(),*/
		},
		success:function(data){
			$('#logText').text(data);
		}
	});
}


//PING--提交
function pingTest(){
	var id = setInterval('getPingLog()',1000);
	$.ajax({
		type:"get",
		url:"/goform/pingTest",
		async:true,
		data:{
			pingAddr:$('#pingAddr').val(),
			pingCount:$('#pingCount').val(),
		},
		success:function(data){
		}
	});
}

function pingcancel(){
	pingAddr:$('#pingAddr').val("");
	pingCount:$('#pingCount').val("");
}
