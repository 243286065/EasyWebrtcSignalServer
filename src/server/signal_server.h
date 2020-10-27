#ifndef SRC_SERVER_SIGNAL_SERVER_H_
#define SRC_SERVER_SIGNAL_SERVER_H_

#include "websocketpp/config/asio_no_tls.hpp"
#include "websocketpp/server.hpp"

class SignalServer {
 public:
  SignalServer(const size_t port);

  ~SignalServer();

  void Start();

  void Stop();

 private:
  typedef websocketpp::server<websocketpp::config::asio> server;

  std::unique_ptr<server> server_;

  size_t port_;
};

#endif  // SRC_SERVER_SIGNAL_SERVER_H_