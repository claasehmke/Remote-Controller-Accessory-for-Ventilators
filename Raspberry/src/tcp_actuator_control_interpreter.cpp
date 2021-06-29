#include "tcp_actuator_control_interpreter.hpp"

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define BLACK   "\033[30m"      /* Black */
#define LOG(text) std::cout<< "[TCPACTCONINTERP] " << text << std::endl;
#define LOG_ERROR(text) std::cout << RED  << "[TCPACTCONINTERP] " << text << RESET << std::endl;

namespace remote_controller_tcp {

    TcpActuatorControlInterpreter::TcpActuatorControlInterpreter(StepperMotor& stepper_motor, uint8_t pin_solenoid) {
        client_ = NULL;
        stepper_motor_ = &stepper_motor;
        pin_solenoid_ = pin_solenoid_;
    }

    bool TcpActuatorControlInterpreter::init(TcpClient& client) {
        client_ = &client;
        return true;
    }

    void TcpActuatorControlInterpreter::onTcpOpen() {}

    bool TcpActuatorControlInterpreter::processMessage(uint32_t id, uint16_t length, const uint8_t* data) {
        bool processed = false;
        if (id == MSG_ACTUATOR_CONTROL_ID) {
            //receive rest of the start message
            uint8_t receive_buffer[sizeof(MSG_ACTUATOR_CONTROL)];
            client_->receiveMessage(&receive_buffer[4], sizeof(MSG_ACTUATOR_CONTROL) - 4); //id
            const MSG_ACTUATOR_CONTROL* msg = reinterpret_cast<const MSG_ACTUATOR_CONTROL*>(receive_buffer);

            LOG("Time: " << msg->time);
            if (msg->rotation_update)
            {
                LOG("Rot Update: 1");
            }
            else
            {
                LOG("Rot Update: 0");
            }
            LOG("Rot: " << msg->rotation);
            if (msg->push_update)
            {
                LOG("Push Update: 1");
            }
            else
            {
                LOG("Push Update: 0");
            }
            if (msg->push)
            {
                LOG("Push: 1");
            }
            else
            {
                LOG("Push: 0");
            }

            if (msg->rotation_update) {
                //Button rotation with stepper motor
                if (msg->rotation > 0)
                {
                    stepper_motor_->run(1, msg->rotation * STEPPER_MOTOR_STEP_RATIO, 100);

                }
                else {
                    stepper_motor_->run(-1, -msg->rotation * STEPPER_MOTOR_STEP_RATIO, 100);
                }
            }

            if (msg->push_update) {
                if (msg->push == true) {
                    digitalWrite(PIN_SOLENOID, SOLENOID_ACTIVE_STATE);
                }
                else {
                    digitalWrite(PIN_SOLENOID, !SOLENOID_ACTIVE_STATE);
                }
            }

            processed = true;
        }

        return processed;
    }

}
