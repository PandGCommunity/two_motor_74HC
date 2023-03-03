#include "stepm_74hc.h"

int stepm_74hc_gpio_init() {
    if(gpio_enable(DATA_GPIO) < 0) exit(1);
    if(gpio_enable(A0_GPIO) < 0) exit(1);
    if(gpio_enable(A1_GPIO) < 0) exit(1);
    if(gpio_enable(A2_GPIO) < 0) exit(1);

    if(gpio_enable(STOP_SENSOR_1_LEFT) < 0) exit(1);
    if(gpio_enable(STOP_SENSOR_1_RIGHT) < 0) exit(1);
    if(gpio_enable(STOP_SENSOR_2_LEFT) < 0) exit(1);
    if(gpio_enable(STOP_SENSOR_2_RIGHT) < 0) exit(1);

    gpio_set_dir(DATA_GPIO, 1);             // 1 = OUT
    gpio_set_dir(A0_GPIO, 1);               // 1 = OUT
    gpio_set_dir(A1_GPIO, 1);               // 1 = OUT
    gpio_set_dir(A2_GPIO, 1);               // 1 = OUT

    gpio_set_dir(STOP_SENSOR_1_LEFT, 0);    // 0 = IN
    gpio_set_dir(STOP_SENSOR_1_RIGHT, 0);   // 0 = IN
    gpio_set_dir(STOP_SENSOR_2_LEFT, 0);    // 0 = IN
    gpio_set_dir(STOP_SENSOR_2_RIGHT, 0);   // 0 = IN
    return SUCCESS;
}

int stepm_74hc_gpio_deinit() {
    if (gpio_check(DATA_GPIO)) gpio_disable(DATA_GPIO);
    if (gpio_check(A0_GPIO)) gpio_disable(A0_GPIO);
    if (gpio_check(A1_GPIO)) gpio_disable(A1_GPIO);
    if (gpio_check(A2_GPIO)) gpio_disable(A2_GPIO);
    if (gpio_check(STOP_SENSOR_1_LEFT)) gpio_disable(STOP_SENSOR_1_LEFT);
    if (gpio_check(STOP_SENSOR_1_RIGHT)) gpio_disable(STOP_SENSOR_1_RIGHT);
    if (gpio_check(STOP_SENSOR_2_LEFT)) gpio_disable(STOP_SENSOR_2_LEFT);
    if (gpio_check(STOP_SENSOR_2_RIGHT)) gpio_disable(STOP_SENSOR_2_RIGHT);
    return SUCCESS;
}

ERROR stepm_74hc_check_motor(MOTOR motor) {
    if ((motor == MOTOR_VERTICAL) || (motor == MOTOR_HORIZONTAL)) return SUCCESS;
    else {
        printf("%d: Invalid motor: %d\n", __LINE__, motor);
        // stepm_74hc_disable(MOTOR_HORIZONTAL);
        // stepm_74hc_disable(MOTOR_VERTICAL);
        stepm_74hc_gpio_deinit();
        exit(1);
    }
}

static inline int X1(MOTOR motor) {
    // H  H  H  = L  L  L  L  L  L  L  H for MOTOR_HORIZONTAL
    // H  H  L  = L  L  L  H  L  L  L  L for MOTOR_VERTICAL
    gpio_set_value(DATA_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 1);     // echo "1" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 1);     // echo "0" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, motor); // echo "1 or 0" > /sys/class/gpio/gpio24/value
    usleep(USLEEP_TIME);
    gpio_set_value(DATA_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio26/value
    return SUCCESS;
}

static inline int X2(MOTOR motor) {
    // L  H  H  = L  L  L  L  L  L  H  L for MOTOR_HORIZONTAL
    // L  H  L  = L  L  H  L  L  L  L  L for MOTOR_VERTICAL
    gpio_set_value(DATA_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 0);     // echo "0" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 1);     // echo "1" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, motor); // echo "1 or 0" > /sys/class/gpio/gpio24/value
    usleep(USLEEP_TIME);
    gpio_set_value(DATA_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio26/value
    return SUCCESS;
}

static inline int X3(MOTOR motor) {
    // H  L  H  = L  L  L  L  L  H  L  L for MOTOR_HORIZONTAL
    // H  L  L  = L  H  L  L  L  L  L  L for MOTOR_VERTICAL
    gpio_set_value(DATA_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 1);     // echo "1" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 0);     // echo "0" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, motor); // echo "1 or 0" > /sys/class/gpio/gpio24/value
    usleep(USLEEP_TIME);
    gpio_set_value(DATA_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio26/value
    return SUCCESS;
}

static inline int X4(MOTOR motor) {
    // L  L  H  = L  L  L  L  H  L  L  L for MOTOR_HORIZONTAL
    // L  L  L  = H  L  L  L  L  L  L  L for MOTOR_VERTICAL
    gpio_set_value(DATA_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 0);     // echo "0" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 0);     // echo "0" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, motor); // echo "1 or 0" > /sys/class/gpio/gpio24/value
    usleep(USLEEP_TIME);
    gpio_set_value(DATA_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio26/value
    return SUCCESS;
}

int stepm_74hc_disable(MOTOR motor) {
    stepm_74hc_check_motor(motor);

    X1(motor);
    X2(motor);
    X3(motor);
    X4(motor);
    return SUCCESS;
}

int stepm_74hc_calibrate(MOTOR motor) {
    stepm_74hc_check_motor(motor);

    int done = 0;
    int counter = 0;

    int sensors[2][2] = {
        { STOP_SENSOR_1_LEFT, STOP_SENSOR_1_RIGHT },
        { STOP_SENSOR_2_LEFT, STOP_SENSOR_2_RIGHT }
    };
    int sensor_side_1 = STOP_SENSOR_1_LEFT;
    int sensor_side_2 = STOP_SENSOR_1_RIGHT;
    while (!done) {
        int left_old_status, left_new_status = 0;
        int right_old_status, right_new_status = 0;
        gpio_get_value(sensors[motor][0], &left_old_status);
        gpio_get_value(sensors[motor][1], &right_old_status);
        stepm_74hc_step(motor, DIRECTION_CLOCKWISE);
        gpio_get_value(sensors[motor][0], &left_new_status);
        gpio_get_value(sensors[motor][1], &right_new_status);
        if ((left_old_status != left_new_status) || (right_old_status != right_new_status)) {
            done = 1;
        }
    }

    done = 0;
    while (!done) {
        counter++;
        int left_old_status, left_new_status = 0;
        int right_old_status, right_new_status = 0;
        gpio_get_value(sensors[motor][0], &left_old_status);
        gpio_get_value(sensors[motor][1], &right_old_status);
        stepm_74hc_step(motor, DIRECTION_UCLOCKWISE);
        gpio_get_value(sensors[motor][0], &left_new_status);
        gpio_get_value(sensors[motor][1], &right_new_status);
        if ((left_old_status != left_new_status) || (right_old_status != right_new_status)) {
            done = 1;
        }

    }
    printf("%d: Motor steps: %d\n", __LINE__, counter);
    return 0;
}

int stepm_74hc_step(MOTOR motor, DIRECTION dir) {
    stepm_74hc_check_motor(motor);

    if (dir == DIRECTION_CLOCKWISE) {
        X4(motor); // L  L  X
        X3(motor); // H  L  X
        X2(motor); // L  H  X
        X1(motor); // H  H  X
    } else if (dir == DIRECTION_UCLOCKWISE) {
        X1(motor); // H  H  X
        X2(motor); // L  H  X
        X3(motor); // H  L  X
        X4(motor); // L  L  X
    } else {
        printf("%d: Invalid direction\n", __LINE__);
        return ERROR_INVALID_DIRECTION;
    }
    return SUCCESS;
}
