//初始化加载
$(function(){
	
});

//重启 --提交
function resetDev(){
	$.ajax({
		type:"get",
		url:"/goform/resetDev",
		async:true,
		success:function(data){
			alert("请重新连接设备！");
		}
	});
}
//恢复出厂设置--提交
function sysReset(){
	$.ajax({
		type:"get",
		url:"/goform/sysReset",
		async:true,
		success:function(data){
			alert("请重新启动设备！");
		}
	});
}

//下载配置文件--获取
function DownloadConfig(){
	$.ajax({
		type:"get",
		url:"",
		async:true,
		success:function(data){
			
		}
	});
}

//恢复配置文件--提交
function RestoreConfig(){
	$.ajax({
		type:"post",
		url:"",
		async:true,
		success:function(data){
			
		}
	});
}
