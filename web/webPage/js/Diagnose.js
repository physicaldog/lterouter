//初始化加载
$(function(){
	cleanLog()
	setInterval('getPingLog()',1000);
	
});

function cleanLog(){
	$.ajax({
		type:"get",
		url:"/goform/cleanLog",
		async:true,
		
		data:{
			/*pingAddr:$('#pingAddr').val(),
			pingCount:$('#pingCount').val(),*/
		},
		success:function(data){
			//$('#logText').text(data);
		}
	});
}

function getPingLog(){
	$.ajax({
		type:"get",
		url:"/goform/getPingLog",
		async:true,
		
		data:{
			/*pingAddr:$('#pingAddr').val(),
			pingCount:$('#pingCount').val(),*/
		},
		success:function(data){
			$('#logText').text(data);
		}
	});
}


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
			//alert(data);
		}
	});
}

function pingcancel(){
	pingAddr:$('#pingAddr').val("");
	pingCount:$('#pingCount').val("");
}
