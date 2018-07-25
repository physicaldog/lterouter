//初始化加载
$(function() {
	checkManualTime();
	checkRadio();
});

function settingTime() {
	if($("input:text[id='system_time']").val().length == 0) {
		alert("参数不能为空");
	} else {
		//alert("2");
		$.ajax({
			type: "get",
			url: "/goform/settingTime",
			async: true,
			data: {
				system_time: $('#system_time').val(),
			},
			success: function(data) {
				alert(data);

			}
		});
	}
}

function queryTime() {
	$.ajax({
		type: "get",
		url: "/goform/queryTime",
		async: true,
		dataType: 'json',
		success: function(data) {
			//alert(data.system_time);
			$('#system_time').val(data.system_time);

		}
	});
}

function addAttr() {

	$('#system_time').attr("disabled", "disabled");
	$('#setTime').attr("disabled", "disabled");
}

function removeAttr() {
	$('#system_time').removeAttr("disabled");
	$('#setTime').removeAttr("disabled");
}

function checkManualTime() {
	queryTime();
	$.ajax({
		type: "get",
		url: "/goform/checkManualTime",
		async: true,
		dataType: 'json',
		success: function(data) {
			//alert(data.value);
			if(data.value == "1") {
				$("input:radio[id='manual']").prop('checked', 'true');
				removeAttr();
			} else {
				$("input:radio[id='auto']").prop('checked', 'true');
				addAttr();
			}

		}
	});
}

function setManualTime() {
	$.ajax({
		type: "get",
		url: "/goform/setManualTime",
		async: true,
		success: function(data) {
			//alert(data);
		}
	});
}

function setAutoTime() {
	$.ajax({
		type: "get",
		url: "/goform/setAutoTime",
		async: true,
		success: function(data) {
			alert(data);
		}
	});
}

function checkRadio() {
	$(":radio").click(function() {
		var id = $(this).attr("id");
		if(id == "manual") {
			setManualTime();
			removeAttr();
		} else {
			setAutoTime();
			addAttr();
		}
	});
}

//重启 --提交
function resetDev() {
	$.ajax({
		type: "get",
		url: "/goform/resetDev",
		async: true,
		success: function(data) {
			alert(data);
		}
	});
}
//恢复出厂设置--提交
function sysReset() {
	$.ajax({
		type: "get",
		url: "/goform/sysReset",
		async: true,
		success: function(data) {
			alert(data);
		}
	});
}