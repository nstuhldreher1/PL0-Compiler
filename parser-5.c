/*
	skeleton for Dr. Montagne's Systems Software project Summer 2022
	you may alter all of this code if desired, but you must provide clear
	instructions if you alter the compilation process or wish us to
	use different files during grading than the ones provided 
	(driver.c, compiler.h, vm.o/.c, or lex.o/.c)
*/

//Nicholas Stuhldreher
//Josh Gumprecht

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "compiler.h"

lexeme *tokens;
int lIndex = 0;
instruction *code;
int cIndex = 0;
symbol *table;
int tIndex = 0;

int level;
lexeme token;
int jmpIdx;
int jpcIdx;
int loopIdx;

int flag = 0;
int error = 0;

void emit(int opname, int level, int mvalue);
void addToSymbolTable(int k, char n[], int v, int l, int a, int m);
void mark();
int multipledeclarationcheck(char name[]);
int findsymbol(char name[], int kind);

void statement();
void getToken();
void constDeclaration();
int varDeclaration();
void procDeclaration();
void BLOCK();
void xprogram();
void expression();
void condition();
void factor();
void term();

void printparseerror(int err_code);
void printsymboltable();
void printassemblycode();

instruction *parser_code_generator(lexeme *list)
{
	int i;
	tokens = list;
	code = malloc(sizeof(instruction) * MAX_ARRAY_SIZE);
	table = malloc(sizeof(symbol) * MAX_ARRAY_SIZE);
	
	// Your code here. 
	// Make sure to remember to free table before returning always
	// and to free code and return NULL if there was an error

	xprogram();

	
	free(table);
	code[cIndex].op = -1;
	if(flag == 1){
		printparseerror(error);
		code = NULL;
	}
	return code;
}

// adds an instruction to the end of the code array
void emit(int opname, int level, int mvalue)
{
	code[cIndex].op = opname;
	code[cIndex].l = level;
	code[cIndex].m = mvalue;
	cIndex++;
}

// adds a symbol to the end of the symbol table
void addToSymbolTable(int k, char n[], int v, int l, int a, int m)
{
	table[tIndex].kind = k;
	strcpy(table[tIndex].name, n);
	table[tIndex].value = v;
	table[tIndex].level = l;
	table[tIndex].addr = a;
	table[tIndex].mark = m;
	tIndex++;
}

// starts at the end of the table and works backward (ignoring already
// marked entries (symbols of subprocedures)) to mark the symbols of
// the current procedure. it knows it's finished the current procedure
// by looking at level: if level is less than the current level and unmarked
// (meaning it belongs to the parent procedure) it stops
void mark()
{
	int i;
	for (i = tIndex - 1; i >= 0; i--)
	{
		if (table[i].mark == 1)
			continue;
		if (table[i].level < level)
			return;
		table[i].mark = 1;
	}
}

// does a linear pass through the symbol table looking for the symbol
// who's name is the passed argument. it's found a match when the names
// match, the entry is unmarked, and the level is equal to the current
// level. it returns the index of the match. returns -1 if there are no
// matches
int multipledeclarationcheck(char name[])
{
	int i;
	for (i = 0; i < tIndex; i++)
		if (table[i].mark == 0 && table[i].level == level && strcmp(name, table[i].name) == 0)
			return i;
	return -1;
}

// returns -1 if a symbol matching the arguments is not in the symbol table
// returns the index of the desired entry if found (maximizing the level value)
int findsymbol(char name[], int kind)
{
	int i;
	int max_idx = -1;
	int max_lvl = -1;
	for (i = 0; i < tIndex; i++)
	{
		if (table[i].mark == 0 && table[i].kind == kind && strcmp(name, table[i].name) == 0)
		{
			if (max_idx == -1 || table[i].level > max_lvl)
			{
				max_idx = i;
				max_lvl = table[i].level;
			}
		}
	}
	return max_idx;
}

void printparseerror(int err_code)
{
	switch (err_code)
	{
		case 1:
			printf("Parser Error: Program must be closed by a period\n");
			break;
		case 2:
			printf("Parser Error: Constant declarations should follow the pattern 'ident := number {, ident := number}'\n");
			break;
		case 3:
			printf("Parser Error: Variable declarations should follow the pattern 'ident {, ident}'\n");
			break;
		case 4:
			printf("Parser Error: Procedure declarations should follow the pattern 'ident ;'\n");
			break;
		case 5:
			printf("“Parser Error: Variables must be assigned using :=, +=, -=, *=, /=, or %= \n");
			break;
		case 6:
			printf("Parser Error: Only variables may be assigned to or read\n");
			break;
		case 7:
			printf("Parser Error: call must be followed by a procedure identifier\n");
			break;
		case 8:
			printf("Parser Error: if must be followed by then\n");
			break;
		case 9:
			printf("Parser Error: while must be followed by do\n");
			break;
		case 10:
			printf("Parser Error: Relational operator missing from condition\n");
			break;
		case 11:
			printf("Parser Error: Arithmetic expressions may only contain arithmetic operators, numbers, parentheses, constants, and variables\n");
			break;
		case 12:
			printf("Parser Error: ( must be followed by )\n");
			break;
		case 13:
			printf("Parser Error: Multiple symbols in variable and constant declarations must be separated by commas\n");
			break;
		case 14:
			printf("Parser Error: Symbol declarations should close with a semicolon\n");
			break;
		case 15:
			printf("Parser Error: Statements within begin-end must be separated by a semicolon\n");
			break;
		case 16:
			printf("Parser Error: begin must be followed by end\n");
			break;
		case 17:
			printf("Parser Error: Bad arithmetic\n");
			break;
		case 18:
			printf("Parser Error: Confliciting symbol declarations\n");
			break;
		case 19:
			printf("Parser Error: Undeclared identifier\n");
			break;
		default:
			printf("Implementation Error: unrecognized error code\n");
			break;
	}
}

void printsymboltable()
{
	int i;
	printf("Symbol Table:\n");
	printf("Kind | Name        | Value | Level | Address | Mark\n");
	printf("---------------------------------------------------\n");
	for (i = 0; i < tIndex; i++)
		printf("%4d | %11s | %5d | %5d | %5d | %5d\n", table[i].kind, table[i].name, table[i].value, table[i].level, table[i].addr, table[i].mark); 
	printf("\n");
}

void printassemblycode()
{
	int i;
	printf("Assembly Code:\n");
	printf("Line\tOP Code\tOP Name\tL\tM\n");
	for (i = 0; i < cIndex; i++)
	{
		printf("%d\t", i);
		printf("%d\t", code[i].op);
		switch (code[i].op)
		{
			case 1:
				printf("LIT\t");
				break;
			case 2:
				switch (code[i].m)
				{
					case 0:
						printf("RTN\t");
						break;
					case 1:
						printf("NEG\t");
						break;
					case 2:
						printf("ADD\t");
						break;
					case 3:
						printf("SUB\t");
						break;
					case 4:
						printf("MUL\t");
						break;
					case 5:
						printf("DIV\t");
						break;
					case 6:
						printf("MOD\t");
						break;
					case 7:
						printf("EQL\t");
						break;
					case 8:
						printf("NEQ\t");
						break;
					case 9:
						printf("LSS\t");
						break;
					case 10:
						printf("LEQ\t");
						break;
					case 11:
						printf("GTR\t");
						break;
					case 12:
						printf("GEQ\t");
						break;
					default:
						printf("err\t");
						break;
				}
				break;
			case 3:
				printf("LOD\t");
				break;
			case 4:
				printf("STO\t");
				break;
			case 5:
				printf("CAL\t");
				break;
			case 6:
				printf("INC\t");
				break;
			case 7:
				printf("JMP\t");
				break;
			case 8:
				printf("JPC\t");
				break;
			case 9:
				printf("SYS\t");
				break;
			default:
				printf("err\t");
				break;
		}
		printf("%d\t%d\n", code[i].l, code[i].m);
	}
	printf("\n");
}


//progresses token list to the next token
void getToken(){
	token = tokens[lIndex];
	lIndex++;
}

void xprogram(){
	emit(7,0,0);
	addToSymbolTable(3, "main", 0,0,0,0);
	level = -1;
	getToken();
	
	BLOCK();
	if(flag == 1){
		return;
	}
	if(token.type != periodsym){
		flag = 1;
		error = 1;
		return;
	}

	emit(9, 0, 3);
	code[0].m  = table[0].addr * 3;
	//correct call instructions
	for(int i = 0; i < cIndex; i++){
		if(code[i].op == 5){
			code[i].m = table[code[i].m].addr * 3;
		}
	}
	printsymboltable();
	printassemblycode();
}

void BLOCK(){
	level++;
	int x;
	int currentProcedure = tIndex - 1;
	if(token.type == constsym){
		constDeclaration();
		if(flag == 1){
			return;
		}
	}
	if(token.type == varsym){
		x = varDeclaration();
		if(flag == 1){
			return;
		}
	}
	if(token.type == procsym){
		procDeclaration();
		if(flag == 1){
			return;
		}
	}
	//fix address for instuction
	
	table[currentProcedure].addr = cIndex;
	emit(6, 0, x + 3);
	
	statement();
	if(flag == 1){
		return;
	}
	
	mark();
	level--;
}

void statement(){
	int index;
	lexeme temp;
    lexeme temp1;
	
	//Identifiers
	if(token.type == identsym) {
		


		if((findsymbol(token.name,1) != -1 || findsymbol(token.name, 3) != -1) && multipledeclarationcheck(token.name) == -1){
			flag = 1;
			error = 6;
			return;
		}

		if(findsymbol(token.name, 2) == -1 || table[findsymbol(token.name, 2)].mark == 1){
				error = 19;
				flag = 1;
				return;
		}	



		temp = token;
		getToken();
		if(token.type != becomessym && token.type != addassignsym && token.type != timesassignsym && token.type != divassignsym && token.type != subassignsym && token.type != modassignsym) {
			flag = 1;
			error = 5;
			return;
		}
        temp1 = token;
		getToken();

        //if arithmetic expression
        if((temp1.type != becomessym) && (temp1.type == addassignsym || temp1.type == timesassignsym || temp1.type == divassignsym || temp1.type == subassignsym || temp1.type == modassignsym)){
            index = multipledeclarationcheck(temp.name);


		    if(index == -1){
			    index = findsymbol(temp.name, 1);
			    if(index == -1){
				    index = findsymbol(temp.name, 2);
			    }
		    }
            
            emit(3, level - table[index].level, table[index].addr);//LOD
           
            expression();
            
            if(flag == 1){
                return;
            }

            //emit based on arithmentic expression
            if(temp1.type == addassignsym){
                emit(2, 0, 2);
            }
            else if(temp1.type == timesassignsym){
                emit(2, 0, 4);
            }
            else if(temp1.type == divassignsym){
                emit(2, 0, 5);
            }
            else if(temp1.type == subassignsym){
                emit(2, 0, 3);
            }
            else if(temp1.type == modassignsym){
                emit(2, 0, 6);
            }

        }
        else{
		    expression();
        }

		if(flag == 1) {
			return;
		}
		emit(4, level - table[findsymbol(temp.name, 2)].level, table[findsymbol(temp.name, 2)].addr);
	}

	//Call symbols
	else if(token.type == callsym) { //
		getToken();

		if(findsymbol(token.name,1) != -1 || findsymbol(token.name, 2) != -1 || token.type != identsym){
			flag = 1;
			error = 7;
			return;
		}
		if(findsymbol(token.name, 3) == -1 || table[findsymbol(token.name, 3)].mark == 1){
				error = 19;
				flag = 1;
				return;
		}		
		


		emit(5, level - table[findsymbol(token.name, 3)].level, findsymbol(token.name,3));
		

		getToken();
	}

	//begin statements
	else if(token.type == beginsym) {
		getToken();
		statement();
		if(flag == 1){
			return;
		}
		
		do{
			getToken();
			statement();
			if(flag == 1){
				return;
			}	
			
		}while(token.type == semicolonsym);


		

		if(token.type != endsym) {
			if(token.type == identsym ||token.type == readsym || token.type == writesym || token.type == beginsym || token.type == callsym || token.type == ifsym || token.type == whilesym) {
				flag = 1;
				error = 15;
				return;
			}
			else {
				flag = 1;
				error = 16;
				return;
			}
		}
		getToken();
		
		

		
	}

	else if(token.type == ifsym) {
		getToken();
		condition();

		if(token.type != thensym) {
			flag = 1;
			error = 8;
			return;
		}
		getToken();
		jpcIdx = cIndex; //JPC index equals current code index
		emit(8, 0, 0); //emit JPC
		statement();
		if(flag == 1){
			return;
		}
	
		
		if(token.type == elsesym) {
			jmpIdx = cIndex;//jump index equals current code index
			emit(7, 0, 0);//emit jump
			code[jpcIdx].m = cIndex * 3;
			getToken();
			
			statement();

			if(flag ==1){
				return;
			}

			code[jmpIdx].m = cIndex * 3;
		}
		else{
			code[jpcIdx].m = cIndex * 3;
		}
	}

	else if(token.type == whilesym) {
		loopIdx = cIndex;
		getToken();
		 //(because we’ll check our condition every time we loop and this is where the condition code will start)
		condition();
		if(flag== 1) return;
		
		
		if(token.type != dosym) {
			flag = 1;
			error = 9;
			return;
		}
		jpcIdx = cIndex; //jpc index equals current code index
		emit(8, 0, 0); //emit jpc
		getToken();
		statement();
		if(flag == 1) return;
		emit(7, 0, loopIdx * 3);
		code[jpcIdx].m = cIndex * 3;
	}
	else if(token.type == readsym) {

		getToken();


		if((token.type != identsym || findsymbol(token.name,1) != -1 || findsymbol(token.name, 3) != -1) && multipledeclarationcheck(token.name) == -1) {
			flag = 1;
			error = 6;
			return;
		}

		if(findsymbol(token.name, 2) == -1 || table[findsymbol(token.name, 2)].mark == 1){
				error = 19;
				flag = 1;
				return;
		}	

		emit(9, 0, 2);
		emit(4, level- table[findsymbol(token.name, 2)].level, table[findsymbol(token.name, 2)].addr);

		getToken();
		
	}
	else if(token.type == writesym) {
		getToken();
		expression();
		if (flag == 1) return;
		emit(9, 0, 1);
	}
}


void constDeclaration(){
	lexeme temp;
	
	while(token.type == commasym || token.type == constsym){

		if(token.type != commasym && token.type != constsym){
			flag = 1;
			error = 2;
			return;
		}
		
		getToken();

		//Missing identifier
		if(token.type != identsym){
			flag = 1;
			error = 2;
			return;
		}
		if(multipledeclarationcheck(token.name) != -1) {
			flag = 1;
			error = 18;
			return;
		}
		temp = token;
		getToken();
		
		//Identifier should be followed by "="
		if(token.type != becomessym){
			flag = 1;
			error = 2;
			return;
		}

		getToken();
		
		//if equal is not followed by number then error
		if(token.type != numbersym){
			flag = 1;
			error = 2;
			return;
		}
		//add identifier to symbol table 
		addToSymbolTable(1, temp.name, token.value, level, 0, 0);

		getToken();

	
		if(token.type == identsym && token.type != semicolonsym){
			flag = 1;
			error = 13;
			return;
		}
		


	}

	if(token.type != semicolonsym){
		flag = 1;
		error = 14;
		return;
	}
	getToken();
}

int varDeclaration(){
	int count = 0;
	lexeme temp;
	while(token.type == commasym || token.type == varsym){

		if(token.type != commasym && token.type != varsym){
			flag = 1;
			error = 3;
			return;
		}
		
		getToken();
		//if not identifier error is thrown
		if(token.type != identsym){
			flag = 1;
			error = 3;
			return;
		}
		if(multipledeclarationcheck(token.name) != -1){
			flag = 1;
			error = 18;
			return;
		}
		temp = token;
	
		getToken();
		addToSymbolTable(2, temp.name, token.value, level, count + 3, 0);
		count++;
		

		if(token.type == identsym && token.type != semicolonsym){
			flag = 1;
			error = 13;
			return;
		}

		
	}
	
	if(token.type != semicolonsym){
		flag = 1;
		error = 14;
		return;
	}


	getToken();
	return count;
}

void procDeclaration(){
	lexeme temp;

	while(token.type == procsym){
		
		getToken();
		//if no ident
		if(token.type != identsym){
			flag = 1;
			error = 4;
			return;
		}
		
		if(multipledeclarationcheck(token.name) != -1){
			flag = 1;
			error = 18;
			return;
		}

		temp = token;
		//address???
		addToSymbolTable(3, temp.name, 0, level, 0, 0);

		getToken();
		//if no semicolon
		if(token.type != semicolonsym){
			flag = 1;
			error = 4;
			break;
		}
		
		getToken();

		BLOCK();
		if(flag == 1) {
			return;
		}

		

		//if no semicolon after end of block
		
		if(token.type != semicolonsym){
			flag = 1;
			error = 14;
			return;
		}
		getToken();
		emit(2, 0, 0);
}
	}
	

void expression(){
	int addop;
	if(token.type == minussym){
		addop = token.type;
		getToken();
		term();
		if(flag == 1) {
			return;
		}
			emit(2,0,1);
	}
	else{
		term();
		if(flag == 1) {
			return;
		}
	}
	while(token.type == plussym || token.type == minussym){
		addop = token.type;
		getToken();
		term();
		if(flag == 1) {
			return;
		}
		
		if(addop == plussym){
			emit(2, 0, 2);
		}
		else{
			emit(2, 0, 3);
		}
	} 

	if(token.type == lparentsym || token.type == identsym || token.type == numbersym){
		flag = 1;
		error = 17;
		return;
	}
}

void term(){
	factor();
	if(flag == 1) {
		return;
	}
	int mulop = token.type;
	while(token.type == multsym || token.type == slashsym || token.type == modsym){
		mulop = token.type;
		
		if(flag == 1) return;

		if(token.type == multsym){
			getToken();
			factor();
			if(flag == 1) {
				return;
			}
			//Multiplication
			emit(2, 0 , 4);
		}
		else if(mulop == slashsym){
			getToken();
			factor();
			if(flag == 1) {
				return;
			}
			//Division
			emit(2, 0 , 5);
		}
		else{
			getToken();
			factor();
			if(flag == 1) {
				return;
			}
			emit(2, 0 , 6);
		}
	}
}

void factor(){
	int index;
	int kind;
	if(token.type == identsym ){
		index = multipledeclarationcheck(token.name);


		if(index == -1){
			index = findsymbol(token.name, 1);
			if(index == -1){
				index = findsymbol(token.name, 2);
				if(index == -1){
					index = findsymbol(token.name, 3);
				}
			}
		}


		
		//error check
		if(findsymbol(token.name, 3) != -1){
			flag = 1;
			error = 11;
			return;
		}
		//error check		
		if(findsymbol(token.name, table[index].kind) == -1 || table[findsymbol(token.name, table[index].kind)].mark == 1){
			error = 19;
			flag = 1;
			return;
		}
		if(index != -1){
			kind = table[index].kind;
		}
		
		if(kind == 1){
			emit(1, 0 , table[index].value); // LIT
		}
		else if(kind == 2){
			emit(3, level - table[index].level, table[index].addr); //LOD
		}

		getToken();
	}
	else if(token.type == numbersym){
		emit(1, 0, token.value); // LIT
		getToken();
	}
	else if(token.type == lparentsym){
		getToken();
		expression();
		if(flag == 1) {
			return;
		}
		if(token.type != rparentsym){
			error = 12;
			flag = 1;
			return;
		}
		getToken();
	}
	else if(token.type != rparentsym && token.type != lparentsym && token.type != numbersym && token.type != identsym && token.type != semicolonsym){
		error = 11;
		flag = 1;
		return;
	}	
}


void condition(){
	int relop;
	expression();
	if(flag == 1) {
		return;
	}
	if(token.type != 9 && token.type != 10 && token.type != 11 && token.type != 12 && token.type != 13 && token.type != 14){
		flag = 1;
		error = 10;
		return;
	}
	relop = token.type;
	getToken();
	expression();
	if(flag == 1) {
		return;
	}
	
	
	switch(relop){
		case 9: emit(2,0,7); break; //equal
		case 10: emit(2,0,8); break; //not equal
		case 11: emit(2,0, 9); break; // <
		case 12: emit(2, 0, 10); break; // <=
		case 13: emit(2,0,11); break;  // >
		case 14: emit(2, 0 ,12); break; //>=

	}
}