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
  // pull out the type of messages sent by our config
  typedef server::message_ptr message_ptr;

  void OnMessageHandler(std::shared_ptr<server> s,
                        websocketpp::conn ection_hdl hdl,
                        message_ptr msg);
  void OnConnectSuccHandler(std::shared_ptr<server> s,
                            websocketpp::connection_hdl hdl);
  void OnConnectFailHandler(std::shared_ptr<server> s,
                            websocketpp::connection_hdl hdl);
  void OnCloseHandler(std::shared_ptr<server> s,
                      websocketpp::connection_hdl hdl);

  void ParseMessage(std::shared_ptr<server> s,
                    websocketpp::connection_hdl hdl,
                    const std::string& message);

  std::shared_ptr<server> server_;

  size_t port_;
};

#endif  // SRC_SERVER_SIGNAL_SERVER_H_