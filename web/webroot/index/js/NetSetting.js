$(function() {
	NetType();
});

function NetType() {
	$("#selectNetType").change(function() {
		if($("#selectNetType option:selected").val() == "自动") {
			$('.selectType2').css("display", "none");
			$('.zidqsw').css("display", "block");
			$('.selectType3').css("display", "none");
		} else if($("#selectNetType option:selected").val() == "固定") {
			$('.zidqsw').css("display", "none");
			$('.selectType2').css("display", "block");
			$('.selectType3').css("display", "none");
		} else if($("#selectNetType option:selected").val() == "拨号") {
			$('.selectType3').css("display", "block");
			$('.selectType2').css("display", "none");
			$('.zidqsw').css("display", "none");
		}
	});
}

//保存设置系列....
function submitType() {
	if($("#selectNetType option:selected").val() == "自动") {
		alert("保存设置1.....");
	} else if($("#selectNetType option:selected").val() == "固定") {
		alert("保存设置2.....");
	} else
	if($("#selectNetType option:selected").val() == "拨号") {
		alert("保存设置3.....");
	}
}


//apn查询
function searchAPN() {
	//var apn = document.getElementById("apn").value;
	alert("search_apn");

	var xmlhttp;
	if(window.XMLHttpRequest) { // code for IE7+, Firefox, Chrome, Opera, Safari
		xmlhttp = new XMLHttpRequest();
	} else { // code for IE6, IE5
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	}
	xmlhttp.onreadystatechange = function() {
		if(xmlhttp.readyState == 4 && xmlhttp.status == 200) {
			document.getElementById("apn").value = xmlhttp.responseText;
		}
	}
	xmlhttp.open("POST", "/goform/lteStatus_apn", true);
	xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xmlhttp.send("apn=");
}
//apn设置
function settingAPN() {
	alert("set_apn");
	var apn = document.getElementById("apn").value;

	var xmlhttp;
	if(window.XMLHttpRequest) { // code for IE7+, Firefox, Chrome, Opera, Safari
		xmlhttp = new XMLHttpRequest();
	} else { // code for IE6, IE5
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	}
	xmlhttp.onreadystatechange = function() {
		if(xmlhttp.readyState == 4 && xmlhttp.status == 200) {
			document.getElementById("apn").value = xmlhttp.responseText;
		}
	}
	xmlhttp.open("POST", "/goform/lteSet_apn", true);
	xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xmlhttp.send("apn=" + apn);
}
//LAN_IP查询
function searchLAN_IP() {
	//var lanip = document.getElementById("lanip").value;
	alert("search_lanip");
	var xmlhttp;
	if(window.XMLHttpRequest) { // code for IE7+, Firefox, Chrome, Opera, Safari
		xmlhttp = new XMLHttpRequest();
	} else { // code for IE6, IE5
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	}
	xmlhttp.onreadystatechange = function() {
		if(xmlhttp.readyState == 4 && xmlhttp.status == 200) {
			document.getElementById("lanip").value = xmlhttp.responseText;
		}
	}
	xmlhttp.open("POST", "/goform/lteStatus_lanip", true);
	xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xmlhttp.send("lanip=");
}
//LAN_IP设置
function settingLAN_IP() {
	alert("set_lanip");
	var lanip = document.getElementById("lanip").value;
	var xmlhttp;
	if(window.XMLHttpRequest) { // code for IE7+, Firefox, Chrome, Opera, Safari
		xmlhttp = new XMLHttpRequest();
	} else { // code for IE6, IE5
		xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
	}
	xmlhttp.onreadystatechange = function() {
		if(xmlhttp.readyState == 4 && xmlhttp.status == 200) {
			document.getElementById("lanip").value = xmlhttp.responseText;
		}
	}
	xmlhttp.open("POST", "/goform/lteSet_lanip", true);
	xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
	xmlhttp.send("lanip=" + lanip);
}

//系统重启
function resetDevice(){
	var xmlhttp;
if(window.XMLHttpRequest) { // code for IE7+, Firefox, Chrome, Opera, Safari
	xmlhttp = new XMLHttpRequest();
} else { // code for IE6, IE5
	xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
}
xmlhttp.onreadystatechange = function() {
	if(xmlhttp.readyState == 4 && xmlhttp.status == 200) {
		//alert()
		//document.getElementById("lanip").value=xmlhttp.responseText;
	}
}
xmlhttp.open("POST", "/goform/sysSet_sysReset", true);
xmlhttp.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
xmlhttp.send("sysReset=");
}
