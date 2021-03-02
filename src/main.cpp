#include "server/signal_server.h"

int main() {
    SignalServer server(9091);
    server.Start();
    return 0;
}