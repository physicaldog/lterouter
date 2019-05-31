//初始化加载
$(function() {
	if(getCookie2('-goahead-session-').length<10){
    parent.location.reload()
    }else{
    	checkNetMode();
	//setInterval('getDtuLog()',3000);
    }
});

function checkNetMode() {
	//alert("mode");
	$.ajax({
		type: "get",
		url: "/goform/checkNetMode",
		async: true,
		data: {},
		success: function(data) {
			if(data == "1") {
				$(".selector").val("1");
			} else if(data == "2") {
				$(".selector").val("2");
			} else {
				alert(data)
			}


		}
	});
}

function settingNetMode() {
	if ($('#netmode').val() == "1") {
			set_mode_1();
		} else{
			set_mode_2();
		}
}

function set_mode_1() {
	//alert("mode1");
	$.ajax({
		type: "get",
		url: "/goform/set_mode_1",
		async: true,
		data: {},
		success: function(data) {
			alert(data);
		}
	});
}
function set_mode_2() {
	//alert("mode2");
	$.ajax({
		type: "get",
		url: "/goform/set_mode_2",
		async: true,
		data: {},
		success: function(data) {
			alert(data)
		}
	});
}
function set_snat() {
	//alert("set_snat");
	$.ajax({
		type: "get",
		url: "/goform/set_snat",
		async: true,
		data: {
			source_ip: $('#source_ip').val(),
			source_port: $('#source_port').val(),
			snat_ip: $('#snat_ip').val(),
			snat_port: $('#snat_port').val(),
			snat_opt: $('#snat_opt').val()
		},
		success: function(data) {
			alert(data)
		}
	});
}

function set_dnat() {
	//alert("set_dnat");
	$.ajax({
		type: "get",
		url: "/goform/set_dnat",
		async: true,
		data: {
			destination_ip: $('#destination_ip').val(),
			destination_port: $('#destination_port').val(),
			dnat_ip: $('#dnat_ip').val(),
			dnat_port: $('#dnat_port').val(),
			dnat_opt: $('#dnat_opt').val()
		},
		success: function(data) {
			alert(data);
		}
	});
}
function settingSNAT() {
	//alert("settingSNAT");
	if (($("input:text[id='source_ip']").val().length == 0) 
	 || ($("input:text[id='snat_ip']").val().length == 0)) {
		alert("请输入IP地址")
	} else{
		if ((($("input:text[id='source_port']").val().length != 0) 
	 || ($("input:text[id='snat_port']").val().length != 0)) && ($('#snat_opt').val() == "1")) {
			alert("请选择相应的协议")
			//alert(($('#mode').val())
		} else{
			set_snat();
		}
	}
}
function settingDNAT() {
	//alert("settingDNAT");
	if (($("input:text[id='destination_ip']").val().length == 0) 
	 || ($("input:text[id='dnat_ip']").val().length == 0)) {
		alert("请输入IP地址")
	} else{
		if ((($("input:text[id='destination_port']").val().length != 0) 
	 || ($("input:text[id='dnat_port']").val().length != 0)) && ($('#dnat_opt').val() == "1")) {
			alert("请选择相应的协议")
			//alert(($('#mode').val())
		} else{
			set_dnat();
			
		}
	}

}


function query_nat() {
	//alert("querynat")
	$.ajax({
		type: "get",
		url: "/goform/query_nat",
		async: true,
		data: {},
		success: function(data) {
			$('#logText').text(data);
		}
	});
}

function clear_nat() {
	//alert("clearnat")
	$.ajax({
		type: "get",
		url: "/goform/clear_nat",
		async: true,
		data: {},
		success: function(data) {
			alert(data);
		}
	});
}