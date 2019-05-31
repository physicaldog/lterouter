//初始化加载
$(function(){
	if(getCookie2('-goahead-session-').length<10){
    parent.location.reload()
    }
});

//FTP配置--提交
function FTPConfig(){
	$.ajax({
		type:"post",
		url:"",
		async:true,
		success:function(data){
			
		}
	});
}
