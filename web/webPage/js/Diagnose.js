
//初始化加载
$(function(){
    if(getCookie2('-goahead-session-').length<10){
    parent.location.reload()
    }else{
    	check_LongPing();
	checkRadio();
	cleanLog();
	//stop_LongPing();
	//start_LongPing();
	
	//setInterval('getPingLog()',1000);
    }
});


function start_LongPing() {
	alert("start")
	if(($("input:text[id='IpAddr']").val().length == 0)){
		alert("参数不能为空");
		$("input:radio[id='stop_LongPing']").prop('checked', 'true');
	}else{
		$.ajax({
			type: "post",
			url: "/goform/start_LongPing",
			async: true,
			data:{
				IpAddr:$('#IpAddr').val(),
			},
			success: function(data) {
				alert(data);
			}
		});
	}
}

function stop_LongPing() {
	//alert("stop")
	/*if(($("input:text[id='IpAddr']").val().length == 0)){
		alert("参数不能为空");
		$("input:radio[id='stop_LongPing']").prop('checked', 'true');
	}else{*/
		$.ajax({
			type: "post",
			url: "/goform/stop_LongPing",
			async: true,
			data:{
				//IpAddr:$('#IpAddr').val(),
			},
			success: function(data) {
				alert(data);
			}
		});
	//}
}

function check_LongPing() {
	//alert("check")
/*	if(($("input:text[id='IpAddr']").val().length == 0)){
		alert("参数不能为空");
		$("input:radio[id='stop_LongPing']").prop('checked', 'true');
	}else{*/
		$.ajax({
			type: "get",
			url: "/goform/check_LongPing",
			async: true,
			data:{
				//IpAddr:$('#IpAddr').val(),
			},
			success: function(data) {
				//alert(data);
				if (data.length <= 4) {
					//alert("monkey");
					$("input:radio[id='stop_LongPing']").prop('checked', 'true');
					
				} else{
					$("input:radio[id='start_LongPing']").prop('checked', 'true');
					$('#IpAddr').val(data);
				}
			}
		});
	//}
}

/*function stop_LongPing() {
	alert("stop")
	$.ajax({
		type: "get",
		url: "/gofrom/stop_LongPing",
		async: true,
		success: function(data) {
			alert(data);
		}
	});
}


*/
function checkRadio() {
	$(":radio").click(function() {
		var id = $(this).attr("id");
		if(id == "start_LongPing") {
			start_LongPing();
		} else {
			stop_LongPing();
		}
	});
}



/*function start_sshd(arg) {
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
}*/

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
