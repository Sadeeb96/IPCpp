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
```Client``` can send message ```Server``` and can either wait for reply or not.
#### Send Message and Wait for Reply
Create a ```Client``` object and use ```connect()``` method to connect to a Server by server name. If connection sucessful send message to Server and pass a function which will process the reply from server. For example:
```
    Client* client  = new Client();
    if(client->connect("ServerName")){
        string msg;
        getline(cin,msg);
        client->send(msg,[](string msg){
            cout<<"Server Reply: "<<msg<<endl;
        });
    }
```
#### Send Message and Don't Wait for Reply
Create a ```Client``` object and use ```connect()``` method to connect to a Server by server name. If connection sucessful send message to Server. For example:
```
    Client* client  = new Client();
    if(client->connect("ServerName")){
        string msg;
        getline(cin,msg);
        client->send(msg);
    }
```

## Reference
1) https://github.com/sol-prog/cpp17-filewatcher
