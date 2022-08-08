# Very Quick Introduction to GDB

> The purpose of a debugger such as GDB is to allow you to see what is going on “inside” another program while it executes—or what another program was doing at the moment it crashed.
>
> GDB can do four main kinds of things (plus other things in support of these) to help you catch bugs in the act:
>
> - Start your program, specifying anything that might affect its behavior.
> - Make your program stop on specified conditions.
> - Examine what has happened, when your program has stopped.
> - Change things in your program, so you can experiment with correcting the effects of one bug and go on to learn about another.

GDB: [The GNU Project Debugger](https://sourceware.org/gdb/documentation/) 

On-line user manual: [Debugging with GDB](https://sourceware.org/gdb/current/onlinedocs/gdb/)

## Debug binary

### Basic

Touch a C++ code named `test.cpp`

```c++
#include <iostream>
using namespace std;

int main()
{
    int arr[] = {1,2,3,4,5};
    for(int i = 0; i < 5; i++)
    {
        cout << arr[i] << endl;
    }

    return 0;
}     
```

instead of compile with `clang++ test.cpp`, use the flag `-g`, so 

```shell
$ clang++ -g test.cpp
```

then use `gdb`

```shell
$ gdb ./a.out
GNU gdb (GDB) Red Hat Enterprise Linux 7.6.1-114.el7
...
(gdb) r # short for run, just run anyway
Starting program: $PATH/a.out 
1
2
3
4
5
[Inferior 1 (process 147214) exited normally]
Missing separate debuginfos, use: debuginfo-install glibc-2.17-325.el7_9.x86_64 libgcc-4.8.5-44.el7.x86_64
(gdb) l 1 # list from the 1st line
1       #include <iostream>
2       using namespace std;
3
4       int main()
5       {
6           int arr[] = {1,2,3,4,5};
7           for(int i = 0; i < 5; i++)
8           {
9               cout << arr[i] << endl;
10          }
(gdb) b main # break on function <main>
Breakpoint 1 at 0x4007ef: file test.cpp, line 6.
(gdb) b 9	# break on line number
Breakpoint 2 at 0x40082d: file test.cpp, line 9.
(gdb) info b
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x00000000004007ef in main() at test.cpp:6
2       breakpoint     keep y   0x000000000040082d in main() at test.cpp:9
(gdb) r # run to the break 
Starting program: $PATH/a.out 

Breakpoint 1, main () at test.cpp:6
6           int arr[] = {1,2,3,4,5};
(gdb) n	# next
7           for(int i = 0; i < 5; i++)
(gdb) n # next

Breakpoint 2, main () at test.cpp:9
9               cout << arr[i] << endl;
(gdb) p arr # print arr
$1 = {1, 2, 3, 4, 5}
(gdb) p arr[0]
$2 = 1
(gdb) p &arr
$3 = (int (*)[5]) 0x7fffffffbca0
(gdb) p &arr[0]
$4 = (int *) 0x7fffffffbca0
(gdb) q # quit
A debugging session is active.

        Inferior 1 [process 149110] will be killed.

Quit anyway? (y or n) y
```

#### Multiple functions

add a function to the `test.cpp`:

```c++
#include <iostream>
using namespace std;

void printHello()
{
    cout << "hello" << endl;
}

int main()
{
    int arr[] = {1,2,3,4,5};
    for(int i = 0; i < 5; i++)
    {
        cout << arr[i] << endl;
    }

    printHello();

    return 0;
}
```

set break at `line17`, use `step` to go into the function.

```shell
$ gdb ./a.out
GNU gdb (GDB) Red Hat Enterprise Linux 7.6.1-114.el7
...
(gdb) l 1 # list from line number 1
1       #include <iostream>
2       using namespace std;
3
4       void printHello()
5       {
6           cout << "hello" << endl;
7       }
8
9       int main()
10      {
(gdb) l # list the rest of lines
11          int arr[] = {1,2,3,4,5};
12          for(int i = 0; i < 5; i++)
13          {
14              cout << arr[i] << endl;
15          }
16
17          printHello();
18
19          return 0;
20      }(gdb) l
Line number 21 out of range; test.cpp has 20 lines.
(gdb) b 17
Breakpoint 1 at 0x40091e: file test.cpp, line 17.
(gdb) info b
Num     Type           Disp Enb Address            What
1       breakpoint     keep y   0x000000000040091e in main() at test.cpp:17
(gdb) r # run
Starting program: $PATH/a.out 
1
2
3
4
5

Breakpoint 1, main () at test.cpp:17
17          printHello();
Missing separate debuginfos, use: debuginfo-install glibc-2.17-325.el7_9.x86_64 libgcc-4.8.5-44.el7.x86_64
(gdb) s # short for step
printHello () at test.cpp:6
6           cout << "hello" << endl;
(gdb) n # short for next
hello
7       }
(gdb) n
main () at test.cpp:19
19          return 0;
(gdb) n
0x00007ffff7517555 in __libc_start_main () from /lib64/libc.so.6
```

### Other

#### shell in gdb

DB support shell command with a `shell` command ahead

```shell
(gdb) shell ls
a.out  gdb_snippet.md  test.C  test.cpp
(gdb) shell rm -f test.C
(gdb) shell  ls
a.out  gdb_snippet.md  test.cpp
```

#### logs

debug with print logs

```shell
(gdb) set logging on
Copying output to gdb.txt.
(gdb) b  17
Breakpoint 1 at 0x40091e: file test.cpp, line 17.
(gdb) r
Starting program: $PATH/a.out 
1
2
3
4
5

Breakpoint 1, main () at test.cpp:17
17          printHello();
Missing separate debuginfos, use: debuginfo-install glibc-2.17-325.el7_9.x86_64 libgcc-4.8.5-44.el7.x86_64
(gdb) s
printHello () at test.cpp:6
6           cout << "hello" << endl;
(gdb) n
hello
7       }
(gdb) n
main () at test.cpp:19
19          return 0;
(gdb) n
0x00007ffff7517555 in __libc_start_main () from /lib64/libc.so.6
(gdb) q
A debugging session is active.

        Inferior 1 [process 446816] will be killed.

Quit anyway? (y or n) y
```

then, a `gdb.log` file is printed, looks like:

```txt
Breakpoint 1 at 0x40091e: file test.cpp, line 17.
Starting program: $PATH/a.out 

Breakpoint 1, main () at test.cpp:17
17          printHello();
Missing separate debuginfos, use: debuginfo-install glibc-2.17-325.el7_9.x86_64 libgcc-4.8.5-44.el7.x86_64
printHello () at test.cpp:6
6           cout << "hello" << endl;
7       }
main () at test.cpp:19
19          return 0;
0x00007ffff7517555 in __libc_start_main () from /lib64/libc.so.6
A debugging session is active.

        Inferior 1 [process 446816] will be killed.

Quit anyway? (y or n)
```

#### watchPoint

other break, other than `breakPoint`, there are`watchPoint`, `touchPoint`

have a look at `watchPoint` first:

```shell
$ gdb ./a.out
GNU gdb (GDB) Red Hat Enterprise Linux 7.6.1-114.el7
...
(gdb) b  12
Breakpoint 1 at 0x4008d1: file test.cpp, line 12.
(gdb) r
Starting program: $PATH/a.out 

Breakpoint 1, main () at test.cpp:12
12          for(int i = 0; i < 5; i++)
Missing separate debuginfos, use: debuginfo-install glibc-2.17-325.el7_9.x86_64 libgcc-4.8.5-44.el7.x86_64
(gdb) print &i
$1 = (int *) 0x7fffffffbccc
(gdb) watch *0x7fffffffbccc
Hardware watchpoint 2: *0x7fffffffbccc
(gdb) info w
Ambiguous info command "w": warranty, watchpoints, win.
(gdb) info wat
Num     Type           Disp Enb Address            What
2       hw watchpoint  keep y                      *0x7fffffffbccc
(gdb) n
14              cout << arr[i] << endl;
(gdb) n
1
12          for(int i = 0; i < 5; i++)
(gdb) n
Hardware watchpoint 2: *0x7fffffffbccc

Old value = 0
New value = 1
0x0000000000400919 in main () at test.cpp:12
12          for(int i = 0; i < 5; i++)
(gdb) n
14              cout << arr[i] << endl;
(gdb) n
2
12          for(int i = 0; i < 5; i++)
(gdb) 
Hardware watchpoint 2: *0x7fffffffbccc

Old value = 1
New value = 2
0x0000000000400919 in main () at test.cpp:12
12          for(int i = 0; i < 5; i++)
(gdb) n
14              cout << arr[i] << endl;
```

## Debug core file

touch a wrong file `wrongTest.cpp`

```c++
#include <iostream>
using namespace std;

int main()
{
    int * temp = NULL;
    *temp = 10;     // can't overwrite address of NULL as 10
    cout << *temp << endl;
    return 0;
}
```

```shell
$ clang++ -g wrongTest.cpp
$ ./a.out 
Segmentation fault (core dumped)
```

a segmentation fault generated.

1. set terminal able to generate `core` file when compiling

   ```shell
   $ ulimit -a
   core file size          (blocks, -c) 0
   data seg size           (kbytes, -d) unlimited
   scheduling priority             (-e) 0
   file size               (blocks, -f) unlimited
   pending signals                 (-i) 1029165
   max locked memory       (kbytes, -l) 64
   max memory size         (kbytes, -m) unlimited
   open files                      (-n) 4096
   pipe size            (512 bytes, -p) 8
   POSIX message queues     (bytes, -q) 819200
   real-time priority              (-r) 0
   stack size              (kbytes, -s) 8192
   cpu time               (seconds, -t) unlimited
   max user processes              (-u) 4096
   virtual memory          (kbytes, -v) unlimited
   file locks                      (-x) unlimited
   ```

   > `ulimit ` is only available in `bash`, not in `csh`.

   change the `core file size` to unlimited,

   ```shell
   $ ulimit -c unlimited
   $ ulimit -a
   core file size          (blocks, -c) unlimited  # change to unlimited
   data seg size           (kbytes, -d) unlimited
   scheduling priority             (-e) 0
   file size               (blocks, -f) unlimited
   pending signals                 (-i) 1029165
   max locked memory       (kbytes, -l) 64
   max memory size         (kbytes, -m) unlimited
   open files                      (-n) 4096
   pipe size            (512 bytes, -p) 8
   POSIX message queues     (bytes, -q) 819200
   real-time priority              (-r) 0
   stack size              (kbytes, -s) 8192
   cpu time               (seconds, -t) unlimited
   max user processes              (-u) 4096
   virtual memory          (kbytes, -v) unlimited
   file locks                      (-x) unlimited
   ```

   Then the system will generated a `core` file on segmentation fault.

2. recompile and generated the `core` file:

   ```shell
   $ ./a.out 
   Segmentation fault (core dumped)
   $ ls
   a.out  core.456484  gdb_snippet.md  gdb.txt  test.cpp  wrongTest.cpp
   ```

3. debug with GDB

   ```shell
   gdb ./a.out core.456484 
   GNU gdb (GDB) Red Hat Enterprise Linux 7.6.1-114.el7
   ...
   Reading symbols from $PATH/a.out...done.
   [New LWP 456484]
   Core was generated by `./a.out'.
   Program terminated with signal 11, Segmentation fault.
   #0  0x0000000000400803 in main () at wrongTest.cpp:7
   7           *temp = 10;     // can't overwrite address of NULL as 10
   Missing separate debuginfos, use: debuginfo-install glibc-2.17-325.el7_9.x86_64 libgcc-4.8.5-44.el7.x86_64
   ```

## Debug a running program

touch a endless file `endlessTest.cpp`

```c++
#include <iostream>
using namespace std;

void nothingTest()
{

}

void iTest()
{
    int i = 0;
    i++;
}

int main()
{
    for(;;) // endless loop
    {
        nothingTest();
        iTest();
    }
}
```

compile and run in the background

```shell
$ clang++ -g endlessTest.cpp 
$ ./a.out &
[1] 458198 # this is a pid
```

debug with GDB as normal

```shell
$ gdb -p 458198
GNU gdb (GDB) Red Hat Enterprise Linux 7.6.1-114.el7
...
Attaching to process 458198
Reading symbols from $PATH/a.out...done.
Reading symbols from /lib64/libstdc++.so.6...done.
Loaded symbols for /lib64/libstdc++.so.6
Reading symbols from /lib64/libm.so.6...(no debugging symbols found)...done.
Loaded symbols for /lib64/libm.so.6
Reading symbols from /lib64/libgcc_s.so.1...(no debugging symbols found)...done.
Loaded symbols for /lib64/libgcc_s.so.1
Reading symbols from /lib64/libc.so.6...(no debugging symbols found)...done.
Loaded symbols for /lib64/libc.so.6
Reading symbols from /lib64/ld-linux-x86-64.so.2...(no debugging symbols found)...done.
Loaded symbols for /lib64/ld-linux-x86-64.so.2
0x0000000000400685 in nothingTest () at endlessTest.cpp:7
7       }
Missing separate debuginfos, use: debuginfo-install glibc-2.17-325.el7_9.x86_64 libgcc-4.8.5-44.el7.x86_64
(gdb) n
main () at endlessTest.cpp:20
20              iTest();
(gdb) s
iTest () at endlessTest.cpp:11
11          int i = 0;
(gdb) n
12          i++;
(gdb) print i
$1 = 0
(gdb) n
13      }
(gdb) n
main () at endlessTest.cpp:21
21          }
```

remember to kill the process

```shell
$ top
   PID    USER      PR  NI    VIRT    RES    SHR S  %CPU %MEM     TIME+ COMMAND                                                                                                                                                
   458198 epblsr    20   0   11136   1340   1136 R 100.0  0.0   0:15.89 a.out 
```

```shell
$ kill 458198
```

