#include <cassert>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <wiringPi.h>
#include "StepperMotor.hpp"
#include <iostream>
using namespace std;

// Default constructor
StepperMotor::StepperMotor() {
    running = false;
    threshold = 0;
    current_pos = 0;
}



// Sets the GPIO outputs needed by inputs of the stepper motor driver (ULN2003APG)
// For more details concerning the wiringPi GPIO table conversion refere here:
// http://wiringpi.com/pins/
void StepperMotor::setGPIOutputs(unsigned pin_dir, unsigned pin_step, unsigned pin_en) {
    this->pin_dir = pin_dir;
    pinMode(pin_dir, OUTPUT);
    this->pin_step = pin_step;
    pinMode(pin_step, OUTPUT);
    this->pin_en = pin_en;
    pinMode(pin_en, OUTPUT);
    digitalWrite(pin_en, HIGH);
}


// Runs the stepper motor.
// * direction: 1 to go clockwise, -1 to go counterclockwise
// * angle: can assume values from 0 to 360 degrees
// * speed: from 20% (minimum speed) to 100% (maximum speed)
void StepperMotor::run(int direction, unsigned angle, unsigned speed) {
    float td;
    unsigned nsteps, count, ndegrees;

    running = true;

    // Check the direction and angle values
    assert(direction == 1 || direction == -1);

    // Check the speed value (5 speed modes allowed, from 20% to 100%)
    switch(speed) {
        case(20): break;
        case(40): break;
        case(60): break;
        case(80): break;
        case(100): break;
        default: return;
    }

    nsteps = angle;

    digitalWrite(pin_en, LOW);
    // To go counterclockwise we need to reverse the switching sequence
    if(direction == -1){
        digitalWrite(pin_dir, HIGH);
    }else{
        digitalWrite(pin_dir, LOW);
    }

    std::cout << "turn" <<std::endl;

    count = 0;
    for(unsigned i = 0; i < nsteps; i++) {

        digitalWrite(pin_step, HIGH);
        delayMicroseconds(STEPPER_MOTOR_DELAY_1);
        digitalWrite(pin_step, LOW);

        count++;
        delayMicroseconds(STEPPER_MOTOR_DELAY_2); // minimum delay 5ms (speed 100%), maximum delay 25ms (speed 20%)
    }

    digitalWrite(pin_en, HIGH);

    running = false;
}


// Sends to sleep the stepper motor for a certain amount of time (in milliseconds)
void StepperMotor::wait(unsigned milliseconds) const {
    delay(milliseconds);
}
