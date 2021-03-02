#include "server/signal_server.h"

#include "base/json/json.h"
#include "base/log/logging.h"

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

SignalServer::SignalServer(const size_t port)
    : port_(port), server_(new server()) {
  // Set logging settings
  server_->set_access_channels(websocketpp::log::alevel::all);
  server_->clear_access_channels(websocketpp::log::alevel::frame_payload);

  // Initialize Asio
  server_->init_asio();
  server_->set_reuse_addr(true);

  // Register our message handler
  server_->set_message_handler(
      bind(&SignalServer::OnMessageHandler, this, server_, _1, _2));
  server_->set_open_handler(
      bind(&SignalServer::OnConnectSuccHandler, this, server_, _1));
  server_->set_fail_handler(
      bind(&SignalServer::OnConnectFailHandler, this, server_, _1));
  server_->set_close_handler(
      bind(&SignalServer::OnCloseHandler, this, server_, _1));
}

SignalServer::~SignalServer() {
  if (server_ && !server_->stopped()) {
    server_->stop();
  }

  server_.reset();
}

void SignalServer::Start() {
  if (server_->is_listening()) {
    LOG(WARNING) << "The server is already running!";
    return;
  }

  // Listen on port
  server_->listen(port_);

  // Start the server accept loop
  server_->start_accept();

  LOG(INFO) << "Start listen on port " << port_;
  // Start the ASIO io_service run loop
  server_->run();
}

void SignalServer::OnMessageHandler(std::shared_ptr<server> s,
                                    websocketpp::connection_hdl hdl,
                                    message_ptr msg) {
  LOG(INFO) << "OnMessageHandler called with hdl: " << hdl.lock().get()
            << " and message: " << msg->get_payload();
  ParseMessage(s, hdl, msg->get_payload());
}

void SignalServer::OnConnectSuccHandler(std::shared_ptr<server> s,
                                        websocketpp::connection_hdl hdl) {
  server::connection_ptr con = s->get_con_from_hdl(hdl);
  LOG(INFO) << "OnConnectSuccHandler called with hdl: " << hdl.lock().get()
            << ", address: " << con->get_remote_endpoint();
}

void SignalServer::OnConnectFailHandler(std::shared_ptr<server> s,
                                        websocketpp::connection_hdl hdl) {
  LOG(INFO) << "OnConnectFailHandler called with hdl: " << hdl.lock().get();
}

void SignalServer::OnCloseHandler(std::shared_ptr<server> s,
                                  websocketpp::connection_hdl hdl) {
  LOG(INFO) << "OnCloseHandler called with hdl: " << hdl.lock().get();
}

void ParseMessage(std::shared_ptr<server> s,
                  websocketpp::connection_hdl hdl,
                  const std::string& message) {
  base::Json json(message);
  LOG(INFO) << "ParseMessage: " << json.Str();
}
