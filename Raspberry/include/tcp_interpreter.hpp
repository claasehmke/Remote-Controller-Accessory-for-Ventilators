#ifndef REMOTE_CONTROLLER_RASPI_TCP_TCP_INTERPRETER_H
#define REMOTE_CONTROLLER_RASPI_TCP_TCP_INTERPRETER_H

#include <iostream>
#include "stdint.h"
#include "tcp_client.hpp"

namespace remote_controller_tcp {

  class TcpInterpreter {
  public:
    virtual ~TcpInterpreter() {};
    virtual bool processMessage(uint32_t id, uint16_t length, const uint8_t msg[]) = 0;
    virtual bool init(TcpClient &client) = 0;
    virtual void onTcpOpen() = 0;
  };

}

#endif  // REMOTE_CONTROLLER_RASPI_TCP_TCP_INTERPRETER_H
