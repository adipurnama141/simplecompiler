#include <stdio.h>
#include <stdlib.h>
FILE *fp;
FILE *fprep;
FILE *file;
char c;
int z;
char token[256];
int line;

//Tokenizer
void Scan();
void GetString();
void GetNumber();

int Look();		
void AdvToken();
void Expect(char *S);
void Error(char *S);

void MathStatement();
void Perbandingan();
void MathTerm();
void MathTerm2();
void Statements();
void CekIf();
void CekFor();
void CekDo();
void Assign();

void GetString(){
//Ekstraksi sebuah kata pada source code, mengubahnya menjadi token
//KAMUS LOKAL
	int i;
//ALGORITMA
	
	//Ekstraksi sebuah kata
	i = 0;
	do {
		if ((c >= 'a') && (c<='z')) {
			c = toupper(c);
		}
		token[i] = c;
		c = getc(fp);
		i++;
	} while (((c >= 'a') && (c<='z')) || ((c >= 'A') && (c<='Z')));
	token[i] = 0;
	
	//Ubah menjadi token
	if (strcmp(token,"BEGIN") == 0) {
		fprintf(fprep, "BEGIN\n");
	}
	else if (strcmp(token,"IF") == 0) {
		fprintf(fprep, "IF\n");
	}
	else if (strcmp(token,"END") == 0) {
		fprintf(fprep, "END\n");
	}
	else if (strcmp(token,"ELSE") == 0) {
		fprintf(fprep, "ELSE\n");
	}
	else if (strcmp(token,"FOR") == 0) {
		fprintf(fprep, "FOR\n");
	}
	else if (strcmp(token,"DO") == 0) {
		fprintf(fprep, "DO\n");
	}
	else if (strcmp(token,"THEN") == 0) {
		fprintf(fprep, "THEN\n");
	}
	else if (strcmp(token,"WHILE") == 0) {
		fprintf(fprep, "WHILE\n");
	}
	else if (strcmp(token,"INPUT") == 0) {
		fprintf(fprep, "INPUT\n");
	}
	else if (strcmp(token,"OUTPUT") == 0) {
		fprintf(fprep, "OUTPUT\n");
	} 
	else if (strcmp(token,"TO") == 0) {
		fprintf(fprep, "TO\n");
	} 
	else if (strcmp(token,"DO") == 0) {
		fprintf(fprep, "DO\n");
	} 
	else {
		fprintf(fprep, "VARIABLE\n");
	}
}


void GetNumber() {
//Ekstraksi sebuah bilangan pada source code, mengubahnya menjadi token
//KAMUS LOKAL
	int i;
//ALGORITMA
	i = 0;
	do {
		token[i] = c;
		c = getc(fp);
		i++;
	} while ((c >= '0') && (c<='9'));
	token[i] = 0;
	fprintf(fprep, "NUMBER\n");
}


void Scan() {
//Tokenizer 
//Membaca file source.txt , mengubahnya menjadi stream of token pada file prep.txt
//KAMUS LOKAL
	long cpos;
	int line;
//ALGORITMA
	line = 1;
	fprep = fopen("prep.txt","w");
	fp = fopen("source.txt","r");
	c = getc(fp);
	do {
	
	if (((c >= 'a') && (c<='z')) || ((c >= 'A') && (c<='Z'))) {
		GetString();
	}
	else if ((c >= '0') && (c<='9')) {
		GetNumber();
	}
	else if (c==10){
		fprintf(fprep, "NEWLINE\n");
		c = getc(fp);
		line++;
	}
	else if (c=='{') {
		c = getc(fp);
		cpos = 0;
		while ((c !=EOF) && (cpos ==0)) {
			if (c=='}') {
				cpos = ftell(fp);
			}
			c = getc(fp);
		}
		if (cpos == 0) {
			c = EOF;
		}
		else {
			fseek(fp,cpos,SEEK_SET);
			c = getc(fp);
		}
	}
	else if (c=='(') {
		fprintf(fprep, "BUKAKURUNG\n");
		c = getc(fp);
	}
	else if (c==')') {
		fprintf(fprep, "TUTUPKURUNG\n");	
		c = getc(fp);
	}
	else if (c=='=') {
		fprintf(fprep, "SAMADENGAN\n");
		c = getc(fp);
	}
	else if (c=='<') {
		cpos = ftell(fp);
		c = getc(fp);
		if (c=='>') {
			fprintf(fprep, "RELASI\n");
		}
		else if (c=='=') {
			fprintf(fprep, "RELASI\n");
		}
		else {
			fprintf(fprep, "RELASI\n");
			fseek(fp,cpos,SEEK_SET);
		}
		c = getc(fp);
	}
	else if (c=='>') {
		cpos = ftell(fp);
		c = getc(fp);
		if (c=='=') {
			fprintf(fprep, "RELASI\n");
		}
		else {
			fprintf(fprep, "RELASI\n");
			fseek(fp,cpos,SEEK_SET);
		}
		c = getc(fp);
	}
	else if (c=='-') {
			fprintf(fprep, "MINUS\n");
			c = getc(fp);
	}
	else if ((c=='+') || (c=='-') || (c=='*')) {
			fprintf(fprep, "OPERATOR\n");
			c = getc(fp);
	}
	else if ((c==' ') || (c=='	')) {	
		c = getc(fp);	
	}
	else {
		printf("Error at line %d : Unrecognized Character '%c'",line,c);
		exit(0);
	}
	}
	while (c!=EOF);
	
	fprintf(fprep, "ENDTOKEN\n");
	fclose(fp);
	fclose(fprep);
}




void AdvToken(){
//Membaca token selanjutnya
//KAMUS LOKAL
	int i;
//ALGORITMA
	c = getc(fp);
	i = 0;
	do {
		token[i] = c;
		c = getc(fp);
		i++;
	} while  ((c >= 'A') && (c<='Z'));
	token[i] = 0;	
	if (Look("NEWLINE")) {
		line++;
		AdvToken();
	}
}


void Expect(char *S) {
//Mengeluarkan pesan error jika current token bukan S
	if (Look(S)) {
		AdvToken();
	}
	else {
		printf("Error at line %d : %s expected , got %s\n" , line, S,token);
		exit(0);
	}
}

int Look(char *S) {
//Bernilai true apabila current token = S
	if (strcmp(token,S) == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

void Error(char *S){
//Menampilkan pesan error S
	printf("Error at line %d : %s",line,S);
	exit(0);
}

void Input() {
//variabel INPUT
	AdvToken();
	Expect("BUKAKURUNG");
	Expect("VARIABLE");
	Expect("TUTUPKURUNG");
}

void Output() {
//variabel OUTPUT
	AdvToken();
	Expect("BUKAKURUNG");
	MathStatement();
	Expect("TUTUPKURUNG");
}

	

void MathStatement(){
//variabel MATHSTATEMENT
	MathTerm();
	if (Look("OPERATOR") || Look("MINUS")) {
		AdvToken();
		MathStatement();
	}
}

void MathTerm() {	
//variabel MATHTERM
	if (Look("MINUS")) {
		AdvToken();
		MathTerm2();
	}	
	else 
		MathTerm2();
}

void MathTerm2() {
//variabel MATHTERM
	if (Look("VARIABLE") || Look("NUMBER")) {
			AdvToken();
		}
		else if (Look("BUKAKURUNG")) {
			Expect("BUKAKURUNG");
			MathStatement();
			Expect("TUTUPKURUNG");
			}
		else {
			Error("Math term expected..\n");
		}
}


void Statement() {
//variabel STATEMENT
	if (Look("INPUT")) {
		Input();
	}
	else if (Look("OUTPUT")) {
		Output();
	}
	else if (Look("IF")) {
		CekIf();
	} 
	else if (Look("FOR")) {
		CekFor();
	}
	else if (Look("VARIABLE")) {
		Assign();
	}
	else if (Look("DO")) {
		CekDo();
	}
	else if (Look("BEGIN")) {
		AdvToken();
		Statements();
		Expect("END");
	}
	else if (Look("END")) {
		//proses selesai, tidak ada statement lagi
	}
	else {
		Error("Unknown statement.\n");
	}
}


void Statements() {
//variabel STATMENTS
	Statement();
	if (CekIsStatement()) {
		Statements();
	}
	
}

int CekIsStatement() {
	if (Look("INPUT") || Look("OUTPUT") || Look("IF") || Look("FOR") || Look("DO") || Look("VARIABLE") || Look("BEGIN")) {
 		return 1;
	}
	else {
		return 0;
	}
}


void CekIf() {
//variabel IF
	Expect("IF");
	Expect("BUKAKURUNG");
	Perbandingan();
	Expect("TUTUPKURUNG");
	Expect("THEN");
	Statement();
	if (Look("ELSE")) {
		AdvToken();
		Statement();
	}
}


void Perbandingan() {
//variabel KONDISI
	MathTerm();
	if (Look("RELASI") || Look("SAMADENGAN")) {
		AdvToken();
		MathTerm();
	}
	else {
		printf("Error at line %d : Relation expected\n", line);
		exit(0);
	}
}

void CekFor() {
//variabel KONDISI
	Expect("FOR");
	Expect("VARIABLE");
	Expect("SAMADENGAN");
	Expect("NUMBER");
	Expect("TO");
	Expect("NUMBER");
	Expect("DO");
	Statement();

} 

void CekDo() {
//variabel DO
	Expect("DO");
	while (!Look("WHILE")) {
		Statement();
	}
	Expect("WHILE");
	Expect("BUKAKURUNG");
	Perbandingan();
	Expect("TUTUPKURUNG");
}


	
void Assign() {
//variabel ASSIGNMENT
	Expect("VARIABLE");
	Expect("SAMADENGAN");
	MathStatement();
}






int main() {
	int i =1;
	Scan();
	fp = fopen("prep.txt","r");
	line = 1;

	AdvToken();
	Expect("BEGIN");
	Statements();
	Expect("END");
	file = fopen("source.txt", "r");
	if (file) {
		printf("%d |", i);
		while ((c=fgetc(file)) != EOF) {
			putchar(c);
			if (c=='\n') {
				i++;
				printf("%d |",i);
			}
		}
		fclose(file);
	}
	if (Look("ENDTOKEN")) {
		printf("\n");
		printf("Compile success! (%d line)\n",line);
	}
	else {
		printf("Error at line %d :Too much %s \n",line,token);
	}
	
	return 0;
}