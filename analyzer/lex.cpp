#include "lex.h"

LexItem getNextToken(istream& in, int& linenum){
    enum TokState{START,INID,INSTRING,ININT,INREAL,INCOMMENT}
    lexstate = START;
    
    string lexeme;
    char ch;
    char beginString;
    
    while(in.get(ch)){
     //   cout << "is it here";
        switch(lexstate){
            case START:
                if(ch=='\n')
                    linenum++;
                if(isspace(ch))
                    continue;
               
                lexeme=ch;
                
                if (ch == '"' || ch == '\'') {
                    beginString = ch;
                    lexstate = INSTRING; 
                    lexeme = "";
                }
                else if (ch == '!') {
                    lexstate = INCOMMENT;
                }
                else if(isalpha(ch)){
                    lexstate = INID;
                    lexeme = toupper(ch);
                }
                else if(isdigit(ch)) {
                    lexstate = ININT;
                }
                else if(ch == '.') {
                    if (isdigit(in.peek())) {
                        lexeme = "0";
                        in.putback(ch);
                        lexstate = INREAL;
                    }
                    else {
                        string err = lexeme;
                        err += in.peek();
                        return LexItem(ERR, err, linenum);
                    }
                }
               	else {
                    Token special;
                    switch( ch ) {
                    case '+':
                        special = PLUS;
                        break;
                    case '-':
                        special = MINUS;
                        break;
                    case '*':
                        special = MULT;
                        break;
                    case '/':
                        if (in.peek() == '/') {
                            special = CONCAT;
                            in.get(ch);
                        }
                        else special = DIV;
                        break;
                    case '=':
                        if (in.peek() == '=') {
                            special = EQUAL;
                            in.get(ch);
                        }
                        else special = ASSOP;
                        break;
                    case '(':
                        special = LPAREN;
                        break;
                    case ')':
                        special = RPAREN;
                        break;
                    case '<':
                        special = LTHAN;
                        break;
                    case ',':
                        special = COMA;
                        break;
                    case ':':
                        special = COLON;
                        break;
                    default:
                        special = ERR;
                        break;
				}

				return LexItem(special, lexeme, linenum);
			}
			break;
                break;
                
            case ININT:
                if( isdigit(ch) ) {
                    lexeme += ch;
                }
                else if (ch =='.') {
                    in.putback(ch);
                    lexstate = INREAL;
                }
                else {
                    in.putback(ch);
                    return LexItem(ICONST, lexeme, linenum);
                }
                break;
                    
            case INREAL:
                if (ch == '.' && !isdigit(in.peek())) {
                    return LexItem(ERR, lexeme, linenum);
                }
                else if( isdigit(ch) || ch == '.' ) {
                    lexeme += ch;
                }
                else {
                    in.putback(ch);
                    return LexItem(RCONST, lexeme, linenum);
                } 
                break;
                    
            case INCOMMENT:
                if( ch == '\n' ) {
                    linenum++;
				    lexstate = START;
			    }
			    break;
                    
            case INSTRING:   
                if (ch == beginString) {
                    return LexItem(SCONST, lexeme, linenum); 
                }
                else if (ch == '\n') {
                    return LexItem(ERR, beginString + lexeme, linenum);
                }
                else if (ch != beginString) {
                    lexeme += ch;
                }
                break;
                    
            case INID:
                if( isalpha(ch) || isdigit(ch) ) {
                    lexeme += toupper(ch);
                }
                else {
                    in.putback(ch);
                    return id_or_kw(lexeme, linenum);
                }
                break;
        }
    }
                    
    if(in.eof()) {
		return LexItem(DONE, "eof", linenum);
    }
    else {
        return LexItem(ERR, "I/O err", linenum);  
    }      
}

LexItem id_or_kw(const string& lexeme, int linenum) {
    Token token;
    
    if (kwmap.count(lexeme) == 1) {
        token = kwmap.find(lexeme)->second;
    }
    else {
        token = IDENT;
    }
    
    return LexItem(token,lexeme,linenum);
}

ostream& operator<<(ostream& out, const LexItem& tok) {
    if (tok.GetToken() == IDENT || tok.GetToken() == ICONST || tok.GetToken() == RCONST || tok.GetToken() == SCONST) {
        string output;
        for (char c : tok.GetLexeme()){ output += toupper(c); }
        return std::cout << tokenPrint.find(tok.GetToken())->second << "(" << output << ")" << std::endl;
    }
    else if (tok.GetToken() == ERR) {
        return std::cout << "error in line " << tok.GetLinenum() << " (" << tok.GetLexeme() << ")" << std::endl;     

    }
 
    return std::cout << tokenPrint.find(tok.GetToken())->second << std::endl;
} 
