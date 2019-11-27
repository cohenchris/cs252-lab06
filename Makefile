sysinfo: sysinfo.c procinfo.c devices.c main.c proc_actions.c graphinfo.c
	gcc -g -o taskmanager sysinfo.c procinfo.c devices.c proc_actions.c graphinfo.c main.c
