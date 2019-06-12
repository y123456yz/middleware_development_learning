获取程序异常的调用栈信息，从而快速定位程序bug  
测试方法如下:  
===================================  
  
[root@localhost 基于redis信号和backtrace机制，快速实现backtrace代码，记录程序bug异常退出的函数调用栈，便于快速定位bug]# gcc backtrace.c -o backtrace  
[root@localhost 基于redis信号和backtrace机制，快速实现backtrace代码，记录程序bug异常退出的函数调用栈，便于快速定位bug]#   
[root@localhost 基于redis信号和backtrace机制，快速实现backtrace代码，记录程序bug异常退出的函数调用栈，便于快速定位bug]# ./backtrace  
I am function1  
I am function2  
receive signal:11  
Segmentation fault (core dumped)  
[root@localhost 基于redis信号和backtrace机制，快速实现backtrace代码，记录程序bug异常退出的函数调用栈，便于快速定位bug]#   
[root@localhost 基于redis信号和backtrace机制，快速实现backtrace代码，记录程序bug异常退出的函数调用栈，便于快速定位bug]# cat trace.log   
./backtrace[0x4007ba]  
./backtrace[0x400900]  
/lib64/libc.so.6(+0x35270)[0x7f0b3ecb7270]  
./backtrace[0x400900]  
./backtrace[0x40091e]  
./backtrace[0x400935]  
/lib64/libc.so.6(__libc_start_main+0xf5)[0x7f0b3eca3c05]  
./backtrace[0x400689]  
[root@localhost 基于redis信号和backtrace机制，快速实现backtrace代码，记录程序bug异常退出的函数调用栈，便于快速定位bug]#   
[root@localhost 基于redis信号和backtrace机制，快速实现backtrace代码，记录程序bug异常退出的函数调用栈，便于快速定位bug]# addr2line 0x4007ba -e backtrace -f   
logStackTrace  
??:?  
[root@localhost 基于redis信号和backtrace机制，快速实现backtrace代码，记录程序bug异常退出的函数调用栈，便于快速定位bug]# addr2line 0x400900 -e backtrace -f   
function2   
??:?  
[root@localhost 基于redis信号和backtrace机制，快速实现backtrace代码，记录程序bug异常退出的函数调用栈，便于快速定位bug]#   
[root@localhost 基于redis信号和backtrace机制，快速实现backtrace代码，记录程序bug异常退出的函数调用栈，便于快速定位bug]# addr2line 0x40091e -e backtrace -f   
function1  
??:?  
[root@localhost 基于redis信号和backtrace机制，快速实现backtrace代码，记录程序bug异常退出的函数调用栈，便于快速定位bug]#   
[root@localhost 基于redis信号和backtrace机制，快速实现backtrace代码，记录程序bug异常退出的函数调用栈，便于快速定位bug]# addr2line 0x400935 -e backtrace -f   
main  
??:?  
[root@localhost 基于redis信号和backtrace机制，快速实现backtrace代码，记录程序bug异常退出的函数调用栈，便于快速定位bug]#   
  
  
工作原理  
===================================  
程序异常将会收到SIGSEGV等异常信号，收到信号后会触发对应回调sigsegvHandler，在这里面借助backtrace、backtrace_symbols_fd来捕捉程序调用栈信息  
  
