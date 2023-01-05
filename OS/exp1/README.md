### run it
```
. install.sh
. remove.sh
dmesg |tail -15.
```

------

出现过的错误:
1. 错误使用reboot()函数
2. kernel_restart()函数也不行

去阅读了reboot.c之后，换用emergency_restart()函数就ok了！
