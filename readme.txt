honeycom 交互式试验箱：
2018-12-17：
修复：红绿灯与清屏之间交替使用的bug
修改：清屏函数设置为全屏清屏
问题：电机疑似损坏，无法驱动   
2018-12-19
优化：工程文件过大，超过了stm32f407ZET6的ram容量，造成程序烧写之后，无法运行的错误。通过删减冗余代码缩减程序大小，已经解决。
2018-12-28
优化：添加数码管开机时的初始化并熄灯，以解决开机有时会亮的问题。
优化：将两个屏幕显示控件中的多余初始化函数删减，以解决码表控件显示不正确的问题。       