#ifndef FILE_HELPER
#define FILE_HELPER
#include <dirent.h>
#include <sys/stat.h>
#include <fstream>
#include "../includes/pch.h"
#include <fstream>
#include <iomanip>
bool directoryExists(string path)
{
    cout << "Checking Directory: " << path << endl;
    DIR *dir = opendir(path.c_str());
    if (dir)
    {
        cout << "Directory Exists" << endl;
        closedir(dir);
        return true;
    }
    cout << "Directory Doesn't Exist" << endl;
    closedir(dir);
    return false;
}
bool createDirectory(string path)
{
    cout << "Creating Directory: " << path << endl;
    if (mkdir(path.c_str(), 0777) == 0)
    {
        cout << "Directory Created Sucesfully" << endl;
        return true;
    }
    cout << "Directory Creation Failed" << endl;
    return false;
}
void createFile(string path)
{
    cout << "Checking File: " << path << endl;
    ofstream outfile(path);
    outfile<<"START"<<endl;
    outfile.close();
}
string readLastLine(string path)
{
    ifstream fin;
    string myLine;
    cout<<"Reading File "<<path<<endl;
    fin.open(path);

    if(fin.is_open())
    {
        fin.seekg(-1,ios_base::end);
        fin.unget();
        while (fin.peek()!='\n')
        {
            myLine = (char)fin.peek() + myLine;
            fin.unget();
        }
        return myLine;
    }
    return "ERROR_";
}
void writeToEnd(string path,string msg){
    ofstream out;
    out.open(path, ios::app);
    out << msg <<endl;
    out.close();
}

bool checkFile (string name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}
#endif