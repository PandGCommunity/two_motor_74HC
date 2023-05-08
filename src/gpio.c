#include "gpio.h"

/****************************************************************
 * gpio_check
 ****************************************************************/
int gpio_check(unsigned int gpio) {
    int fd, len;
    char filename[PATH_MAX];

    len = snprintf(filename, sizeof(filename), SYSFS_GPIO_DIR  "/gpio%d", gpio);

    if (access(filename, F_OK) == 0) {
        return 1;
    } else {
        return 0;
    }
}

/****************************************************************
 * gpio_poll set GPIO line polling mode
 ****************************************************************/
int gpio_poll(unsigned int gpio) {
    char filename[PATH_MAX];
    int fd;
    char c;
    int err;

    snprintf(filename, sizeof(filename), SYSFS_GPIO_DIR  "/gpio%d/value", gpio);
    fd = open(filename, O_RDONLY);
    if (fd < 0) return -1;

    read(fd, &c, sizeof(c));

    return fd;
}

/****************************************************************
 * gpio_enable
 ****************************************************************/// get GPIO line value
int gpio_get(int fd, int timeout) {
    struct pollfd pollfd[1];
    char c;
    int err;

    pollfd[0].fd = fd;
    pollfd[0].events = POLLPRI | POLLERR;
    pollfd[0].revents = 0;

    err =  poll(pollfd, 1, timeout);
    if(err != 1) return -1;

    lseek(fd, 0, SEEK_SET);
    err = read(fd, &c, sizeof(c));
    if(err != 1) return -1;

    return c - '0';
}

/****************************************************************
 * gpio_enable
 ****************************************************************/
int gpio_enable(unsigned int gpio) {
    int fd, len;
    char filename[PATH_MAX];

    fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }

    len = snprintf(filename, sizeof(filename), "%d", gpio);
    write(fd, filename, len);
    close(fd);

    return 0;
}

/****************************************************************
 * gpio_disable
 ****************************************************************/
int gpio_disable(unsigned int gpio) {
    int fd, len;
    char filename[PATH_MAX];

    fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
    if (fd < 0) {
        perror("gpio/export");
        return fd;
    }

    len = snprintf(filename, sizeof(filename), "%d", gpio);
    write(fd, filename, len);
    close(fd);
    return 0;
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int gpio_set_dir(unsigned int gpio, unsigned int out_flag) {
    int fd, len;
    char filename[PATH_MAX];

    len = snprintf(filename, sizeof(filename), SYSFS_GPIO_DIR  "/gpio%d/direction", gpio);

    fd = open(filename, O_WRONLY);
    if (fd < 0) {
        perror("gpio/direction");
        return fd;
    }

    if (out_flag)
        write(fd, "out", 4);
    else
        write(fd, "in", 3);

    close(fd);
    return 0;
}

/****************************************************************
 * gpio_set_active_low
 ****************************************************************/
int gpio_set_active_low(unsigned int gpio, unsigned int mode_flag) {
    int fd, len;
    char filename[PATH_MAX];

    len = snprintf(filename, sizeof(filename), SYSFS_GPIO_DIR  "/gpio%d/active_low", gpio);

    fd = open(filename, O_WRONLY);
    if (fd < 0) {
        perror("gpio/active_low");
        return fd;
    }

    if (mode_flag)
        write(fd, "1", 2);
    else
        write(fd, "0", 2);

    close(fd);
    return 0;
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int gpio_set_value(unsigned int gpio, unsigned int value) {
    int fd, len;
    char filename[PATH_MAX];

    len = snprintf(filename, sizeof(filename), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(filename, O_WRONLY);
    if (fd < 0) {
        perror("gpio/set-value");
        return fd;
    }

    if (value)
        write(fd, "1", 2);
    else
        write(fd, "0", 2);

    close(fd);
    return 0;
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int gpio_get_value(unsigned int gpio, unsigned int *value) {
    int fd, len;
    char filename[PATH_MAX];
    char ch;

    len = snprintf(filename, sizeof(filename), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(filename, O_RDONLY);
    if (fd < 0) {
        perror("gpio/get-value");
        return fd;
    }

    read(fd, &ch, 1);

    if (ch != '0') {
        *value = 1;
    } else {
        *value = 0;
    }

    close(fd);
    return 0;
}


/****************************************************************
 * gpio_set_edge
 ****************************************************************/

int gpio_set_edge(unsigned int gpio, char *edge) {
    int fd, len;
    char filename[PATH_MAX];

    len = snprintf(filename, sizeof(filename), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

    fd = open(filename, O_WRONLY);
    if (fd < 0) {
        perror("gpio/set-edge");
        return fd;
    }

    write(fd, edge, strlen(edge) + 1);
    close(fd);
    return 0;
}

/****************************************************************
 * gpio_fd_open
 ****************************************************************/

int gpio_fd_open(unsigned int gpio) {
    int fd, len;
    char filename[PATH_MAX];

    len = snprintf(filename, sizeof(filename), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

    fd = open(filename, O_RDONLY | O_NONBLOCK );
    if (fd < 0) {
        perror("gpio/fd_open");
    }
    return fd;
}

/****************************************************************
 * gpio_fd_close
 ****************************************************************/

int gpio_fd_close(int fd) {
    return close(fd);
}
