//初始化加载
$(function(){
	
});

//LAN主机配置 --提交
function settingLAN(){
	$.ajax({
		type:"get",
		url:"/goform/settingLAN",
		async:true,
		data:{
			lanip:$('#lanip').val(),
			netmask:$('#netmask').val(),
		},
		
		success:function(data){ 
			alert(data)
		}
	});
}

//LAN主机配置 --查询
function queryLAN(){
	$.ajax({
		type:"get",
		url:"/goform/queryLAN",
		async:true,
		dataType:'json',
		success:function(data){
			$('#lanip').val(data.lanip);
			$('#netmask').val(data.netmask)
		}
	});
}

//DHCP配置--提交
function DHCPConfig(){
	$.ajax({
		type:"post",
		url:"",
		async:true,
		success:function(data){
			
		}
	});
}

//绑定地址列表--获取
function DeviceList(){
	$.ajax({
		type:"get",
		url:"",
		async:true,
		success:function(data){
			
		}
	});
}

//添加列表--提交
function AddDevice(){
	$.ajax({
		type:"post",
		url:"",
		async:true,
		success:function(data){
			
		}
	});
}
