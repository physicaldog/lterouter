//初始化加载
$(function(){
	if(getCookie2('-goahead-session-').length<10){
   		parent.location.reload()
    }else{
    	checkAcsEnable();
		checkRadio();
    }
});

//IMSI--查询
function queryIMSI(){
	$.ajax({
		type:"get",
		url:"/goform/queryIMSI",
		async:true,
		success:function(data){
			//console.log("fanhuizhi"+data)
			apn:$('#IMSI').val(data)
			
		}
	});
}
//APN设置--提交
function SettingAPN(){
	$.ajax({
		type:"get",
		url:"/goform/settingAPN",
		async:true,
		data:{
			apn:$('#apn').val(),
		},
		success:function(data){
			//console.log("fanhuizhi"+data)
			alert(data)
		}
	});
}

//APN设置--提交
function QueryAPN(){
	$.ajax({
		type:"get",
		url:"/goform/queryAPN",
		async:true,
		success:function(data){
			//console.log("fanhuizhi"+data)
			apn:$('#apn').val(data)
			
		}
	});
}

function SettingAcs_addr(){
	$.ajax({
		type:"get",
		url:"/goform/settingAcs_url",
		async:true,
		data:{
			acs_addr:$('#acs_addr').val(),
		},
		success:function(data){
			//console.log("fanhuizhi"+data)
			alert(data)
		}
	});
}

//APN设置--提交
function QueryAcs_addr(){
	$.ajax({
		type:"get",
		url:"/goform/queryAcs_url",
		async:true,
		data: {},
		success:function(data){
			//console.log("fanhuizhi"+data)
		$('#acs_addr').val(data)
			
		}
	});
}

function checkAcsEnable() {
	$.ajax({
		type: "get",
		url: "/goform/checkAcsEnable",
		async: true,
		success: function(data) {
			//alert(data);
			if(data == "true") {
				$("input:radio[id='acs_enable']").prop('checked', 'true');
				$('#set').attr("disabled", "disabled");			
			} else {
				$("input:radio[id='acs_disable']").prop('checked', 'true');
				$('#set').removeAttr("disabled");
			}
		}
	});
}

function checkRadio() {
	$(":radio").click(function() {
		var id = $(this).attr("id");
		if(id == "acs_enable") {
			$("input:radio[id='acs_enable']").prop("checked", 'true');
			acs_enable();
		} else {
			$("input:radio[id='acs_disable']").prop("checked", 'true');
			acs_disable();
		}
	});
}

function acs_enable() {
	$.ajax({
		type: "post",
		url: "/goform/acs_enable",
		async: true,
		data: {
		},
		success: function(data) {
			alert(data);
		}
	});
}

function acs_disable() {
	$.ajax({
		type: "post",
		url: "/goform/acs_disable",
		async: true,
		data: {
		},
		success: function(data) {
			alert(data);
		}
	});
}