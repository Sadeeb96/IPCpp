#ifndef IPC_CLIENT
#define IPC_CLIENT
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>

#include "../Utils/FileHelper.h"
#include "../Utils/FileWatcher.h"
#include "../includes/pch.h"
class Client {
 private:
  string path;
  string fileName;
  const int retryCount = 50;
  const string clientPrefix = "_CLIENT_:";
  const string serverPrefix = "_SERVER_:";

 private:
  void init() {
    this->path = getpwuid(getuid())->pw_dir;
    this->path += "/ascipc";
  }
  string getFullPath() { return path + "/" + fileName; }
  bool checkServer() { return checkFile(getFullPath()); }
  bool isServerMessage(string s) { return s.rfind(serverPrefix, 0) == 0; }
  void waitForReply(function<void(string)> callback) {
    while (!isServerMessage(readLastLine(getFullPath()))) {
    }
    callback(readLastLine(getFullPath()));
  }

 public:
  Client() { init(); }
  /*
  Description:
  Try to connect to IPC server with given name.

  Parameters:
  serverName: Name of the Server.

  Returns:
  True if connected succesfully or else false
   */
  bool connect(string serverName) {
    this->fileName = serverName;
    for (int i = 0; i < retryCount; i++) {
      if (checkServer()) {
        return true;
      }
    }
    return false;
  }

  /*
  Description:
  Send a message to Server. Dont wait for reply.

  Parameters:
  msg: Message in string format
  */
  bool send(string msg) {
    writeToEnd(getFullPath(), clientPrefix + msg);
    return true;
  }
  /*
  Description:
  Send a message to Server and wait for reply.

  Parameters:
  msg: Message in string format
  callback: Function to process the reply
  */
  bool send(string msg, function<void(string)> callback) {
    writeToEnd(getFullPath(), clientPrefix + msg);
    this_thread::sleep_for(chrono::milliseconds(1));

    thread waitThread(&Client::waitForReply, this, callback);
    waitThread.detach();
    return true;
  }
  ~Client() {}
};

#endif