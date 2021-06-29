CMAKE_MINIMUM_REQUIRED(VERSION 3.5)

SET(BINARY_NAME Remote_Controller_Raspi)
PROJECT(${BINARY_NAME})
set(CMAKE_CXX_STANDARD 11)
## Compile as C++11
add_compile_options(-std=c++11)
#add_compile_options(-fext-numeric-literals)
add_compile_options(-lwiringPiDev -lwiringPi -lcrypt)


find_package(Boost REQUIRED COMPONENTS system thread date_time)

#set(HEADERS
#  ${CMAKE_CURRENT_SOURCE_DIR}/WiringPi/wiringPi/wiringPi.h

INCLUDE_DIRECTORIES(include
		stepper/include
		stepperSimple/include
		ws28128-rpi
		Raspberry-PI-Servo
		${Boost_INCLUDE_DIRS}
		${boost_INCLUDE_DIRS})


# Add binaries
ADD_EXECUTABLE(${BINARY_NAME}
		src/main.cpp
		src/tcp_client.cpp
		src/tcp_actuator_control_interpreter.cpp
		stepper/src/StepperMotor.cpp
		Raspberry-PI-Servo/libServo.cpp
		ws28128-rpi/ws2812-rpi.cpp
		)


find_library(WIRINGPI_LIBRARIES NAMES wiringPi )

target_link_libraries(${BINARY_NAME}
		${Boost_LIBRARIES}
		${boost_LIBRARIES}
		${WIRINGPI_LIBRARIES}
		crypt
		)
