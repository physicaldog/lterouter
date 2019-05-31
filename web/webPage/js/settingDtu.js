//初始化加载
$(function() {
if(getCookie2('-goahead-session-').length<10){
    parent.location.reload()
    }else{
    	checkDtuEnable();
		checkRadio();
	//setInterval('getDtuLog()',3000);
    }
});

function addAttr() {
	//var objArr = new Array("mode","server_ip","server_port","local_port","baudrate","data_bit","parity","stop_bit");
	$('#mode').attr("disabled", "disabled");
	$('#server_ip').attr("disabled", "disabled");
	$('#server_port').attr("disabled", "disabled");
	$('#local_port').attr("disabled", "disabled");
	$('#baudrate').attr("disabled", "disabled");
	$('#data_bit').attr("disabled", "disabled");
	$('#parity').attr("disabled", "disabled");
	$('#stop_bit').attr("disabled", "disabled");

}

function removeAttr() {
	$('#mode').removeAttr("disabled");
	$('#server_ip').removeAttr("disabled");
	$('#server_port').removeAttr("disabled");
	$('#local_port').removeAttr("disabled");
	$('#baudrate').removeAttr("disabled");
	$('#data_bit').removeAttr("disabled");
	$('#parity').removeAttr("disabled");
	$('#stop_bit').removeAttr("disabled");
}

function checkDtuEnable() {
	$.ajax({
		type: "get",
		url: "/goform/checkDtuEnable",
		async: true,
		dataType: 'json',
		data: {},
		success: function(data) {
			if(data.start == "1") {
				$("input:radio[id='enable']").prop('checked', 'true');
				addAttr();
			} else {
				$("input:radio[id='disable']").prop('checked', 'true');
				removeAttr();
			}

			$('#mode').val(data.mode);
			$('#server_ip').val(data.server_ip);
			$('#server_port').val(data.server_port);
			$('#local_port').val(data.local_port);
			$('#baudrate').val(data.baudrate);
			$('#parity').val(data.parity);
			$('#data_bit').val(data.data_bit);
			$('#stop_bit').val(data.stop_bit);

		}
	});
}

function startDtu() {
	$.ajax({
		type: "post",
		url: "/goform/startDtu",
		async: true,
		data: {
			mode: $('#mode').val(),
			server_ip: $('#server_ip').val(),
			server_port: $('#server_port').val(),
			local_port: $('#local_port').val(),
			baudrate: $('#baudrate').val(),
			parity: $('#parity').val(),
			data_bit: $('#data_bit').val(),
			stop_bit: $('#stop_bit').val(),
		},

		success: function(data) {
			alert(data);
		}
	});
}

function cancelDtu() {
	$.ajax({
		type: "get",
		url: "/goform/cancelDtu",
		async: true,
		data: {},
		success: function(data) {
			alert(data);
		}
	});
}

function enableDtu() {
	addAttr();
	startDtu();

}

function disableDtu() {
	removeAttr();
	cancelDtu();
}

function checkRadio() {
	$(":radio").click(function() {
		var id = $(this).attr("id");
		if(id == "enable") {
			if(($('#mode').val() == "2") || ($('#mode').val() == "4")) {
				if(($("input:text[id='server_ip']").val().length == 0) || ($("input:text[id='server_port']").val().length == 0)) {
					alert("参数不能为空");
					$("input:radio[id='disable']").prop("checked", 'true');
				} else {
					enableDtu();
				}
			} else {
				enableDtu();
			}
		} else {
			disableDtu();
		}
	});
}

function getDtuLog() {
	$.ajax({
		type: "get",
		url: "/goform/getDtuLog",
		async: true,
		data: {},
		success: function(data) {
			$('#logText').text(data);
		}
	});
}