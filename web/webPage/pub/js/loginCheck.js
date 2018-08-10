//打开字滑入效果
window.onload = function(){
	$(".connect p").eq(0).animate({"left":"0%"}, 600);
	$(".connect p").eq(1).animate({"left":"0%"}, 400);
};
//jquery.validate表单验证
$(document).ready(function(){
	jQuery.validator.addMethod("phone_number", function(value, element) { 
		var length = value.length; 
		var phone_number = /^(((13[0-9]{1})|(15[0-9]{1}))+\d{8})$/ 
		return this.optional(element) || (length == 11 && phone_number.test(value)); 
	}, "手机号码格式错误"); 
});
function validate(){
	return $("#loginForm").validate({
		rules:{
			username:{
				required:true,//必填
			},
			password:{
				required:true,
			},
		},
		//错误信息提示
		messages:{
			username:{
				required:"必须填写用户名",
			},
			password:{
				required:"必须填写密码",
			},
		},

	})
}

var key  = CryptoJS.enc.Latin1.parse('monkeymonkeymonk'); //密钥
var iv   = CryptoJS.enc.Latin1.parse('monkeymonkeymonk'); //向量

function loginCheck(){
	//alert($('#uname').val()+"     "+$('#uPW').val());
	$('#uname').val(CryptoJS.AES.encrypt($('#uname').val(),key,{iv:iv,mode:CryptoJS.mode.CBC,padding:CryptoJS.pad.ZeroPadding}));
	$('#uPW').val(CryptoJS.AES.encrypt($('#uPW').val(),key,{iv:iv,mode:CryptoJS.mode.CBC,padding:CryptoJS.pad.ZeroPadding}));
	//alert($('#uname').val()+"     "+$('#uPW').val());
	//登陆表单验证
/*	if(validate().form()){
		$.ajax({
		type:"post",
		url:"/action/login",
		async:true,
		data:$("#loginForm").serialize(),
		success:function(data){
			alert(1);
			alert(data);
		if (parseInt(data)==1) {
				var loginstate=new Date(); 
              loginstate.setTime(new Date().getTime()+5*60*1000); //设置有效时间为5分钟
              $.cookie('netstats', '1', { expires: loginstate });  
	            window.location.replace("index.html");
			} else{
				//alert(data);
				//layer.msg('用户名或者密码错误!', {icon: 2});
				layer.msg('用户名或者密码错误!');
	            window.location.replace("index.html");
			}
			console.log("fanhuizhi"+data)
		}
	});
	}*/
}

//解密
function jiemi(encrypted){
	var decrypted = CryptoJS.AES.decrypt(encrypted,key,{iv:iv,padding:CryptoJS.pad.ZeroPadding});
	/*console.log(decrypted.toString(CryptoJS.enc.Utf8));*/
	return decrypted;
}
	
	
	
function encrypt(word){
    var key = CryptoJS.enc.Utf8.parse("abcdefgabcdefg12");
    var srcs = CryptoJS.enc.Utf8.parse(word);
    var encrypted = CryptoJS.AES.encrypt(srcs, key, {mode:CryptoJS.mode.ECB,padding: CryptoJS.pad.Pkcs7});
    return encrypted.toString();
}
