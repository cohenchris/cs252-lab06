sysinfo: sysinfo.c procinfo.c devices.c main.c proc_actions.c
	gcc -g -o taskmanager sysinfo.c procinfo.c devices.c proc_actions.c main.c
