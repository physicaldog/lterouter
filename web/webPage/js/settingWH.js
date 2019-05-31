//初始化加载
$(function() {
	if(getCookie2('-goahead-session-').length<10){
    parent.location.reload()
    }else{
    	getRebootTime();
	getRebootCount();
	//checkManualTime();
	checkRadio();
    }
	
});


function settingAddr() {
	if(($("input:text[id='city_name']").val().length == 0) && ($("input:text[id='county_name']").val().length == 0) && ($("input:text[id='location_name']").val().length == 0)) {
		alert("参数不能为空");
	} else {
		$.ajax({
			type: "get",
			url: "/goform/settingAddr",
			async: true,
			data: {
				city_name: $('#city_name').val(),
				county_name: $('#county_name').val(),
				location_name: $('#location_name').val(),
			},
			success: function(data) {
				alert(data);
			}
		});
	}
}

function queryAddr() {
	$.ajax({
		type: "get",
		url: "/goform/queryAddr",
		async: true,
		dataType: 'json',
		success: function(data) {
			$('#city_name').val(data.city_name);
			$('#county_name').val(data.county_name);
			$('#location_name').val(data.location_name);
		}
	});
}

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

function getRebootTime() {
	$.ajax({
		type: "get",
		url: "/goform/getRebootTime",
		async: true,
		success: function(data) {
			$('#RebootTime').val(data);
		}
	});
}

function setRebootTime() {
	var time = Number($('#RebootTime').val());
	if (isNaN(time)) {
		alert("fail");
		return;
	} 
	if ( 0 == time ) {
	} else if (30 >= time) {
		$('#RebootTime').val("30");
	} 

	$.ajax({
			type: "get",
			url: "/goform/setRebootTime",
			async: true,
			data: {
				time: $('#RebootTime').val(),
			},
			success: function(data) {
				alert(data);
			}
		});
	
}

function queryTime() {
	$.ajax({
		type: "get",
		url: "/goform/queryTime",
		async: true,
		dataType: 'json',
		success: function(data) {
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

function getRebootCount() {
	$.ajax({
		type: "get",
		url: "/goform/getRebootCount",
		async: true,
		success: function(data) {
			$('#rebootCount').text(data);
		}
	});
}

function resetDev() {
	$.ajax({
		type: "get",
		url: "/goform/resetDev",
		async: true,
		success: function(data) {
			window.top.restart();
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
			window.top.Restoration(); 
		}
	});
}