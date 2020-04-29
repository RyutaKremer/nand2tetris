#include <bits/stdc++.h>
using namespace std;

#define A_COMMAND 0
#define C_COMMAND 1
#define L_COMMAND 2
#define NULL_COMMAND 3

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

        if (cmd.find("//") != -1) {
            cmd = cmd.substr(0, cmd.find("//"));
        }
        if (cmd.find("\r") != -1) {
            cmd = cmd.substr(0, cmd.find("\r"));
        }
        if (cmd.find("\n") != -1) {
            cmd = cmd.substr(0, cmd.find("\n"));
        }
        cout<<"--"<<cmd<<"--"<<endl;
    }

    int commandType() { //返り値
        if (cmd.find("@") != -1) {
            return A_COMMAND;
        } else if (cmd.find("(") != -1) {
            return L_COMMAND;
        } else if (cmd.find("=") != -1 || cmd.find(";") != -1) {
            return C_COMMAND;
        } else {
            return NULL_COMMAND;
        }
    }

    string symbol() {
        //とりあえずコメントアウトは考えない
        int ctype = commandType();
        string rstr;
        if (ctype == A_COMMAND) {
            rstr = cmd.substr(cmd.find("@"));
            if (rstr.find(" ") != -1)
                rstr = rstr.substr(0, rstr.find(" "));
            if (rstr.find("\t") != -1)
                rstr = rstr.substr(0, rstr.find("\t"));
        } else if (ctype == L_COMMAND) {
            rstr = cmd.substr(cmd.find("("), cmd.find(")"));
        } else {
            cerr<<"paser::symbol"<<endl;
        }

        return rstr;
    }

    string dest() {
        string rstr;

        for (int i = 0; i < 10; ++i) {
            if (cmd[i] == ' ' || cmd[i] == '\t') {
                continue;
            } else if (cmd[i] == '=') {
                break;
            } else {
                rstr += cmd[i];
            }
        }

        return rstr;
    }

    string comp() {
        string rstr;

        set<char> st = {'A', 'M', 'D', '0', '1',
                        '-', '+', '!', '&', '|'};

        if (cmd.find("=") == -1) {
            for (int i = 0; i < cmd.find(";"); ++i) {
                if (st.count(cmd[i])) {
                    rstr += cmd[i];
                }
            }

            return rstr;
        }

        for (int i = cmd.find("=") + 1; i < cmd.find("=") + 4; ++i) {
            if (st.count(cmd[i])) {
                rstr += cmd[i];
            } else {
                break;
            }
        }

        return rstr;
    }

    string jump() {
        string rstr;

        if (cmd.find(";") == -1) {
            return "";
        }

        for (int i = cmd.find(";") + 1; i < cmd.find(";") + 4; ++i) {
            if (cmd[i] == ' ' || cmd[i] == '\t') {
                break;
            } else {
                rstr += cmd[i];
            }
        }

        return rstr;
    }
};

class Code {
public:
    string dest(string str) {
        string rstr;

        map<string, string> mp = {
                {"", "000"}, {"M", "001"},
                {"D", "010"}, {"MD", "011"},
                {"A", "100"}, {"AM", "101"},
                {"AD", "110"}, {"AMD", "111"},
        };

        rstr = mp[str];
        if (rstr.size() != 3) {
            rstr = "000";
        }

        return rstr;
    }

    string comp(string str) {
        string rstr;

        if (str.find("M") != -1) {
            rstr += "1";
            str.replace(str.find("M"), 1, "A");
        } else {
            rstr += "0";
        }

        map<string, string> mp = {
                {"0", "101010"}, {"1", "111111"},
                {"-1", "111010"}, {"D", "001100"},
                {"A", "110000"}, {"!D", "001101"},
                {"!A", "110001"}, {"-D", "001111"},
                {"-A", "110011"}, {"D+1", "011111"},
                {"A+1", "110111"}, {"D-1", "001110"},
                {"A-1", "110010"}, {"D+A", "000010"},
                {"D-A", "010011"}, {"A-D", "000111"},
                {"D&A", "000000"}, {"D|A", "010101"},
        };

        rstr += mp[str];
        if (rstr.size() != 7) {
            rstr = "0000000";
        }

        return rstr;
    }

    string jump(string str) {
        map<string, string> mp = {
                {"", "000"}, {"JGT", "001"},
                {"JEQ", "010"}, {"JGE", "011"},
                {"JLT", "100"}, {"JNE", "101"},
                {"JLE", "110"}, {"JMP", "111"},
        };

        return mp[str];
    }
};

class SymbolTable{
public:
    map<string, int> stable;

    SymbolTable() {
        vector<pair<string, int>> dtable
        = {{"SP", 0}, {"LCL", 1},
           {"ARG", 2}, {"THIS", 3},
           {"THAT", 4}, {"SCREEN", 16384},
           {"KBD", 24576}};
        for (int i = 0; i < 16; ++i) {
            dtable.push_back({"R"+to_string(i), i});
        }

        for (auto itr : dtable) {
            stable.insert(itr);
        }
    }

    void addEntry(string symbol, int address) {
        stable[symbol] = address;
    }

    bool contains(string symbol) {
        return stable.find(symbol) != stable.end();
    }

    int getAddress(string symbol) {
        return stable[symbol];
    }
};

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr<<"invalid arguments"<<endl;
        exit(0);
    }

    inputfile = (string)argv[1] + ".asm";
    outputfile = (string)argv[1] + ".hack";

    Paser paser;
    Code code;
    SymbolTable symboltable;

    ofstream ofs(outputfile);

    int counter = 0;
    while (paser.hasMoreCommand()) {
        paser.advance();
        int ctype = paser.commandType();
        string symbol;

        if (ctype == A_COMMAND) {
            counter++;
        } else if (ctype == C_COMMAND) {
            counter++;
        } else if (ctype == L_COMMAND) {
            symbol = paser.symbol();
            symbol = symbol.substr(symbol.find("(") + 1, symbol.find(")") - 2);
            symboltable.addEntry(symbol, counter);
        } else {
            continue;
        }
    }

    int ramnum = 16;
    paser.ifs.clear();
    paser.ifs.seekg(0,ios_base::beg);
    while (paser.hasMoreCommand()) {
        paser.advance();
        int ctype = paser.commandType();
        string symbol, ins;

        if (ctype == A_COMMAND) {
            symbol = paser.symbol();
            symbol = symbol.substr(1, symbol.size() - 1);
            //cout<<symbol<<endl;
            //cout<<symbol.size()<<endl;
            try {
                stoi(symbol);
                bitset<16> bs(stoi(symbol));
                ins = bs.to_string();
                cout<<"A: "<<ins<<endl;
                ofs<<ins<<endl;
                continue;
            }
            catch (...) {
                cout<<"non numeric symbol: "<<symbol<<endl;
            }

            cout<<"~~~"<<symbol.size()<<"~~~"<<endl;
            if (symboltable.contains(symbol)) {
                bitset<16> bs(symboltable.getAddress(symbol));
                ins = bs.to_string();
                cout<<"A: "<<ins<<endl;
            } else {
                symboltable.addEntry(symbol, ramnum);
                bitset<16> bs(ramnum);
                ins = bs.to_string();
                cout<<"A: "<<ins<<endl;
                ramnum++;
            }
        } else if (ctype == C_COMMAND) {
            cout<<paser.comp()<<"_"<<code.comp(paser.comp())<<endl;
            cout<<paser.dest()<<"_"<<code.dest(paser.dest())<<endl;
            cout<<paser.jump()<<"_"<<code.jump(paser.jump())<<endl;
            ins = "111" + code.comp(paser.comp())
                    + code.dest(paser.dest()) + code.jump(paser.jump());
            cout<<"C: "<<ins<<endl;
        } else if (ctype == L_COMMAND) {
            cout<<"L"<<endl;
            continue;
        } else {
            cout<<"blank"<<endl;
            continue;
        }

        ofs<<ins<<endl;
    }

    return 0;
}
 
