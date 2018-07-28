#include "LArStudies/tools.h"

std::vector<string> split(const string &text, char sep) {
  std::vector<string> tokens;
  int start = 0, end = 0;
  while ((end = text.find(sep, start)) != int(string::npos) ) {
    string tok=text.substr(start, end - start);
    //cout<<tok<<endl;
    tokens.push_back(tok);
    start = end + 1;
  }
  tokens.push_back(text.substr(start));
  return tokens;
}

std::string findMatchInDir(const char* dirpath, const char* match){
  DIR * Dir = opendir(dirpath);
  if(Dir == NULL){ cout<<"path not found: "<<dirpath<<endl; return "";}
  struct dirent * DirEntry;
  while((DirEntry=readdir(Dir)) != NULL )
    if ( TString(DirEntry->d_name).Contains(match))
      return std::string(DirEntry->d_name);
  closedir(Dir);
  return "";
}


std::vector<std::string> findSamplesInDir(const char* dirpath, const char* match){
  std::vector<std::string> samples;
  DIR * Dir = opendir(dirpath);
  if(Dir == NULL){ cout<<"findSamplesInDir() : path not found: "<<dirpath<<endl; return samples;}
  struct dirent * DirEntry;
  while((DirEntry=readdir(Dir)) != NULL )
    if ( TString(DirEntry->d_name).Contains(match))
      samples.push_back(std::string(DirEntry->d_name));
  closedir(Dir);  
  return samples;
}



