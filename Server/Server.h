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

private:
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
        fw = new FileWatcher("/home/ahsan/ascipc", std::chrono::milliseconds(1000));
    }
    void dispatch(string path){
        if(path != (this->path + "/" + this->fileName)){
            return;
        }
        callback(readLastLine(path));
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
    Server(string fn,function<void(string)>cf) : fileName(fn+"ServerFile"),callback(cf)
    {
        init();
        
    }
    
    bool startServer()
    {
        thread watcherThread(&Server::startWatching,this);
        watcherThread.detach();
        return true;
    }
    bool stopServer(){
        fw->stop();
        return true;
    }
    ~Server() {}
};

#endif
