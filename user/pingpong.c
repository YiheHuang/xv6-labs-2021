#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int parent_fd[2];  // 父进程向子进程写的管道
    int child_fd[2];   // 子进程向父进程写的管道
    pipe(parent_fd);   // 创建父->子管道
    pipe(child_fd);    // 创建子->父管道

    if (fork() == 0) {
        // 子进程
        char buf[10];
        read(parent_fd[0], buf, sizeof(buf)); // 从父进程读取数据
        int id = getpid();
        printf("%d: received %s\n", id, buf);
        write(child_fd[1], "pong", 4);       // 回写数据给父进程
        close(child_fd[0]);
        close(child_fd[1]);
    } else {
        // 父进程
        char buf[10];
        int id = getpid();
        write(parent_fd[1], "ping", 4);      // 写数据给子进程
        close(parent_fd[1]);
        int status;
        wait(&status);                        // 等待子进程结束
        read(child_fd[0], buf, sizeof(buf));  // 读取子进程回写的数据
        printf("%d: received %s\n", id, buf);
        close(child_fd[0]);
    }

    exit(0);
}
