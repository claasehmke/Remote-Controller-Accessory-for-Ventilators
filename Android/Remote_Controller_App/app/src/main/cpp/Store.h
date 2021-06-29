#ifndef INMEMORYSTORAGE_STORE_H
#define INMEMORYSTORAGE_STORE_H

#include <cstdint>
#include "jni.h"
#include <android/log.h>
#include <iostream>

const uint32_t MSG_ACTUATOR_CONTROL_ID = 0x00;

struct MSG_ACTUATOR_CONTROL{
    const int32_t message_id = MSG_ACTUATOR_CONTROL_ID;
    int32_t time;
    int32_t rotation_update; //bool
    int32_t rotation; //steps in both directions
    int32_t push_update; //bool
    int32_t push; //bool
} __attribute__((packed));


MSG_ACTUATOR_CONTROL msg;

#endif //INMEMORYSTORAGE_STORE_H
