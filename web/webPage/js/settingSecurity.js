//初始化加载
$(function() {
	if(getCookie2('-goahead-session-').length<10){
    parent.location.reload()
    }else{
    	checkSecurityEnable();
	checkRadio();
	setInterval("getSecurityLog()", 3000);
    }
});

function checkSecurityEnable() {
	$.ajax({
		type: "get",
		url: "/goform/checkSecurityEnable",
		async: true,
		dataType: 'json',
		data: {},
		success: function(data) {
			$('#gw_ip').val(data.gw_ip);
			$('#gw_port').val(data.gw_port);
			if(data.start == "1") {
				$("input:radio[id='enable']").prop('checked', 'true');
				addAttr();
			} else {
				$("input:radio[id='disable']").prop('checked', 'true');
				removeAttr();
			}
		}
	});
}

function addAttr() {
	$('#gw_ip').attr("disabled", "disabled");
	$('#gw_port').attr("disabled", "disabled");
}

function removeAttr() {
	$('#gw_ip').removeAttr("disabled");
	$('#gw_port').removeAttr("disabled");
}

function startSecurity() {

	$.ajax({
		type: "get",
		url: "/goform/startSecurity",
		async: true,
		data: {
			gw_ip: $('#gw_ip').val(),
			gw_port: $('#gw_port').val(),
		},
		success: function(data) {
			alert(data);
		}
	});
}

function cancelSecurity() {
	$.ajax({
		type: "get",
		url: "/goform/cancelSecurity",
		async: true,
		success: function(data) {
			alert(data);
		}
	});
}

function enableSecurity() {
	addAttr();
	startSecurity();
}

function disableSecurity() {
	removeAttr();
	cancelSecurity();
}

function checkRadio() {
	$(":radio").click(function() {
		var id = $(this).attr("id");
		if(id == "enable") {
			if(($("input:text[id='gw_ip']").val().length == 0) || ($("input:text[id='gw_port']").val().length == 0)) {
				alert("参数不能为空");
				$("input:radio[id='disable']").prop('checked', 'true');
			} else {
				enableSecurity();
			}
		} else {
			disableSecurity();
		}
	});
}

function getSecurityLog() {
	$.ajax({
		type: "get",
		url: "/goform/getSecurityLog",
		async: true,
		data: {},
		success: function(data) {
			$('#logText').text(data);
		}
	});
}