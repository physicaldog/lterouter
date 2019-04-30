//初始化加载
$(function() {
	getRebootCount();
	checkManualTime();
	checkRadio();
	
});

function settingTime() {
	if($("input:text[id='system_time']").val().length == 0) {
		alert("参数不能为空");
	} else {
		//alert("2");
		$.ajax({
			type: "get",
			url: "/goform/settingTime",
			async: true,
			data: {
				system_time: $('#system_time').val(),
			},
			success: function(data) {
				alert(data);

			}
		});
	}
}

function queryTime() {
	$.ajax({
		type: "get",
		url: "/goform/queryTime",
		async: true,
		dataType: 'json',
		success: function(data) {
			//alert(data.system_time);
			$('#system_time').val(data.system_time);

		}
	});
}

function addAttr() {

	$('#system_time').attr("disabled", "disabled");
	$('#setTime').attr("disabled", "disabled");
}

function removeAttr() {
	$('#system_time').removeAttr("disabled");
	$('#setTime').removeAttr("disabled");
}

function checkManualTime() {
	queryTime();
	$.ajax({
		type: "get",
		url: "/goform/checkManualTime",
		async: true,
		dataType: 'json',
		success: function(data) {
			//alert(data.value);
			if(data.value == "1") {
				$("input:radio[id='manual']").prop('checked', 'true');
				removeAttr();
			} else {
				$("input:radio[id='auto']").prop('checked', 'true');
				addAttr();
			}

		}
	});
}

function setManualTime() {
	$.ajax({
		type: "get",
		url: "/goform/setManualTime",
		async: true,
		success: function(data) {
			//alert(data);
		}
	});
}

function setAutoTime() {
	$.ajax({
		type: "get",
		url: "/goform/setAutoTime",
		async: true,
		success: function(data) {
			alert(data);
		}
	});
}

function checkRadio() {
	$(":radio").click(function() {
		var id = $(this).attr("id");
		if(id == "manual") {
			setManualTime();
			removeAttr();
		} else {
			setAutoTime();
			addAttr();
		}
	});
}

//重启 --提交

function getRebootCount() {
	$.ajax({
		type: "get",
		url: "/goform/getRebootCount",
		async: true,
		success: function(data) {
			alert(data);
			$('#rebootCount').text(data);
		}
	});
}
var layerindex;
function resetDev() {
	var ajaxTimeOut =$.ajax({
		type: "get",
		url: "/goform/resetDev",
		success: function(data) {
			    layerindex=layer.load(2, { //icon支持传入0-2
                shade: [0.5, 'gray'], //0.5透明度的灰色背景
                content: '设备正在重启，请稍后...',
//              time: 10*1000, //锁住浏览器10秒，不让操作
                success: function (layero) {
                    layero.find('.layui-layer-content').css({
                        'padding-top': '39px',
                        'width': '60px'
                    });
                    //定时检测服务端是否返回cookie数据
                    setInterval(function(){
                    	if(getCookie("ecology_JSessionId")==null){
                    		layer.close(ajaxTimeOut);
                    		//重启成功，跳转父级登录页面
                  			parent.location.reload(); 
                    	}
                    },1000);
                }

            });
			
		},
		complete : function(XMLHttpRequest,status){ //请求完成后最终执行参数
　　　　if(status=='timeout'){  //超时,status还有success,error等值的情况
　　　　　 ajaxTimeOut.abort(); //取消请求
　　　　　 alert("超时,重启异常");
　　　　} 
      }
		
	});
}
//获取cookie登录信息
function getCookie(cookieName){  
        var cookieValue="";  
        if (document.cookie && document.cookie != '') {   
            var cookies = document.cookie.split(';');  
            for (var i = 0; i < cookies.length; i++) {   
                 var cookie = cookies[i];  
                 if (cookie.substring(0, cookieName.length + 2).trim() == cookieName.trim() + "=") {  
                       cookieValue = cookie.substring(cookieName.length + 2, cookie.length);   
                       break;  
                 }  
             }  
        }   
        return cookieValue;  
    }

//恢复出厂设置--提交
function sysReset() {
	$.ajax({
		type: "get",
		timeout : 10000,
		url: "/goform/sysReset",
//		async: true,
		success: function(data) {
			alert("恢复出厂设置成功....");
			//成功，刷新父级页面
			parent.location.reload(); 
		},
		complete : function(XMLHttpRequest,status){ //请求完成后最终执行参数
　　　　if(status=='timeout'){  //超时,status还有success,error等值的情况
　　　　　 ajaxTimeOut.abort(); //取消请求
　　　　　 alert("超时");
　　　　}
      }
	});
}