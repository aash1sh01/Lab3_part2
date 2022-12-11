default:
	make shm_task
	make shm_proc
shm_task: shm_task.c
	gcc shm_task.c -D_SVID_SOURCE -D_GNU_SOURCE -Wall -std=c99 -Werror=cpp -pedantic  -o shm_task
shm_proc: shm_processes.c
	gcc shm_processes.c -D_SVID_SOURCE -D_GNU_SOURCE -Wall -std=c99 -Werror=cpp -pedantic  -o shm_proc
