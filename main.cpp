#include "includes/pch.h"
#include "Server/Server.h"
int main(){
    Server svr = Server("demo",[](string msg){
        cout<<"Server Recieved Message: "<<msg<<endl;
    });
    svr.startServer();
    int w;
    cin>>w;
    cout<<w<<endl;
    svr.stopServer();
    getchar();
    
    
}

