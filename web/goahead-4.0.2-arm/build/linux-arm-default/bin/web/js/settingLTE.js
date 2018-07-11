//初始化加载
$(function(){
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
			console.log("fanhuizhi"+data)
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
			console.log("fanhuizhi"+data)
			apn:$('#apn').val(data)
			
		}
	});
}