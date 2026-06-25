#include <stdio.h>
#include  <stdlib.h>
#include<fcntl.h>
#include <unistd.h>

int main(void)
{
    char buffer[128];

    int fd=open("/proc/mydriver", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    ssize_t n= read(fd, buffer, sizeof(buffer) - 1);
    if (n < 0) {
        perror("read");
        close(fd);
        return 1;
    }


    buffer[n]= '\0';
    printf("kernel said: %s", buffer);

    close(fd);
    return 0;
}
