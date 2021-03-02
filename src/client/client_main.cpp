#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include "base/log/logging.h"
#include "protocol/message.h"

typedef websocketpp::client<websocketpp::config::asio_client> client;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef websocketpp::config::asio_client::message_type::ptr message_ptr;

// This message handler will be invoked once for each incoming message. It
// prints the message and then sends a copy of the message back to the server.
void on_message(client* c, websocketpp::connection_hdl hdl, message_ptr msg) {
    LOG(INFO) << "on_message called with hdl: " << hdl.lock().get()
              << " and message: " << msg->get_payload()
              << std::endl;


    websocketpp::lib::error_code ec;

    c->send(hdl, msg->get_payload(), msg->get_opcode(), ec);
    if (ec) {
        LOG(INFO) << "Echo failed because: " << ec.message() << std::endl;
    }
}

void on_open(client *c, websocketpp::connection_hdl hdl) {
    LOG(INFO) << "Connect server success: " << hdl.lock().get();
    std::string msg = "hello";
    c->send(hdl, msg, websocketpp::frame::opcode::text);
    c->get_alog().write(websocketpp::log::alevel::app, "Tx: " + msg);

    auto con = c->get_con_from_hdl(hdl);

    if(!con) {
        LOG(ERROR) << "get_con_from_hdl failed: ";
        return;
    }
    LoginMessage message;
    message.set_device_name("ClientDemo_1");
    LOG(INFO) << "Register info: " << message.SerializeAsJsonString();
    c->send(hdl, message.SerializeAsJsonString(), websocketpp::frame::opcode::text);
}

int main(int argc, char* argv[]) {
    // Create a client endpoint
    client c;

    std::string uri = "ws://localhost:9091";

    if (argc == 2) {
        uri = argv[1];
    }

    try {
        // Set logging to be pretty verbose (everything except message payloads)
        c.set_access_channels(websocketpp::log::alevel::all);
        c.clear_access_channels(websocketpp::log::alevel::frame_payload);

        // Initialize ASIO
        c.init_asio();

        // Register our message handler
        c.set_message_handler(bind(&on_message,&c,::_1,::_2));
        c.set_open_handler(bind(on_open, &c, ::_1));

        websocketpp::lib::error_code ec;
        client::connection_ptr con = c.get_connection(uri, ec);
        if (ec) {
            std::cout << "could not create connection because: " << ec.message() << std::endl;
            return 0;
        }

        // Note that connect here only requests a connection. No network messages are
        // exchanged until the event loop starts running in the next line.
        c.connect(con);

        // Start the ASIO io_service run loop
        // this will cause a single connection to be made to the server. c.run()
        // will exit when this connection is closed.
        c.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    }
}