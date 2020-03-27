//初始化加载
$(function() {
	if(getCookie2('-goahead-session-').length<10){
    parent.location.reload()
    }else{
    	checkSecurityEnable();
    	checkdataforwardEnable();
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
			$('#mac_list').val(data.mac_list);
			if(data.start == "1") {
				$("input:radio[id='enable_1']").prop('checked', 'true');
				$('#enable_2').attr("disabled", "disabled");
				$('#disable_2').attr("disabled", "disabled");
				addAttr();
			} else if(data.start == "2"){
				$("input:radio[id='enable_2']").prop('checked', 'true');
				$('#enable_1').attr("disabled", "disabled");
				$('#disable_1').attr("disabled", "disabled");
				addAttr();
			} else {
				//$("input:radio[id='disable_1']").prop('checked', 'true');
				$("input:radio[id='disable_2']").prop('checked', 'true');
				$('#enable_1').removeAttr("disabled");
				$('#disable_1').removeAttr("disabled");
				$('#enable_2').removeAttr("disabled");
				
				removeAttr();
			}
		}
	});
}

function checkdataforwardEnable() {
	$.ajax({
		type: "get",
		url: "/goform/checkdataforwardEnable",
		async: true,
		dataType: 'json',
		data: {},
		success: function(data) {
			
			if(data.status == "1") {
				$("input:radio[id='enable_data_forward']").prop('checked', 'true');
				$('#set_data_forward').attr("disabled", "disabled");
				$('#clear_data_forward').attr("disabled", "disabled");
			} else {
				$("input:radio[id='disable_data_forward']").prop('checked', 'true');
				$('#set_data_forward').removeAttr("disabled");
				$('#clear_data_forward').removeAttr("disabled");

			}
		}
	});
}

function addAttr() {
	$('#gw_ip').attr("disabled", "disabled");
	$('#gw_port').attr("disabled", "disabled");
	$('#mac_list').attr("disabled", "disabled");
}

function removeAttr() {
	$('#gw_ip').removeAttr("disabled");
	$('#gw_port').removeAttr("disabled");
	$('#mac_list').removeAttr("disabled");
}

function startSecurity(vpn) {
	//alert(vpn);
	$.ajax({
		type: "get",
		url: "/goform/startSecurity",
		async: true,
		data: {
			vpn:vpn,
			gw_ip: $('#gw_ip').val(),
			gw_port: $('#gw_port').val(),
			mac_list: $('#mac_list').val(),
		},
		success: function(data) {
			alert(data);
		}
	});
}

function cancelSecurity(vpn) {
	$.ajax({
		type: "get",
		url: "/goform/cancelSecurity",
		async: true,
		data: {
			vpn:vpn
		},
		success: function(data) {
			alert(data);
		}
	});
}

function enableSecurity(vpn) {
	addAttr();
	startSecurity(vpn);
}

function disableSecurity(vpn) {
	removeAttr();
	cancelSecurity(vpn);
}

function checkRadio() {
	$(":radio").click(function() {
		var id = $(this).attr("id");
		if((id == "enable_1")) {
			if(($("input:text[id='gw_ip']").val().length == 0) || ($("input:text[id='gw_port']").val().length == 0)) {
				alert("参数不能为空");
				$("input:radio[id='disable_1']").prop('checked', 'true');
			} else {
				enableSecurity(1);
				$('#enable_2').attr("disabled", "disabled");
				$('#disable_2').attr("disabled", "disabled");

			}
		} else if((id == "disable_1")) {
			disableSecurity(1);
			$('#enable_2').removeAttr("disabled");
			$('#disable_2').removeAttr("disabled");
		} else if((id == "enable_2")) {
			if(($("input:text[id='gw_ip']").val().length == 0) || ($("input:text[id='gw_port']").val().length == 0)) {
				alert("参数不能为空");
				$("input:radio[id='disable_2']").prop('checked', 'true');
			} else {
				enableSecurity(2);
				$('#enable_1').attr("disabled", "disabled");
				$('#disable_1').attr("disabled", "disabled");
			}
		} else if((id == "disable_2")) {
			disableSecurity(2);
			$('#enable_1').removeAttr("disabled");
			$('#disable_1').removeAttr("disabled");
		} else if ((id == "enable_data_forward")) {
			data_forward(1);
			$('#set_data_forward').attr("disabled", "disabled");
			$('#clear_data_forward').attr("disabled", "disabled");
		} else if (id == "disable_data_forward"){
			data_forward(0);
			$('#set_data_forward').removeAttr("disabled");
			$('#clear_data_forward').removeAttr("disabled");
		}
	});
}

function data_forward(status) {
	$.ajax({
		type: "get",
		url: "/goform/data_forward",
		async: true,
		data: {
			status:status,
		},
		success: function(data) {
			alert(data);
		}
	});
}

function set_data_forward() {
	if(($("input:text[id='data_forward_local_port']").val().length == 0) || ($("input:text[id='data_forward_protocol']").val().length == 0) || ($("input:text[id='data_forward_remote_ip']").val().length == 0) || ($("input:text[id='data_forward_remote_port']").val().length == 0)) {
		alert("参数不能为空");
	} else{
		$.ajax({
			type: "get",
			url: "/goform/set_data_forward",
			async: true,
			data: {
				data_forward_local_port: $('#data_forward_local_port').val(),
				data_forward_protocol: $('#data_forward_protocol').val(),
				data_forward_remote_ip: $('#data_forward_remote_ip').val(),
				data_forward_remote_port: $('#data_forward_remote_port').val(),
			
			},
			success: function(data) {
				alert(data);
			}
		});
	}
}

function query_data_forward() {
	$.ajax({
		type: "get",
		url: "/goform/query_data_forward",
		async: true,
		data: {
		},
		success: function(data) {
			alert(data);
		}
	});
}

function clear_data_forward() {
	$.ajax({
		type: "get",
		url: "/goform/clear_data_forward",
		async: true,
		data: {
		},
		success: function(data) {
			alert(data);
		}
	});
}

function getSecurityLog() {
	var radios = document.getElementsByName("switch");
/*	for(var i=0;i<radios.length;i++)
    { 
        //判断那个单选按钮为选中状态
        if(radios[i].checked)
        {
            //弹出选中单选按钮的值
            alert(radios[i].value);
        } 
    } */
	if (radios[0].checked){
		$.ajax({
			type: "get",
			url: "/goform/getSecurityLog",
			async: true,
			data: {},
			success: function(data) {
				$('#logText').text(data);
			}
		});
	} /*else {
		alert("no vpn1");
	}
	*/
}