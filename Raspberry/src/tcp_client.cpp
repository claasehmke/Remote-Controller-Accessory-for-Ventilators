#include "tcp_client.hpp"
#include <cstring>

#define RESET   "\033[0m"
#define RED     "\033[31m"      /* Red */
#define BLACK   "\033[30m"      /* Black */
#define LOG(text) std::cout<< "[TCPCLINET] " << text << std::endl;
#define LOG_ERROR(text) std::cout << RED  << "[TCPCLINET] " << text << RESET << std::endl;


namespace remote_controller_tcp {

  TcpClient::TcpClient(tcpMessageCallback receive_callback,
                       const TcpClientSettings &settings)
      : tcp_settings_(settings) {
    socket_state_ = kTcpSocketConstructed;
    receive_thread_ = NULL;

    receive_callback_ = receive_callback;
    tcp_server_fd_ = -1;

    // Allocate receive buffer
    receive_buffer_ = new uint8_t[tcp_settings_.receive_buffer_length];
  }

  TcpClient::~TcpClient() {
    closeAll();
    if (receive_thread_) {
      try {
        receive_thread_->join();
      } catch (const boost::thread_interrupted &) {
        /* Not important */
      }
      delete receive_thread_;
    }
    delete[] receive_buffer_;
  }


  bool TcpClient::openTcpInterface() {

    // Creating socket file descriptor
    LOG("Opening TCP socket.");
    tcp_server_fd_ = socket(AF_INET, SOCK_STREAM, 0); //todo: check parameter
    if (tcp_server_fd_ == 0) {
      LOG_ERROR("Opening TCP socket failed: " << std::strerror(-tcp_server_fd_));
      return false;
    }

    LOG("Device opened with handle: " << tcp_server_fd_);
    return true;
  }

  bool TcpClient::closeTcpInterface() {
    LOG("Closing TCP socket");
    if (tcp_server_fd_ >= 0) {
      int status = close(tcp_server_fd_);
      if (status < 0) {
        LOG_ERROR("Closing TCP device failed: " << std::strerror(-status));
        return false;
      }
    }
    return true;
  }


  bool TcpClient::sendMessage(const void *msg, uint16_t length) {

  /*  if (socket_state_ != kTcpSocketOpened) {
      LOG_ERROR("TCP only send TCP message over open socket (state=)", socket_state_);
      return false;
    }

    int status = send(tcp_server_fd_, &msg, length, 0); //check for what the zero stands for
    if (status < 0) {
      LOG_ERROR("Could not send message over TCP: ", std::strerror(-status));
      return false;
    }*/
    return true;
  }


  void TcpClient::receiveMessage(uint8_t buf[], int count)
  {
    int num_bytes_received  = 0;
    int num_bytes_read  = 0;
    while (num_bytes_received <  count) {
      if ((count - num_bytes_received) > count) {
        num_bytes_read = read(tcp_server_fd_, &buf[num_bytes_received], count);
      }else {
        num_bytes_read = read(tcp_server_fd_, &buf[num_bytes_received],  count - num_bytes_received);
      }
      if (num_bytes_read > 0) {
        num_bytes_received += num_bytes_read;
      }else {
        LOG_ERROR("Receive error");
      }
    }
  };

  bool TcpClient::open() {
    if (socket_state_ == kTcpSocketConstructed) {
      if (!openTcpInterface()) {
        return false;
      }

      serv_addr.sin_family = AF_INET;
      serv_addr.sin_port = htons(tcp_settings_.port);

      // Convert IPv4 and IPv6 addresses from text to binary form
      if(inet_pton(AF_INET, TCP_HOST_IP_ADDRESS, &serv_addr.sin_addr)<=0)
      {
        LOG_ERROR("Invalid address/ Address not supported");
        return -1;
      }

      LOG("Connect to other socket on Port " << tcp_settings_.port << "...");
      //accept new socket
      int status_connection = connect(tcp_server_fd_, (struct sockaddr *) &serv_addr,
                                  sizeof(serv_addr));
      if (status_connection < 0) {
        LOG_ERROR("accept TCP socket failed: " << std::strerror(-status_connection));
        return false;
      }

      socket_state_ = kTcpSocketOpened;
      receive_thread_ = new boost::thread(&TcpClient::receiveThreadLoop, this);
      LOG("TCP socket successfully created.");
      return true;
    }

    return false;
  }

  bool TcpClient::closeAll() {
    if (socket_state_ == kTcpSocketOpened) {
      socket_state_ = kTcpSocketClosed;
      return closeTcpInterface();
    } else {
      return false;
    }
  }

  void TcpClient::receiveThreadLoop() {
    bool error = false;

    while (this->socket_state_ == kTcpSocketOpened && !error) {
      int length = read(tcp_server_fd_, receive_buffer_, 4);
      LOG("length of received message: "<< length);

      if (length < 0) {
        error = true;
        LOG_ERROR("Error during TCP receive: " << std::strerror(-length));
      } else {
        uint8_t id = receive_buffer_[0];
        LOG("RECEIVED " << length);
        receive_callback_(id, length, receive_buffer_);

      }
    }
  }

  int TcpClient::getReceiveBufferLength() {
    return tcp_settings_.receive_buffer_length;
  }



}