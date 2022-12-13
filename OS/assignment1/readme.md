
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