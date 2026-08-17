#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

void server(int rfd, int wfd)
{
    char fname[100], buff[1000];
    int n, fd;

    n = read(rfd, fname, sizeof(fname));
    fname[n] = '\0';

    fd = open(fname, O_RDONLY);

    if (fd < 0)
        write(wfd, "can't open", 10);
    else
    {
        n = read(fd, buff, sizeof(buff));
        write(wfd, buff, n);
        close(fd);
    }
}

void client(int wfd, int rfd)
{
    char fname[100], buff[1000];
    int n;

    scanf("%s", fname);
    write(wfd, fname, sizeof(fname));

    n = read(rfd, buff, sizeof(buff));
    write(1, buff, n);
}

int main()
{
    int p1[2], p2[2], pid;

    pipe(p1);
    pipe(p2);

    pid = fork();

    if (pid == 0)
    {
        close(p1[1]);
        close(p2[0]);
        server(p1[0], p2[1]);
    }
    else
    {
        close(p1[0]);
        close(p2[1]);
        client(p1[1], p2[0]);
        wait(NULL);
    }

    return 0;
}
