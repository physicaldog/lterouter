//初始化加载
$(function(){
	if(getCookie2('-goahead-session-').length<10){
    parent.location.reload()
    }
});

//模块信息--获取
function ModuleInfo(){
	$.ajax({
		type:"get",
		url:"",
		async:true,
		success:function(data){
			
		}
	});
}

//模块升级--提交
function ModuleUpdate(){
	$.ajax({
		type:"post",
		url:"",
		async:true,
		success:function(data){
			
		}
	});
	
}
