
String.prototype.trim = function() {
    return this.replace(/^\s+|\s+$/g,"");
}
String.prototype.ltrim = function() {
    return this.replace(/^\s+/,"");
}
String.prototype.rtrim = function() {
    return this.replace(/\s+$/,"");
}



/***
**   Valid MAC IP
***/

/*adjust the data of network format*/
function isValidLastByteZeroIp(StartIp)
{
    addrParts1 = StartIp.split(".");
    if ( ( 0 == addrParts1[3] ) )
    {
        return false;
    }
    return true;
}

function isValidDhcpAddress(dhcpaddress,ipaddress) {
    var dhcpaddr = dhcpaddress.split('.');
    var ipaddr = ipaddress.split('.');
    for (i = 0; i < 3; i++) {
    if (dhcpaddr[i]!= ipaddr[i])
    {
     return false;
    }
    }
    return true;
}


function isValidIpAddress(address) {
   //if(isIpv6Address(address) == true)
   //    return true;
    
   var i = 0;

    if ( address == '0.0.0.0' ||
            address == '255.255.255.255' )
    {
        return false;
    }

    var addrParts = address.split('.');
    if ( addrParts.length != 4 )
    {
        return false;
    }
   for (i = 0; i < 4; i++) {
      if (isNaN(addrParts[i]) || addrParts[i] ==""
          || addrParts[i].charAt(0) == '+' ||  addrParts[i].charAt(0) == '-'
          || (addrParts[i].charAt(0) == '0' && addrParts[i].length > 1))
        {
            return false;
        }
      if (addrParts[i].length > 3 || addrParts[i].length < 1)
      {
          return false;
      }
      if (!isInteger(addrParts[i]) || addrParts[i] < 0)
      {
          return false;
      }
      num = parseInt(addrParts[i], 10);
      if (i == 0 && num == 0)
      {
          return false;
      }
        if ( num < 0 || num > 255 )
        {
            return false;
        }
   }
   return true;
}

function isBroadcastAddress(address) {

    var addrParts = address.split('.');

    if (addrParts[3]==255)
    {
      return false;
    }

    return true;
}


function checkDomain(urlValue) 
{
	//zmtel.com1234567890123456789023456789012345678会导致挂起
	//zmtel.567890123456789012345678901234567890123456789d11fffff22com OK
	var array = urlValue.split(".");
	var domain1 = array[array.length-1];
	//for (var i=0 ; i< array.length ; i++)
	if ( /[0-9]+/.test(domain1)==true )
		return false;
    var strRegex = "^((https|http|ftp|rtsp|mms)?://)?"
                   + "([0-9a-zA-Z_!~*'()-]+\.)*" // 域名- www.
                   + "([0-9a-zA-Z][0-9a-z-]{0,61})*\." // 二级域名
                   + "[a-zA-Z]{2,6}$" // first level domain- .com or .museum

    var re=new RegExp(strRegex);
    
    if (re.test(urlValue))
        return (true);
    else
        return (false);
}
function isAbcIpAddress(address)
{
    if (isValidIpAddress(address) == false)
    {
        return false;
    }

    var addrParts = address.split('.');
    var num1 = 0;
    var num2 = 0;
    var num3 = 0;
    var num4 = 0;

    num1 = parseInt(addrParts[0], 10);
    num2 = parseInt(addrParts[1], 10);
    num3 = parseInt(addrParts[2], 10);
    num4 = parseInt(addrParts[3], 10);

    if (num1 < 1 || num1 >= 224 || num1 == 127)
    {
        return false;
    }
    if (num2 == 0 && num3 == 0 && num4 == 0)
    {
        if(num1 == 1 || num1 == 128 || num1 == 192 || num1 == 224)
        {
            return false;
        }     
    }
    if (num2 == 255 && num3 == 255 && num4 == 255)
    {
        if(num1 == 126 || num1 == 191 || num1 == 223)
        {
            return false;
        }
    }

    return true;
}
function checkSrcIpRange(startIp, endIp){
    if(startIp != endIp) {
        if('' === startIp) 
            return false;
        if('' === endIp) 
            return false;
    }
    if('' !==  startIp) {
        if (false === isValidIpAddress(startIp)) 
            return false;
        if(false === isAbcIpAddress(startIp)) 
            return false;

        if ( !isValidLastByteZeroIp(startIp )) 
            return false;
    }

    if('' !== endIp)
    {
        if (false === isValidIpAddress(endIp))        
            return false;
        if(false === isAbcIpAddress(endIp))        
            return false;

        if ( !isValidLastByteZeroIp(endIp ))        
            return false;
    }

    if(('' !== startIp) && ('' !== endIp))    
        if (ipAddress2DecNum(startIp) > ipAddress2DecNum(endIp))
            return false;

    return true;
}
function checkIPofIPfilter(ipaddr){
    var aftersplit = null;
    var mask;
    aftersplit = ipaddr.split('/');

    if(aftersplit.length!=1) {
        mask=parseInt(aftersplit[1],10);
        if(mask==false|| mask>32 || mask<0)
            return false;
        return isAbcIpAddress(aftersplit[0]);
    }
    else
        return isAbcIpAddress(ipaddr);

}
function checkSrcPortRange(startPort, endPort)
{

    if(startPort != endPort)
    {
        if('' === startPort)
            return false;

        if('' === endPort)
            return false;
    }

    if(('' !==  startPort)&&(false === isValidPort(startPort)))
             return false;

     if(('' !== endPort)&&(false === isValidPort(endPort)))
             return false;

     if(('' !== startPort) && ('' !== endPort))
         if (false === isValidPortPair(startPort, endPort))
             return false;

    return true;
}function ipAddress2DecNum(address)
{
    if (isValidIpAddress(address) == false)
    {
        return -1;
    }
    var addrParts = address.split('.');
    var num = 0;
    for (i = 0; i < 4; i++) {
        num += parseInt(addrParts[i], 10) * Math.pow(256, 3 - i);
    }
    return num;
}
function isValidMaskAddress(address) {
    obj=address;
    var exp=/^(254|252|248|240|224|192|128|0)\.0\.0\.0|255\.(254|252|248|240|224|192|128|0)\.0\.0|255\.255\.(254|252|248|240|224|192|128|0)\.0|255\.255\.255\.(252|248|240|224|192|128|0)$/;
    var reg = obj.match(exp);
    if(reg==null)
    {
       return false;
    }
    else
    {
       return true;
    }
}

function isValidLanMaskAddress(address) {
    obj=address;
    var exp=/^255\.255\.255\.(252|248|240|224|192|128|0)$/;
    var reg = obj.match(exp);
    if(reg==null)
    {
       return false;
    }
    else
    {
       return true;
    }
}

function isValidMacAddress(address)
{
   var c = '';
   var i = 0, j = 0;

   if ( address.toLowerCase() == 'ff:ff:ff:ff:ff:ff' )
   {
       return false;
   }

   addrParts = address.split(':');
   if ( addrParts.length != 6 ) return false;

   for (i = 0; i < 6; i++) {
      if ( addrParts[i] == '' )
         return false;

      if ( addrParts[i].length != 2)
      {
        return false;
      }

      if ( addrParts[i].length != 2)
      {
         return false;
      }

      for ( j = 0; j < addrParts[i].length; j++ ) {
         c = addrParts[i].toLowerCase().charAt(j);
         if ( (c >= '0' && c <= '9') ||
              (c >= 'a' && c <= 'f') )
            continue;
         else
            return false;
      }
   }

   return true;
}
function isInteger(value)
{
    if (/^(\+|-)?\d+$/.test(value))
    {
       return true;
    }
    else
    {
        return false;
    }
}
function isValidPort(port)
{
   if (!isInteger(port) || port < 1 || port > 65535)
   {
       return false;
   }

   return true;
}
function isValidPortPair(StartPort,EndPort)
{
   if (!isValidPort(StartPort) || !isValidPort(EndPort))
   {
       return false;
   }

   if (parseInt(StartPort,10) > parseInt(EndPort,10) )
   {
        return false;
    }

   return true;
}
function isASCIIData(str)
{
  if(str == null)
    {
        return true;
    }
    var i=0;
    var char_i;
    var num_char_i;
    for(i=0; i < str.length; i++)
    {
        char_i = str.charAt(i);
        num_char_i = char_i.charCodeAt();
        if (num_char_i > 127)
        {
            return false;
        }
    }
    return true;
}
function isHexaDigit(digit) {
   var hexVals = new Array("0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                           "A", "B", "C", "D", "E", "F", "a", "b", "c", "d", "e", "f");
   var len = hexVals.length;
   var i = 0;
   var ret = false;

   for ( i = 0; i < len; i++ ) {
      if ( digit == hexVals[i] ) {
        break;
      }
   }

   if ( i < len )
      ret = true;

   return ret;
}
function isHexString(str) {
    var i = 0;

    for ( i = 0; i < str.length; i++ )
    {
        if ( isHexaDigit(str.charAt(i)) == false )
        {
            return false;
        }

    }
    return true;
}
function fiterZero(para)
{
    if('0' == para)
        return '&nbsp;';
    else
        return para;
}
function isNeedSpit(inputStr)
{
    if(inputStr.indexOf('-') >= 0)
        return true;
    else
        return false;
}
function getSplitStr(inputStr){
    var aftersplit = null;
    aftersplit = inputStr.split('-');
    return aftersplit;
}
function filterBlank(inputStr) {
    var tmpValue = inputStr;
    tmpValue = tmpValue.replace(/^[ ]*/, "") ;   // ȥͷ���ո�
    tmpValue = tmpValue.replace(/[ ]*$/, "") ;   // ȥβ���ո�
    return tmpValue;
}

/**
*   Get element
**/
function getElById(sId)
{
    return getElement(sId);
}

function getElementById(sId)
{
    if (document.getElementById)
    {
        return document.getElementById(sId);
    }
    else if (document.all)
    {
        // old IE
        return document.all(sId);
    }
    else if (document.layers)
    {
        // Netscape 4
        return document.layers[sId];
    }
    else
    {
        return null;
    }
}
function paddingLeft(str,lenght){
    str=""+str
	if(str.length >= lenght)
	return str;
	else
	return paddingLeft("0" +str,lenght);
}
function paddingRight(str,lenght){
    str=""+str
	if(str.length >= lenght)
	return str;
	else
	return paddingRight(str+"0",lenght);
}

/*getElByName*/
function getElementByName(sId)
{    // standard
    if (document.getElementsByName)
    {
        var element = document.getElementsByName(sId);

        if (element.length == 0)
        {
            return null;
        }
        else if (element.length == 1)
        {
            return     element[0];
        }

        return element;
    }
}

function getElement(sId)
{
     var ele = getElementByName(sId);
     if (ele == null)
     {
         return getElementById(sId);
     }
     return ele;
}

function getOptionIndex(sId,sValue)
{
    var selObj = getElement(sId);
    if (selObj == null)
    {
        return -1;
    }

    for (i = 0; i < selObj.length; i++)
    {
        if (selObj.options[i].value == sValue)
        {
            return i;
        }
    }

    return -1;
}
function setSelect(sId, sValue)
{
    var item;
    if (null == (item = getElement(sId)))
    {
        return false;
    }

    for (var i = 0; i < item.options.length; i++)
    {
        if (item.options[i].value == sValue)
        {

            item.options[i].selected =true;
            return true;
        }
    }

    return false;
}
function setText (sId, sValue)
{
    var item;
    if (null == (item = getElement(sId)))
    {
        return false;
    }

    item.value = sValue;
    return true;
}


function setCheck(sId, value)
{
    var item;
    if (null == (item = getElement(sId)))
    {
        return false;
    }

    if ( value == '1' )
    {
       item.checked = true;
    }
    else
    {
       item.checked = false;
    }

    return true;
}

function setRadio(sId, sValue)
{
    var item;
    if (null == (item = getElement(sId)))
    {
        return false;
    }

    for (i=0; i<item.length; i++)
    {
        if (item[i].value == sValue)
        {
            item[i].checked = true;
            return true;
        }
    }

    return false;
}

function debug(sValue) {
    alert(sValue);
}
/**
*   Add html item
**/
function removeOption(sId,sValue)
{
    var selItem;
    if ((selItem = getElement(sId)) != null)
    {
        var index = getOptionIndex(sId,sValue);
        if (index >= 0)
        {

            selItem.removeChild(selItem.options[index]);
            return true;
        }
        else
        {
            debug("the option which value is " + sValue + " is not existed" );
            return false;
        }
    }

    debug(sId + " is not existed" );
    return false;
}

function create_button(content, button_id) {

    var button = "<span class='button_wrapper' id='" + button_id + "'>";

    button += "<span class='button_left'>" + "<span class='button_right'>" + "<span class='button_center'>" + content + '</span></span></span></span>';
    document.write(button);
}
function add_maintitle(content) {
    if (content=="") {
        debug("add_maintitle_start is error" );
        return false;
    }
    document.write("<div class='content_right_title'> <span>"+content+"</span></div>");
}

function add_itemtitle(content) {
    document.write("<div class='content_item_title'>"+content+"</div>");
}
function add_itemtitle1(id,content) {
    document.write("<div id='"+id+"' class='content_item_title'>"+content+"</div>");
}
function add_text(content,name,id,starenable,note) {

    var title;
    title = "<div class='content_item' id='"+id+"_contentitem'><div class='content_item_left' id='"+id+"_content_item_left'>"+content+"</div><div class='content_item_right'>";
    title += "<input name='"+name+"' id='"+id+"' maxlength='128' type='text'>";
    
    if(starenable!="0" && note!="")
        title += "<span class='asterisk'>&nbsp;&nbsp;&nbsp;*&nbsp;&nbsp;</span><span id='"+id+"_note'>"+note+"</span>";
    else if(starenable!="0" && note=="")
        title += "<span class='asterisk'>&nbsp;&nbsp;&nbsp;*</span>";
    else if(starenable=="0" && note!="")
        title += "<span class='asterisk'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span id='"+id+"_note'>"+note+"</span>";

    title += "</div></div>";
    document.write(title);
    return true;

}
function add_password(content,name,id,starenable,note) {

    var title;
    title = "<div class='content_item' id='"+id+"_contentitem'><div class='content_item_left'>"+content+"</div><div class='content_item_right'>";
    title += "<input name='"+name+"' id='"+id+"' maxlength='128' type='text'>";
    title += "<input name='"+name+"_pwd' id='"+id+"_pwd' maxlength='128' type='password'>";
    
    if(starenable!="0" && note!="")
        title += "<span class='asterisk'>&nbsp;&nbsp;&nbsp;*&nbsp;&nbsp;</span><span id='"+id+"_note'>"+note+"</span>";
    else if(starenable!="0" && note=="")
        title += "<span class='asterisk'>&nbsp;&nbsp;&nbsp;*</span>";
    else if(starenable=="0" && note!="")
        title += "<span class='asterisk'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span id='"+id+"_note'>"+note+"</span>";

    title += "</div></div>";
    document.write(title);
    return true;

}
function add_password1(content,name,id,starenable,note) {

    var title;
    title = "<div class='content_item' id='"+id+"_contentitem'><div class='content_item_left'>"+content+"</div><div class='content_item_right'>";
    title += "<input name='"+name+"' id='"+id+"' maxlength='128' type='password'>";
    
    if(starenable!="0" && note!="")
        title += "<span class='asterisk'>&nbsp;&nbsp;&nbsp;*&nbsp;&nbsp;</span><span id='"+id+"_note'>"+note+"</span>";
    else if(starenable!="0" && note=="")
        title += "<span class='asterisk'>&nbsp;&nbsp;&nbsp;*</span>";
    else if(starenable=="0" && note!="")
        title += "<span class='asterisk'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span><span id='"+id+"_note'>"+note+"</span>";

    title += "</div></div>";
    document.write(title);
    return true;

}

function add_checkbox(content,name,id,content1) {
    var out;
    out = "<div class='content_item' id='"+id+"_contentitem'><div class='content_item_left'>"+content+"</div><div class='content_item_right'>";
    out += "<input id='"+id+"' type='checkbox' name='"+name+"' ";

    out += "></input><label for='"+id+"'>"+content1+"</label></div></div>";
    document.write(out);
    
    return true;
}
function add_radio() {
    var out;
    out = "<div class='content_item' id='"+arguments[1]+"_contentitem'><div class='content_item_left'>"+arguments[0]+"</div><div class='content_item_right'>";
    
    for(var j=2;j<arguments.length;j+=4) {
        out += "<input type='radio' name='"+arguments[j]+"' id='"+arguments[j+1]+"' value='"+arguments[j+2]+"'><label for='"+arguments[j+1]+"'>"+arguments[j+3]+"</label>";
    }
    out += "</div></div>";
    document.write(out);
    
    return true;
}
function add_select() {
    var out;

    if(arguments.length<3) {
        debug("add_select num "+arguments.length+"is error" );
        return false;
    }
    out = "<div class='content_item' id='"+arguments[2]+"_contentitem'><div class='content_item_left'>"+arguments[0]+"</div><div class='content_item_right'>";
    out += "<select name='"+arguments[1]+"' id='"+arguments[2]+"' class='wide_wlan'>";

    if(arguments.length>3)
    {
        for(var j=3;j<arguments.length;j+=2) {
            out += "<option value='"+arguments[j]+"' >"+arguments[j+1]+"</option>";
        }
    }
    out += "</select></div></div>";
    document.write(out);
    return true;
}

function add_table_edit() {
    var out;
    if (arguments.length==0) {
        debug("add_table num "+arguments.length+"is error" );
        return false;
    }
    out = "<div class='table_edit'><div class='content_table_title'>"+arguments[0]+"</div><div class='table_button_container'>";
    for(var j=1;j<arguments.length;j+=2) {
        out += "<span class='button_wrapper' id='" + arguments[j+1] + "'>";
        out += "<span class='button_left'>" + "<span class='button_right'>" + "<span class='button_center'>" + arguments[j] + '</span></span></span></span>';
    }
    out += "</div></div>";
    document.write(out);

    return true;
}
function add_table() {
    var out;
    if (arguments.length==0) {
        debug("add_table num "+arguments.length+"is error" );
        return false;
    }
    out = "<table id ='tables' class='table_list'> <tr>";
    for(var j=0;j<arguments.length;j++) {
        out += "<th>"+arguments[j]+"</th>";
    }
    out += "</tr></table>";
    document.write(out);

    return true;
}
function add_table_by_id() {
    var out;
    if (arguments.length==0) {
        debug("add_table num "+arguments.length+"is error" );
        return false;
    }
    out = "<table id ='"+arguments[0]+"' class='table_list'> <tr>";
    for(var j=1;j<arguments.length;j++) {
        out += "<th>"+arguments[j]+"</th>";
    }
    out += "</tr></table>";
    document.write(out);

    return true;
}
function add_half_table_by_id() {
    var out;
    if (arguments.length==0) {
        debug("add_table num "+arguments.length+"is error" );
        return false;
    }
    out = "<table id ='"+arguments[0]+"' class='half_table_list'> <tr>";
    for(var j=1;j<arguments.length;j++) {
        out += "<th>"+arguments[j]+"</th>";
    }
    out += "</tr></table>";
    document.write(out);

    return true;
}
function add_txt(content,id) {

    var title;
    title = "<div class='content_item' id='"+id+"_contentitem' style='min-height:20px;'><div class='content_item_left'>"+content+"</div><div class='content_item_right' id='" +id+ "'>";
    
    title += "</div></div>";
    document.write(title);
    return true;

}
function setDisplay (sId, sh)
{
    var status;
    if (sh > 0)
        $("#"+sId).show();
    else
         $("#"+sId).hide();
}

function getElementById(sId)
{
    if (document.getElementById)
    {
        return document.getElementById(sId);
    }
    else if (document.all)
    {
        // old IE
        return document.all(sId);
    }
    else if (document.layers)
    {
        // Netscape 4
        return document.layers[sId];
    }
    else
    {
        return null;
    }
}

function getElementByName(sId)
{    // standard
    if (document.getElementsByName)
    {
        var element = document.getElementsByName(sId);

        if (element.length == 0)
        {
            return null;
        }
        else if (element.length == 1)
        {
            return     element[0];
        }

        return element;
    }
}

function getElement(sId)
{
     var ele = getElementByName(sId);
     if (ele == null)
     {
         return getElementById(sId);
     }
     return ele;
}

function getValue(sId)
{
    var item;
    if (null == (item = getElement(sId)))
    {
        debug(sId + " is not existed" );
        return -1;
    }

    return item.value;
}

function setText (sId, sValue)
{
    var item;
    if (null == (item = getElement(sId)))
    {
        debug(sId + " is not existed" );
        return false;
    }

    item.value = sValue;
    return true;
}

function isButtonEnable(button_id) {
	var disable = true;
	var $button = $("#" + button_id);
	if ($button) {
		if ($button.children()) {
			disable = $button.hasClass("disable_btn");
			// enable = $button.children().hasClass("button_left");
		}
	}
	return !disable;
}

function button_enable(button_id, enable) {
    var parent = $('#' + button_id);

    if (enable == '1') {
        parent.removeClass('disable_btn');

    }
    else if (enable == '0') {
        parent.addClass('disable_btn');

    }
}

function isNull(v)
{
    if(v === null)
    {
        return true;
    }

    if(v === undefined)
    {
        return true;
    }

    return false;
}

function isPlusInteger(value)
{
    if (isInteger(value) && parseInt(value, 10) >= 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

function disableMenu()
{

	$("a", window.parent.document).each(function(){
		$(this).click(function() {
			return false;
		});
	});
}

function enableMenu()
{
	$("a", window.parent.document).each(function(){
		$(this).unbind( "click" );

	});
	
}

function isValidName(name)
{
   //return isSafeStringExc(name,'!"~<>;{}|%*\\^[]`+$,=\'#&: \t');
   return isSafeStringExc(name,'\\');
}

function isSafeStringExc(compareStr, UnsafeStr)
{
    for (var i = 0; i < compareStr.length; i++)
    {
        var c = compareStr.charAt(i);
        if (isValidAscii(c) != '')
        {
             return false;
        }
        else
        {
            if (UnsafeStr.indexOf(c) > -1)
            {
                return false;
            }
        }
    }
    return true;
}

function isValidAscii(val)
{
    for ( var i = 0 ; i < val.length ; i++ )
    {
        var ch = val.charAt(i);
        if ( ch < ' ' || ch > '~' )
        {
            return ch;
        }
    }
    return '';
}

function isValidString(name)
{
    if (isValidAscii(name) == '')
    {
        return true;
    }
    else
    {
        return false;
    }
}

function getTimeText(leaseTime)
{
    var result = "";
    
    if(leaseTime == "N/A")
        return "N/A";
    
    var time = parseInt(leaseTime,10);
    var day = 0;
    var hour = 0;
    var minute = 0;
    var sec = 0;
    
    day = parseInt(time / (60*60*24),10);    
    time = (time % (60*60*24));
    
    hour = parseInt(time/(60*60),10);
    time = (time % (60*60));
    
    minute = parseInt(time/60,10);    
    time = (time % 60);
    
    sec = parseInt(time,10);
    
    return day +"天 " + hour + ":" + minute + ":" + sec;
}


function IsLoginPage(str)
{
    var pattern = new RegExp('<head><title>.+</title>',"gi"); 

    if(typeof(str)=="string")
    {
        if(pattern.test(str))
        {
            return true;
        }
    }
    return false
}


function URLEncode(sStr)   
{  
   return escape(sStr).replace(/\+/g, '%2B');  
}  

function ParseIpv6Array(str)
{
    var Num;
    var i,j;
    var finalAddrArray = new Array();
    var falseAddrArray = new Array();

    var addrArray = str.split(':');
    Num = addrArray.length;
    if (Num > 8)
    {
        return falseAddrArray;
    }

    for (i = 0; i < Num; i++)
    {
        if ((addrArray[i].length > 4)
            || (addrArray[i].length < 1))
        {
            return falseAddrArray;
        }
        for (j = 0; j < addrArray[i].length; j++)
        {
            if ((addrArray[i].charAt(j) < '0')
                || (addrArray[i].charAt(j) > 'f')
                || ((addrArray[i].charAt(j) > '9') &&
                (addrArray[i].charAt(j) < 'a')))
            {
                return falseAddrArray;
            }
        }

        finalAddrArray[i] = '';
        for (j = 0; j < (4 - addrArray[i].length); j++)
        {
            finalAddrArray[i] += '0';
        }
        finalAddrArray[i] += addrArray[i];
    }

    return finalAddrArray;
}

function getFullIpv6Address(address)
{
    var c = '';
    var i = 0, j = 0, k = 0, n = 0;
    var startAddress = new Array();
    var endAddress = new Array();
    var finalAddress = '';
    var startNum = 0;
    var endNum = 0;
    var lowerAddress;
    var totalNum = 0;

    lowerAddress = address.toLowerCase();

    var addrParts = lowerAddress.split('::');
    if (addrParts.length == 2)
    {
        if (addrParts[0] != '')
        {
            startAddress = ParseIpv6Array(addrParts[0]);
            if (startAddress.length == 0)
            {
                return '';
            }
        }
        if (addrParts[1] != '')
        {
            endAddress = ParseIpv6Array(addrParts[1]);
            if (endAddress.length == 0)
            {
               return '';
            }
        }

        if (startAddress.length +  endAddress.length >= 8)
        {
            return '';
        }
    }
    else if (addrParts.length == 1)
    {
        startAddress = ParseIpv6Array(addrParts[0]);
        if (startAddress.length != 8)
        {
            return '';
        }
    }
    else
    {
        return '';
    }

    for (i = 0; i < startAddress.length; i++)
    {
        finalAddress += startAddress[i];
        if (i != 7)
        {
            finalAddress += ':';
        }
    }
    for (; i < 8 - endAddress.length; i++)
    {
        finalAddress += '0000';
        if (i != 7)
        {
            finalAddress += ':';
        }
    }
    for (; i < 8; i++)
    {
        finalAddress += endAddress[i - (8 - endAddress.length)];
        if (i != 7)
        {
            finalAddress += ':';
        }
    }

    return finalAddress;

}

function isIpv6Address(address)
{
    if (getFullIpv6Address(address) == '')
    {
        return false;
    }

    return true;
}

function isUnicastIpv6Address(address)
{
    var tempAddress = getFullIpv6Address(address);

    if ((tempAddress == '')
        || (tempAddress == '0000:0000:0000:0000:0000:0000:0000:0000')
        || (tempAddress == '0000:0000:0000:0000:0000:0000:0000:0001')
        || (tempAddress.substring(0, 2) == 'ff'))
    {
        return false;
    }

    return true;
}

function isGlobalIpv6Address(address)
{
    var tempAddress = getFullIpv6Address(address);

    if ((tempAddress == '')
        || (tempAddress == '0000:0000:0000:0000:0000:0000:0000:0000')
        || (tempAddress == '0000:0000:0000:0000:0000:0000:0000:0001')
        || (tempAddress.substring(0, 3) == 'fe8')
        || (tempAddress.substring(0, 3) == 'fe9')
        || (tempAddress.substring(0, 3) == 'fea')
        || (tempAddress.substring(0, 3) == 'feb')
        || (tempAddress.substring(0, 2) == 'ff'))
    {
        return false;
    }

    return true;
}

function isExistOption(id,value) {  
    var isExist = false;  
    var count = $('#'+id).find('option').length;
    for(var i=0;i<count;i++)     
    {     
        if($('#'+id).get(0).options[i].value == value)     
        {
            isExist = true;     
            break;     
        }     
    }     
    return isExist;  
}  

function SendRequestToServer(location, cmd, value, successcb, errcb) {
	ret=$.ajax({
		timeout : "20000",
		url : 'request.xml',
		dataType : "xml",
		data : 'location='+location+'&'+'param="cmd='+cmd+' '+value+'"',

		error : function(XMLHttpRequest, textStatus, errorThrown) {
            if(errcb)
                errcb();
		},
		success : function(data) {
            if(IsLoginPage(data))
            {
                if(errcb)
                    errcb();
            }

            if(successcb)
                successcb(data);
		}
	});
}

function RequestToServerIsOK(data) {
    ret= $(data).find("result").text();
    if(ret == 'success')
      return true;
    else
      return false;
}

/**
 * unicode编码
 * @method encodeMessage
 * @param textString {String}
 * @return {String} 
 */
function SubmitEncodeMessage(textString) {
	var haut = 0;
	var result = '';
    if (!textString) return result;
	for ( var i = 0; i < textString.length; i++) {
		var b = textString.charCodeAt(i);
		if (haut != 0) {
			if (0xDC00 <= b && b <= 0xDFFF) {
				result += dec2hex(0x10000 + ((haut - 0xD800) << 10) + (b - 0xDC00));
				haut = 0;
				continue;
			} else {
				haut = 0;
			}
		}
		if (0xD800 <= b && b <= 0xDBFF) {
			haut = b;
		} else {
			cp = dec2hex(b);
			while (cp.length < 4) {
				cp = '0' + cp;
			}
			result += cp;
		}
	}
	return result;
}

function dec2hex(textString) {
	return (textString + 0).toString(16).toUpperCase();
}
function hex2char(hex) {
	var result = '';
	var n = parseInt(hex, 16);
	if (n <= 0xFFFF) {
		result += String.fromCharCode(n);
	} else if (n <= 0x10FFFF) {
		n -= 0x10000;
		result += String.fromCharCode(0xD800 | (n >> 10)) + String.fromCharCode(0xDC00 | (n & 0x3FF));
	}
	return result;
}


/**
 * 显示等待遮罩
 * @method showLoading
 * @param {String} msg
 * @param {String} content loading 文本
 */
function showLoading(msg, content) {
	if (msg) {
		$('#loadMsg',top.document).html(msg);
	}
	top.$("#loading",top.doucment).modal({
		position : [ '30%' ],
		overlayId : 'confirm-overlay',
		containerId : 'confirm-container',
		minHeight : 140,
		persist : true,
        focus: false,
        escClose: false
	});
	if(content){
		$("#loading #loading_container",top.document).html(content);
	} else {
		$("#loading #loading_container",top.document).html("");
	}
}

/**
 * 修改等待遮罩的信息
 * @method loadingMsgChange
 * @param {String} msg
 */
function loadingMsgChange(msg) {
	$('#loadMsg',top.document).html(msg);
}

/**
 * 隐藏等待遮罩
 * @method hideLoading
 */
function hideLoading() {
	$('#confirm-overlay',top.document).css("cursor","default");
	top.$.modal.close();
	$('#loadMsg',top.document).html('');
}
