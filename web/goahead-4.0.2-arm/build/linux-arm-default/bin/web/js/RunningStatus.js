//初始化加载
$(function(){
	sysInfo();
	doWork();
	deviceInfo();
/*<<<<<<< HEAD
	//setInterval('sysInfo()',60);
=======
	setInterval('sysInfo()',1000);
>>>>>>> 7da1a33a67583f3250c652ced70fdf881e52d150*/
	setInterval('doWork()',3000);
});
function doWork(){
	//sysInfo();
	WANStatus();
	LANStatus();
}
//系统信息
var totalTime = 0;
function sysInfo(){
	//alert(1);
	$.ajax({
		type:"get",
		url:"/goform/sysInfo",
		async:true,
		dataType:'json',
		success:function(data){
		//	alert(data.uptime);
			totalTime = parseInt(data.uptime);
			setInterval('jisuan()',1000);
			
			
		}
	});
}
function jisuan(){
	totalTime = totalTime+1;
	siteTime(totalTime)
}
function siteTime(diff){
//      window.setTimeout("siteTime()", 1000);
        var seconds = 1;
        var minutes = seconds * 60;
        var hours = minutes * 60;
        var days = hours * 24;
        var years = days * 365;
        /*var today = new Date();
        var todayYear = today.getFullYear();
        var todayMonth = today.getMonth()+1;
        var todayDate = today.getDate();
        var todayHour = today.getHours();
        var todayMinute = today.getMinutes();
        var todaySecond = today.getSeconds();*/
        /* Date.UTC() -- 返回date对象距世界标准时间(UTC)1970年1月1日午夜之间的毫秒数(时间戳)
        year - 作为date对象的年份，为4位年份值
        month - 0-11之间的整数，做为date对象的月份
        day - 1-31之间的整数，做为date对象的天数
        hours - 0(午夜24点)-23之间的整数，做为date对象的小时数
        minutes - 0-59之间的整数，做为date对象的分钟数
        seconds - 0-59之间的整数，做为date对象的秒数
        microseconds - 0-999之间的整数，做为date对象的毫秒数 */
        /*var t1 = Date.UTC(2016,12,01,00,00,00); //北京时间2016-12-1 00:00:00
        var t2 = Date.UTC(todayYear,todayMonth,todayDate,todayHour,todayMinute,todaySecond);*/
        var diffYears = Math.floor(diff/years);
        var diffDays = Math.floor((diff/days)-diffYears*365);
        var diffHours = Math.floor((diff-(diffYears*365+diffDays)*days)/hours);
        var diffMinutes = Math.floor((diff-(diffYears*365+diffDays)*days-diffHours*hours)/minutes);
        var diffSeconds = Math.floor((diff-(diffYears*365+diffDays)*days-diffHours*hours-diffMinutes*minutes)/seconds);
       $('#uptime').text(" 已运行"+diffYears+" 年 "+diffDays+" 天 "+diffHours+" 小时 "+diffMinutes+" 分钟 "+diffSeconds+" 秒");
    }
//设备信息
function deviceInfo(){
	$.ajax({
		type:"get",
		url:"/goform/deviceInfo",
		async:true,
		dataType:'json',
		success:function(data){
			$('#productName').text(data.pName);
			$('#productModle').text(data.pModle);
			$('#softVersion').text(data.sVer);

		}
	});
}

//内存使用率
function MemoryUsageRate(){
	$.ajax({
		type:"get",
		url:"/goform/queryVer",
		async:true,
		dateType:"json",
		success:function(data){
			
		}
	});
}
//WAN口状态
function WANStatus(){
	$.ajax({
		type:"get",
		url:"/goform/WANStatus",
		async:true,
		dataType:'json',
		data:{
			//name:"zhangsan",
			//age:18
		},
		success:function(data){	
			$('#netmode').text(data.netmode);
			$('#netStatus').text(data.netStatus);
			$('#simStatus').text(data.simStatus);
			$('#ecmStatus').text(data.ecmStatus);

			//$('#rssi').text(data.rssi);
			$('#rsrp').text(data.rsrp);
			$('#sinr').text(data.sinr);
			//$('#rsrq').text(data.rsrq);
			
			$('#APN').text(data.apn);
			$('#wanip').text(data.wanip);
			$('#wanmask').text(data.wanmask);
			$('#wanmac').text(data.wanmac);
		}
	});
}

//LAN口状态
function LANStatus(){
	$.ajax({
		type:"get",
		url:"/goform/LANStatus",
		async:true,
		dataType:'json',
		success:function(data){
			$('#lanIp').text(data.lanip);
			$('#lanMask').text(data.lanmask);
			$('#lanMac').text(data.lanmac);			
		}
	});
}
//设备数量
function deviceCount(){
	$.ajax({
		type:"get",
		url:"",
		async:true,
		success:function(data){
			
		}
	});
}