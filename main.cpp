#include "Client/Client.h"
#include "Server/Server.h"
#include "includes/pch.h"

#define CLIENT

Server *server;
Client *client;
void testServer() {
  server = new Server("demo1", [&](string msg) {
    cout << "Server Recieved Message: " << msg << endl;
    server->reply("Hello Client");
  });
  server->startServer();
}
void testClient() {
  client = new Client();
  if (client->connect("demo1")) {
    string msg;
    getline(cin, msg);
    client->send(msg,
                 [](string msg) { cout << "Server Reply: " << msg << endl; });
  }
  int x;
  cin >> x;
}
int main() {
#ifdef CLIENT
  testClient();
#endif

#ifdef SERVER
  testServer();
  int x;
  cin >> x;
  server->stopServer();
#endif
}
