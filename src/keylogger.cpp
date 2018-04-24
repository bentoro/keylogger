#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "epoll.h"
int main(int argc, char **argv){
    int fd, epollfd;
    struct epoll_event event;
    struct epoll_event *events;


    epollfd = createEpollFd();
    if(fd = open("/dev/input/event0",O_RDONLY) == -1){
        perror("open");
    }
    if(fcntl(fd, F_SETFL, O_NONBLOCK) == -1){
        perror("nonblocking");
    }

    event.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET;
    event.data.fd = fd;
    addEpollSocket(epollfd, fd, &event);

    events = (epoll_event *) calloc(64, sizeof(event));

    while(1){
        int fds;
        struct input_event ev;

        fds = waitForEpollEvent(epollfd, events);
        for(int i = 0; i < fds; i++){
            read(fd, &ev, sizeof(struct input_event));
            printf("Key %i state %i \n", ev.code, ev.value);
        }
    }
}
