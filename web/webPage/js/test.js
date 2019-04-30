//加密
function compile()  {  
   var code='alert("黑客防线");'  
   var c=String.fromCharCode(code.charCodeAt(0)+code.length);  
   for(var i=1;i<code.length;i++){  
   c+=String.fromCharCode(code.charCodeAt(i)+code.charCodeAt(i-1));  
   }  
   alert(escape(c));  
}  

//解密
function uncompile(code) {  
   code=unescape(code);  
   var c=String.fromCharCode(code.charCodeAt(0)-code.length);  
   for(var i=1;i<code.length;i++){  
   c+=String.fromCharCode(code.charCodeAt(i)-c.charCodeAt(i-1));  
   }  
   return c;  
}  
function testjiemi(){
	eval(uncompile("o%CD%D1%D7%E6%9CJ%u9EF3%uFA73%uF1D4%u14F1%u7EE1Kd"));  
}
