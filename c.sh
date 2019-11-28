gcc `pkg-config --cflags gtk+-3.0` -g -o sys_monitor sys_monitor.c procinfo.c sysinfo.c `pkg-config --libs gtk+-3.0`
