//初始化加载
$(function(){
	VersionInfo();
});


//版本信息--获取
function VersionInfo(){
	//alert(1);
	$.ajax({
		type:"get",
		url:"/goform/deviceInfo",
		async:true,
		dataType:'json',
		success:function(data){
			//alert(2);
			//$('#productName').text(data.pName);
			//$('#productModle').text(data.pModle);
			$('#softversion').text(data.sVer);

		}
	});
}

//版本升级--提交
function upldForm(){
	$.ajax({
		type:"post",
		url:"/goform/upldForm",
		enctype:"multipart/form-data",
		async:true,
		success:function(data){
			alert(data);
		}
	});
}

function uploadFileSubmit(){
	alert("ok")
}
