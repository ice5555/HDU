
```
. install.sh
. remove.sh
dmesg |tail -15.
```
现在这个不行
```
#include <unistd.h>
#include <linux/reboot.h>
```
可能是导致报错的原因


kernel_restart()不行，去阅读了reboot.c之后换用emergency_restart就ok了！
