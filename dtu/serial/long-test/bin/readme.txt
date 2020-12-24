使用方法：

程序运行之前，请先将UTX0和URX1短接、URX0和UTX1短接，然后执行long_test即可。

此时可以看到终端打印串口对发的次数、对发失败的次数以及程序运行的时间，如下所示：

open serila name:/dev/ttySP0
open serila name:/dev/ttySP1
serial test 1 times,failed 0 times
use time: 0-0 0:0:0
serial test 2 times,failed 0 times
use time: 0-0 0:0:0
serial test 3 times,failed 0 times
...
...

程序运行后会在当前目录下生成report.txt文件，只用来记录失败的操作。