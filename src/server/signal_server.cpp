#include "server/signal_server.h"

#include <iostream>

SignalServer::SignalServer(const size_t port) : port_(port) {}

SignalServer::~SignalServer() {
  if (server_ && !server_->stopped()) {
    server_->stop();
  }

  server_.reset();
}

void SignalServer::Start() {
  if(!server_) {
    server_.reset(new std::unique_ptr<server>());
  }

  if(!server_->stopped()) {

  }
}