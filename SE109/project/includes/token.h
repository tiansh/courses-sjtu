#ifndef TOKEN_H
#define TOKEN_H

#include <fstream>
#include <map>
#include <string>
using namespace std;

typedef map<string, void*> tptk;
extern tptk tokens;

void* gettoken(ifstream &f);
#endif
