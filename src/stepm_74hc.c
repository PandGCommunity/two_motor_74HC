#include "stepm_74hc.h"

int stepm_74hc_gpio_init() {
    gpio_enable(DATA_GPIO);
    gpio_enable(A0_GPIO);
    gpio_enable(A1_GPIO);
    gpio_enable(A2_GPIO);
    gpio_set_dir(DATA_GPIO, 1); // 1 = OUT
    gpio_set_dir(A0_GPIO, 1);   // 1 = OUT
    gpio_set_dir(A1_GPIO, 1);   // 1 = OUT
    gpio_set_dir(A2_GPIO, 1);   // 1 = OUT
    return 0;
}

int stepm_74hc_gpio_deinit() {
    gpio_disable(DATA_GPIO);
    gpio_disable(A0_GPIO);
    gpio_disable(A1_GPIO);
    gpio_disable(A2_GPIO);
    return 0;
}

static inline void A() {
    // H  H  H  = L  L  L  L  L  L  L  H
    gpio_set_value(DATA_GPIO, 0); // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 1);   // echo "0" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio24/value
    usleep(USLEEP_TIME);
    gpio_set_value(DATA_GPIO, 1); // echo "1" > /sys/class/gpio/gpio26/value
}

static inline void B() {
    // L  H  H  = L  L  L  L  L  L  H  L
    gpio_set_value(DATA_GPIO, 0); // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio24/value
    usleep(USLEEP_TIME);
    gpio_set_value(DATA_GPIO, 1); // echo "1" > /sys/class/gpio/gpio26/value
}

static inline void C() {
    // H  L  H  = L  L  L  L  L  H  L  L
    gpio_set_value(DATA_GPIO, 0); // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio24/value
    usleep(USLEEP_TIME);
    gpio_set_value(DATA_GPIO, 1); // echo "1" > /sys/class/gpio/gpio26/value
}

static inline void D() {
    // L  L  H  = L  L  L  L  H  L  L  L
    gpio_set_value(DATA_GPIO, 0); // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio24/value
    usleep(USLEEP_TIME);
    gpio_set_value(DATA_GPIO, 1); // echo "1" > /sys/class/gpio/gpio26/value
}

static inline void E() {
    // H  H  L  = L  L  L  H  L  L  L  L
    gpio_set_value(DATA_GPIO, 0); // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio24/value
    usleep(USLEEP_TIME);
    gpio_set_value(DATA_GPIO, 1); // echo "1" > /sys/class/gpio/gpio26/value
}

static inline void F() {
    // L  H  L  = L  L  H  L  L  L  L  L
    gpio_set_value(DATA_GPIO, 0); // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio24/value
    usleep(USLEEP_TIME);
    gpio_set_value(DATA_GPIO, 1); // echo "1" > /sys/class/gpio/gpio26/value
}

static inline void G() {
    // H  L  L  = L  H  L  L  L  L  L  L
    gpio_set_value(DATA_GPIO, 0); // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio24/value
    usleep(USLEEP_TIME);
    gpio_set_value(DATA_GPIO, 1); // echo "1" > /sys/class/gpio/gpio26/value
}

static inline void H() {
    // L  L  L  = H  L  L  L  L  L  L  L
    gpio_set_value(DATA_GPIO, 0); // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio24/value
    usleep(USLEEP_TIME);
    gpio_set_value(DATA_GPIO, 1); // echo "1" > /sys/class/gpio/gpio26/value
}

int stepm_74hc_disable(MOTOR motor) {
    if (motor == MOTOR_HORIZONTAL) {
        A(); // H  H  H  = L  L  L  L  L  L  L  H
        B(); // L  H  H  = L  L  L  L  L  L  H  L
        C(); // H  L  H  = L  L  L  L  L  H  L  L
        D(); // L  L  H  = L  L  L  L  H  L  L  L
    } else if (MOTOR_VERTICAL) {
        E(); // H  H  L  = L  L  L  H  L  L  L  L
        F(); // L  H  L  = L  L  H  L  L  L  L  L
        G(); // H  L  L  = L  H  L  L  L  L  L  L
        H(); // L  L  L  = H  L  L  L  L  L  L  L
    } else {
        printf("Unknown motor\n");
        return -1;
    }
    return 0;
}

int motor_clockwise(MOTOR motor) {
    if (motor == MOTOR_HORIZONTAL) {
        D(); // L  L  H  = H  L  L  L  L  L  L  L
        C(); // H  L  H  = L  L  H  L  L  L  L  L
        B(); // L  H  H  = L  L  L  L  L  L  H  L
        A(); // H  H  H  = L  L  H  L  L  L  L  L
    } else if (MOTOR_VERTICAL) {
        H(); // L  L  L  = H  L  L  L  L  L  L  L
        G(); // H  L  L  = L  H  L  L  L  L  L  L
        F(); // L  H  L  = L  L  H  L  L  L  L  L
        E(); // H  H  L  = L  L  L  H  L  L  L  L
    } else {
        printf("Unknown motor\n");
        return -1;
    }
    return 0;
}

int motor_unclockwise(MOTOR motor) {
    if (motor == MOTOR_HORIZONTAL) {
        A(); // H  H  H  = L  L  H  L  L  L  L  L
        B(); // L  H  H  = L  L  L  L  L  L  H  L
        C(); // H  L  H  = L  L  H  L  L  L  L  L
        D(); // L  L  H  = H  L  L  L  L  L  L  L
    } else if (MOTOR_VERTICAL) {
        E(); // H  H  L  = L  L  L  H  L  L  L  L
        F(); // L  H  L  = L  L  H  L  L  L  L  L
        G(); // H  L  L  = L  H  L  L  L  L  L  L
        H(); // L  L  L  = H  L  L  L  L  L  L  L
    } else {
        printf("Unknown motor\n");
        return -1;
    }
    return 0;
}
