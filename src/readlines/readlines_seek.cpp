#include "readlines.h"

using string = std::string;

template <typename T>
using vector = std::vector<T>;


//! From the beginning of the file, find the line in which keyword str appears.
/*!
  If str is found, the cursor is moved to the line just below.
  If noul is true, capital and small letters are not distinguished.
*/
bool ReadLines::seek_from_top(string str, bool noul){
  
  if(noul) transform(str.begin(), str.end(), str.begin(),::toupper);
  
  // Unless doing as follows, str with a letter added to its head or end would be detected.
  str = " " + str + " ";
  
  int i = 0;
  bool found = false;
  while(!found && i < lines.size()){
    string line = lines[i++];
    if(noul) transform(line.begin(), line.end(), line.begin(), ::toupper);
    if(line.find(str) != string::npos) found = true;
  }
  if(found){
    present = i;
    get_strs_present();
  }

  return found;
}


//! Look for the line from the present positon.
bool ReadLines::seek_from_here(string str, bool noul){
  int i = present;
  if(i >= lines.size()) return false;
  
  if(noul) transform(str.begin(), str.end(), str.begin(), ::toupper);
  
  // Unless doing as follows, str with a letter added to its head or end would be detected.
  str = " " + str + " ";
  
  bool found = false;
  while(!found && i < lines.size()){
    string line = lines[i++];
    if(noul) transform(line.begin(), line.end(), line.begin(), ::toupper);
    if(line.find(str) != string::npos) found = true;
  }
  if(found){
    present = i;
    get_strs_present();
  }

  return found;
}


//! Look for the line within the specified range.
bool ReadLines::seek_within(string str, int start, int end, bool noul = capital_small_distinguished){
  
  int i = start;
  if(i >= lines.size()) return false;
  
  if(noul) transform(str.begin(), str.end(), str.begin(), ::toupper);
  
  // Unless doing as follows, str with a letter added to its head or end would be detected.
  str = " " + str + " ";
  
  bool found = false;
  while(!found && i <= end){
    string line = lines[i++];
    if(noul) transform(line.begin(), line.end(), line.begin(), ::toupper);
    if(line.find(str) != string::npos) found = true;
  }
  if(found){
    present = i;
    get_strs_present();
  }

  return found;
}


//! Find the ind-th str from the beginning of the file.
/*!
  For the first str, ind must be set to 0.
*/
bool ReadLines::seek_str(string str, int ind, bool noul = capital_small_distinguished){
  bool found;
  found = seek_from_top(str, noul);
  for(int i = 0; i < ind; i++) found = seek_from_here(str, noul);
  return found;
}
