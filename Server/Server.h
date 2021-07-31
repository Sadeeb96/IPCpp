#ifndef IPC_SERVER
#define IPC_SERVER
#include "../includes/pch.h"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include "../Utils/FileHelper.h"
#include "../Utils/FileWatcher.h"
class Server
{
private:
    string path;
    string fileName;
    FileWatcher *fw = nullptr;
    function<void(string)> callback;
    const string clientPrefix = "_CLIENT_:";
    const string ServerPrefix = "_SERVER_:";

private:
    /* 
    Description:
    Check if IPC Directory exists if Not create

    Parameters:

    */    
    bool checkAndCreate()
    {
        if (directoryExists(this->path))
        {
            return true;
        }
        return createDirectory(this->path);
    }
    void init()
    {
        this->path = getpwuid(getuid())->pw_dir;
        this->path += "/ascipc";
        if (!checkAndCreate())
        {
            return;
        }
        createFile(this->path + "/" + this->fileName);
        fw = new FileWatcher(this->path, std::chrono::milliseconds(1000));
    }
    void dispatch(string path){
        if(path != (this->path + "/" + this->fileName)){
            return;
        }
        string lastMessage = readLastLine(path);
        if(isClientMessage(lastMessage)){
            callback(lastMessage);
        }
    }
    bool isClientMessage(string s){
        return s.rfind(clientPrefix,0)==0;
    }
    void startWatching()
    {
        fw->start([&](std::string path_to_watch, FileStatus status) -> void
                  {
                      // Process only regular files, all other file types are ignored
                      if (!std::filesystem::is_regular_file(std::filesystem::path(path_to_watch)) && status != FileStatus::erased)
                      {
                          return;
                      }

                      switch (status)
                      {
                      case FileStatus::created:
                         // std::cout << "File created: " << path_to_watch << '\n';
                          break;
                      case FileStatus::modified:
                          //std::cout << "File modified: " << path_to_watch << '\n';
                          dispatch(path_to_watch);
                          break;
                      case FileStatus::erased:
                         // std::cout << "File erased: " << path_to_watch << '\n';
                          break;
                      default:
                          std::cout << "Error! Unknown file status.\n";
                      }
                  });
    }

public:
    /* 
     Summary:
     Creates an IPC Server
     
     Paramaters:
        serverName: Name of the Server
        callBack: Callback function of type void(string)
     */    
    Server(string serverName,function<void(string)>callBack) : fileName(serverName),callback(callBack)
    {
        init();
    }
    bool reply(string msg){
        cout<<msg<<endl;
        return true;
    }

    /* 
    Description:
    Start IPC Server and listen for client messages

    Parameters:

    */
    bool startServer()
    {
        thread watcherThread(&Server::startWatching,this);
        watcherThread.detach();
        return true;
    }

    /* 
    Description:
    Stop IPC Server

    Parameters:
    
    */
    bool stopServer(){
        fw->stop();
        return true;
    }
    ~Server() {
        stopServer();
    }
};

#endif
