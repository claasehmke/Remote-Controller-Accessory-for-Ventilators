#ifndef REMOTE_CONTROLLER_RASPI_TCP_CLIENT_HPP
#define REMOTE_CONTROLLER_RASPI_TCP_CLIENT_HPP

#include <iostream>
#include <ctime>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <boost/thread/thread.hpp>
#include <arpa/inet.h>
#include "settings.hpp"


namespace remote_controller_tcp {

  typedef void (*tcpMessageCallback) (uint32_t id,
                                      uint16_t length,
                                      const uint8_t data[]);

  enum TcpSocketState {
    kTcpSocketConstructed = 0,
    kTcpSocketOpened,
    kTcpSocketClosed
  };

  struct TcpClientSettings {
    int port ;
    int receive_buffer_length;
  };

  class TcpClient {
  public:
      /**
      * Cosntructor of TcpClient
      * @param receive_callback Id of the message
      * @param settings 
      * @return
      */
    TcpClient(tcpMessageCallback receive_callback, const TcpClientSettings &settings);
    /**
    * Destructor of TcpClient
    */
    ~TcpClient();
    /**
    * Send a TCP message onto the bus
    * @param id Id of the message
    * @param length Length of the message in bytes
    * @param data Data of the message
    * @return
    */
    bool sendMessage(const void *msg, uint16_t length);

    void receiveMessage(uint8_t buf[], int count);
    /**
   * Open the tcp client, after this call messages tcp be sent and received
   * @return true if opening succeeded, false otherwise
   */
    bool open();
    /**
   * Close the tcp socket
   * @return true if closing succeeded, false otherwise
   */
    bool closeAll();
    /**
   * Get socket buffer length
   * @return length
   */
    int getReceiveBufferLength();

  private:
    bool openTcpInterface();
    bool closeTcpInterface();
    void receiveThreadLoop();
    int logTcpMessage(const uint8_t &message, const int length) const;
    boost::thread *receive_thread_;
    TcpSocketState socket_state_;
    tcpMessageCallback receive_callback_;
    const TcpClientSettings tcp_settings_;
    int tcp_server_fd_;
    const int opt_ = 1;
    struct sockaddr_in serv_addr;
    uint8_t *receive_buffer_;
  };
}

#endif //REMOTE_CONTROLLER_RASPI_TCP_CLIENT_HPP
