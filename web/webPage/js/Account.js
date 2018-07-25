//初始化加载
$(function(){
	
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
