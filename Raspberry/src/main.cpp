#include "wiringPi.h"
#include "settings.hpp"
#include "StepperMotor.hpp"
#include "libServo.h"
#include "tcp_client.hpp"
#include "tcp_interpreter.hpp"
#include "tcp_actuator_control_interpreter.hpp"
#include <string>
#include <iostream>
#include <max9611.h>

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define BLACK   "\033[30m"      /* Black */
#define LOG(text) std::cout<< "[MAIN] " << text << std::endl;
#define LOG_ERROR(text) std::cout << RED  << "[MAIN] " << text << RESET << std::endl;

using namespace remote_controller_tcp;

static TcpClient* client_ = NULL;
static std::vector<TcpInterpreter*> interpreters;
static TcpActuatorControlInterpreter* tcp_actuator_control_interpreter = NULL;
StepperMotor stepper_motor_;

void tcpMessageReceived(uint32_t id, uint16_t length, const uint8_t data[]) {

    LOG("TCP message received with id " << id << " (" << length << " bytes)");

    std::vector<TcpInterpreter*>::iterator it;
    bool processed = false;
    for (it = interpreters.begin(); it < interpreters.end(); it++) {
        processed = (*it)->processMessage(id, length, data);
        if (processed) break;
    }

    if (!processed) {
        LOG_ERROR("Unknown tcp message with id " << id);
    }
}


int main(int argc, char* argv[]) {        
    // wiringPi initialization
    wiringPiSetup();

    // Stepper motor initialization
    stepper_motor_.setGPIOutputs(PIN_STEPPER_MOTOR_DIR, PIN_STEPPER_MOTOR_STEP, PIN_STEPPER_MOTOR_ENABLE);
    
    // Write the stepper motor configuration in the log file
    LOG("PIN_STEPPER_DIR: " << PIN_STEPPER_MOTOR_DIR);
    LOG("PIN_STEPPER_STEP: " << PIN_STEPPER_MOTOR_STEP);
    LOG("PIN_STEPPER_ENABLE: " << PIN_STEPPER_MOTOR_ENABLE);

    // Solenoid initialization
    pinMode(PIN_SOLENOID, OUTPUT);
    digitalWrite(PIN_SOLENOID, !SOLENOID_ACTIVE_STATE);

    // Write the solenoid configuration in the log file
    LOG("PIN_SOLENOID: " << PIN_SOLENOID);

    //check for tablet connection
    string ping_command = string("ping -c 1 ") + string(TCP_HOST_IP_ADDRESS);
    while (system(ping_command.c_str()) != 0) {
        LOG(ping_command);
        LOG_ERROR("TABLET NOT IN NETWORK. IP: " << TCP_HOST_IP_ADDRESS);
        delay(1000);
    }

    // TCP connection initialization
    TcpClientSettings settings;
    settings.port = TCP_HOST_PORT;
    settings.receive_buffer_length = 100; //todo: change 

    // Retrieve TCP configuration parameters
    client_ = new TcpClient(tcpMessageReceived, settings);

    // Write the TCP configuration in the log file
    LOG("CONFIG | PORT: " << settings.port << ", RBL: " << settings.receive_buffer_length);

    // Actuator Control TCP interpreter initialization
    LOG("Enabling Actuator Control Interpreter...");
    tcp_actuator_control_interpreter = new TcpActuatorControlInterpreter(stepper_motor_, PIN_SOLENOID);
    tcp_actuator_control_interpreter->init(*client_);
    interpreters.push_back(tcp_actuator_control_interpreter);

    // If there are no interpreters then we might as well stop
    if (interpreters.size() == 0) {
        LOG_ERROR("Running TCP node without interpreters, this is useless, shutting down");
        return 0;
    }

    while (1) {

        // Open the TCP socket_ and start our loop
        if (client_->open()) {
            for (unsigned int i = 0; i < interpreters.size(); i++) {
                interpreters[i]->onTcpOpen();
            }
            while (true); //todo: handle this one
        }
        else {
            LOG_ERROR("Could not open TCP socket_ (socket_->open() failed)");
            //return -1;
        }
        delay(1000);
    }

        // Destroy all the interpreters
        std::vector<TcpInterpreter*>::iterator it;
        for (it = interpreters.begin(); it < interpreters.end(); it++) {
            delete* it;
        }


    return 0;
}
