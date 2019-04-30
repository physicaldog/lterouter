//初始化加载
$(function(){

//zhezhaoBlock()
});

//重启
function restart(){
	layer.msg('设备正在重启中，请稍后...', {
  icon: 16,
  shade: 0.3,
  time:40000
});
setTimeout(function(){
	layer.msg("重启完成，准备跳转登录页面！");
	window.location.reload();
},40010);
}
//恢复出厂设置
function Restoration(){
	layer.msg('设备正在恢复出厂设置，请稍后...', {
  icon: 16,
  shade: 0.3,
  time:40000
});
setTimeout(function(){
	layer.msg("成功恢复出厂设置，准备跳转登录页面！");
	window.location.reload();
},40010);
}
