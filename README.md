# IPCpp
IPCpp is a light weight IPC library which uses shared-file to communicate between two seperate C++ applications.

## Build
Build using the following command
```
g++ -std=c++17 -pthread -o main.o main.cpp
```
## Run
Run using the following commad
```
chmod +x main.o
./main.o
```
## How to Use
### Server
First create a ```Server``` object with Name and Callback for handling recieved messages and call ```startServer```. For example:
```
    Server* server = new Server("ServerName",[&](string msg){
        cout<<"Server Recieved Message: "<<msg<<endl;
        // Process the recieved message
        server->reply("Recieved");// Optional
    });
    server->startServer();
```

### Client
Create a ```Client``` object and use ```connect()``` method to connect to a Server by server name. If connection sucessful send message to Server. For example:
```
    Client* client  = new Client();
    if(client->connect("ServerName")){
        string msg;
        getline(cin,msg);
        client->send(msg,false);
    }
```
## Reference
1) https://github.com/sol-prog/cpp17-filewatcher
