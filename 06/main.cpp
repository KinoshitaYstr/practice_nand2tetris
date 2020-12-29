#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <bitset>
#include <string>
#include <map>

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
                for(int i = 0; i < tmpSize; i++) {
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
    int i = 1;
    if(this->commandType() == CommadType::A_COMMAND) {
        // @symbol
        while(true) {
            if(('a' <= this->str[i] && this->str[i] <= 'z') || ('A' <= this->str[i] && this->str[i] <= 'Z') || ('0' <= this->str[i] && this->str[i] <= '9') || (this->str[i] == '_') || (this->str[i] == '.') || (this->str[i] == '_') || (this->str[i] == '$')) {
                i++;
            } else {
                break;
            }
            // if(this->str[i] == ' ' || this->str[i] == '\n' || this->str[i] == '\0') {
            //     break;
            // } else {
            //     i++;
            // }
        }
        // std::cout << "->(" << 1 << ", " << i-1 << ") [" << this->str << "]" << std::ends;
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



class Code {
public:
    int dest(std::string str);
    int comp(std::string str);
    int jump(std::string str);
};

int Code::dest(std::string str) {
    if(str.length() > 3) return 0b000;
    int r = 0b000;
    r += str.find("M") != std::string::npos ? 0b001 : 0b000;
    r += str.find("D") != std::string::npos ? 0b010 : 0b000;
    r += str.find("A") != std::string::npos ? 0b100 : 0b000;
    return r;
}

int Code::comp(std::string str) {
    // A = M de kangaeru
    str = std::regex_replace(str, std::regex("M"), "A");
    
    if(str == "0") {
        return 0b101010;
    } else if(str == "1") {
        return 0b111111;
    } else if(str == "-1") {
        return 0b111010;
    } else if(str == "D") {
        return 0b001100;
    } else if(str == "A") {
        return 0b110000;
    } else if(str == "!D") {
        return 0b001101;
    } else if(str == "!A") {
        return 0b110001;
    } else if(str == "-D") {
        return 0b001111;
    } else if(str == "-A") {
        return 0b110011;
    } else if(str == "D+1") {
        return 0b011111;
    } else if(str == "A+1") {
        return 0b110111;
    } else if(str == "D-1") {
        return 0b001110;
    } else if(str == "A-1") {
        return 0b110010;
    } else if(str == "D+A") {
        return 0b000010;
    } else if(str == "D-A") {
        return 0b010011;
    } else if(str == "A-D") {
        return 0b000111;
    } else if(str == "D&A") {
        return 0b000000;
    } else if(str == "D|A") {
        return 0b010101;
    } else {
        return 0;
    }

}

int Code::jump(std::string str) {
    if(str == "JGT") {
        return 0b001;
    } else if(str == "JEQ") {
        return 0b010;
    } else if(str == "JGE") {
        return 0b011;
    } else if(str == "JLT") {
        return 0b100;
    } else if(str == "JNE") {
        return 0b101;
    } else if(str == "JLE") {
        return 0b110;
    } else if(str == "JMP") {
        return 0b111;
    } else {
        return 0b000;
    }
}



class SymbolTable {
public:
    void addEntity(std::string symbol, int address);
    bool contains(std::string symbol);
    int getAddress(std::string symbol);
private:
    std::map<std::string, int> table;
};

void SymbolTable::addEntity(std::string symbol, int address) {
    this->table[symbol] = address;
}

bool SymbolTable::contains(std::string symbol) {
    return this->table.find(symbol) != this->table.end();
}

int SymbolTable::getAddress(std::string symbol) {
    if(this->contains(symbol)) {
        return this->table[symbol];
    } else {
        return 0;
    }
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        std::cout << "arg error" << std::endl;
        return 0;
    }

    char* fname = argv[1];
    Parser parser(fname);
    Code code;
    SymbolTable symbolTable;
    
    // // parser test
    // while (parser.hasMoreCommands()) {
    //     parser.advance();
    //     std::cout << "symbol[" << parser.symbol() << "(" << parser.symbol().length() << ")]" << "-dest[" << parser.dest() << "(" << parser.dest().length() << ")] - comp[" << parser.comp() << "(" << parser.comp().length() << ")] - jump[" << parser.jump() << "(" << parser.jump().length() << ")]" << std::endl;
    // }
    
    // // code test
    // std::ofstream ofs("test.hack");
    // while (true)
    // {
    //     parser.advance();
    //     if(!parser.hasMoreCommands()) break;
    //     if(parser.commandType() == CommadType::A_COMMAND) {
    //         try
    //         {
    //             std::string symbol = parser.symbol();
    //             int symbol_num = std::stoi(symbol);
    //             // std::cout << std::bitset<1>(0) << std::bitset<15>(symbol_num) << std::endl;
    //             ofs << std::bitset<1>(0) << std::bitset<15>(symbol_num) << "\n";
    //         }
    //         catch(const std::exception& e)
    //         {
    //             std::cerr << e.what() << '\n';
    //             return 0;
    //         }
    //     } else if(parser.commandType() == CommadType::C_COMMAND) {
    //         int a = parser.comp().find("M") != std::string::npos ? 1 : 0;
    //         // std::cout << 111 << std::bitset<1>(a) << std::bitset<6>(code.comp(parser.comp())) << std::bitset<3>(code.dest(parser.dest())) << std::bitset<3>(code.jump(parser.jump())) << std::endl;
    //         ofs << 111 << std::bitset<1>(a) << std::bitset<6>(code.comp(parser.comp())) << std::bitset<3>(code.dest(parser.dest())) << std::bitset<3>(code.jump(parser.jump())) << "\n";
    //     } else {

    //     }
    // }
    // ofs.close();

    // symbol test
    symbolTable.addEntity("SP", 0);
    symbolTable.addEntity("LCL", 1);
    symbolTable.addEntity("ARG", 2);
    symbolTable.addEntity("THIS", 3);
    symbolTable.addEntity("THAT", 4);

    symbolTable.addEntity("R0", 0);
    symbolTable.addEntity("R1", 1);
    symbolTable.addEntity("R2", 2);
    symbolTable.addEntity("R3", 3);
    symbolTable.addEntity("R4", 4);
    symbolTable.addEntity("R5", 5);
    symbolTable.addEntity("R6", 6);
    symbolTable.addEntity("R7", 7);
    symbolTable.addEntity("R8", 8);
    symbolTable.addEntity("R9", 9);
    symbolTable.addEntity("R10", 10);
    symbolTable.addEntity("R11", 11);
    symbolTable.addEntity("R12", 12);
    symbolTable.addEntity("R13", 13);
    symbolTable.addEntity("R14", 14);
    symbolTable.addEntity("R15", 15);

    symbolTable.addEntity("SCREEN", 16384);
    symbolTable.addEntity("KBD", 24576);

    int labelSymbol_address = 16;
    
    // add symbol
    // for label symbol
    Parser forLabelSymbol(fname);
    int i = 0;
    while (true) {
        forLabelSymbol.advance();
        if(!forLabelSymbol.hasMoreCommands()) break;
        if(forLabelSymbol.commandType() == CommadType::L_COMMAND) {
            symbolTable.addEntity(forLabelSymbol.symbol(), i);
        } else {
            i++;
        }
    }
    
    // for variable symbol
    Parser forSymbol(fname);
    while (true) {
        forSymbol.advance();
        if(!forSymbol.hasMoreCommands()) break;
        if(forSymbol.commandType() == CommadType::A_COMMAND) {
            std::string symbol = forSymbol.symbol();
            if(!('0' <= symbol[0] && symbol[0] <= '9') && !symbolTable.contains(symbol)) {
                symbolTable.addEntity(symbol, labelSymbol_address);
                labelSymbol_address++;
            }
        }
    }

    std::ofstream ofs("test.hack");
    while (true)
    {
        parser.advance();
        if(!parser.hasMoreCommands()) break;
        if(parser.commandType() == CommadType::A_COMMAND) {
            std::string symbol = parser.symbol();
            if('0' <= symbol[0] && symbol[0] <= '9') {
                try
                {
                    int symbol_num = std::stoi(symbol);
                    // std::cout << std::bitset<1>(0) << std::bitset<15>(symbol_num) << " - " << symbol << std::endl;
                    ofs << std::bitset<1>(0) << std::bitset<15>(symbol_num) << "\n";
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                    return 0;
                }
            } else {
                // std::cout << std::bitset<1>(0) << std::bitset<15>(symbolTable.getAddress(symbol)) << " - " << symbol << std::endl;
                ofs << std::bitset<1>(0) << std::bitset<15>(symbolTable.getAddress(symbol)) << "\n";
            }
        } else if(parser.commandType() == CommadType::C_COMMAND) {
            int a = parser.comp().find("M") != std::string::npos ? 1 : 0;
            // std::cout << 111 << std::bitset<1>(a) << std::bitset<6>(code.comp(parser.comp())) << std::bitset<3>(code.dest(parser.dest())) << std::bitset<3>(code.jump(parser.jump())) << std::endl;
            ofs << 111 << std::bitset<1>(a) << std::bitset<6>(code.comp(parser.comp())) << std::bitset<3>(code.dest(parser.dest())) << std::bitset<3>(code.jump(parser.jump())) << "\n";
        } else {

        }
    }
    ofs.close();

    return 0;
}