#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <sys/inotify.h>
#include "epoll.h"

int main(int argc, char **argv){
    int fd, epollfd;
    struct epoll_event event;
    struct epoll_event *events;


    epollfd = createEpollFd();

    fd = inotify_init1(IN_NONBLOCK | IN_CLOEXEC);
    if((inotify_add_watch(fd, "/dev/input/event0", IN_MODIFY)) != -1){
        perror("inotify_add_watch");
    }

    event.events = EPOLLIN | EPOLLET;
    event.data.fd = fd;
    addEpollSocket(epollfd, fd, &event);

    events = (epoll_event *) calloc(64, sizeof(event));

    while(1){
        int fds;
        struct input_event ev;

        fds = waitForEpollEvent(epollfd, events);
        for(int i = 0; i < fds; i++){
            printf("New Keystroke");
            read(fd, &ev, sizeof(struct input_event));
            printf("Key %i state %i \n", ev.code, ev.value);
        }
    }
}
