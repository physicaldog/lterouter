//初始化加载
$(function(){
	sysInfo();
	doWork();
	deviceInfo();

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
			
			$('#system_addr').text(data.system_addr);
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
			$('#system_date').text(data.system_date);
			$('#netmode').text(data.netmode);
			$('#netStatus').text(data.netStatus);
			$('#simStatus').text(data.simStatus);
			$('#ecmStatus').text(data.ecmStatus);

			$('#rssi').text(data.rssi);
			$('#rsrp').text(data.rsrp);
			$('#sinr').text(data.sinr);
			$('#rsrq').text(data.rsrq);
			$('#APN').text(data.apn);
			$('#imei').text(data.imei);
			$('#wanip').text(data.wanip);
			$('#wanmask').text(data.wanmask);
			$('#wanmac').text(data.wanmac);
			$('#gw_ip').text(data.gw_ip);
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
