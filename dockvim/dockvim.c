/*
 * Written by ryuichi1208 (ryucrosskey@gmail.com)
 */

#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>

#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/stat.h>

/*
#include <linux/errno.h>
#include <linux/types.h>
*/

typedef unsigned short u_16;
typedef unsigned int u_32;
typedef unsigned long long int u_64;

#undef DEBUG

#ifdef DEBUG
#define dprintf(x...)	printf(x)
#else
#define dprintf(x...)
#endif

typedef struct namespace {
    int pid;
    char *type;
} namespace;

void err_exit(char *msg, int err)
{
    perror(msg);
    exit(1);
}

int get_container_pid(char *container_name)
{
    int fd;
    int ret;
    u_32 pid;
    char *endptr;
    char dock_cmd[64];
    char tmp_file[64];
    char exec_cmd[128];
    char pid_buf[16];
    struct stat stat_buf;

    sprintf(dock_cmd, "docker container inspect %s --format={{.Id}}", container_name);
    sprintf(tmp_file, "/tmp/%s_%d.tmp", container_name, getpid());
    sprintf(exec_cmd, "%s > %s", dock_cmd, tmp_file);

    ret = system(exec_cmd);
    if (ret != 0)
        err_exit("failed system", errno);

    fd = open(tmp_file, O_RDONLY|O_EXCL);
    if (fd == -1)
        err_exit("failed open", errno);

    ret = stat(tmp_file, &stat_buf);
    if (ret)
        err_exit("failed stat", errno);

    ret = read(fd, pid_buf, stat_buf.st_size);
    if (ret == -1)
        err_exit("failed read", errno);

    return strtol(pid_buf, &endptr, 10);
    //return ret;

}

void open_namespace(namespace *n)
{
    u_16 i;
    int fd;
    char procfile[2][64];

    for (i = 0; i < 2; i++) {
        sprintf(*(procfile+i), "/prot/%d/ns/%s", (n+1)->pid, (n+i)->type);
    }
}

void checl_enviroment()
{
    int ret;
    struct utsname s;

	ret = uname(&s);
    if (ret) {
        err_exit("failed uname", errno);
    }

    if (strcmp(s.sysname, "Linux")) {
        err_exit("Not supported OS", errno);
    }
}

int main(int argc, char **argv)
{
    u_16 i;
    namespace *n = malloc(sizeof(namespace) * 2);

    checl_enviroment();

    for (i = 0; i < 2; i++) {
        (n+i)->pid = 20;
        (n+i)->type = "abs";
    }
    open_namespace(n);

    int pid = get_container_pid("nginx_1");
    printf("%d\n", pid);

    return 0;
}
