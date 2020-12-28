#include <iostream>
#include <fstream>
#include <sstream>

enum CommadType {
    A_COMMAND = 0,
    C_COMMAND,
    L_COMMAND,
};

class Parser {
public:
    Parser(char* fname);
    bool hasMoreCommands();
    void advance();
    CommadType commandType();
    std::string symbol();
    std::string dest();
    std::string comp();
    std::string jump();
private:
    std::ifstream ifs;
    std::string str;
};

Parser::Parser(char* fname) {
    this->ifs.open(fname);
    this->str = "";
};

void Parser::advance() {
    std::string tmp;
    int tmpSize;
    while(true) {
        // ファイル終わり確認
        if(this->ifs.eof()) {
            break;
        }

        // 読み込み
        std::getline(this->ifs, tmp);
        
        // 最初の空白つぶし
        while(true) {
            if(tmp[0] == ' ' || tmp[0] == '\t') {
                tmpSize = tmp.length();
                for(int i = 0; i < tmpSize-1; i++) {
                    tmp[i] = tmp[i+1];
                }
            } else {
                break;
            }
        }

        // コメントアウト関係
        if(tmp[0] == '/' && tmp[0] == '/') {
            continue;
        }

        // 開業とか見えないもの
        if(tmp[0] == '\0' || tmp[0] == '\n') {
            continue;
        }

        // 大丈夫なもの
        this->str = tmp;
        break;
    }
}

bool Parser::hasMoreCommands() {
    return !this->ifs.eof();
}

CommadType Parser::commandType() {
    if(this->str[0] == '@') {
        // std::cout << "A command" << std::ends;
        return CommadType::A_COMMAND;
    } else if(this->str[0] == '(') {
        // std::cout << "L command" << std::ends;
        return CommadType::L_COMMAND;
        
    } else {
        // std::cout << "C command" << std::ends;
        return CommadType::C_COMMAND;
    }
}

std::string Parser::symbol() {
    int i = 0;
    if(this->commandType() == CommadType::A_COMMAND) {
        // @symbol
        while(true) {
            if(this->str[i] == ' ' || this->str[i] == '\n' || this->str[i] == '\0') {
                break;
            } else {
                i++;
            }
        }
        return this->str.substr(1, i-1);
    } else if(this->commandType() == CommadType::L_COMMAND) {
        // (symbol)
        i = this->str.find(")");
        if(i != std::string::npos) {
            return this->str.substr(1, i-1);
        } else {
            return "";
        }
    } else {
        return "";
    }
}

std::string Parser::dest() {
    if(this->commandType() == CommadType::C_COMMAND) {
        int num = this->str.find("=");
        if(num != std::string::npos) {
            // = arutoki!
            return this->str.substr(0, num);
        } else {
            return "";
        }
    } else {
        return "";
    }
}

std::string Parser::comp() {
    if(this->commandType() == CommadType::C_COMMAND) {
        int s = this->str.find("=") != std::string::npos ? this->str.find("=")+1 : 0;
        int e = this->str.find(";") != std::string::npos ? this->str.find(";") : s;
        while (this->str.find(";") == std::string::npos) {
            if(this->str[e] == '/' || this->str[e] == ' ' || this->str[e] == '\n' || this->str[e] == '\t' || this->str[e] == '\0') {
                break;
            } else {
                e++;
            }
        }
        return this->str.substr(s, e-s);
        if(s+1 == e) {
            return this->str.substr(s, s+1);
        } else {
        }
    } else {
        return "";
    }
}

std::string Parser::jump() {
    if(this->commandType() == CommadType::C_COMMAND) {
        int i = this->str.find(";"); 
        if(i != std::string::npos) {
            return this->str.substr(i+1, i+2);
        } else {
            return "";
        }
    } else {
        return "";
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "arg error" << std::endl;
        return 0;
    }

    char* fname = argv[1];
    Parser parser(fname);
    
    // parser test
    while (parser.hasMoreCommands()) {
        parser.advance();
        std::cout << "symbol[" << parser.symbol() << "(" << parser.symbol().length() << ")]" << "-dest[" << parser.dest() << "(" << parser.dest().length() << ")] - comp[" << parser.comp() << "(" << parser.comp().length() << ")] - jump[" << parser.jump() << "(" << parser.jump().length() << ")]" << std::endl;
    }
    
    

    return 0;
}