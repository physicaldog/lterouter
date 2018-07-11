//初始化加载
$(function(){
	
});

//PING--提交
function pingTest(){
	$.ajax({
		type:"get",
		url:"/goform/pingTest",
		async:true,
		data:{
			pingAddr:$('#pingAddr').val(),
			pingCount:$('#pingCount').val(),
		},
		success:function(data){
			$('#pingText').text(data);
		}
	});
}

function pingcancel(){
	pingAddr:$('#pingAddr').val("");
	pingCount:$('#pingCount').val("");
}
