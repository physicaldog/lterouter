$(function() {
$(".fileBrowse").click(function(){
  $("#fileName").click();
  });
})

function xzwjsj(){
	 $('#textfield').val($("#fileName").val());
}




/*$(document).on('change', '#fileName', function () { //PictureUrl为input file 的id  
        //console.log(this.files[0]);  
        function getObjectURL(file) {  
            var url = null;  
            if (window.createObjcectURL != undefined) {  
                url = window.createOjcectURL(file);  
            } else if (window.URL != undefined) {  
                url = window.URL.createObjectURL(file);  
            } else if (window.webkitURL != undefined) {  
                url = window.webkitURL.createObjectURL(file);  
            }  
            return url;  
        }  
        var objURL = getObjectURL(this.files[0]);//这里的objURL就是input file的真实路径  
        $('#textfield').val(objURL);
    }); 
*/
//在线升级
function kszxsj(){
	alert("开始在线升级......");
}
//本地升级
function skbdsj(){
	alert("开始本地升级......");
}


//获取真是路劲
function getPath(obj) {
 if (obj) {
  if (window.navigator.userAgent.indexOf("MSIE") >= 1) {
   obj.select();
   return document.selection.createRange().text;
  } else if (window.navigator.userAgent.indexOf("Firefox") >= 1) {
   if (obj.files) {
    return obj.files.item(0).getAsDataURL();
   }
   return obj.value;
  }
  return obj.value;
 }
}

