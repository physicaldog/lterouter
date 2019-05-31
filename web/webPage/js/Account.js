//初始化加载
$(function(){
    if(getCookie2('-goahead-session-').length<10){
    parent.location.reload()
    }
});


//修改密码--提交
function settingPW(){
	$.ajax({
		type:"get",
		url:"/goform/settingPW",
		async:true,
		//dateType:'json',
		data:{
			//userName:$('#userName').val(),
			currPW:$('#currPW').val(),
			newPW:$('#newPW').val()
		},
		success:function(data){
			alert(data); 
		}
	});
}

//配置 --提交
function cancelPW(){
	currPW:$('#currPW').val("");
	newPW:$('#newPW').val("");
}
