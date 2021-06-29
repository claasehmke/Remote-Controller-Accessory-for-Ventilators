#ifndef STEPPER_MOTOR_HPP
#define STEPPER_MOTOR_HPP

#include <vector>
#include <settings.hpp>

using namespace std;

class StepperMotor {
    public:
    StepperMotor();
        bool isRunning() const { return running; }
        unsigned getThreshold() const { return threshold; }
        int getCurrentPosition() const { return current_pos; }
        unsigned getSteps(unsigned angle) const;
        unsigned getNumOfSteps() const { return nsteps; }
        void setGPIOutputs(unsigned pin_dir, unsigned pin_step, unsigned pin_en);
        void setThreshold(unsigned threshold);
        void run(int direction, unsigned angle, unsigned speed);
        void wait(unsigned milliseconds) const;

    private:
        vector< vector<unsigned> > sequence;   // the switching sequence
        bool running;                          // state of the stepper motor
        unsigned threshold;                    // symmetric threshold in degrees
        int current_pos;                       // current position in degrees
        unsigned nsteps;                       // total number of steps from the beginning
        unsigned pin_dir, pin_step, pin_en;           // stepper motor driver inputs
};

#endif
