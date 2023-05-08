#include <stdio.h>
// #include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <linux/limits.h>

 /****************************************************************
 * Constants
 ****************************************************************/
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

 /****************************************************************
 * Definitions
 ****************************************************************/
int gpio_check(unsigned int gpio);
int gpio_enable(unsigned int gpio);
int gpio_poll(unsigned int gpio);
int gpio_get(int fd, int timeout);
int gpio_disable(unsigned int gpio);
int gpio_set_dir(unsigned int gpio, unsigned int out_flag);
int gpio_set_active_low(unsigned int gpio, unsigned int mode_flag);
int gpio_set_value(unsigned int gpio, unsigned int value);
int gpio_get_value(unsigned int gpio, unsigned int *value);
int gpio_set_edge(unsigned int gpio, char *edge);
int gpio_fd_open(unsigned int gpio);
int gpio_fd_close(int fd);
