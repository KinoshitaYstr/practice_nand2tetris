#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

enum VMCommandType {
    C_ARITHMETIC = 0,
    C_PUSH,
    C_POP,
    C_LABEL,
    C_GOTO,
    C_IF,
    C_FUNCTION,
    C_RETURN,
    C_CALL
};

class Parser {
public:
    Parser(std::string fname);
    bool hasMoreCommands();
    void advance();
    VMCommandType commandType();
    std::string arg1();
    int arg2();
private:
    std::ifstream ifs;
    std::string str;
};

Parser::Parser(std::string fname) {
    this->ifs.open(fname);
    this->str = "";
}

void Parser::advance() {
    std::string tmp;
    int tmpSize;
    while(true) {
        // ファイル終わり確認
        if(this->ifs.eof()) {
            this->str = "";
            break;
        }

        // 読み込み
        std::getline(this->ifs, tmp);

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

        if(tmp[0] == '/' && tmp[1] == '/') {
            continue;
        }

        if(tmp[0] == '\n' || tmp[0] == '\0') {
            continue;
        }

        this->str = tmp;
        break;
    }

    std::cout << " parser result -> " << this->str << std::endl;
}

bool Parser::hasMoreCommands() {
    return !this->ifs.eof();
}

VMCommandType Parser::commandType() {
    if((this->str.find("add") == 0) || (this->str.find("sub") == 0) || (this->str.find("neg") == 0) || (this->str.find("eq") == 0) || (this->str.find("gt") == 0) || (this->str.find("lt") == 0) || (this->str.find("and") == 0) || (this->str.find("or") == 0) || (this->str.find("not") == 0)) {
        return VMCommandType::C_ARITHMETIC;
    } else if(this->str.find("pop") == 0) {
        return VMCommandType::C_POP;
    } else if(this->str.find("push") == 0) {
        return VMCommandType::C_PUSH;
    } else if(this->str.find("label") == 0) {
        return VMCommandType::C_LABEL;
    } else if(this->str.find("if-goto") == 0) {
        return VMCommandType::C_IF;
    } else if(this->str.find("goto") == 0) {
        return VMCommandType::C_GOTO;
    } else if(this->str.find("function") == 0) {
        return VMCommandType::C_FUNCTION;
    } else if(this->str.find("return") == 0) {
        return VMCommandType::C_RETURN;
    } else {
        return VMCommandType::C_CALL;
    }
}

std::string Parser::arg1() {
    int i;
    std::string tmp;
    i = this->str.find(" ") != std::string::npos ? this->str.find(" ") : this->str.find("\t");
    if(this->commandType() == VMCommandType::C_ARITHMETIC) {
        return this->str.substr(0, i);
    } else if(this->commandType() == VMCommandType::C_PUSH || this->commandType() == VMCommandType::C_POP) {
        if(i != std::string::npos) {
            tmp = this->str.substr(i+1);

            while(true) {
                if(tmp[0] == ' ' || tmp[0] == '\t') {
                    tmp = tmp.substr(1);
                } else {
                    break;
                }
            }

            i = tmp.find(" ") != std::string::npos ? tmp.find(" ") : tmp.find("\t");
            if(i != std::string::npos) {
                tmp = tmp.substr(0, i);
                if(tmp == "argument" || tmp == "local" || tmp == "static" || tmp == "constant" || tmp == "this" || tmp == "that" || tmp == "pointer" || tmp == "temp") {
                    return tmp;
                }
            }
        }
        return "";
    } else if(this->commandType() == VMCommandType::C_LABEL || this->commandType() == VMCommandType::C_GOTO || this->commandType() == VMCommandType::C_IF) {
        tmp = this->str.substr(i+1);

        while(true) {
            if(tmp[0] == ' ' || tmp[0] == '\t') {
                tmp = tmp.substr(1);
            } else {
                break;
            }
        }

        i = tmp.find(" ");
        i = (tmp.find("\t") > i && i != std::string::npos) ? i : tmp.find("\t");
        i = (tmp.find("\n") > i && i != std::string::npos) ? i : tmp.find("\n");
        if(i != std::string::npos) {
            tmp = tmp.substr(0, i);
        }

        if('0' <= tmp[0] && tmp[0] <= '9') {
            return "";
        }
        for(i = 1; i < tmp.length(); i++) {
            if(!(('0' <= tmp[i] && tmp[i] <= '9') || ('a' <= tmp[i] && tmp[i] <= 'z') || ('A' <= tmp[i] && tmp[i] <= 'Z') || tmp[i] == '_' || tmp[i] == '.' || tmp[i] == ':')) {
                return "";
            }
        }
        return tmp;
    }
    return "";
}

int Parser::arg2() {
    int i = this->str.find(" ") != std::string::npos ? this->str.find(" ") : this->str.find("\t");
    if(i != std::string::npos) {
        std::string tmp = this->str.substr(i+1);
        while(true) {
            if(tmp[0] == ' ' || tmp[0] == '\t') {
                tmp = tmp.substr(1);
            } else {
                break;
            }
        }

        i = tmp.find(" ") != std::string::npos ? tmp.find(" ") : tmp.find("\t");
        if(i != std::string::npos) {
            tmp = tmp.substr(i+1);
            while(true) {
                if(tmp[0] == ' ' || tmp[0] == '\t') {
                    tmp = tmp.substr(1);
                } else {
                    break;
                }
            }

            i = 0;
            while(true) {
                if('0' <= tmp[i] && tmp[i] <= '9') {
                    i++;
                } else {
                    break;
                }
            }
            tmp = tmp.substr(0, i);

            try
            {
                return std::stoi(tmp);
            }
            catch(const std::exception& e) {
                return 0;
            }
        }

    }
    
    return 0;
}



class CodeWriter {
public:
    CodeWriter(std::string fname);
    void setFileName(std::string fname);
    void writeArithmetic(std::string str);
    void writePushPop(VMCommandType command, std::string segment, int index);
    void close();
    void writeInit();
    void writeLabel(std::string label);
    void writeGoto(std::string label);
    void writeIf(std::string label);
    void writeCall(std::string functionName, int numArgs);
    void writeReturn();
    void wtiteFunction(std::string functionName, int numLocals);

private:
    std::ofstream ofs;
    std::string nowFileName;
    int label_num;
};


CodeWriter::CodeWriter(std::string fname) {
    this->ofs.open(fname);
    this->nowFileName = "";
    this->label_num = 0;
}

void CodeWriter::setFileName(std::string fname) {
    this->nowFileName = fname;
}

void CodeWriter::writeArithmetic(std::string str) {
    if(str == "add" || str == "sub" || str == "and" || str == "or") {
        // 値いっことる(スタックポインタの位置もずらしてからとる)
        this->ofs << "\t@SP\n";
        this->ofs << "\tM=M-1\n";
        this->ofs << "\tA=M\n";
        this->ofs << "\tD=M\n";
    } else if(str == "neg" || str == "not") {
        // 値とるだけ
        this->ofs << "\t@SP\n";
        this->ofs << "\tA=M-1\n";
        this->ofs << "\tD=M\n";
    } else if(str == "eq" || str == "gt" || str == "lt") {
        this->ofs << "\t@SP\n";
        this->ofs << "\tM=M-1\n";
        this->ofs << "\tA=M\n";
        this->ofs << "\tD=M\n";
        this->ofs << "\t@SP\n";
        this->ofs << "\tM=M-1\n";
        this->ofs << "\tA=M\n";
        this->ofs << "\tD=M-D\n";
    }

    if(str == "add") {        
        // 足し算する
        this->ofs << "\t@SP\n";
        this->ofs << "\tA=M-1\n";
        this->ofs << "\tM=M+D\n";
    } else if(str == "sub") {
        this->ofs << "\t@SP\n";
        this->ofs << "\tA=M-1\n";
        this->ofs << "\tM=M-D\n";
    } else if(str == "neg") {
        this->ofs << "\tM=-D\n";
    } else if(str == "eq") {
        this->ofs << "\t@EQUAL" << this->label_num << "\n";
        this->ofs << "\tD;JEQ\n";
        
        // !=
        this->ofs << "\t@SP\n";
        this->ofs << "\tA=M\n";
        this->ofs << "\tM=0\n";
        this->ofs << "\t@FINISH" << this->label_num << "\n";
        this->ofs << "\tD;JMP\n";
        
        // ==
        this->ofs << "(EQUAL" << this->label_num << ")\n";
        this->ofs << "\t@SP\n";
        this->ofs << "\tA=M\n";
        this->ofs << "\tM=-1\n";
        // this->ofs << "\t@FINISH" << this->label_num << "\n";
        // this->ofs << "\tJMP\n";

        this->ofs << "(FINISH" << this->label_num << ")\n";
        this->ofs << "\t@SP\n";
        this->ofs << "\tM=M+1\n";
        this->label_num++;
    } else if(str == "gt") {
        this->ofs << "\t@GREAT" << this->label_num << "\n";
        this->ofs << "\tD;JGT\n";
        
        // <
        this->ofs << "\t@SP\n";
        this->ofs << "\tA=M\n";
        this->ofs << "\tM=0\n";
        this->ofs << "\t@FINISH" << this->label_num << "\n";
        this->ofs << "\tD;JMP\n";
        
        // >
        this->ofs << "(GREAT" << this->label_num << ")\n";
        this->ofs << "\t@SP\n";
        this->ofs << "\tA=M\n";
        this->ofs << "\tM=-1\n";
        // this->ofs << "\t@FINISH" << this->label_num << "\n";
        // this->ofs << "\tJMP\n";

        this->ofs << "(FINISH" << this->label_num << ")\n";
        this->ofs << "\t@SP\n";
        this->ofs << "\tM=M+1\n";
        this->label_num++;
    } else if(str == "lt") {
        this->ofs << "\t@LITTLE" << this->label_num << "\n";
        this->ofs << "\tD;JLT\n";
        
        // >
        this->ofs << "\t@SP\n";
        this->ofs << "\tA=M\n";
        this->ofs << "\tM=0\n";
        this->ofs << "\t@FINISH" << this->label_num << "\n";
        this->ofs << "\tD;JMP\n";
        
        // <
        this->ofs << "(LITTLE" << this->label_num << ")\n";
        this->ofs << "\t@SP\n";
        this->ofs << "\tA=M\n";
        this->ofs << "\tM=-1\n";
        // this->ofs << "\t@FINISH" << this->label_num << "\n";
        // this->ofs << "\tJMP\n";

        this->ofs << "(FINISH" << this->label_num << ")\n";
        this->ofs << "\t@SP\n";
        this->ofs << "\tM=M+1\n";
        this->label_num++;
    } else if(str == "and") {
        this->ofs << "\t@SP\n";
        this->ofs << "\tA=M-1\n";
        this->ofs << "\tM=M&D\n";
    } else if(str == "or") {
        this->ofs << "\t@SP\n";
        this->ofs << "\tA=M-1\n";
        this->ofs << "\tM=M|D\n";
    } else if(str == "not") {
        this->ofs << "\tM=!D\n";
    }
}

void CodeWriter::writePushPop(VMCommandType command, std::string segment, int index) {
    if(command == VMCommandType::C_PUSH) {
        if(segment == "constant") {
            this->ofs << "\t@" << index <<"\n";
            this->ofs << "\tD=A\n";

            this->ofs << "\t@SP\n";
            this->ofs << "\tA=M\n";
            this->ofs << "\tM=D\n";
            this->ofs << "\t@SP\n";
            this->ofs << "\tM=M+1\n";
        } else if(segment == "local" || segment == "argument" || segment == "this" || segment == "that") {
            if(segment == "local") {
                this->ofs << "\t@LCL\n";
            } else if(segment == "argument") {
                this->ofs << "\t@ARG\n";
            } else if(segment == "this") {
                this->ofs << "\t@THIS\n";
            } else if(segment == "that") {
                this->ofs << "\t@THAT\n";
            }
            this->ofs << "\tD=M\n";
            this->ofs << "\t@" << index << "\n";
            this->ofs << "\tA=D+A\n";
            this->ofs << "\tD=M\n";

            this->ofs << "\t@SP\n";
            this->ofs << "\tM=M+1\n";
            this->ofs << "\tA=M-1\n";
            this->ofs << "\tM=D\n";
        } else if(segment == "pointer" || segment == "temp") {
            int base = segment == "pointer" ? 3 : 5;
            this->ofs << "\t@R" << base+index << "\n";
            this->ofs << "\tD=M\n";

            this->ofs << "\t@SP\n";
            this->ofs << "\tM=M+1\n";
            this->ofs << "\tA=M-1\n";
            this->ofs << "\tM=D\n";
        } else if(segment == "static") {
            this->ofs << "\t@" << this->nowFileName << "." << index <<"\n";
            this->ofs << "\tD=M\n";

            this->ofs << "\t@SP\n";
            this->ofs << "\tM=M+1\n";
            this->ofs << "\tA=M-1\n";
            this->ofs << "\tM=D\n";
        }
    } else if(command == VMCommandType::C_POP) {
        if(segment == "local" || segment == "argument" || segment == "this" || segment == "that") {
            // this->ofs << "\t@SP\n";
            // this->ofs << "\tM=M-1\n";
            // this->ofs << "\tD=M+1\n";

            if(segment == "local") {
                this->ofs << "\t@LCL\n";
            } else if(segment == "argument") {
                this->ofs << "\t@ARG\n";
            } else if(segment == "this") {
                this->ofs << "\t@THIS\n";
            } else if(segment == "that") {
                this->ofs << "\t@THAT\n";
            }
            this->ofs << "\tD=M\n";
            this->ofs << "\t@" << index << "\n";
            // 送り先のメモ
            this->ofs << "\tD=D+A\n";
            this->ofs << "\t@SP\n";
            this->ofs << "\tA=M\n";
            this->ofs << "\tM=D\n";

            this->ofs << "\t@SP\n";
            this->ofs << "\tA=M-1\n";
            this->ofs << "\tD=M\n";

            this->ofs << "\t@SP\n";
            this->ofs << "\tA=M\n";
            this->ofs << "\tA=M\n";
            this->ofs << "\tM=D\n";

            this->ofs << "\t@SP\n";
            this->ofs << "\tM=M-1\n";
        } else if(segment == "pointer" || segment == "temp") {
            int base = segment == "pointer" ? 3 : 5;
            this->ofs << "\t@SP\n";
            this->ofs << "\tM=M-1\n";
            this->ofs << "\tA=M\n";
            this->ofs << "\tD=M\n";
            
            this->ofs << "\t@R" << base+index << "\n";
            this->ofs << "\tM=D\n";
        } else if(segment == "static") {
            this->ofs << "\t@SP\n";
            this->ofs << "\tM=M-1\n";
            this->ofs << "\tA=M\n";
            this->ofs << "\tD=M\n";

            this->ofs << "\t@" << this->nowFileName << "." << index <<"\n";
            this->ofs << "\tM=D\n";
        }
    }
}

void CodeWriter::close() {
    this->ofs.close();
}

void CodeWriter::writeInit() {
    // this->ofs << "SP=256";
    this->ofs << "\t@256\n";
    this->ofs << "\tD=A\n";
    this->ofs << "\t@SP\n";
    this->ofs << "\tM=D\n";

    // for test
    this->ofs << "\t@300\n";
    this->ofs << "\tD=A\n";
    this->ofs << "\t@LCL\n";
    this->ofs << "\tM=D\n";

    this->ofs << "\t@400\n";
    this->ofs << "\tD=A\n";
    this->ofs << "\t@ARG\n";
    this->ofs << "\tM=D\n";

    this->ofs << "\t@3\n";
    this->ofs << "\tD=A\n";
    this->ofs << "\t@400\n";
    this->ofs << "\tM=D\n";
    // this->ofs << "call Sys.init\n";
}

void CodeWriter::writeLabel(std::string label) {
    this->ofs << "(label$" << label <<")\n";
}

void CodeWriter::writeGoto(std::string label) {
    this->ofs << "\t@label$" << label << "\n";
    this->ofs << "\t0;JMP\n";
}

void CodeWriter::writeIf(std::string label) {
    this->ofs << "\t@SP\n";
    this->ofs << "\tM=M-1\n";
    this->ofs << "\tA=M\n";
    this->ofs << "\tD=M\n";

    this->ofs << "\t@label$" << label << "\n";
    this->ofs << "\tD;JNE\n";
}

void CodeWriter::writeCall(std::string functionName, int numArgs) {

}

void CodeWriter::writeReturn() {

}

void CodeWriter::wtiteFunction(std::string functionName, int numLocals) {

}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cout << "input file count error" << std::endl;
        return 0;
    }

    for(int i = 0; i < argc; i++) {
        std::cout << argv[i] << std::endl;
    }

    // test
    Parser parser(argv[1]);
    CodeWriter codeWriter("test.asm");

    // for test
    // codeWriter.writeInit();

    while(parser.hasMoreCommands()) {
        parser.advance();
        std::cout << " -> arg1 = [" << parser.arg1() << "] arg2 = [" << parser.arg2() << "](" << parser.commandType() << ")" << std::endl;
        if(parser.commandType() == VMCommandType::C_ARITHMETIC) {
            codeWriter.writeArithmetic(parser.arg1());
        } else if(parser.commandType() == VMCommandType::C_POP || parser.commandType() == VMCommandType::C_PUSH) {
            codeWriter.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
        } else if(parser.commandType() == VMCommandType::C_LABEL) {
            codeWriter.writeLabel(parser.arg1());
        } else if(parser.commandType() == VMCommandType::C_GOTO) {
            codeWriter.writeGoto(parser.arg1());
        } else if(parser.commandType() == VMCommandType::C_IF) {
            codeWriter.writeIf(parser.arg1());
        }
    }
}