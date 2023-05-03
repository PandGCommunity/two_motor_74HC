#include "stepm_74hc.h"
#include "app_version.h"

int main(int argc, char *argv[]) {
    printf("[%s] version: %s\n", argv[0], APP_VERSION);

    for (int i = 0; i < argc; ++i)
        printf("[%d]: %s\n", i, argv[i]);

    if (argc < 2) {
        printf("Usage: %s motor direction count_steps (motor[0..1], direction [0..1] and count_steps [1..sn] is numbers)\n", argv[0]);
        return 0;
    }

    int count = (int)atoi(argv[3]);
    MOTOR motor = (MOTOR)atoi(argv[1]);
    DIRECTION dir = (DIRECTION)atoi(argv[2]);

    printf("count: %d\n", count);
    printf("motor: %d\n", motor);
    printf("dir:   %d\n", dir);

    stepm_74hc_check_motor(motor);

    stepm_74hc_gpio_init();
    stepm_74hc_disable(MOTOR_HORIZONTAL);
    stepm_74hc_disable(MOTOR_VERTICAL);

    for (int i = 0; i < count; ++i) {
        stepm_74hc_step(motor, dir);
    }

    // stepm_74hc_disable(MOTOR_HORIZONTAL);
    // stepm_74hc_disable(MOTOR_VERTICAL);
    stepm_74hc_gpio_deinit();
    return 0;
}
