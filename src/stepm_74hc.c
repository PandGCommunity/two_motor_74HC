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
    gpio_set_value(DATA_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio26/value
    usleep(USLEEP_TIME);
    return SUCCESS;
}

static inline int X2(MOTOR motor) {
    // L  H  H  = L  L  L  L  L  L  H  L for MOTOR_HORIZONTAL
    // L  H  L  = L  L  H  L  L  L  L  L for MOTOR_VERTICAL
    gpio_set_value(DATA_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 0);     // echo "0" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 1);     // echo "1" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, motor); // echo "1 or 0" > /sys/class/gpio/gpio24/value
    gpio_set_value(DATA_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio26/value
    usleep(USLEEP_TIME);
    return SUCCESS;
}

static inline int X3(MOTOR motor) {
    // H  L  H  = L  L  L  L  L  H  L  L for MOTOR_HORIZONTAL
    // H  L  L  = L  H  L  L  L  L  L  L for MOTOR_VERTICAL
    gpio_set_value(DATA_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 1);     // echo "1" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 0);     // echo "0" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, motor); // echo "1 or 0" > /sys/class/gpio/gpio24/value
    gpio_set_value(DATA_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio26/value
    usleep(USLEEP_TIME);
    return SUCCESS;
}

static inline int X4(MOTOR motor) {
    // L  L  H  = L  L  L  L  H  L  L  L for MOTOR_HORIZONTAL
    // L  L  L  = H  L  L  L  L  L  L  L for MOTOR_VERTICAL
    gpio_set_value(DATA_GPIO, 0);   // echo "0" > /sys/class/gpio/gpio26/value
    gpio_set_value(A0_GPIO, 0);     // echo "0" > /sys/class/gpio/gpio22/value
    gpio_set_value(A1_GPIO, 0);     // echo "0" > /sys/class/gpio/gpio23/value
    gpio_set_value(A2_GPIO, motor); // echo "1 or 0" > /sys/class/gpio/gpio24/value
    gpio_set_value(DATA_GPIO, 1);   // echo "1" > /sys/class/gpio/gpio26/value
    usleep(USLEEP_TIME);
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

int check_position(MOTOR motor) {
    stepm_74hc_check_motor(motor);
    int sensors[2][2] = {
        { STOP_SENSOR_1_LEFT, STOP_SENSOR_1_RIGHT },
        { STOP_SENSOR_2_LEFT, STOP_SENSOR_2_RIGHT }
    };
    int left_status = 0;
    int right_status = 0;
    gpio_get_value(sensors[motor][0], &left_status);
    if (left_status) return MAX_LEFT_POSITION;
    gpio_get_value(sensors[motor][1], &right_status);
    if (right_status) return MAX_RIGHT_POSITION;
    return UNKNOWN_POSITION;
}

int goto_end(MOTOR motor, DIRECTION dir, int sensor_fd) {
    int quit = 0;
    int steps = 0;

    while (!quit) {
        steps++;
        stepm_74hc_step(motor, dir);
        int result = gpio_get(sensor_fd, POLL_WAIT_TIME);
        if (result != -1) {
            // fprintf(stderr, "gpio triggered == %i\n", result);
            quit = 1;
        }
    }
    return steps;
}

CALIBRATION stepm_74hc_calibrate(MOTOR motor) {
    stepm_74hc_check_motor(motor);

    int sensors[2][2] = {
        { STOP_SENSOR_1_LEFT, STOP_SENSOR_1_RIGHT },
        { STOP_SENSOR_2_LEFT, STOP_SENSOR_2_RIGHT }
    };

    gpio_set_edge(STOP_SENSOR_1_LEFT,  "rising");
    gpio_set_edge(STOP_SENSOR_1_RIGHT, "rising");
    gpio_set_edge(STOP_SENSOR_2_LEFT,  "rising");
    gpio_set_edge(STOP_SENSOR_2_RIGHT, "rising");

    // gpio_set_active_low(STOP_SENSOR_1_LEFT,  1);
    // gpio_set_active_low(STOP_SENSOR_1_RIGHT, 1);
    // gpio_set_active_low(STOP_SENSOR_2_LEFT,  1);
    // gpio_set_active_low(STOP_SENSOR_2_RIGHT, 1);

    int left_1_fd  = gpio_poll(STOP_SENSOR_1_LEFT);
    int right_1_fd = gpio_poll(STOP_SENSOR_1_RIGHT);
    int left_2_fd  = gpio_poll(STOP_SENSOR_2_LEFT);
    int right_2_fd = gpio_poll(STOP_SENSOR_2_RIGHT);

    int fds[2][2] = {
        left_2_fd,
        right_2_fd,
        left_1_fd,
        right_1_fd
    };

    CALIBRATION ssf = {
        {
            .sensor_fd = fds[motor][0],
            .dir = DIRECTION_UCLOCKWISE,
        }, {
            .sensor_fd = fds[motor][1],
            .dir = DIRECTION_CLOCKWISE
        },
        .motor = motor,
        .max_steps = 0,
        .current_position = 0
    };

    goto_end(ssf.motor, ssf.info1.dir, ssf.info1.sensor_fd);

    ssf.max_steps = goto_end(ssf.motor, ssf.info2.dir, ssf.info2.sensor_fd);

    printf("%d: Motor steps: %d\n", __LINE__, ssf.max_steps);
    printf("%d: done, go to center\n", __LINE__);

    for (int i = 0; i < ssf.max_steps / 2; ++i) {
        stepm_74hc_step(motor, ssf.info1.dir);
    }
    ssf.current_position = ssf.max_steps / 2;

    return ssf;
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
