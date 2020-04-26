#include <bits/stdc++.h>
using namespace std;

#define A_COMMAND 0;
#define C_COMMAND 1;
#define L_COMMAND 2;

string inputfile, outputfile;

class Paser {
public:
    ifstream ifs;
    string cmd;

    Paser() {
        ifs.open(inputfile);

        if (ifs.fail()) {
            cerr<<"Failed to open file."<<endl;
            exit(0);
        }
    }

    bool hasMoreCommand() {
        return !ifs.eof();
    }

    void advance() {
        getline(ifs, cmd);
    }

    int commandType() { //返り値
        if (cmd[0] == '@') {
            return A_COMMAND;
        } else if (cmd[0] == '(') {
            return L_COMMAND;
        } else {
            return C_COMMAND;
        }
    }

    string dest() {

    }

    string comp() {

    }

    string jump() {

    }
};

class Code {
public:
    string dest(string s) {

    }
};

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr<<"invalid arguments"<<endl;
        exit(0);
    }

    inputfile = "./" + (string)argv[2] + ".asm";

    Paser();

    return 0;
}
 
