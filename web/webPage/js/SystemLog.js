//初始化加载
$(function(){
	if(getCookie2('-goahead-session-').length<10){
    parent.location.reload()
    }
});

//配置网络--提交
function InternetConfig(){
	$.ajax({
		type:"post",
		url:"",
		async:true,
		success:function(data){
			
		}
	});
}

//配置本地--提交
function localConfig(){
	$.ajax({
		type:"post",
		url:"",
		async:true,
		success:function(data){
			
		}
	});
}
//提取本地--提交
function lcoalExtract(){
	$.ajax({
		type:"post",
		url:"",
		async:true,
		success:function(data){
			
		}
	});
	
}
