#ifndef IPC_CLIENT
#define IPC_CLIENT
#include "../includes/pch.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "../Utils/FileHelper.h"
#include "../Utils/FileWatcher.h"
class Client
{
private:
    string path;
    string fileName;
    const int retryCount = 50;
    const string clientPrefix = "_CLIENT_:";
    const string ServerPrefix = "_SERVER_:";
private:
    void init(){
        this->path = getpwuid(getuid())->pw_dir;
        this->path += "/ascipc";
    }
    string getFullPath(){
        return path+"/"+fileName;
    }
    bool checkServer(){
        return checkFile(getFullPath());
    }
public:
    Client(){
        init();
    }
    /* 
    Description:
    Try to connect to IPC server with given name.

    Parameters:
    serverName: Name of the Server.

    Returns:
    True if connected succesfully or else false
     */
    bool connect(string serverName){
        this->fileName = serverName;
        for(int i=0;i<retryCount;i++){
            if(checkServer()){
                return true;
            }
        }
        return false;
    }

    /* 
    Description:
    Send a message to Server.

    Parameters:
    msg: Message in string format
    wait: Wait for Reply
    */
    bool send(string msg,bool wait){
        writeToEnd(getFullPath(),clientPrefix+msg);
        return true;
    }
    ~Client(){

    }
};

#endif