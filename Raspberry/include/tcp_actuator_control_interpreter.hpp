#ifndef REMOTE_CONTROLLER_RASPI_TCP_ACTUATOR_CONTROL_INTERPRETER_HPP
#define REMOTE_CONTROLLER_RASPI_TCP_ACTUATOR_CONTROL_INTERPRETER_HPP


#include <iostream>
#include "settings.hpp"
#include "stdint.h"
#include "tcp_interpreter.hpp"
#include "tcp_client.hpp"
#include <tcp_messages.hpp>
#include "wiringPi.h"
#include "StepperMotor.hpp"
#include "libServo.h"

namespace remote_controller_tcp {

  class TcpActuatorControlInterpreter : public TcpInterpreter {
  public:
    TcpActuatorControlInterpreter(StepperMotor& stepper_motor, uint8_t pin_solenoid);
    ~TcpActuatorControlInterpreter(){};
    bool processMessage(uint32_t id, uint16_t length, const uint8_t data[]);
    bool init(TcpClient &socket);
    void onTcpOpen();
  private:
    TcpClient *client_;
    StepperMotor *stepper_motor_;
    Servo *servo_;
    uint8_t pin_solenoid_;
  };

}


#endif //REMOTE_CONTROLLER_RASPI_TCP_ACTUATOR_CONTROL_INTERPRETER_HPP
