---
layout: post
title:  "Kernel debugging through a Segger J-Link JTAG adapter"
date:   2023-08-25 00:00:00 -0400
---

# Kernel debugging through J-Link

A Linux kernel can be debugged using GDB. Here are some notes of what worked for me.

Tested:
- AT91SAM9260-EK evaluation board (console through DBGU serial ttyS0)
- Segger J-Link EDU JTAG adapter.
- Segger GDB server 7.88 (free download)

Steps:

- Run J-Link GDB Server and configure it with the correct CPU type
- Compile the kernel with the following options:
```
CONFIG_GDB_SCRIPTS=y
CONFIG_FRAME_POINTER=y
CONFIG_KGDB=y
CONFIG_KGDB_SERIAL_CONSOLE=y
CONFIG_KGDB_KDB=y
```
- Enable kgdboc against the console ttyS0
`echo ttyS0 > /sys/module/kgdboc/parameters/kgdboc`
- Trigger SysRQ (this will hang the kerneli and go into KDB mode)
`echo g > /proc/sysrq-trigger`
- Switch KDB to KGDB
`kgdb`
- Now close any terminal emulators on the host to release the serial port. (Close teraterm/minicom/screen etc)
- Run gdb (`gdb ./vmlinux`). I used my buildroot host gdb in the kernel build dir:
`/test/host/bin/arm-buildroot-linux-uclibcgnueabi-gdb vmlinux`
- This was on a machine different from where GDBServer was running so:
`target remote 192.168.2.51:2331`
- Running lx-symbols should load symbols, modules and you can start debugging stuff:
```
(gdb) lx-symbols
loading vmlinux
scanning for modules in /test/build/linux-4.19.292
loading @0xbf04a000: /test/build/linux-4.19.292/crypto/algif_skcipher.ko
loading @0xbf042000: /test/build/linux-4.19.292/crypto/md5.ko
loading @0xbf03a000: /test/build/linux-4.19.292/crypto/algif_hash.ko
loading @0xbf032000: /test/build/linux-4.19.292/drivers/misc/atmel-ssc.ko
loading @0xbf028000: /test/build/linux-4.19.292/drivers/iio/adc/at91_adc.ko
loading @0xbf021000: /test/build/linux-4.19.292/drivers/iio/buffer/industrialio-triggered-buffer.ko
loading @0xbf01a000: /test/build/linux-4.19.292/drivers/iio/buffer/kfifo_buf.ko
loading @0xbf000000: /test/build/linux-4.19.292/drivers/iio/industrialio.ko
(gdb) apropos lx
function lx_current -- Return current task.
function lx_module -- Find module by name and return the module variable.
function lx_per_cpu -- Return per-cpu variable.
function lx_task_by_pid -- Find Linux task by PID and return the task_struct variable.
function lx_thread_info -- Calculate Linux thread_info from task variable.
function lx_thread_info_by_pid -- Calculate Linux thread_info from task variable found by pid
lx-cmdline --  Report the Linux Commandline used in the current kernel.
lx-cpus -- List CPU status arrays
lx-dmesg -- Print Linux kernel log buffer.
lx-fdtdump -- Output Flattened Device Tree header and dump FDT blob to the filename
lx-iomem -- Identify the IO memory resource locations defined by the kernel
lx-ioports -- Identify the IO port resource locations defined by the kernel
lx-list-check -- Verify a list consistency
lx-lsmod -- List currently loaded modules.
lx-mounts -- Report the VFS mounts of the current process namespace.
lx-ps -- Dump Linux tasks.
lx-symbols -- (Re-)load symbols of Linux kernel and currently loaded modules.
lx-version --  Report the Linux Version of the current kernel.
```


