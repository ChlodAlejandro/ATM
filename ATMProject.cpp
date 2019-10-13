/*

ATM Simulation Program

Created by Chlod Aidan Alejandro

This program is distributed under GNU-AGPLv3.

Listen to the teacher. C++ is a hard thing to learn. Wait for Grade 9
and then don't listen. (Grade 9 Chlod here, and I just want to say to PLEASE
STILL LISTEN TO THE TEACHER)

The code for the SHA256 snippet is of the ownership of it's official developer. Omittion from the
code may be allowed anytime if deemed fit.

**/

#include <iostream> // input and output stream
#include <cstring> // string management
#include <ctime> // time management
#include <fstream> // file io library
#include <time.h> // randomization and time management
#include <string> // string management
#include <string.h> // string management
#include <stdlib.h> // standard library
#include <conio.h> // character capture
#include <windows.h> // windows implementation code

/**
This is the SHA256 code. This may be removed by turning the following variable false.
**/
bool enableSHA256EncryptionCode = true;
/**
The area below contains the SHA256 code, and will only stop as indicated.
**/
#define uchar unsigned char
#define uint unsigned int

#define DBL_INT_ADD(a,b,c) if (a > 0xffffffff - (c)) ++b; a += c;
#define ROTLEFT(a,b) (((a) << (b)) | ((a) >> (32-(b))))
#define ROTRIGHT(a,b) (((a) >> (b)) | ((a) << (32-(b))))

#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x) (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x) (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTRIGHT(x,17) ^ ROTRIGHT(x,19) ^ ((x) >> 10))

using namespace std;

typedef struct {
	uchar data[64];
	uint datalen;
	uint bitlen[2];
	uint state[8];
} SHA256_CTX;

uint k[64] = {
	0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
	0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
	0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
	0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
	0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
	0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
	0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
	0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

void SHA256Transform(SHA256_CTX *ctx, uchar data[])
{
	uint a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

	for (i = 0, j = 0; i < 16; ++i, j += 4)
		m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
	for (; i < 64; ++i)
		m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

	a = ctx->state[0];
	b = ctx->state[1];
	c = ctx->state[2];
	d = ctx->state[3];
	e = ctx->state[4];
	f = ctx->state[5];
	g = ctx->state[6];
	h = ctx->state[7];

	for (i = 0; i < 64; ++i) {
		t1 = h + EP1(e) + CH(e, f, g) + k[i] + m[i];
		t2 = EP0(a) + MAJ(a, b, c);
		h = g;
		g = f;
		f = e;
		e = d + t1;
		d = c;
		c = b;
		b = a;
		a = t1 + t2;
	}

	ctx->state[0] += a;
	ctx->state[1] += b;
	ctx->state[2] += c;
	ctx->state[3] += d;
	ctx->state[4] += e;
	ctx->state[5] += f;
	ctx->state[6] += g;
	ctx->state[7] += h;
}

void SHA256Init(SHA256_CTX *ctx)
{
	ctx->datalen = 0;
	ctx->bitlen[0] = 0;
	ctx->bitlen[1] = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

void SHA256Update(SHA256_CTX *ctx, uchar data[], uint len)
{
	for (uint i = 0; i < len; ++i) {
		ctx->data[ctx->datalen] = data[i];
		ctx->datalen++;
		if (ctx->datalen == 64) {
			SHA256Transform(ctx, ctx->data);
			DBL_INT_ADD(ctx->bitlen[0], ctx->bitlen[1], 512);
			ctx->datalen = 0;
		}
	}
}

void SHA256Final(SHA256_CTX *ctx, uchar hash[])
{
	uint i = ctx->datalen;

	if (ctx->datalen < 56) {
		ctx->data[i++] = 0x80;

		while (i < 56)
			ctx->data[i++] = 0x00;
	}
	else {
		ctx->data[i++] = 0x80;

		while (i < 64)
			ctx->data[i++] = 0x00;

		SHA256Transform(ctx, ctx->data);
		memset(ctx->data, 0, 56);
	}

	DBL_INT_ADD(ctx->bitlen[0], ctx->bitlen[1], ctx->datalen * 8);
	ctx->data[63] = ctx->bitlen[0];
	ctx->data[62] = ctx->bitlen[0] >> 8;
	ctx->data[61] = ctx->bitlen[0] >> 16;
	ctx->data[60] = ctx->bitlen[0] >> 24;
	ctx->data[59] = ctx->bitlen[1];
	ctx->data[58] = ctx->bitlen[1] >> 8;
	ctx->data[57] = ctx->bitlen[1] >> 16;
	ctx->data[56] = ctx->bitlen[1] >> 24;
	SHA256Transform(ctx, ctx->data);

	for (i = 0; i < 4; ++i) {
		hash[i] = (ctx->state[0] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 4] = (ctx->state[1] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 8] = (ctx->state[2] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0x000000ff;
		hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0x000000ff;
	}
}

string SHA256(char* data) {
	int strLen = strlen(data);
	SHA256_CTX ctx;
	unsigned char hash[32];
	string hashStr = "";

	SHA256Init(&ctx);
	SHA256Update(&ctx, (unsigned char*)data, strLen);
	SHA256Final(&ctx, hash);

	char s[3];
	for (int i = 0; i < 32; i++) {
		sprintf(s, "%02x", hash[i]);
		hashStr += s;
	}

	if (enableSHA256EncryptionCode) {
		return hashStr;
	} else {
		return data;
	}
}

/**
This is the end of the SHA256 Encryption Code. The following is now a combination of scratch-written
code and snippets coming from our Computer Science 2 Teacher.
**/
bool dirExists(const string& dirName_in) {
	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
	if (ftyp == INVALID_FILE_ATTRIBUTES) return false;
	if (ftyp & FILE_ATTRIBUTE_DIRECTORY) return true;
	return false;
}


string DATA_LOC = "data/";
string ACC_LOC = DATA_LOC + "accounts/";
string LOG_LOC = DATA_LOC + "log.txt";
bool LOG = true;
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE); // For use of SetConsoleTextAttribute()

void setcursor(bool, DWORD);
void setcursor();

void setcursor(bool visible, DWORD size) // set bool visible = 0 - invisible, bool visible = 1 - visible
{
	if(size == 0)
	{
		size = 20;	// default cursor size Changing to numbers from 1 to 20, decreases cursor width
	}
	CONSOLE_CURSOR_INFO lpCursor;	
	lpCursor.bVisible = visible;
	lpCursor.dwSize = size;
	SetConsoleCursorInfo(console,&lpCursor);
}

void cursor(bool visible) {
	if (visible) {
		setcursor(1, 20);
	} else {
		setcursor(0, 0);
	}
}

olog(string in) {
	if (LOG) {
		ofstream log(LOG_LOC, std::ios_base::app);
		log << in << endl;
		log.flush();
		log.close();
	}
}

void gotoxy(int x, int y){
	COORD pt = {x,y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pt);
}

lineclear(int y) {
	gotoxy(0,y);
	cout << "                                                                               ";
}

headerWrite() {
	SetConsoleTextAttribute(console, 11);
	lineclear(1);
	gotoxy(29, 1);
	cout << "ATM Simulation Program";
	SetConsoleTextAttribute(console,6);
	lineclear(2);
	gotoxy(13, 2);
	cout << "Philippine Science High School - Cagayan Valley Campus";
	SetConsoleTextAttribute(console,6);
	lineclear(3);
	gotoxy(25, 3);
	cout << "Masoc, Bayombong, Nueva Vizcaya";
	lineclear(4);
	SetConsoleTextAttribute(console,7);
}

footerWrite() {
	lineclear(23);
	SetConsoleTextAttribute(console,8);
	lineclear(24);
	gotoxy(21, 24);
	cout << "By Chlod Aidan Alejandro | Batch 2022";
	lineclear(25);
	gotoxy(17, 25);
	cout << "GNU Afferro General Public License 3 (see about)";
	SetConsoleTextAttribute(console,7);
}

ol(int len, int y, string message) {
	lineclear(y);
	gotoxy((80 - len) / 2, y);
	cout << message;
}

ollc(int len, int y, string message) {
	lineclear(y);
	gotoxy(40 - len, y);
	cout << message;
}

oll(int y, string message) {
	lineclear(y);
	gotoxy(0, y);
	cout << message;
}

olrnc(int len, int y, string message) {
	gotoxy(80 - len, y);
	cout << message;
}

menusetting(int len1, int len2, int y, string message1, string message2) {
	lineclear(y);
	gotoxy(35 - len1, y);
	SetConsoleTextAttribute(console,15);
	cout << "[";
	SetConsoleTextAttribute(console,11);
	cout << message1.at(0);
	SetConsoleTextAttribute(console,15);
	cout << "]";
	SetConsoleTextAttribute(console,10);
	cout << message1.substr(1);
	gotoxy(42, y);
	SetConsoleTextAttribute(console,15);
	cout << "[";
	SetConsoleTextAttribute(console,11);
	cout << message2.at(0);
	SetConsoleTextAttribute(console,15);
	cout << "]";
	SetConsoleTextAttribute(console,10);
	cout << message2.substr(1);
	SetConsoleTextAttribute(console,7);
}

menusetting(int x, int y, string message2) {
	gotoxy(x,y);
	SetConsoleTextAttribute(console,15);
	cout << "[";
	SetConsoleTextAttribute(console,11);
	cout << message2.at(0);
	SetConsoleTextAttribute(console,15);
	cout << "]";
	SetConsoleTextAttribute(console,10);
	cout << message2.substr(1);
	SetConsoleTextAttribute(console,7);
}

keyval(int y, string key, string value) {
	gotoxy(1,y);
	SetConsoleTextAttribute(console,13);
	cout << key;
	cout << ": ";
	SetConsoleTextAttribute(console,14);
	cout << value;
	SetConsoleTextAttribute(console,7);
}

string getpass(bool show_asterisk=true)
{
  const char BACKSPACE=8;
  const char RETURN=13;

  string password;
  unsigned char ch=0;

  while((ch=getch())!=RETURN)
    {
       if(ch==BACKSPACE)
         {
            if(password.length()!=0)
              {
                 if(show_asterisk)
                 cout <<"\b \b";
                 password.resize(password.length()-1);
              }
         }
       else if(ch==0 || ch==224) // handle escape sequences
         {
             getch(); // ignore non printable chars
             continue;
         }
       else
         {
             password+=ch;
             if(show_asterisk)
                 cout <<'*';
         }
    }
  cout <<endl;
  return password;
}

/*
HEADER1
HEADER2
HEADER3
SPACE
CONTENT(17)
SPACE
FOOTER1
FOOTER2
FOOTER3
*/

clearscr() {
	system("cls");
	headerWrite();
	footerWrite();
}

title(int y, string text) {
	gotoxy((80 - (text.length() + 8)) / 2, y);
	SetConsoleTextAttribute(console, 11);
	cout << "[ ";
	SetConsoleTextAttribute(console, 10);
	cout << "~ ";
	SetConsoleTextAttribute(console, 15);
	cout << text;
	SetConsoleTextAttribute(console, 10);
	cout << " ~";
	SetConsoleTextAttribute(console, 11);
	cout << " ]";
}

string USER_ACCNO = "", USER_NAME = "", USER_PASS = "", USER_BALANCE = "", USER_ADDRESS = "", USER_PHONENO = "";
int MAINMENUCODE;
bool EXIT = false;

newacc() {
	srand(time(NULL));
	system("title ATM - New Account");
	olog("Entered New Account mode");
	newaccstart:
	clearscr();
	//statements start
	olog("Creating new account...");
	title(5, "New Account");
	SetConsoleTextAttribute(console, 13);
	ollc(19, 7, "   Account Number: ");
	ollc(19, 8, "        Full Name: ");
	ollc(19, 9, "         Password: ");
	ollc(19, 10, "  Initial Deposit: ");
	ollc(19, 11, "          Address: ");
	ollc(19, 12, " Mobile Phone No.: ");
	string acc;
	char name[2048], deposit[2048], address[2048], mobile[2048];
	string pass,password;
	gotoxy(40, 7);
	SetConsoleTextAttribute(console, 14);
	getline(cin, acc);
	olog("Account number set.");
	
	ifstream accf;
	accf.open(ACC_LOC + acc.c_str() + ".dat");
	if (accf.good()) { 
		string termtest;
		accf >> termtest;
		if (termtest != "TERMINATED") {
			olog("Account selected exists.");
			MessageBox(NULL, "The account already exists!", "Error", MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
			goto newaccYorN;
		}
	}
	
	if (acc == "rand") {
		randstart:
		int acc3 = rand() % 99 + 100;
		int acc6 = rand() % 999999 + 100000;
		acc = to_string(acc3) + "-" + to_string(acc6);
		accf.open(ACC_LOC + acc.c_str() + ".dat");
		if (accf.good()) { 
			goto randstart;
		}
		olog("Random account number requested. Generated: " + acc);
		gotoxy(40, 7);
		cout << acc;
	} else {
		olog("Requested account number: " + acc);
	}
	term:
	gotoxy(40, 8);
	gets(name);
	if (name == "TERMINATED") {
		if (MessageBox(NULL, "Name should not be \"TERMINATED\". Try again?", "New Account", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
			goto term;
		} else {
			goto newaccYorN;
		}
	}
	olog("user full name: " + string(name));
	gotoxy(40, 9);
	password = SHA256(_strdup(getpass(true).c_str()));
	pass = "STRING DISPOSED FOR SECURITY";
	olog("user password: " + string(password));
	id:
	gotoxy(40, 10);
	gets(deposit);
	if ((deposit) == "") {
		if (MessageBox(NULL, "Deposit should not be blank. Try again?", "New Account", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
			goto id;
		} else {
			goto newaccYorN;
		}
	}
	if (stoi(deposit) < 0) {
		if (MessageBox(NULL, "Deposit should 0 or more. Try again?", "New Account", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
			goto id;
		} else {
			goto newaccYorN;
		}
	}
	olog("user deposit: " + string(deposit));
	gotoxy(40, 11);
	gets(address);
	olog("user address: " + string(address));
	gotoxy(40, 12);
	gets(mobile);
	olog("user mobile phone no: " + string(mobile));
	
	SetConsoleTextAttribute(console, 11);
	ol(34, 14, "Confirm Account Creation? [y/n]: ");
	char ans;
	do {
		ans = toupper(getch());
	} while (!strchr("YN", ans));
	
	if (ans == 'Y') {
		olog("Writing to file...");
		ofstream account;
		account.open(ACC_LOC + string(acc) + ".dat");
		account << name << endl;
		account << password << endl;
		account << deposit << endl;
		account << address << endl;
		account << mobile;
		account.close();
		SetConsoleTextAttribute(console, 10);
		ol(16, 14, "Account created.");
		olog("Written to file.");
		olog("Account created.");
	} else {
		SetConsoleTextAttribute(console, 12);
		ol(10, 14, "Cancelled.");
		olog("Write to file cancelled.");
	}
	//statements end
	
	newaccYorN:
	char again;
	SetConsoleTextAttribute(console, 11);
	ol(32, 15, "Create another account? [Y/n]: ");
	again = tolower(getch()); 
	if (again == 'y') goto newaccstart;
	else if (again == 'n') goto newaccend;
	else goto newaccillegal;
	
	newaccillegal:
		SetConsoleTextAttribute(console, 12);
	ol(26, 16, "You may only input Y or N.");
	goto newaccYorN;
	
	newaccend:
	olog("Exiting new account mode...");
	clearscr();
	SetConsoleTextAttribute(console, 7);
	return 0;
}

void withdraw(string HASH) {
	if (stoi(USER_BALANCE) <= 0) { if (MessageBox(NULL, "You have no remaining balance", "Error", MB_ICONERROR | MB_SYSTEMMODAL | MB_OK) == IDOK) goto depend; }
	withdrawStart:
	clearscr();
	SetConsoleTextAttribute(console, 13);
	title(5, "Withdraw");
	lineclear(6);
	ollc(20, 7, "        Account No.: ");
	ollc(20, 9, "    Current Balance: ");
	ollc(20, 8, "          Full Name: ");
	ollc(20, 10, "        To Withdraw: ");
	ollc(20, 12, "  Remaining Balance: ");
	char twd[2048];
	long toWithdraw;
	SetConsoleTextAttribute(console, 13);
	ollc(20, 7, "       Account No.: ");
	SetConsoleTextAttribute(console, 14);
	cout << USER_ACCNO;
	SetConsoleTextAttribute(console, 13);
	ollc(20, 8, "         Full Name: ");
	SetConsoleTextAttribute(console, 14);
	cout << USER_NAME;
	SetConsoleTextAttribute(console, 13);
	ollc(20, 9, "   Current Balance: ");
	SetConsoleTextAttribute(console, 14);
	cout << USER_BALANCE;
	wda:
	SetConsoleTextAttribute(console, 13);
	ollc(20, 10, "       To Withdraw: ");
	SetConsoleTextAttribute(console, 14);
	gets(twd);
	SetConsoleTextAttribute(console, 13);
	toWithdraw = stoi(string(twd));
	if (toWithdraw <= 0) {
		if (MessageBox(NULL, "Withdraw should more than 0. Try again?", "Withdraw", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
			goto wda;
		}
	} else if (stoi(USER_BALANCE) < toWithdraw) {
		if (MessageBox(NULL, "This is more than your balance. Try again?", "Withdraw", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
			goto wda;
		}
	} else {
		ollc(20, 12, " Remaining Balance: ");
		SetConsoleTextAttribute(console, 14);
		cout << stoi(USER_BALANCE) - toWithdraw;
		
		switch(MessageBox(NULL, "Proceed with withdrawal?", "Withdraw", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNOCANCEL)) {
			case IDYES: {
				ofstream newaccount(ACC_LOC + USER_ACCNO + ".dat");
				newaccount << USER_NAME << endl;
				newaccount << HASH << endl;
				newaccount << (stoi(USER_BALANCE) - toWithdraw) << endl;
				newaccount << USER_ADDRESS << endl;
				newaccount << USER_PHONENO;
				newaccount.close();
				USER_BALANCE = to_string(stoi(USER_BALANCE) - toWithdraw);
				olog("Account " + USER_ACCNO + "(" + USER_NAME + ") withdrew " + to_string(toWithdraw) + ". New balance: " + USER_BALANCE);
				SetConsoleTextAttribute(console, 10);
				MessageBox(NULL, "Successful withdrawal.", "Withdraw", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_OK);
				if (MessageBox(NULL, "Withdraw again?", "Withdraw", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
					goto withdrawStart;
				}
				break;
			}
			case IDNO: {
				if (MessageBox(NULL, "Withdraw again?", "Withdraw", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
					goto withdrawStart;
				}
				break;
			}
			case IDCANCEL: {
				break;
			}
		}
	}
	depend:
	clearscr();
}

void deposit(string HASH) {
	olog("Deposit menu opened");
	depositStart:
	clearscr();
	char td[2048];
	long toDeposit;
	SetConsoleTextAttribute(console, 13);
	ollc(20, 7, "       Account No.: ");
	SetConsoleTextAttribute(console, 14);
	cout << USER_ACCNO;
	SetConsoleTextAttribute(console, 13);
	ollc(20, 8, "         Full Name: ");
	SetConsoleTextAttribute(console, 14);
	cout << USER_NAME;
	SetConsoleTextAttribute(console, 13);
	ollc(20, 9, "   Current Balance: ");
	SetConsoleTextAttribute(console, 14);
	cout << USER_BALANCE;
	wda:
	SetConsoleTextAttribute(console, 13);
	ollc(20, 10, "        To Deposit: ");
	SetConsoleTextAttribute(console, 14);
	gets(td);
	if (stoi(td) < 1) {
		if (MessageBox(NULL, "Deposit should be more than 0. Try again?", "Deposit", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
			goto wda;
		}
	} else {
		SetConsoleTextAttribute(console, 13);
		toDeposit = stoi(string(td));
		ollc(20, 11, "       New Balance: ");
		SetConsoleTextAttribute(console, 14);
		cout << stoi(USER_BALANCE) + toDeposit;
		
		switch(MessageBox(NULL, "Proceed with deposit?", "Deposit", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNOCANCEL)) {
			case IDYES: {
				ofstream newaccount(ACC_LOC + USER_ACCNO + ".dat");
				newaccount << USER_NAME << endl;
				newaccount << HASH << endl;
				newaccount << (stoi(USER_BALANCE) + toDeposit) << endl;
				newaccount << USER_ADDRESS << endl;
				newaccount << USER_PHONENO;
				newaccount.close();
				USER_BALANCE = to_string(stoi(USER_BALANCE) + toDeposit);
				olog("Account " + USER_ACCNO + "(" + USER_NAME + ") deposited " + to_string(toDeposit) + ". New balance: " + USER_BALANCE);
				if (MessageBox(NULL, "Deposit again?", "Deposit", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
					goto depositStart;
				}
				break;
			}
			case IDNO: {
				if (MessageBox(NULL, "Deposit again?", "Deposit", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
					goto depositStart;
				}
				break;
			}
			case IDCANCEL: {
				break;
			}
		}
		clearscr();
		olog("Deposit menu exiting...");
	}
}

void fundtransfer(string HASH) {
	transferStart:
	if (stoi(USER_BALANCE) <= 0) {
		if (MessageBox(NULL, "You have no remaining balance", "Error", MB_ICONERROR | MB_SYSTEMMODAL | MB_OK) == IDOK) {return;}
	} else {
		olog("Fund transfer menu open.");
		int toTransfer;
		clearscr();
		string RECEIVER_ACCNO, RECEIVER_NAME, RECEIVER_PASS, RECEIVER_BALANCE, RECEIVER_ADDRESS, RECEIVER_PHONENO;
		
		SetConsoleTextAttribute(console, 13);
		ollc(20, 7, "       Account No.: ");
		ollc(20, 8, "         Full Name: ");
		ollc(20, 9, "Receiving Acc. No.: ");
		ollc(20, 10, "          Receiver: ");
		ollc(20, 11, "       To Transfer: ");
		ollc(20, 12, "       New Balance: ");
		char acc2[2048], totransfer[2048];
		SetConsoleTextAttribute(console, 13);
		ollc(20, 7, "       Account No.: ");
		SetConsoleTextAttribute(console, 14);
		cout << USER_ACCNO;
		SetConsoleTextAttribute(console, 13);
		ollc(20, 8, "         Full Name: ");
		SetConsoleTextAttribute(console, 14);
		cout << USER_NAME;
		backtorecacc:
		SetConsoleTextAttribute(console, 13);
		ollc(20, 9, "Receiving Acc. No.: ");
		SetConsoleTextAttribute(console, 14);
		
		gets(acc2);
		ifstream receiver(ACC_LOC + string(acc2) + ".dat");
		if (receiver.good()) {
			getline(receiver, RECEIVER_NAME);
			if (RECEIVER_NAME == "TERMINATED") {
				MessageBox(NULL, "This account has been terminated.", "Terminated Account", MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
				goto backtorecacc;
			}
			getline(receiver, RECEIVER_PASS);
			getline(receiver, RECEIVER_BALANCE);
			getline(receiver, RECEIVER_ADDRESS);
			getline(receiver, RECEIVER_PHONENO);
			olog("Transferring from " + USER_ACCNO + "(" + USER_NAME + ") to " + RECEIVER_ACCNO + "(" + RECEIVER_NAME + ")");
		} else {
			switch(MessageBox(NULL, "Receiving account does not exist.", "Error", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_OKCANCEL)){
				case IDOK:
					goto transferStart;
				case IDCANCEL:
					goto transferEnd;
			}		
		}
		SetConsoleTextAttribute(console, 13);
		ollc(20, 10, "          Receiver: ");
		SetConsoleTextAttribute(console, 14);
		cout << RECEIVER_NAME;
		SetConsoleTextAttribute(console, 13);
		wda:
		ollc(20, 11, "       To Transfer: ");
		SetConsoleTextAttribute(console, 14);
		gets(totransfer);
		if (stoi(totransfer) <= 0) {
			if (MessageBox(NULL, "Transfer amount should more than 0. Try again?", "Fund Transfer", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
				goto wda;
			}
		} else if (stoi(USER_BALANCE) < stoi(totransfer)) {
			if (MessageBox(NULL, "This is more than your balance. Try again?", "Fund Transfer", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
				goto wda;
			}
		} else {
			toTransfer = stoi(string(totransfer));
			SetConsoleTextAttribute(console, 13);
			ollc(20, 12, "       New Balance: ");
			SetConsoleTextAttribute(console, 14);
			cout << stoi(USER_BALANCE) - toTransfer;
			
			switch(MessageBox(NULL, "Proceed with transfer?", "Transfer", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNOCANCEL)) {
				case IDYES: {
					USER_BALANCE = to_string(atoi(USER_BALANCE.c_str()) - toTransfer);
					olog("TRANSFER STARTED. Transferring " + to_string(toTransfer));
					olog("Writing user...");
					ofstream newaccount(ACC_LOC + USER_ACCNO + ".dat");
					newaccount << USER_NAME << endl;
					olog("Written: USER_NAME - " + USER_NAME);
					newaccount << HASH << endl;
					olog("Written: HASH - " + HASH);
					newaccount << USER_BALANCE << endl;
					olog("Written: USER_BALANCE - " + USER_BALANCE);
					newaccount << USER_ADDRESS << endl;
					olog("Written: USER_ADDRESS - " + USER_ADDRESS);
					newaccount << USER_PHONENO;
					olog("Written: USER_PHONENO - " + USER_PHONENO);
					newaccount.close();
					olog("Current user written. New balance of sender: " + USER_BALANCE);
					olog("Writing target user...");
					ofstream newraccount(ACC_LOC + string(acc2) + ".dat");
					int NRECEIVER_BALANCE = (atoi(RECEIVER_BALANCE.c_str())) + toTransfer;
					newraccount << RECEIVER_NAME << endl;
					olog("Written: RECEIVER_NAME - " + RECEIVER_NAME);
					newraccount << RECEIVER_PASS << endl;
					olog("Written: RECEIVER_PASS - " + RECEIVER_PASS);
					newraccount << NRECEIVER_BALANCE << endl;
					olog("Written: NRECEIVER_BALANCE - " + NRECEIVER_BALANCE);
					newraccount << RECEIVER_ADDRESS << endl;
					olog("Written: RECEIVER_ADDRESS - " + RECEIVER_ADDRESS);
					newraccount << RECEIVER_PHONENO;
					olog("Written: RECEIVER_PHONENO - " + RECEIVER_PHONENO);
					newraccount.close();
					olog("Target user written. New balance of target: " + NRECEIVER_BALANCE);
					if (MessageBox(NULL, "Transfer to another account?", "Transfer", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
						goto transferStart;
					}
					break;
				}
				case IDNO: {
					if (MessageBox(NULL, "Transfer to another account?", "Transfer", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
						goto transferStart;
					}
					break;
				}
				case IDCANCEL: {
					break;
				}
			}
		}
	}
	transferEnd:
	clearscr();
	olog("Fund transfer menu exiting...");
}

closeaccount(string USER_PASS) {
	olog("Account termination menu opened.");
	clearscr();
	SetConsoleTextAttribute(console, 12);
	ol(15, 5, "[!] WARNING [!]");
	ol(61, 7, "This will delete your account which will remove your data and");
	ol(61, 8, "reset your current balance. Are you sure you want to do this?");
	ollc(17, 10, "Closing Account: ");
	cout << USER_ACCNO << " (" << USER_NAME << ")";
	ol(54, 11, "Typing an incorrect password will cancel this process.");
	ollc(40, 12, "Please enter your password to continue: ");
	string password = SHA256(_strdup(getpass().c_str()));
	if (password == USER_PASS) {
		if (MessageBox(NULL, ("There is still balance in the account. Continue?"), "Close Account", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDNO) {
			goto closeend;
		}
		if (MessageBox(NULL, "Are you sure you want to close this account?", "Close Account", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
			string remacc = (ACC_LOC + USER_ACCNO + ".dat");
			olog("Attempting deletion of: " + remacc);
			ofstream del(ACC_LOC + USER_ACCNO + ".dat");
			del << "TERMINATED";
			int rem = remove(remacc.c_str());
			if (rem != 0) {
				olog("Error deleting account " + USER_ACCNO + " /w err " + to_string(rem) + "(" + strerror(rem) + "). terminated instead.");
			} else {
				USER_NAME == "";
				USER_PASS == "";
				USER_BALANCE == "";
				USER_ADDRESS == "";
				USER_PHONENO == "";
				USER_ACCNO == "";
				olog("Account closed." + USER_ACCNO);
			}
			MessageBox(NULL, "Account closed successfully.", "Close Account", MB_ICONWARNING | MB_SYSTEMMODAL | MB_OK);
			return true;
		}
	} else {
		ol(49, 14, "Incorrect password. Cancelled. Exit in 3 seconds.");
		system("timeout 1 > nul");
		ol(49, 14, "Incorrect password. Cancelled. Exit in 2 seconds.");
		system("timeout 1 > nul");
		ol(49, 14, "Incorrect password. Cancelled. Exit in 1 seconds.");
		system("timeout 1 > nul");
		goto closeend;
	}
	closeend:
	clearscr();
	return false;
}

void manageaccount(string USER_PASS) {
	bool maExit = false;
	do {
		clearscr();
		system("title ATM - Account Management");
		keyval(5, "Account No.      ", USER_ACCNO);
		if (USER_NAME.length() >= 33) {
			keyval(6, "Name             ", (USER_NAME.substr(0, 32)) + "...");
		} else {
			keyval(6, "Name             ", USER_NAME);
		}
		keyval(7, "Balance          ", USER_BALANCE);
		if (USER_ADDRESS.length() >= 33) {
			keyval(8, "Address          ", (USER_ADDRESS.substr(0, 32)) + "...");
		} else {
			keyval(8, "Address          ", USER_ADDRESS);
		}
		keyval(9, "Mobile Phone No. ", USER_PHONENO);
		menusetting(72 - 16, 5, "Withdraw");
		menusetting(72 - 16, 7, "Deposit");
		menusetting(72 - 16, 9, "Transfer");
		menusetting(72 - 16, 11, "Close");
		menusetting(72 - 16, 13, "Logout");
		char ans;
		cursor(false);
		do {
			ans = toupper(getch());
		} while (!strchr("WDTCL", ans));
		cursor(true);
		switch(ans) {
			case 'W':
				olog("Entering withdraw mode");
				withdraw(USER_PASS);
				break;
			case 'D':
				olog("Entering deposit mode");
				deposit(USER_PASS);
				break;
			case 'T':
				olog("Entering fund transfer mode");
				fundtransfer(USER_PASS);
				break;
			case 'C':
				olog("Entering account deletion mode");
				if (closeaccount(USER_PASS)) {
					olog("Leaving account management");
					USER_ACCNO == "";
					USER_NAME == "";
					USER_PASS == "";
					USER_BALANCE == "";
					USER_ADDRESS == "";
					USER_PHONENO == "";
					maExit = true;
					MAINMENUCODE = 1;
				}
				break;
			case 'L': {
				if (MessageBox(NULL, "Logout?", "Logout", MB_SYSTEMMODAL | MB_ICONQUESTION | MB_YESNO) == IDYES) {
					olog("Leaving account management");
					USER_ACCNO == "";
					USER_NAME == "";
					USER_PASS == "";
					USER_BALANCE == "";
					USER_ADDRESS == "";
					USER_PHONENO == "";
					maExit = true;
					MAINMENUCODE = 1;
				}
				break;
			}
		}
		clearscr();
	} while (!maExit && USER_ACCNO != "");
}

login() {
	olog("Entered Login mode");
	loginstart:
	system("title ATM - Login");
	clearscr();
	SetConsoleTextAttribute(console, 13);
	ollc(19, 7, "   Account Number: ");
	ollc(19, 8, "        Full Name: ");
	ollc(19, 9, "         Password: ");
	SetConsoleTextAttribute(console, 14);
	//statements start
	title(5, "Login");
	char acc[2048] = {};
	SetConsoleTextAttribute(console, 13);
	ollc(19, 7, "   Account Number: ");
	SetConsoleTextAttribute(console, 14);
	gets(acc);
	olog("Account Number: " + string(acc));
	
	ifstream accountfile(ACC_LOC + string(acc) + ".dat");
	if (!accountfile.good()) {
		olog("Account does not exist.");
		MessageBox(NULL, "There are no accounts under this number.", "Error", MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
		int stat = MessageBox(NULL, "Retry logging in?", "Retry", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO);
		if (stat == IDYES) {
			goto loginstart;
		} else {
			goto loginend;
		}
	} else {
		olog("Reading account information...");
		getline(accountfile, USER_NAME);
		if (USER_NAME == "TERMINATED") {
			MessageBox(NULL, "This account has been terminated.", "Terminated Account", MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
			int stat = MessageBox(NULL, "Retry logging in?", "Retry", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO);
			if (stat == IDYES) {
				goto loginstart;
			} else {
				goto loginend;
			}
		} else {
			olog("Account login attempt made to: " + USER_NAME);
			getline(accountfile, USER_PASS);
			SetConsoleTextAttribute(console, 13);
			ollc(19, 8, "        Full Name: ");
			SetConsoleTextAttribute(console, 14);
			cout << USER_NAME;
			SetConsoleTextAttribute(console, 13);
			ollc(19, 9, "         Password: ");
			SetConsoleTextAttribute(console, 14);
			string password = SHA256(_strdup(getpass(true).c_str()));
			olog("Password: " + password);
			// authenticate
			if (USER_PASS == password) {
				olog("Logged in successfully.");
				USER_ACCNO = acc;
				getline(accountfile, USER_BALANCE);
				getline(accountfile, USER_ADDRESS);
				getline(accountfile, USER_PHONENO);
				manageaccount(password);
			} else {
				olog("Wrong password. Stored: " + USER_PASS + ", Input: " + password);
				USER_NAME = "";
				USER_PASS = "";
				MessageBox(NULL, "Incorrect password", "Incorrect password.", MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
				int stat = MessageBox(NULL, "Retry logging in?", "Retry", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO);
				if (stat == IDYES) {
					goto loginstart;
				} else {
					goto loginend;
				}
			}
		}
	}
	loginend:
	clearscr();
	return 0;
}

void opkey(int y, string key, string value) {
	gotoxy(1,y);
	SetConsoleTextAttribute(console,13);
	cout << key;
	cout << ": ";
	SetConsoleTextAttribute(console,14);
	cout << value;
	SetConsoleTextAttribute(console,7);
}

void switchkey(int y, string key, string value, bool good) {
	gotoxy(1,y);
	SetConsoleTextAttribute(console,13);
	cout << key;
	cout << ": ";
	if (good) {
		SetConsoleTextAttribute(console,10);
	} else {
		SetConsoleTextAttribute(console,12);
	}
	cout << value;
	SetConsoleTextAttribute(console,7);
}

void menuop(string message) {
	SetConsoleTextAttribute(console,15);
	cout << "[";
	SetConsoleTextAttribute(console,11);
	cout << message.at(0);
	SetConsoleTextAttribute(console,15);
	cout << "]";
	SetConsoleTextAttribute(console,10);
	cout << message.substr(1);
	SetConsoleTextAttribute(console,7);
}

void center(int y, string s) {
	ol(s.length(), y, s);
}

void about() {
	clearscr();
	olog("Entered About section.");
	cursor(false);
	SetConsoleTextAttribute(console,10);
	system("title ATM - About");
	center(5, "This program was designed to simulate the activity of a");
	center(6, "bank management program, but is labeled an ATM due to");
	center(7, "regulations imposed by this requirement.");
	
	center(9, "Users may be able to sign up and log in at their own");
	center(10, "discretion, and can easily start depositing immediately.");
	
	center(12, "Users' passwords are hashed into the SHA256 algorithm, ");
	center(13, "making it hard to determine the password but easy to log in.");
	center(14, "We keep your account completely private and secure.");
	
	center(16, "Data is stored in the data/ folder in the directory of this");
	center(17, "program. It is normally hidden, but is not encrypted. It");
	center(18, "may be deleted anytime. The data location can also be changed");
	center(19, "in the provided options menu, along with enabling the log.");
	
	center(21, "In partial fulfillment of requirements in Computer Science 2");
	getch();
	olog("Exited About section.");
	cursor(true);
	clearscr();
}

void credits() {
	clearscr();
	cursor(false);
	SetConsoleTextAttribute(console,11);
	system("title ATM - Credits");
	center(5, "Written by Chlod Aidan Alejandro from scratch.");
	
	center(7, "Created with the assistance of: ");
	center(8, "Stack Overflow");
	center(9, "C++ Forums");
	center(10, "Gil P. De Luna, Computer Science 2 Teacher");
	center(11, "Microsoft Developer Network (MSDN)");
	
	center(13, "Written in C++ and compiled using Dev-C++ compilers.");
	center(14, "File created on April 4, 2018. Development stopped on April 5.");
	center(15, "Development continued May 1, 2018. Finished May 8, 2018.");
	center(16, "Total working time: 20 hours");
	
	center(18, "An external library, named \"sha256.cpp\" was used in the development");
	center(19, "of this program. You may copy it in the root folder.");
	getch();
	cursor(true);
	clearscr();
}

void option() {
	olog("Options screen opened.");
	bool EXIT = false;
	while (!EXIT) {
	cursor(false);
	clearscr();
	opkey(6, "Primary Data Folder ", DATA_LOC);
	opkey(7, "Accounts Folder     ", ACC_LOC);
	if (LOG) {
		switchkey(8, "Logging             ", "[ON]", true);
	} else {
		switchkey(8, "Logging             ", "[OFF]", false);
	}
	opkey(9, "Log File Location   ", LOG_LOC);
	ol(16, 21, "Modify Setting: ");
	gotoxy(10, 22); menuop("Data Location");
	cout << "  "; menuop("Accounts Location");
	cout << "  "; menuop("Logging");
	cout << "  "; menuop("Exit");
	cursor(false);
		char ans;
		do {
			ans = toupper(getch());
		} while (!strchr("DALE", ans));
		cursor(true);
		switch(ans) {
			case 'D':
				{
					clearscr();
					SetConsoleTextAttribute(console, 13);
					ollc(19, 7, "Past Location: "); cout << DATA_LOC;
					string dataloc;
					ollc(19, 7, " New Location: "); getline(cin, dataloc);
					if (MessageBox(NULL, "Change data location?", "Configuration", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDNO) {
						MessageBox(NULL, "Cancelled.", "Configuration", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_OK);
					} else {
						olog("Switching data locations...");
						DATA_LOC = dataloc + "/";
						ACC_LOC = DATA_LOC + "accounts/";
						LOG_LOC = DATA_LOC + "log.txt";
						if (!dirExists(DATA_LOC)) CreateDirectory((DATA_LOC).c_str(), NULL);
						if (!dirExists(ACC_LOC)) CreateDirectory((ACC_LOC).c_str(), NULL);
						SetFileAttributes(DATA_LOC.c_str(), FILE_ATTRIBUTE_HIDDEN);
						olog("New data location set.");
						MessageBox(NULL, "New Data Location saved successfully.", "Configuration", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_OK);
					}
					break;
				}
			case 'A':
				{
					clearscr();
					SetConsoleTextAttribute(console, 13);
					ollc(19, 7, "Past Location: "); cout << ACC_LOC;
					string accloc;
					ollc(19, 7, " New Location: "); cout << DATA_LOC; getline(cin, accloc);
					if (MessageBox(NULL, "Change accounts location?", "Configuration", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDNO) {
						MessageBox(NULL, "Cancelled.", "Configuration", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_OK);
					} else {
						olog("Switching accounts location.");
						ACC_LOC = DATA_LOC + accloc + "/";
						if (!dirExists(ACC_LOC)) CreateDirectory((ACC_LOC).c_str(), NULL);
						olog("Accounts location switched.");
						MessageBox(NULL, "New Data Location saved successfully.", "Configuration", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_OK);
					}
					break;
				}
			case 'L':
				{
					lineclear(22);
					gotoxy(17, 22);
					menuop("Turn On/Off");
					cout << "  ";
					menuop("Change Location");
					cout << "  ";
					menuop("Exit");
					char ans1;
					do {
						ans1 = toupper(getch());
					} while (!strchr("TEC", ans1));
					if (ans1 == 'T') {
						if (LOG) {
							olog("Logging engine disabled.");
							LOG = false; 
						} else {
							LOG = true;
							olog("Logging engine enabled.");
						}
					} else if (ans1 == 'C') {
						clearscr();
						SetConsoleTextAttribute(console, 13);
						ollc(19, 7, "Past Location: "); cout << LOG_LOC;
						string logloc;
						ollc(19, 7, " New Location: "); cout << DATA_LOC; getline(cin, logloc);
						if (MessageBox(NULL, "Change log location?", "Configuration", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDNO) {
							olog("Cancelled log location switch.");
							MessageBox(NULL, "Cancelled.", "Configuration", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_OK);
						} else {
							olog("Switching logging location.");
							LOG_LOC = DATA_LOC + logloc;
							olog("New logging location established.");
							MessageBox(NULL, "New Data Location saved successfully.", "Configuration", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_OK);
						}
					} else {
						
					}
					break;
				}
			case 'E':
				{
					EXIT = true;
					olog("Exiting options screen...");
					break;
				}
		}
		
		ofstream config("atm.cfg");
		config << DATA_LOC << endl;
		config << ACC_LOC << endl;
		config << LOG_LOC << endl;
		config << LOG << endl;
		config.close();
	}
}

//wstring s2ws(const string& s) {
//	int len;
//	int slength = (int) s.length() + 1;
//	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
//	wchar_t* buf = new wchar_t[len];
//	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
//	wstring r(buf);
//	delete[] buf;
//	return r;		
//}
//
//LPCSTR getws(const string& s) {
//	wstring stemp = s2ws(s);
//	LPCSTR result = stemp.c_str();
//	return result;
//}

main() {
	clearscr();
	//reading config
	ifstream config("atm.cfg");
	if (config.good()) {
		ifstream config("atm.cfg");
		getline(config, DATA_LOC);
		getline(config, ACC_LOC);
		getline(config, LOG_LOC);
		config >> LOG;
		if (!dirExists(DATA_LOC)) CreateDirectory(DATA_LOC.c_str(), NULL);
		if (!dirExists(ACC_LOC)) CreateDirectory((ACC_LOC).c_str(), NULL);
		SetFileAttributes(DATA_LOC.c_str(), FILE_ATTRIBUTE_HIDDEN);
		SetFileAttributes("atm.cfg", FILE_ATTRIBUTE_HIDDEN);
	} else {
		DATA_LOC = "data/";
		ACC_LOC = DATA_LOC + "accounts/";
		LOG_LOC = DATA_LOC + "log.txt";
		LOG = true;
		if (!dirExists(DATA_LOC)) CreateDirectory(DATA_LOC.c_str(), NULL);
		if (!dirExists(ACC_LOC)) CreateDirectory((ACC_LOC).c_str(), NULL);
		ofstream config("atm.cfg");
		config << DATA_LOC << endl;
		config << ACC_LOC << endl;
		config << LOG_LOC << endl;
		config << LOG << endl;\
		config.close();
		olog("Config initialized.");
		SetFileAttributes(DATA_LOC.c_str(), FILE_ATTRIBUTE_HIDDEN);
		SetFileAttributes("atm.cfg", FILE_ATTRIBUTE_HIDDEN);
	}
	system("title ATM");
	do {	
		olog("Main Menu.");
		clearscr();
		ol(30, 5, "[ === Account Management === ]");
		menusetting(9, 5, 6, "Sign Up  ", "Login");
		ol(23, 8, "[ === Information === ]");
		menusetting(9, 7, 9, "About    ", "Credits");
		ol(19, 11, "[ === Program === ]");
		menusetting(9, 4, 12, "Options   ", "Exit");
		
		if (MAINMENUCODE != 0) {
			switch(MAINMENUCODE) {
				case 1: {
					SetConsoleTextAttribute(console, 10);
					ol(24, 15, "Successfully logged out.");
					SetConsoleTextAttribute(console, 7);
					break;
				}
				default: {
					break;
				}
			}
		}
		MAINMENUCODE = 0;
		char ans;
		cursor(false);
		do {
			ans = toupper(getch());
		} while (!strchr("SLOACE", ans));
		cursor(true);
		switch(ans) {
			case 'S':
				newacc();
				break;
			case 'L':
				login();
				break;
			case 'O':
				option();
				break;
			case 'A': 
				about();
				break;
			case 'C':
				credits();
				break;
			case 'E':
				{
					if (MessageBox(NULL, "Exit?", "Exit", MB_SYSTEMMODAL | MB_ICONQUESTION | MB_YESNO) == IDYES) {
						EXIT = true;
						olog("Exiting...");
						clearscr();
						SetConsoleTextAttribute(console, 12);
						system("title ATM Simulation Program");
						system("cls");
						ol(19, 5, "Program terminated.");
						ol(34, 7, "Press any key to close the window.");
						
						SetConsoleTextAttribute(console, 11);
						ol(22, 9, "ATM Simulation Program");
						SetConsoleTextAttribute(console,6);
						ol(54, 10, "Philippine Science High School - Cagayan Valley Campus");
						ol(31, 11, "Masoc, Bayombong, Nueva Vizcaya");
						SetConsoleTextAttribute(console,10);
						ol(41, 15, "GNU - Afferro General Public License v3.0");
						SetConsoleTextAttribute(console,11);
						ol(36, 17, "by Chlod Aidan Alejandro, Batch 2022");
						SetConsoleTextAttribute(console,7);
						cursor(false);
						system("pause > nul");
						cursor(true);
						system("exit");
					}
				}
				break;
		}
	} while (EXIT == false);
	end:
	return 0;
}
