#include <iostream>
#include <fstream>
#include <sstream>

enum TokenType {
    KEYWORD = 0,
    SYMBOL,
    IDENTIFIER,
    INT_CONST,
    STRING_CONST,
    NONE
};

enum Keyword {
    CLASS,
    MEYHOD,
    FUNCTION,
    CONSTRUCTOR,
    INT,
    BOOLEAN,
    CHAR,
    VOID,
    VAR,
    STATIC,
    FIELD,
    LET,
    DO,
    IF,
    ELSE,
    WHILE,
    RETURN,
    TRUE,
    FALSE,
    KEYWORD_NULL,
    THIS,
    KEYWORD_NONE
};

class JackTokenizer {
public:
    JackTokenizer(char fname[]);
    JackTokenizer();
    void open(char fname[]);
    void advance();
    bool hasMoreTokens();
    TokenType tokenType();
    Keyword keyword();
    std::string symbol();
    std::string identifier();
    int intVal();
    std::string stringVal();
private:
    std::ifstream ifs;
    std::string str;
    std::string token;
};

JackTokenizer::JackTokenizer(char fname[]) {
    this->ifs.open(fname);
    this->str = "";
    this->token = "";
}

JackTokenizer::JackTokenizer() {
}

void JackTokenizer::open(char fname[]) {
    this->ifs.open(fname);
    this->str = "";
    this->token = "";
}

void JackTokenizer::advance() {
    int i;
    if(this->str == "" && !this->ifs.eof()) {
        std::getline(this->ifs, this->str);
    }
    this->token == "";
    while(true) {
        while(true) {
            if(this->str == "" && !this->ifs.eof()) {
                std::getline(this->ifs, this->str);
            } else if(this->str[0] == ' ' || this->str[0] == '\t' || this->str[0] == '\n') {
                this->str = this->str.substr(1);
            } else {
                break;
            }
        }
        
        // comment
        if((this->str[0] == '/' && this->str[1] == '/')) {
            if(this->ifs.eof()) {
                this->str = "";
            } else {
                std::getline(this->ifs, this->str);
            }
            continue;
        } else if(this->str[0] == '/' && this->str[1] == '*') {
            while(true) {
                i = this->str.find("*/");
                if(i != std::string::npos) {
                    this->str = this->str.substr(i+2);
                    break;
                } else if(this->ifs.eof()) {
                    break;
                } else {
                    std::getline(this->ifs, this->str);
                }
            }
            continue;
        }

        // string
        if(this->str[0] == '\"') {
            // std::cout << "  ==> check string [" << this->str << "]" << std::endl;
            i = this->str.substr(1).find("\"");
            if(i != std::string::npos) {
                i = i+1;
                // std::cout << "   -> string [" << this->str.substr(0, i+1) << "][" << this->str.substr(i) << "]-[" << this->str[i] << ", "<< this->str[i+1] << "]" << std::endl;
                this->token = this->str.substr(0, i+1);
                this->str = this->str.substr(i+1);
                break;
            } else {
                // std::cout << "string error" << std::endl;
                return;
            }
        }

        i = this->str.find("(") != std::string::npos ? this->str.find("(") : this->str.length()-1;
        i = this->str.find(")") != std::string::npos && i > this->str.find(")") ? this->str.find(")") : i;
        
        i = this->str.find("[") != std::string::npos && i > this->str.find("[") ? this->str.find("[") : i;
        i = this->str.find("]") != std::string::npos && i > this->str.find("]") ? this->str.find("]") : i;
        
        i = this->str.find("{") != std::string::npos && i > this->str.find("{") ? this->str.find("{") : i;
        i = this->str.find("}") != std::string::npos && i > this->str.find("}") ? this->str.find("}") : i;
        
        i = this->str.find(",") != std::string::npos && i > this->str.find(",") ? this->str.find(",") : i;
        i = this->str.find(";") != std::string::npos && i > this->str.find(";") ? this->str.find(";") : i;
        
        i = this->str.find("=") != std::string::npos && i > this->str.find("=") ? this->str.find("=") : i;
        
        i = this->str.find(".") != std::string::npos && i > this->str.find(".") ? this->str.find(".") : i;

        i = this->str.find("+") != std::string::npos && i > this->str.find("+") ? this->str.find("+") : i;
        i = this->str.find("-") != std::string::npos && i > this->str.find("-") ? this->str.find("-") : i;
        i = this->str.find("*") != std::string::npos && i > this->str.find("*") ? this->str.find("*") : i;
        i = this->str.find("/") != std::string::npos && i > this->str.find("/") ? this->str.find("/") : i;
        
        i = this->str.find("&") != std::string::npos && i > this->str.find("&") ? this->str.find("&") : i;
        i = this->str.find("|") != std::string::npos && i > this->str.find("|") ? this->str.find("|") : i;
        i = this->str.find("~") != std::string::npos && i > this->str.find("~") ? this->str.find("~") : i;
        
        i = this->str.find("<") != std::string::npos && i > this->str.find("<") ? this->str.find("<") : i;
        i = this->str.find(">") != std::string::npos && i > this->str.find(">") ? this->str.find(">") : i;

        i = this->str.find(" ") != std::string::npos && i > this->str.find(" ") ? this->str.find(" ") : i;
        i = this->str.find("\t") != std::string::npos && i > this->str.find("\t") ? this->str.find("\t") : i;
        if(i == 0) {
            this->token = this->str.substr(0, 1);
            this->str = this->str.substr(1);
        } else if(i > 0) {
            this->token = this->str.substr(0, i);
            this->str = this->str.substr(i);
        } else {
            this->token = "";
            this->str = "";
        }
        // std::cout << "now advance " << i << "->" << this->str << std::endl;
        break;
    }
    // std::cout << "advance result -> \'" << this->token << "\' \'" << this->str << "\'" << std::endl;
}

bool JackTokenizer::hasMoreTokens() {
    return !this->ifs.eof();
}

TokenType JackTokenizer::tokenType() {
    // std::cout << " ==> tokenType -> [" << this->token << "]" << std::endl;
    if(this->token == "(" || this->token == ")" || this->token == "[" || this->token == "]" || this->token == "{" || this->token == "}" || this->token == "," || this->token == ";" || this->token == "=" || this->token == "." || this->token == "+" || this->token == "-" || this->token == "*" || this->token == "/" || this->token == "&" || this->token == "|" || this->token == "~" || this->token == "<" || this->token == ">") {
        return TokenType::SYMBOL;
    } else if(this->token == "class" || this->token == "constructor" || this->token == "method" || this->token == "function" || this->token == "int" || this->token == "boolean" || this->token == "char" || this->token == "void" || this->token == "var" || this->token == "static" || this->token == "field" || this->token == "let" || this->token == "do" || this->token == "if" || this->token == "else" || this->token == "while" || this->token == "return" || this->token == "true" || this->token == "false" || this->token == "null" || this->token == "this") {
        return TokenType::KEYWORD;
    } else if(('0' <= this->token[0] && this->token[0] <= '9') || this->token[0] == '-') {
        for(int i = 1; i <= this->token[i]; i++) {
            if(!('0' <= this->token[i] && this->token[i] <= '9')) {
                std::cout << "number token error" << std::endl;
                return TokenType::NONE;
            }
        }
        return TokenType::INT_CONST;
    } else if(this->token[0] == '\"') {
        return TokenType::STRING_CONST;
    } else if(('a' <= this->token[0] && this->token[0] <= 'z') || ('A' <= this->token[0] && this->token[0] <= 'Z') || this->token[0] == '_') {
        for(int i = 1; i <= this->token[i]; i++) {
            if(!(('a' <= this->token[i] && this->token[i] <= 'z') || ('A' <= this->token[i] && this->token[i] <= 'Z') || ('0' <= this->token[i] && this->token[i] <= '9') || this->token[i] == '_')) {
                std::cout << "identifier token error" << std::endl;
                return TokenType::NONE;
            }
        }
        return TokenType::IDENTIFIER;
    } else {
        return TokenType::NONE;
    }
}

Keyword JackTokenizer::keyword() {
    if(this->tokenType() == TokenType::KEYWORD) {
        if(this->token == "class") {
            // std::cout << "   [class]" << std::endl;
            return Keyword::CLASS;
        } else if(this->token == "method") {
            // std::cout << "   [method]" << std::endl;
            return Keyword::MEYHOD;
        } else if(this->token == "function") {
            // std::cout << "   [function]" << std::endl;
            return Keyword::FUNCTION;
        } else if(this->token == "constructor") {
            // std::cout << "   [constructor]" << std::endl;
            return Keyword::CONSTRUCTOR;
        } else if(this->token == "int") {
            // std::cout << "   [int]" << std::endl;
            return Keyword::INT;
        } else if(this->token == "boolean") {
            // std::cout << "   [boolean]" << std::endl;
            return Keyword::BOOLEAN;
        } else if(this->token == "char") {
            // std::cout << "   [char]" << std::endl;
            return Keyword::CHAR;
        } else if(this->token == "void") {
            // std::cout << "   [void]" << std::endl;
            return Keyword::VOID;
        } else if(this->token == "var") {
            // std::cout << "   [var]" << std::endl;
            return Keyword::VAR;
        } else if(this->token == "static") {
            // std::cout << "   [static]" << std::endl;
            return Keyword::STATIC;
        } else if(this->token == "field") {
            // std::cout << "   [field]" << std::endl;
            return Keyword::FIELD;
        } else if(this->token == "let") {
            // std::cout << "   [let]" << std::endl;
            return Keyword::LET;
        } else if(this->token == "do") {
            // std::cout << "   [do]" << std::endl;
            return Keyword::DO;
        } else if(this->token == "if") {
            // std::cout << "   [if]" << std::endl;
            return Keyword::IF;
        } else if(this->token == "else") {
            // std::cout << "   [else]" << std::endl;
            return Keyword::ELSE;
        } else if(this->token == "while") {
            // std::cout << "   [while]" << std::endl;
            return Keyword::WHILE;
        } else if(this->token == "return") {
            // std::cout << "   [return]" << std::endl;
            return Keyword::RETURN;
        } else if(this->token == "true") {
            // std::cout << "   [true]" << std::endl;
            return Keyword::TRUE;
        } else if(this->token == "false") {
            // std::cout << "   [false]" << std::endl;
            return Keyword::FALSE;
        } else if(this->token == "null") {
            // std::cout << "   [null]" << std::endl;
            return Keyword::KEYWORD_NULL;
        } else if(this->token == "this") {
            // std::cout << "   [this]" << std::endl;
            return Keyword::THIS;
        } else {
            // std::cout << "   []" << std::endl;
            return Keyword::KEYWORD_NONE;
        }
    } else {
        return Keyword::KEYWORD_NONE;
    }
}


std::string JackTokenizer::symbol() {
    if(this->tokenType() == TokenType::SYMBOL) {
        return this->token;
    } else {
        return "";
    }
}

std::string JackTokenizer::identifier() {
    if(this->tokenType() == TokenType::IDENTIFIER) {
        return this->token;
    } else {
        return "";
    }
}

int JackTokenizer::intVal() {
    if(this->tokenType() == TokenType::INT_CONST) {
        return std::stoi(this->token);
    } else {
        return 0;
    }
}

std::string JackTokenizer::stringVal() {
    if(this->tokenType() == TokenType::STRING_CONST) {
        std::string tmp = this->token.substr(1);
        tmp = tmp.substr(0, tmp.length()-1);
        return tmp;
    } else {
        return "";
    }
}



class CompileEngine {
public:
    CompileEngine(JackTokenizer *_tokenizer, std::ofstream *_ofs);
    void compile();
    void compileClass();
    void compileClassVarDec();
    void compileSubroutine();
    void compileParameterList();
    void compileVarDec();
    void compileStatements();
    void compileDo();
    void compileLet();
    void compileWhile();
    void compileReturn();
    void compileIf();
    void compileExpression();
    void compileTerm();
    void compileExpressionList();
private:
    JackTokenizer *tokenizer;
    std::ofstream *ofs;
    void compileType();
};


CompileEngine::CompileEngine(JackTokenizer *_tokenizer, std::ofstream *_ofs) {
    this->tokenizer = _tokenizer;
    this->ofs = _ofs;
}

void CompileEngine::compile() {
    this->tokenizer->advance();
    this->compileClass();
}

void error(std::string content) {
    std::cout << content << std::endl;
    std::exit(1);
}

void CompileEngine::compileClass() {
    if(this->tokenizer->tokenType() == TokenType::KEYWORD && this->tokenizer->keyword() == Keyword::CLASS) {
        *(this->ofs) << "<class>\n";
        this->tokenizer->advance();
        
        // class name
        if(this->tokenizer->tokenType() != TokenType::IDENTIFIER) {
            error("syntax error : class name - compileClass");
        }
        *(this->ofs) << "<identifier>" << this->tokenizer->identifier() << "</identifier>\n";
        this->tokenizer->advance();

        if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "{")) {
            error("syntax error : { - compileClass");
        }
        *(this->ofs) << "<symbol>{</symbol>\n";
        this->tokenizer->advance();

        while(true) {
            if(this->tokenizer->tokenType() == TokenType::KEYWORD && (this->tokenizer->keyword() == Keyword::STATIC || this->tokenizer->keyword() == Keyword::FIELD)) {
                this->compileClassVarDec();
            } else if(this->tokenizer->tokenType() == TokenType::KEYWORD && (this->tokenizer->keyword() == Keyword::CONSTRUCTOR || this->tokenizer->keyword() == Keyword::FUNCTION || this->tokenizer->keyword() == Keyword::MEYHOD)) {
                this->compileSubroutine();
            } else {
                break;
            }
        }

        if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "}")) {
            error("syntax error : } - compileClass");
        }
        *(this->ofs) << "<symbol>}</symbol>\n";
        this->tokenizer->advance();

        *(this->ofs) << "</class>\n";
    } else {
        error("syntax error : class - compileClass");
    }
}

void CompileEngine::compileType() {
    if(this->tokenizer->tokenType() == TokenType::KEYWORD) {
        if(this->tokenizer->keyword() == Keyword::INT) {
            *(this->ofs) << "<keyword>int</keyword>\n";
            this->tokenizer->advance();
        } else if(this->tokenizer->keyword() == Keyword::CHAR) {
            *(this->ofs) << "<keyword>char</keyword>\n";
            this->tokenizer->advance();
        } else if(this->tokenizer->keyword() == Keyword::BOOLEAN) {
            *(this->ofs) << "<keyword>boolean</keyword>\n";
            this->tokenizer->advance();
        } else {
            error("syntax error : int|char|boolean - compileType");
        }
    } else if(this->tokenizer->tokenType() == TokenType::IDENTIFIER) {
        *(this->ofs) << "<identifier>" << this->tokenizer->identifier() << "</identifier>\n";
        this->tokenizer->advance();
    } else {
        error("syntax error : type - compileType");
    }
}

void CompileEngine::compileClassVarDec() {
    if(this->tokenizer->tokenType() == TokenType::KEYWORD) {
        if(this->tokenizer->keyword() == Keyword::STATIC) {
            *(this->ofs) << "<keyword>static</keyword>\n";
        } else if(this->tokenizer->keyword() == Keyword::FIELD) {
            *(this->ofs) << "<keyword>field</keyword>\n";
        } else {
            return;
        }
        this->tokenizer->advance();

        this->compileType();

        if(this->tokenizer->tokenType() != TokenType::IDENTIFIER) {
            error("syntax error : var name - compileClassVarDec");
        }
        *(this->ofs) << "<identifier>" << this->tokenizer->identifier() << "</identifier>\n";
        this->tokenizer->advance();

        while(true) {
            if(this->tokenizer->tokenType() == TokenType::SYMBOL) {
                if(this->tokenizer->symbol() == ";") {
                    *(this->ofs) << "<symbol>;</symbol>\n";
                    this->tokenizer->advance();
                    break;
                } else if(this->tokenizer->symbol() == ",") {
                    *(this->ofs) << "<symbol>,</symbol>\n";
                    this->tokenizer->advance();

                    if(this->tokenizer->tokenType() != TokenType::IDENTIFIER) {
                        error("syntax error : var name - compileClassVarDec");
                    }
                    *(this->ofs) << "<identifier>" << this->tokenizer->identifier() << "</identifier>\n";
                    this->tokenizer->advance();

                    continue;
                } else {
                    error("syntax error : ,|; - compileClassVarDec");
                }
            } else {
                error("syntax error : *(, var name; - compileClassVarDec");
            }
        }
    }
}

void CompileEngine::compileSubroutine() {
    if(this->tokenizer->tokenType() == TokenType::KEYWORD) {
        if(this->tokenizer->keyword() == Keyword::CONSTRUCTOR) {
            *(this->ofs) << "<keyword>constructor</keyword>\n";
        } else if(this->tokenizer->keyword() == Keyword::FUNCTION) {
            *(this->ofs) << "<keyword>function</keyword>\n";
        } else if(this->tokenizer->keyword() == Keyword::MEYHOD) {
            *(this->ofs) << "<keyword>method</keyword>\n";
        } else {
            error("syntax error : constructor|function|method - compileSubroutine");
        }
        this->tokenizer->advance();

        if(this->tokenizer->tokenType() == TokenType::KEYWORD && this->tokenizer->keyword() == Keyword::VOID) {
            *(this->ofs) << "<keyword>void</keyword>\n";
            this->tokenizer->advance();
        } else {
            this->compileType();
        }

        if(this->tokenizer->tokenType() != TokenType::IDENTIFIER) {
            error("syntax error : subroutine name - compileSubroutine");
        }
        *(this->ofs) << "<identifier>" << this->tokenizer->identifier() << "</identifier>\n";
        this->tokenizer->advance();

        if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "(")) {
            error("syntax error : ( - compileSubroutine");
        }
        *(this->ofs) << "<symbol>(</symbol>\n";
        this->tokenizer->advance();

        this->compileParameterList();

        if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == ")")) {
            error("syntax error : ) - compileSubroutine");
        }
        *(this->ofs) << "<symbol>)</symbol>\n";
        this->tokenizer->advance();

        if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "{")) {
            error("syntax error : { - compileSubroutine");
        }
        *(this->ofs) << "<symbol>{</symbol>\n";
        this->tokenizer->advance();
        
        // *varDec statements
        while(true) {
            if(this->tokenizer->tokenType() == TokenType::KEYWORD && this->tokenizer->keyword() == Keyword::VAR) {
                this->compileVarDec();
                continue;
            } else {
                break;
            }
        }

        this->compileStatements();

        if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "}")) {
            error("syntax error : } - compileSubroutine");
        }
        *(this->ofs) << "<symbol>}</symbol>\n";
        this->tokenizer->advance();
        
    } else {
        error("syntax error : constructor|function|method - compileSubroutine");
    }
}

void CompileEngine::compileParameterList() {
    if(this->tokenizer->tokenType() == TokenType::IDENTIFIER || (this->tokenizer->tokenType() == TokenType::KEYWORD && (this->tokenizer->keyword() == Keyword::INT || this->tokenizer->keyword() == Keyword::CHAR || this->tokenizer->keyword() == Keyword::BOOLEAN))) {
        this->compileType();

        if(this->tokenizer->tokenType() != TokenType::IDENTIFIER) {
            error("syntax error : var name - compileParameterList");
        }
        *(this->ofs) << "<identifier>" << this->tokenizer->identifier() << "</identifier>\n";
        this->tokenizer->advance();

        while(true) {
            if(this->tokenizer->tokenType() == TokenType::IDENTIFIER || (this->tokenizer->tokenType() == TokenType::KEYWORD && (this->tokenizer->keyword() == Keyword::INT || this->tokenizer->keyword() == Keyword::CHAR || this->tokenizer->keyword() == Keyword::BOOLEAN))) {
                this->compileType();

                if(this->tokenizer->tokenType() != TokenType::IDENTIFIER) {
                    error("syntax error : var name - compileParameterList");
                }
                *(this->ofs) << "<identifier>" << this->tokenizer->identifier() << "</identifier>\n";
                this->tokenizer->advance();                
                continue;
            } else {
                break;
            }
        }
    }
}

void CompileEngine::compileVarDec() {
    if(!(this->tokenizer->tokenType() == TokenType::KEYWORD && this->tokenizer->keyword() == Keyword::VAR)) {
        error("syntax error : var - compileVarDec");
    }
    *(this->ofs) << "<keyword>var</keyword>\n";
    this->tokenizer->advance();

    this->compileType();

    if(this->tokenizer->tokenType() != TokenType::IDENTIFIER) {
        error("syntax error : var name - compileVarDec");
    }
    *(this->ofs) << "<identifier>" << this->tokenizer->identifier() << "</identifier>\n";
    this->tokenizer->advance();

    while(true) {
        if(this->tokenizer->tokenType() == TokenType::SYMBOL) {
            if(this->tokenizer->symbol() == ";") {
                *(this->ofs) << "<symbol>;</symbol>\n";
                this->tokenizer->advance();
                break;
            } else if(this->tokenizer->symbol() == ",") {
                *(this->ofs) << "<symbol>,</symbol>\n";
                this->tokenizer->advance();

                if(this->tokenizer->tokenType() != TokenType::IDENTIFIER) {
                    error("syntax error : var name - compileVarDec");
                }
                *(this->ofs) << "<identifier>" << this->tokenizer->identifier() << "</identifier>\n";
                this->tokenizer->advance();
                continue;
            } else {
                error("syntax error : ,|; - compileVarDec");
            }
        } else {
            error("syntax error : , var name|; - compileVarDec");
        }
    }
}

void CompileEngine::compileStatements() {
    while(true) {
        if(this->tokenizer->tokenType() == TokenType::KEYWORD) {
            if(this->tokenizer->keyword() == Keyword::LET) {
                this->compileLet();
            } else if(this->tokenizer->keyword() == Keyword::IF) {
                this->compileIf();
            } else if(this->tokenizer->keyword() == Keyword::WHILE) {
                this->compileWhile();
            } else if(this->tokenizer->keyword() == Keyword::DO) {
                this->compileDo();
            } else if(this->tokenizer->keyword() == Keyword::RETURN) {
                this->compileReturn();
            } else {
                break;
            }
        } else {
            break;
        }
    }
}

void CompileEngine::compileDo() {
    if(!(this->tokenizer->tokenType() == TokenType::KEYWORD && this->tokenizer->keyword() == Keyword::DO)) {
        error("syntax error : do - compileDo");
    }
    *(this->ofs) << "<keyword>do</keyword>\n";
    this->tokenizer->advance();

    // subroutine call
    if(this->tokenizer->tokenType() != TokenType::IDENTIFIER) {
        error("syntax error : subroutine name or class name or varname - compileDo");
    }
    *(this->ofs) << "<identifier>" << this->tokenizer->identifier() << "</identifier>\n";
    this->tokenizer->advance();

    if(this->tokenizer->tokenType() == TokenType::SYMBOL) {
        if(this->tokenizer->symbol() == "(") {
            *(this->ofs) << "<symbol>(</symbol>\n";
            this->tokenizer->advance();

            this->compileExpressionList();

            if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == ")")) {
                error("syntax error : ) - compileDo");
            }
            *(this->ofs) << "<symbol>)</symbol>\n";
            this->tokenizer->advance();
        } else if(this->tokenizer->symbol() == ".") {
            *(this->ofs) << "<symbol>.</symbol>\n";
            this->tokenizer->advance();

            if(this->tokenizer->tokenType() != TokenType::IDENTIFIER) {
                error("syntax error : subroutine name - compileDo");
            }
            *(this->ofs) << "<identifier>" << this->tokenizer->identifier() << "</identifier>\n";
            this->tokenizer->advance();

            if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "(")) {
                error("syntax error : ( - compileDo");
            }
            *(this->ofs) << "<symbol>(</symbol>\n";
            this->tokenizer->advance();

            this->compileExpressionList();

            if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == ")")) {
                error("syntax error : ) - compileDo");
            }
            *(this->ofs) << "<symbol>)</symbol>\n";
            this->tokenizer->advance();
        } else {
            error("syntax error : ( or . - compileDo");
        }
    }

    if(!(this->tokenizer->tokenType() != TokenType::SYMBOL && this->tokenizer->symbol() == ";")) {
        error("syntax error : ; - compileDo");
    }
    *(this->ofs) << "<symbol>;</symbol>\n";
    this->tokenizer->advance();
}

void CompileEngine::compileLet() {
    if(!(this->tokenizer->tokenType() == TokenType::KEYWORD && this->tokenizer->keyword() == Keyword::LET)) {
        error("syntax error : let - compileLet");
    }
    *(this->ofs) << "<keyword>let</keyword>\n";
    this->tokenizer->advance();

    if(this->tokenizer->tokenType() != TokenType::IDENTIFIER) {
        error("syntax error : var name - compileLet");
    }
    *(this->ofs) << "<identifier>" << this->tokenizer->identifier() << "</identifier>\n";
    this->tokenizer->advance();

    if(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "[") {
        *(this->ofs) << "<symbol>[</symbol>\n";
        this->tokenizer->advance();

        this->compileExpression();

        if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "]")) {
            error("syntax error : ] - compileLet");
        }
        *(this->ofs) << "<symbol>]</symbol>\n";
        this->tokenizer->advance();
    }

    if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "=")) {
        error("syntax error : = - compileLet");
    }
    *(this->ofs) << "<symbol>=</symbol>\n";
    this->tokenizer->advance();

    this->compileExpression();

    if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == ";")) {
        error("syntax error : ; - compileLet");
    }
    *(this->ofs) << "<symbol>;</symbol>\n";
    this->tokenizer->advance();
}

void CompileEngine::compileWhile() {
    if(!(this->tokenizer->tokenType() == TokenType::KEYWORD && this->tokenizer->keyword() == Keyword::WHILE)) {
        error("syntax error : while - compileWhile");
    }
    *(this->ofs) << "<keyword>while</keyword>\n";
    this->tokenizer->advance();

    if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "(")) {
        error("syntax error : ( - compileWhile");
    }
    *(this->ofs) << "<symbol>(</symbol>\n";
    this->tokenizer->advance();

    this->compileExpression();

    if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == ")")) {
        error("syntax error : ) - compileWhile");
    }
    *(this->ofs) << "<symbol>)</symbol>\n";
    this->tokenizer->advance();

    if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "{")) {
        error("syntax error : { - compileWhile");
    }
    *(this->ofs) << "<symbol>{</symbol>\n";
    this->tokenizer->advance();

    this->compileStatements();

    if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "}")) {
        error("syntax error : } - compileWhile");
    }
    *(this->ofs) << "<symbol>}</symbol>\n";
    this->tokenizer->advance();

}

void CompileEngine::compileReturn() {
    if(!(this->tokenizer->tokenType() == TokenType::KEYWORD && this->tokenizer->keyword() == Keyword::RETURN)) {
        error("syntax error : return - compileReturn");
    }
    *(this->ofs) << "<keyword>return</keyword>\n";
    this->tokenizer->advance();

    if(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == ";") {
        *(this->ofs) << "<symbol>;</symbol>\n";
        this->tokenizer->advance();
    } else {
        // expression?
        this->compileExpression();

        if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == ";")) {
            error("syntax error : ; - compileReturn");
        }
        *(this->ofs) << "<symbol>;</symbol>\n";
        this->tokenizer->advance();
    }
}

void CompileEngine::compileIf() {
    if(!(this->tokenizer->tokenType() == TokenType::KEYWORD && this->tokenizer->keyword() == Keyword::IF)) {
        error("syntax error : if - compileIf");
    }
    *(this->ofs) << "<keyword>if</keyword>\n";
    this->tokenizer->advance();

    if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "(")) {
        error("syntax error : ( - compileIf");
    }
    *(this->ofs) << "<symbol>(</symbol>\n";
    this->tokenizer->advance();

    this->compileExpression();
    
    if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == ")")) {
        error("syntax error : ) - compileIf");
    }
    *(this->ofs) << "<symbol>)</symbol>\n";
    this->tokenizer->advance();

    if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "{")) {
        error("syntax error : { - compileIf");
    }
    *(this->ofs) << "<symbol>{</symbol>\n";
    this->tokenizer->advance();

    this->compileStatements();
    
    if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "}")) {
        error("syntax error : } - compileIf");
    }
    *(this->ofs) << "<symbol>}</symbol>\n";
    this->tokenizer->advance();

    if(this->tokenizer->tokenType() == TokenType::KEYWORD && this->tokenizer->keyword() == Keyword::ELSE) {
        *(this->ofs) << "<keyword>else</keyword>\n";
        this->tokenizer->advance();

        if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "{")) {
            error("syntax error : { - compileIf");
        }
        *(this->ofs) << "<symbol>{</symbol>\n";
        this->tokenizer->advance();

        this->compileStatements();
        
        if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "}")) {
            error("syntax error : } - compileIf");
        }
        *(this->ofs) << "<symbol>}</symbol>\n";
        this->tokenizer->advance();
    }
}

void CompileEngine::compileExpression() {
    this->compileTerm();

    while(true) {
        if(this->tokenizer->tokenType() == TokenType::SYMBOL && (this->tokenizer->symbol() == "+" || this->tokenizer->symbol() == "-" || this->tokenizer->symbol() == "*" || this->tokenizer->symbol() == "/" || this->tokenizer->symbol() == "&" || this->tokenizer->symbol() == "|" || this->tokenizer->symbol() == "<" || this->tokenizer->symbol() == ">" || this->tokenizer->symbol() == "=")) {
            *(this->ofs) << "<symbol>" << this->tokenizer->symbol() << "</symbol>\n";
            this->tokenizer->advance();

            this->compileTerm();
        } else {
            break;
        }
    }
}

void CompileEngine::compileTerm() {
    if(this->tokenizer->tokenType() == TokenType::INT_CONST) {
        *(this->ofs) << "<integerConstant>" << this->tokenizer->intVal() << "</integerConstant>\n";
        this->tokenizer->advance();
    } else if(this->tokenizer->tokenType() == TokenType::STRING_CONST) {
        *(this->ofs) << "<stringConstant>" << this->tokenizer->stringVal() << "</stringConstant>\n";
        this->tokenizer->advance();
    } else if(this->tokenizer->tokenType() == TokenType::IDENTIFIER) {
        *(this->ofs) << "<identifier>" << this->tokenizer->identifier() << "</identifier>\n";
        this->tokenizer->advance();

        if(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "[") {
            *(this->ofs) << "<symbol>[</symbol>\n";
            this->tokenizer->advance();

            this->compileExpression();

            if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "]")) {
                error("syntax error : ] - compileTerm");
            }
            *(this->ofs) << "<symbol>]</symbol>\n";
            this->tokenizer->advance();
        } else if(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "(") {
            *(this->ofs) << "<symbol>(</symbol>\n";
            this->tokenizer->advance();

            this->compileExpressionList();

            if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == ")")) {
                error("syntax error : ) - compileTerm");
            }
            *(this->ofs) << "<symbol>)</symbol>\n";
            this->tokenizer->advance();
        } else if(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == ".") {
            *(this->ofs) << "<symbol>.</symbol>\n";
            this->tokenizer->advance();

            if(this->tokenizer->tokenType() != TokenType::IDENTIFIER) {
                error("syntax error : subroutine name - compileTerm");
            }
            *(this->ofs) << "<identifier>" << this->tokenizer->identifier() << "</identifier>\n";
            this->tokenizer->advance();

            if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "(")) {
                error("syntax error : ( - compileTerm");
            }
            *(this->ofs) << "<symbol>(</symbol>\n";
            this->tokenizer->advance();

            this->compileExpressionList();

            if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == ")")) {
                error("syntax error : ) - compileTerm");
            }
            *(this->ofs) << "<symbol>)</symbol>\n";
            this->tokenizer->advance();
        }
    } else if(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == "(") {
        *(this->ofs) << "<symbol>(</symbol>\n";
        this->tokenizer->advance();

        this->compileExpression();

        if(!(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == ")")) {
            error("syntax error : ) - compileTerm");
        }
        *(this->ofs) << "<symbol>)</symbol>\n";
        this->tokenizer->advance();
    } else if(this->tokenizer->tokenType() == TokenType::SYMBOL && (this->tokenizer->symbol() == "-" || this->tokenizer->symbol() == "~")) {
        *(this->ofs) << "<symbol>" << this->tokenizer->symbol() << "</symbol>\n";
        this->tokenizer->advance();

        this->compileTerm();
    } else {
        error("syntax error : term - compileTerm");
    }
}

void CompileEngine::compileExpressionList() {
    this->compileExpression();

    while(true) {
        if(this->tokenizer->tokenType() == TokenType::SYMBOL && this->tokenizer->symbol() == ",") {
            *(this->ofs) << "<symbol>,</symbol>\n";
            this->tokenizer->advance();

            this->compileExpression();
        } else {
            break;
        }
    }
}

void write(char fname[], std::ofstream* ofs) {
    JackTokenizer jackTokenizer(fname);

    while(jackTokenizer.hasMoreTokens()) {
        jackTokenizer.advance();
        if(jackTokenizer.tokenType() == TokenType::IDENTIFIER) {
            // std::cout << "-> identifier - [" << jackTokenizer.identifier() << "]" << std::endl;
            *ofs << "<identifier>" << jackTokenizer.identifier() << "</identifier>\n";
        } else if(jackTokenizer.tokenType() == TokenType::INT_CONST) {
            // std::cout << "-> int const [" << jackTokenizer.intVal() << "]" << std::endl;
            *ofs << "<integerConstant>" << jackTokenizer.intVal() << "</integerConstant>\n";
        } else if(jackTokenizer.tokenType() == TokenType::KEYWORD) {
            // std::cout << "-> keyword" << std::endl;
            // jackTokenizer.keyword();
            if(jackTokenizer.keyword() == Keyword::CLASS) {
                *ofs << "<keyword>class</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::CLASS) {
                *ofs << "<keyword>class</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::MEYHOD) {
                *ofs << "<keyword>method</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::FUNCTION) {
                *ofs << "<keyword>function</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::CONSTRUCTOR) {
                *ofs << "<keyword>constructor</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::INT) {
                *ofs << "<keyword>int</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::BOOLEAN) {
                *ofs << "<keyword>boolean</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::CHAR) {
                *ofs << "<keyword>char</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::VOID) {
                *ofs << "<keyword>void</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::VAR) {
                *ofs << "<keyword>var</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::STATIC) {
                *ofs << "<keyword>static</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::FIELD) {
                *ofs << "<keyword>field</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::LET) {
                *ofs << "<keyword>let</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::DO) {
                *ofs << "<keyword>do</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::IF) {
                *ofs << "<keyword>if</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::ELSE) {
                *ofs << "<keyword>else</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::WHILE) {
                *ofs << "<keyword>while</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::RETURN) {
                *ofs << "<keyword>return</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::TRUE) {
                *ofs << "<keyword>true</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::FALSE) {
                *ofs << "<keyword>false</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::KEYWORD_NULL) {
                *ofs << "<keyword>null</keyword>\n";
            } else if(jackTokenizer.keyword() == Keyword::THIS) {
                *ofs << "<keyword>this</keyword>\n";
            }
        } else if(jackTokenizer.tokenType() == TokenType::STRING_CONST) {
            // std::cout << "-> string const [" << jackTokenizer.stringVal() << "]" << std::endl;
            *ofs << "<stringConstant>" << jackTokenizer.stringVal() << "</stringConstant>\n";
        } else if(jackTokenizer.tokenType() == TokenType::SYMBOL) {
            // std::cout << "-> symbol [" << jackTokenizer.symbol() << "]" << std::endl;
            if(jackTokenizer.symbol() == "&") {
                *ofs << "<symbol>&amp;</symbol>\n";
            } else if(jackTokenizer.symbol() == "<") {
                *ofs << "<symbol>&lt;</symbol>\n";
            } else if(jackTokenizer.symbol() == ">") {
                *ofs << "<symbol>&gt;</symbol>\n";
            } else {
                *ofs << "<symbol>" << jackTokenizer.symbol() << "</symbol>\n";
            }
        } else {
            // std::cout << "-> null" << std::endl;
        }
    }
}

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cout << "file count error" << std::endl;
        return 0;
    }

    std::ofstream ofs("a.xml");
    JackTokenizer jackTokenizer(argv[1]);

    // ofs << "<tokens>\n";

    // std::string arg(argv[1]);

    // for(int i = 1; i < argc; i++) {
    //     write(argv[i], &ofs);
    // }

    // ofs << "</tokens>";
    // ofs.close();

    CompileEngine engine(&jackTokenizer, &ofs);
    engine.compile();
}