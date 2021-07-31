#include "includes/pch.h"
#include "Server/Server.h"
#include "Client/Client.h"

#define CLIENT

Server* server;
Client* client;
void testServer(){
    server = new Server("demo",[&](string msg){
        cout<<"Server Recieved Message: "<<msg<<endl;
        server->reply("Recieved");
    });
    server->startServer();
}
void testClient(){
    client  = new Client();
    if(client->connect("demo")){
        string msg;
        getline(cin,msg);
        client->send(msg,false);
    }
}
int main(){
    
    #ifdef CLIENT
    testClient();
    #endif

    #ifdef SERVER
    testServer();
    int x;
    cin>>x;
    #endif

}

