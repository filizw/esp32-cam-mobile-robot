#include "Vehicle4WD.hpp"

namespace ab {
    Vehicle4WD::Vehicle4WD() {
        m_leftWheels = 0;
        m_rightWheels = 0;
        m_speed = 50;
        m_pwmResolution = 256;
    }
    Vehicle4WD::Vehicle4WD(int left, int right, int speed) : m_leftWheels(left), m_rightWheels(right), m_speed(speed) {
        gpio_config_t io_conf = {
            .pin_bit_mask = (1ULL << MOTOR_IN1_PIN),
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE
        };
        gpio_config(&io_conf);

        gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
        gpio_set_direction(MOTOR_IN2_PIN, GPIO_MODE_OUTPUT);
        gpio_set_direction(MOTOR_IN3_PIN, GPIO_MODE_OUTPUT);
        gpio_set_direction(MOTOR_IN4_PIN, GPIO_MODE_OUTPUT);
        gpio_set_direction(MOTOR_ENA_PIN, GPIO_MODE_OUTPUT);
        gpio_set_direction(MOTOR_ENB_PIN, GPIO_MODE_OUTPUT);
        gpio_set_direction(SERVO_STEERING_PIN, GPIO_MODE_OUTPUT);

        leftWheelsDirection(m_leftWheels);
        rightWheelsDirection(m_rightWheels);

        ledc_timer_config_t ledc_timer_1 = {};
        ledc_timer_1.speed_mode = LEDC_LOW_SPEED_MODE;  // Tryb niskiej prędkości (wolniejsze czasy odpowiedzi)
        ledc_timer_1.timer_num = LEDC_TIMER_1;         // Wybór jednego z dostępnych timerów
        ledc_timer_1.duty_resolution = LEDC_TIMER_13_BIT; // Rozdzielczość PWM (2^13 = 8192 poziomów)
        ledc_timer_1.freq_hz = 50;                   // Częstotliwość sygnału PWM w Hz
        ledc_timer_1.clk_cfg = LEDC_AUTO_CLK;

        ledc_channel_config_t ledc_channel_1 = {};
        ledc_channel_1.speed_mode = LEDC_LOW_SPEED_MODE;
        ledc_channel_1.channel = LEDC_CHANNEL_1;
        ledc_channel_1.timer_sel = LEDC_TIMER_1;
        ledc_channel_1.intr_type = LEDC_INTR_DISABLE;
        ledc_channel_1.gpio_num = MOTOR_ENA_PIN;
        ledc_channel_1.duty = 0;
        ledc_channel_1.hpoint = 0;

        ledc_channel_config_t ledc_channel_2 = {};
        ledc_channel_2.speed_mode = LEDC_LOW_SPEED_MODE;
        ledc_channel_2.channel = LEDC_CHANNEL_1;
        ledc_channel_2.timer_sel = LEDC_TIMER_1;
        ledc_channel_2.intr_type = LEDC_INTR_DISABLE;
        ledc_channel_2.gpio_num = MOTOR_ENB_PIN;
        ledc_channel_2.duty = 0;
        ledc_channel_2.hpoint = 0;

        ledc_channel_config_t ledc_channel_3 = {};
        ledc_channel_3.speed_mode = LEDC_LOW_SPEED_MODE;
        ledc_channel_3.channel = LEDC_CHANNEL_3;
        ledc_channel_3.timer_sel = LEDC_TIMER_1;
        ledc_channel_3.intr_type = LEDC_INTR_DISABLE;
        ledc_channel_3.gpio_num = SERVO_STEERING_PIN;
        ledc_channel_3.duty = 0;
        ledc_channel_3.hpoint = 0;

        m_pwmResolution = pow(2, ledc_timer_1.duty_resolution)-1;

        ledc_timer_config(&ledc_timer_1);
        ledc_channel_config(&ledc_channel_1);
        ledc_channel_config(&ledc_channel_2);
        ledc_channel_config(&ledc_channel_3);

        //esp_rom_gpio_connect_out_signal(MOTOR_ENB_PIN, LEDC_LS_SIG_OUT1_IDX, false, false);

        setSpeed(m_speed);
        
        ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3, m_pwmResolution*(1.5/20.0));
        ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_3);
    }
    Vehicle4WD::~Vehicle4WD() {}

    void Vehicle4WD::leftWheelsDirection(int dir) {
        switch (dir) {
            case 0:
                m_leftWheels = 0;
                gpio_set_level(MOTOR_IN3_PIN, 0);
                gpio_set_level(MOTOR_IN4_PIN, 0);
                break;
            case 1:
                m_leftWheels = 1;
                gpio_set_level(MOTOR_IN3_PIN, 1);
                gpio_set_level(MOTOR_IN4_PIN, 0);
                break;
            case 2:
                m_leftWheels = 2;
                gpio_set_level(MOTOR_IN3_PIN, 0);
                gpio_set_level(MOTOR_IN4_PIN, 1);
                break;
            default:
                m_leftWheels = 0;
                gpio_set_level(MOTOR_IN3_PIN, 0);
                gpio_set_level(MOTOR_IN4_PIN, 0);
                break;
        }
    }
    void Vehicle4WD::rightWheelsDirection(int dir) {
        switch (dir) {
            case 0:
                m_rightWheels = 0;
                gpio_set_level(MOTOR_IN1_PIN, 0);
                gpio_set_level(MOTOR_IN2_PIN, 0);
                break;
            case 1:
                m_rightWheels = 1;
                gpio_set_level(MOTOR_IN1_PIN, 1);
                gpio_set_level(MOTOR_IN2_PIN, 0);
                break;
            case 2:
                m_rightWheels = 2;
                gpio_set_level(MOTOR_IN1_PIN, 0);
                gpio_set_level(MOTOR_IN2_PIN, 1);
                break;
            default:
                m_rightWheels = 0;
                gpio_set_level(MOTOR_IN1_PIN, 0);
                gpio_set_level(MOTOR_IN2_PIN, 0);
                break;
        }
    }

    void Vehicle4WD::setSpeed(int speed) {
        if (speed >= 0 && speed <= 100) {
            m_speed = speed;
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1, m_pwmResolution*(m_speed/100.0));
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_1);
        }
    }

    int Vehicle4WD::getPwmResolution() const {
        return m_pwmResolution;
    }

    int Vehicle4WD::getSpeed() const {
        return m_speed;
    }

    std::vector<int> Vehicle4WD::getDirection() const {
        std::vector<int> dirVector = {m_leftWheels, m_rightWheels};
        return dirVector;
    }

    void Vehicle4WD::driveForward(int duration) {
            leftWheelsDirection(1);
            rightWheelsDirection(1);

        if (duration > 0) {
            vTaskDelay(duration/portTICK_PERIOD_MS);
            stop();
        }
    }

    void Vehicle4WD::driveBackwards(int duration) {
            leftWheelsDirection(2);
            rightWheelsDirection(2);

        if (duration > 0) {
            vTaskDelay(duration/portTICK_PERIOD_MS);
            stop();
        }
    }

    void Vehicle4WD::stop() {
        leftWheelsDirection(0);
        rightWheelsDirection(0);
    }

    void Vehicle4WD::turnLeft(int turn) {
        leftWheelsDirection(2);
        rightWheelsDirection(1);

        if (turn > 0) {
            vTaskDelay(turn/portTICK_PERIOD_MS);
            stop();
        }
    }

    void Vehicle4WD::turnRight(int turn) {
        leftWheelsDirection(1);
        rightWheelsDirection(2);

        if (turn > 0) {
            vTaskDelay(turn/portTICK_PERIOD_MS);
            stop();
        }
    }

    void Vehicle4WD::makeSquare(int lr, int duration) {
        driveForward(duration);
        for (int i=0; i<3; i++) {
            if (lr) {
                turnRight(1500);
            } else {
                turnLeft(1500);
            }
            driveForward(duration);
        }
    }
}