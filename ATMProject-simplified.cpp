/**

ATM Simulation Program

Created by Chlod Aidan Alejandro

This program is distributed under Apache-2.0.

Listen to the teacher. C++ is a hard thing to learn. Wait for Grade 9
and then don't listen. (Grade 9 Chlod here, and I just want to say to PLEASE
STILL LISTEN TO THE TEACHER) (Grade 10 Chlod here, and again, please listen
to the damn teacher.) (2nd year college Chlod here. LISTEN!!!)

I also suggest that you read the "What CS2 Doesn't Teach" in the README.md.

Unlike ATMProject.cpp, the SHA25 code has been removed completely. The PIN
will be stored as an integer in the data files. In reality, this is a big
no-no in terms of security, but this specific G8 Computer Science Teacher
is extremely lax with security (trust me, I've seen his code.)

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
 * Saves us the pain of having to put `std::` behind almost all things.
 **/
using namespace std;

/**
 * Complicated. All you have to know is that it converts std::string
 * into LPCWSTR, for use in WinAPI (a.k.a. windows.h) calls.
 **/
LPCWSTR s2ws(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r.c_str();
}

/**
 * Checks if a directory exists.
 **/
bool dirExists(const string& dirName_in) {
    DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES) return false;
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY) return true;
    return false;
}

/**
 * The location of the data files (the same directory with the executable, but
 * in a subfolder named "data").
 **/
string DATA_LOC = "data/";
/**
 * The location of the account files (under the data folder, in a subfolder named
 * "accounts".)
 **/
string ACC_LOC = DATA_LOC + "accounts/";
/**
 * The location of the log file (under the data folder, in a file named
 * "log.txt".) This helps you debug issues with the project.
 **/
string LOG_LOC = DATA_LOC + "log.txt";

/**
 * Is the log enabled? If yes, then this is `true`.
 **/
bool LOG = true;

// For use of SetConsoleTextAttribute()
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

/**
 * Hacky-hacky console things. Specifically, this hides the blinking cursor of
 * the command prompt.
 **/
void setCursorVisibility(bool, DWORD);
void setCursorVisibility();

/**
 * Sets the visibility of the terminal cursor.
 * 
 * @param visible false if invisible, true if visible.
 * @param size 0 if invisible; 20 if visible. 
 **/
void setCursorVisibility(bool visible, DWORD size) // set bool visible = 0 - invisible, bool visible = 1 - visible
{
    if (size == 0)
    {
        size = 20;	// default cursor size Changing to numbers from 1 to 20, decreases cursor width
    }
    // This is mostly low-level C things, so just let the beast sleep.
    CONSOLE_CURSOR_INFO lpCursor;	
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console,&lpCursor);
}

/**
 * Quickly sets the visibility of the cursor.
 * 
 * I made this on my own since I got sick of typing `setCursor`.
 * 
 * @param visible false if invisible, true if visible.
 **/
void cursor(bool visible) {
    if (visible) {
        setCursorVisibility(1, 20);
    } else {
        setCursorVisibility(0, 0);
    }
}

/**
 * `olog` means "output to log". This outputs information to the file log, if the
 * log is enabled.
 **/
void olog(string in) {
    if (LOG) {
        ofstream log(LOG_LOC, std::ios_base::app);
        log << in << endl;
        log.flush();
        log.close();
    }
}

/**
 * Sets the position of the cursor.
 * 
 * If the cursor is outside of the window, there could be issues with the look.
 * 
 * @param x The position of the cursor from the left of the window, starting at 0.
 * @param y The position of the cursor from the top of the window, starting at 0.
 **/
void gotoxy(short x, short y){
    COORD pt = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pt);
}

/**
 * Erases the entire line. This works under the assumption that the window is 80
 * columns (characters) per line.
 **/
void lineclear(int y) {
    gotoxy(0, y);
    cout << "                                                                               ";
}

int COLOR_BLACK     = 0;
int COLOR_MAROON    = 1;
int COLOR_GREEN     = 2;
int COLOR_OLIVE     = 3;
int COLOR_NAVY      = 4;
int COLOR_VIOLET    = 5;
int COLOR_TEAL      = 6;
int COLOR_LGRAY     = 7;
int COLOR_DGRAY     = 8;
int COLOR_BLUE      = 9;
int COLOR_LIME      = 10;
int COLOR_YELLOW    = 11;
int COLOR_RED       = 12;
int COLOR_PURPLE    = 13;
int COLOR_AQUA      = 14;
int COLOR_WHITE     = 15;

int COLOR_DEFAULT = COLOR_LGRAY;

/**
 * Sets the color of the terminal text.
 * 
 * @param color The color of the terminal text, as an int from 0 to 15.
 **/
void setTextColor(int color)
{
    SetConsoleTextAttribute(console, color);
}

/**
 * `ol` means "output line". This literally just outputs something
 * on the center of the current line, and clears anything that came
 * before it.
 * 
 * @param y The position of the line from the top of the terminal, starting at 0.
 * @param message The message to output.
 **/
void ol(int y, string message) {
    lineclear(y);
    gotoxy((80 - message.length()) / 2, y);
    cout << message;
}

/**
 * `ollc` means "output line - left centered". This outputs something
 * to the center of the current line, with the text going to the left
 * as more and more text is added, and clears anything that came
 * before it.
 * 
 * @param y The position of the line from the top of the terminal, starting at 0.
 * @param message The message to output.
 **/
void ollc(int y, string message) {
    lineclear(y);
    gotoxy(40 - message.length(), y);
    cout << message;
}

/**
 * `ollc` means "output line - left". This outputs something
 * to the left of the current line, and clears anything that came
 * before it.
 * 
 * @param y The position of the line from the top of the terminal, starting at 0.
 * @param message The message to output.
 **/
void oll(int y, string message) {
    lineclear(y);
    gotoxy(0, y);
    cout << message;
}

/**
 * Writes the header. It's a three-line statement with a bottom padding.
 **/
void headerWrite() {
    setTextColor(COLOR_YELLOW);
    ol(1, "ATM Simulation Program");
    setTextColor(COLOR_TEAL);
    ol(2, "Philippine Science High School - Cagayan Valley Campus");
    setTextColor(COLOR_TEAL);
    ol(3, "ATM Simulation Program");
    lineclear(4);
    setTextColor(COLOR_DEFAULT);
}

/**
 * Writes the footer. It's a two-line statement with a top padding.
 **/
void footerWrite() {
    lineclear(23);
    setTextColor(COLOR_DGRAY);
    lineclear(24);
    gotoxy(21, 24);
    cout << "By Chlod Aidan Alejandro | Batch 2022";
    lineclear(25);
    gotoxy(25, 25);
    cout << "Apache 2.0 License (see about)";
    setTextColor(COLOR_DEFAULT);
}

/**
 * Creates an option which highlights the first letter.
 * 
 * e.g. `showMenuOptions(1, 1, "About")`
 * ----- [ OUTPUT ] -----
 * [A]bout
 * ----------------------
 * 
 * @param x The position of the line from the left of the window, starting at 0.
 * @param y The position of the line from the top of the window, starting at 0.
 * @param message Content of the first message.
 **/
void showMenuOptions(int x, int y, string message) {
    gotoxy(x,y);
    setTextColor(COLOR_WHITE);
    cout << "[";
    setTextColor(COLOR_YELLOW);
    cout << message.at(0);
    setTextColor(COLOR_WHITE);
    cout << "]";
    setTextColor(COLOR_LIME);
    cout << message.substr(1);
    setTextColor(COLOR_DEFAULT);
}

/**
 * Similar to `showMenuOptions` but create two options next to each other.
 * 
 * This is automatically centered.
 * 
 * e.g. `showMenuOptions(9, 7, 1, "About    ", "Credits")`
 * ----- [ OUTPUT ] -----
 * [A]bout  [C]redits
 * ----------------------
 * 
 * @param len1 Length of the first message.
 * @param len2 Length of the second message.
 * @param y The position of the line from the top of the terminal, starting at 0.
 * @param message1 Content of the first message.
 * @param message2 Content of the second message.
 **/
void showMenuOptions(int len1, int len2, int y, string message1, string message2) {
    lineclear(y);
    gotoxy(35 - len1, y);
    setTextColor(COLOR_WHITE);
    cout << "[";
    setTextColor(COLOR_YELLOW);
    cout << message1.at(0);
    setTextColor(COLOR_WHITE);
    cout << "]";
    setTextColor(COLOR_LIME);
    cout << message1.substr(1);
    gotoxy(42, y);
    setTextColor(COLOR_WHITE);
    cout << "[";
    setTextColor(COLOR_YELLOW);
    cout << message2.at(0);
    setTextColor(COLOR_WHITE);
    cout << "]";
    setTextColor(COLOR_LIME);
    cout << message2.substr(1);
    setTextColor(COLOR_DEFAULT);
}

/**
 * Writes a field display. Basically just the name of the field and its
 * value.
 * 
 * e.g. `keyval(1, "Intellect", "0%")`
 * ----- [ OUTPUT ] -----
 * Intellect: 0%
 * ----------------------
 * 
 * @param y The position of the line from the top of the terminal, starting at 0.
 * @param key The key of the field
 * @param value The value of the field
 **/
void keyval(int y, string key, string value) {
    gotoxy(1,y);
    setTextColor(COLOR_PURPLE);
    cout << key;
    cout << ": ";
    setTextColor(COLOR_AQUA);
    cout << value;
    setTextColor(COLOR_DEFAULT);
}

/**
 * Gets a password from user input.
 * 
 * @param showAsterisk If false, nothing is placed in replacement of the password.
 **/
string getpass(bool showAsterisk=true)
{
    // ASCII character code for the backspace character (BS)
    const char BACKSPACE = 8;
    // ASCII character code for the new line character (LF)
    const char RETURN = 13;

    // The password, as a string.
    string password;
    // The current character.
    unsigned char ch = 0;

    // What the hell is happening here?!?!?!?
    // `ch` is set to the next character typed by the user (hence `ch = getch()`).
    // 
    // If you wrap the assignment in parentheses, you can get the value of said
    // assignment. i.e. ((variable = 3) == 3) will return true, and variable is equal
    // to 3.
    //
    // This is the same thing, except it compares the given character with the
    // RETURN character code, since all ASCII characters have an integer equivalent.
    // e.g. A = 65, a = 95
    //
    // So basically, if the user hasn't hit the Enter key,
    while ((ch = getch()) != RETURN) {
        // Check if they pressed the Backspace key
        if (ch == BACKSPACE) { // If they did, 
            // If the password is not empty (to prevent over-erasing)
            if (password.length() != 0) {
                // If we're showing asterisks
                if (showAsterisk)
                    // This is backspace, erase, backspace. a.k.a.,
                    // Go back one lettecharacterr, place a space (clears the character
                    // that was there before), then go back one character again.
                    cout <<"\b \b";

                // Cut the password short by one character (since we erased it.)
                password.resize(password.length() - 1);
            }
        } else if (ch == 0 || ch == 224) { // Handle escape sequences
            getch(); // Ignore anything that can't be put into the console.
            continue;
        } else {
            // Otherwise, if they put in a normal character,
            password += ch;
            // If we're showing an asterisk,
            if (showAsterisk)
                // Show the damn asterisk.
                cout <<'*';
        }
    }

    // Put a new line at the end.
    cout << endl;
    // Return the password given.
    return password;
}


/**
 * Clears the screen and rewrites the header and footer.
 **/
void clearscr() {
    system("cls");
    headerWrite();
    footerWrite();
}

/**
 * Print a title for the current menu.
 * 
 * e.g. `title(1, "Title")`
 * ----- [ OUTPUT ] -----
 * [ ~ Title ~ ]
 * ----------------------
 * 
 * @param y The position of the line from the top of the terminal, starting at 0.
 * @param text The text to be used as the title.
 **/
void title(int y, string text) {
    gotoxy((80 - (text.length() + 8)) / 2, y);
    setTextColor(COLOR_YELLOW);
    cout << "[ ";
    setTextColor(COLOR_LIME);
    cout << "~ ";
    setTextColor(COLOR_WHITE);
    cout << text;
    setTextColor(COLOR_LIME);
    cout << " ~";
    setTextColor(COLOR_YELLOW);
    cout << " ]";
}

// Current user data. Classes weren't introduced for G8 C++, so instead
// we're saving session variables as variables themselves.
string USER_ACCNO = "";
string USER_NAME = "";
string USER_PASS = "";
string USER_BALANCE = "";
string USER_ADDRESS = "";
string USER_PHONENO = "";

// This is 1 if the main menu is being displayed.
int MAINMENUCODE;
// True if we're going to exit.
bool EXIT = false;

/**
 * The new account menu. Handles account creation.
 **/
void newacc() {
    // Seed the randomizer (a.k.a. shuffle the box, per se)
    srand(time(NULL));

    system("title ATM - New Account");
    olog("Entered New Account mode");

    // A label! What is this for? It's for repeating tasks!
    newaccstart:
    clearscr();
    
    olog("Creating new account...");
    title(5, "New Account");
    setTextColor(COLOR_PURPLE);
    ollc(7, "   Account Number: ");
    ollc(8, "        Full Name: ");
    ollc(9, "         Password: ");
    ollc(10, "  Initial Deposit: ");
    ollc(11, "          Address: ");
    ollc(12, " Mobile Phone No.: ");

    string acc;
    char name[2048], deposit[2048], address[2048], mobile[2048];
    string pass, password;
    gotoxy(40, 7);
    setTextColor(COLOR_AQUA);
    getline(cin, acc);
    olog("Account number set.");
    
    ifstream accf;
    accf.open(ACC_LOC + acc.c_str() + ".dat");
    if (accf.good()) { 
        string termtest;
        accf >> termtest;
        if (termtest != "TERMINATED") {
            olog("Account selected exists.");
            MessageBoxW(NULL, L"The account already exists!", L"Error", MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
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
        if (MessageBoxW(NULL, L"Name should not be \"TERMINATED\". Try again?", L"New Account", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
            goto term;
        } else {
            goto newaccYorN;
        }
    }
    olog("user full name: " + string(name));
    gotoxy(40, 9);
    password = getpass(true).c_str();
    pass = "STRING DISPOSED FOR SECURITY";
    olog("user password: " + string(password));
    id:
    gotoxy(40, 10);
    gets(deposit);
    if ((deposit) == "") {
        if (MessageBoxW(NULL, L"Deposit should not be blank. Try again?", L"New Account", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
            goto id;
        } else {
            goto newaccYorN;
        }
    }
    if (stoi(deposit) < 0) {
        if (MessageBoxW(NULL, L"Deposit should 0 or more. Try again?", L"New Account", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
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
    
    setTextColor(COLOR_YELLOW);
    ol(14, "Confirm Account Creation? [y/n]: ");
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
        setTextColor(COLOR_LIME);
        ol(14, "Account created.");
        olog("Written to file.");
        olog("Account created.");
    } else {
        setTextColor(COLOR_RED);
        ol(14, "Cancelled.");
        olog("Write to file cancelled.");
    }
    //statements end
    
    newaccYorN:
    char again;
    setTextColor(COLOR_YELLOW);
    ol(15, "Create another account? [Y/n]: ");
    again = tolower(getch()); 
    if (again == 'y') goto newaccstart;
    else if (again == 'n') goto newaccend;
    else goto newaccillegal;
    
    newaccillegal:
        setTextColor(COLOR_RED);
    ol(16, "You may only input Y or N.");
    goto newaccYorN;
    
    newaccend:
    olog("Exiting new account mode...");
    clearscr();
    setTextColor(COLOR_DEFAULT);
    return;
}

void withdraw(string HASH) {
    if (stoi(USER_BALANCE) <= 0) { if (MessageBoxW(NULL, L"You have no remaining balance", L"Error", MB_ICONERROR | MB_SYSTEMMODAL | MB_OK) == IDOK) goto depend; }
    withdrawStart:
    clearscr();
    setTextColor(COLOR_PURPLE);
    title(5, "Withdraw");
    lineclear(6);
    ollc(7, "        Account No.: ");
    ollc(9, "    Current Balance: ");
    ollc(8, "          Full Name: ");
    ollc(10, "        To Withdraw: ");
    ollc(12, "  Remaining Balance: ");
    char twd[2048];
    long toWithdraw;
    setTextColor(COLOR_PURPLE);
    ollc(7, "       Account No.: ");
    setTextColor(COLOR_AQUA);
    cout << USER_ACCNO;
    setTextColor(COLOR_PURPLE);
    ollc(8, "         Full Name: ");
    setTextColor(COLOR_AQUA);
    cout << USER_NAME;
    setTextColor(COLOR_PURPLE);
    ollc(9, "   Current Balance: ");
    setTextColor(COLOR_AQUA);
    cout << USER_BALANCE;
    wda:
    setTextColor(COLOR_PURPLE);
    ollc(10, "       To Withdraw: ");
    setTextColor(COLOR_AQUA);
    gets(twd);
    setTextColor(COLOR_PURPLE);
    toWithdraw = stoi(string(twd));
    if (toWithdraw <= 0) {
        if (MessageBoxW(NULL, L"Withdraw should more than 0. Try again?", L"Withdraw", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
            goto wda;
        }
    } else if (stoi(USER_BALANCE) < toWithdraw) {
        if (MessageBoxW(NULL, L"This is more than your balance. Try again?", L"Withdraw", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
            goto wda;
        }
    } else {
        ollc(12, " Remaining Balance: ");
        setTextColor(COLOR_AQUA);
        cout << stoi(USER_BALANCE) - toWithdraw;
        
        switch(MessageBoxW(NULL, L"Proceed with withdrawal?", L"Withdraw", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNOCANCEL)) {
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
                setTextColor(COLOR_LIME);
                MessageBoxW(NULL, L"Successful withdrawal.", L"Withdraw", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_OK);
                if (MessageBoxW(NULL, L"Withdraw again?", L"Withdraw", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
                    goto withdrawStart;
                }
                break;
            }
            case IDNO: {
                if (MessageBoxW(NULL, L"Withdraw again?", L"Withdraw", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
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
    setTextColor(COLOR_PURPLE);
    ollc(7, "       Account No.: ");
    setTextColor(COLOR_AQUA);
    cout << USER_ACCNO;
    setTextColor(COLOR_PURPLE);
    ollc(8, "         Full Name: ");
    setTextColor(COLOR_AQUA);
    cout << USER_NAME;
    setTextColor(COLOR_PURPLE);
    ollc(9, "   Current Balance: ");
    setTextColor(COLOR_AQUA);
    cout << USER_BALANCE;
    wda:
    setTextColor(COLOR_PURPLE);
    ollc(10, "        To Deposit: ");
    setTextColor(COLOR_AQUA);
    gets(td);
    if (stoi(td) < 1) {
        if (MessageBoxW(NULL, L"Deposit should be more than 0. Try again?", L"Deposit", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
            goto wda;
        }
    } else {
        setTextColor(COLOR_PURPLE);
        toDeposit = stoi(string(td));
        ollc(11, "       New Balance: ");
        setTextColor(COLOR_AQUA);
        cout << stoi(USER_BALANCE) + toDeposit;
        
        switch(MessageBoxW(NULL, L"Proceed with deposit?", L"Deposit", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNOCANCEL)) {
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
                if (MessageBoxW(NULL, L"Deposit again?", L"Deposit", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
                    goto depositStart;
                }
                break;
            }
            case IDNO: {
                if (MessageBoxW(NULL, L"Deposit again?", L"Deposit", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
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
        if (MessageBoxW(NULL, L"You have no remaining balance", L"Error", MB_ICONERROR | MB_SYSTEMMODAL | MB_OK) == IDOK) {return;}
    } else {
        olog("Fund transfer menu open.");
        int toTransfer;
        clearscr();
        string RECEIVER_ACCNO, RECEIVER_NAME, RECEIVER_PASS, RECEIVER_BALANCE, RECEIVER_ADDRESS, RECEIVER_PHONENO;
        
        setTextColor(COLOR_PURPLE);
        ollc(7, "       Account No.: ");
        ollc(8, "         Full Name: ");
        ollc(9, "Receiving Acc. No.: ");
        ollc(10, "          Receiver: ");
        ollc(11, "       To Transfer: ");
        ollc(12, "       New Balance: ");
        char acc2[2048], totransfer[2048];
        setTextColor(COLOR_PURPLE);
        ollc(7, "       Account No.: ");
        setTextColor(COLOR_AQUA);
        cout << USER_ACCNO;
        setTextColor(COLOR_PURPLE);
        ollc(8, "         Full Name: ");
        setTextColor(COLOR_AQUA);
        cout << USER_NAME;
        backtorecacc:
        setTextColor(COLOR_PURPLE);
        ollc(9, "Receiving Acc. No.: ");
        setTextColor(COLOR_AQUA);
        
        gets(acc2);
        ifstream receiver(ACC_LOC + string(acc2) + ".dat");
        if (receiver.good()) {
            getline(receiver, RECEIVER_NAME);
            if (RECEIVER_NAME == "TERMINATED") {
                MessageBoxW(NULL, L"This account has been terminated.", L"Terminated Account", MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
                goto backtorecacc;
            }
            getline(receiver, RECEIVER_PASS);
            getline(receiver, RECEIVER_BALANCE);
            getline(receiver, RECEIVER_ADDRESS);
            getline(receiver, RECEIVER_PHONENO);
            olog("Transferring from " + USER_ACCNO + "(" + USER_NAME + ") to " + RECEIVER_ACCNO + "(" + RECEIVER_NAME + ")");
        } else {
            switch(MessageBoxW(NULL, L"Receiving account does not exist.", L"Error", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_OKCANCEL)){
                case IDOK:
                    goto transferStart;
                case IDCANCEL:
                    goto transferEnd;
            }		
        }
        setTextColor(COLOR_PURPLE);
        ollc(10, "          Receiver: ");
        setTextColor(COLOR_AQUA);
        cout << RECEIVER_NAME;
        setTextColor(COLOR_PURPLE);
        wda:
        ollc(11, "       To Transfer: ");
        setTextColor(COLOR_AQUA);
        gets(totransfer);
        if (stoi(totransfer) <= 0) {
            if (MessageBoxW(NULL, L"Transfer amount should more than 0. Try again?", L"Fund Transfer", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
                goto wda;
            }
        } else if (stoi(USER_BALANCE) < stoi(totransfer)) {
            if (MessageBoxW(NULL, L"This is more than your balance. Try again?", L"Fund Transfer", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
                goto wda;
            }
        } else {
            toTransfer = stoi(string(totransfer));
            setTextColor(COLOR_PURPLE);
            ollc(12, "       New Balance: ");
            setTextColor(COLOR_AQUA);
            cout << stoi(USER_BALANCE) - toTransfer;
            
            switch(MessageBoxW(NULL, L"Proceed with transfer?", L"Transfer", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNOCANCEL)) {
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
                    if (MessageBoxW(NULL, L"Transfer to another account?", L"Transfer", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
                        goto transferStart;
                    }
                    break;
                }
                case IDNO: {
                    if (MessageBoxW(NULL, L"Transfer to another account?", L"Transfer", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
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

bool closeaccount(string USER_PASS) {
    olog("Account termination menu opened.");
    clearscr();
    setTextColor(COLOR_RED);
    ol(5, "[!] WARNING [!]");
    ol(7, "This will delete your account which will remove your data and");
    ol(8, "reset your current balance. Are you sure you want to do this?");
    ollc(10, "Closing Account: ");
    cout << USER_ACCNO << " (" << USER_NAME << ")";
    ol(11, "Typing an incorrect password will cancel this process.");
    ollc(12, "Please enter your password to continue: ");
    string password = getpass().c_str();
    if (password == USER_PASS) {
        if (MessageBoxW(NULL, L"There is still balance in the account. Continue?", L"Close Account", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDNO) {
            goto closeend;
        }
        if (MessageBoxW(NULL, L"Are you sure you want to close this account?", L"Close Account", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDYES) {
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
            MessageBoxW(NULL, L"Account closed successfully.", L"Close Account", MB_ICONWARNING | MB_SYSTEMMODAL | MB_OK);
            return true;
        }
    } else {
        ol(14, "Incorrect password. Cancelled. Exit in 3 seconds.");
        system("timeout 1 > nul");
        ol(14, "Incorrect password. Cancelled. Exit in 2 seconds.");
        system("timeout 1 > nul");
        ol(14, "Incorrect password. Cancelled. Exit in 1 seconds.");
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
        showMenuOptions(72 - 16, 5, "Withdraw");
        showMenuOptions(72 - 16, 7, "Deposit");
        showMenuOptions(72 - 16, 9, "Transfer");
        showMenuOptions(72 - 16, 11, "Close");
        showMenuOptions(72 - 16, 13, "Logout");
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
                if (MessageBoxW(NULL, L"Logout?", L"Logout", MB_SYSTEMMODAL | MB_ICONQUESTION | MB_YESNO) == IDYES) {
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

void login() {
    olog("Entered Login mode");
    loginstart:
    system("title ATM - Login");
    clearscr();
    setTextColor(COLOR_PURPLE);
    ollc(7, "   Account Number: ");
    ollc(8, "        Full Name: ");
    ollc(9, "         Password: ");
    setTextColor(COLOR_AQUA);
    //statements start
    title(5, "Login");
    char acc[2048] = {};
    setTextColor(COLOR_PURPLE);
    ollc(7, "   Account Number: ");
    setTextColor(COLOR_AQUA);
    gets(acc);
    olog("Account Number: " + string(acc));
    
    ifstream accountfile(ACC_LOC + string(acc) + ".dat");
    if (!accountfile.good()) {
        olog("Account does not exist.");
        MessageBoxW(NULL, L"There are no accounts under this number.", L"Error", MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
        int stat = MessageBoxW(NULL, L"Retry logging in?", L"Retry", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO);
        if (stat == IDYES) {
            goto loginstart;
        } else {
            goto loginend;
        }
    } else {
        olog("Reading account information...");
        getline(accountfile, USER_NAME);
        if (USER_NAME == "TERMINATED") {
            MessageBoxW(NULL, L"This account has been terminated.", L"Terminated Account", MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
            int stat = MessageBoxW(NULL, L"Retry logging in?", L"Retry", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO);
            if (stat == IDYES) {
                goto loginstart;
            } else {
                goto loginend;
            }
        } else {
            olog("Account login attempt made to: " + USER_NAME);
            getline(accountfile, USER_PASS);
            setTextColor(COLOR_PURPLE);
            ollc(8, "        Full Name: ");
            setTextColor(COLOR_AQUA);
            cout << USER_NAME;
            setTextColor(COLOR_PURPLE);
            ollc(9, "         Password: ");
            setTextColor(COLOR_AQUA);
            string password = getpass(true).c_str();
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
                MessageBoxW(NULL, L"Incorrect password", L"Incorrect password.", MB_ICONERROR | MB_SYSTEMMODAL | MB_OK);
                int stat = MessageBoxW(NULL, L"Retry logging in?", L"Retry", MB_ICONQUESTION | MB_SYSTEMMODAL | MB_YESNO);
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
    return;
}

void opkey(int y, string key, string value) {
    gotoxy(1,y);
    setTextColor(COLOR_PURPLE);
    cout << key;
    cout << ": ";
    setTextColor(COLOR_AQUA);
    cout << value;
    setTextColor(COLOR_DEFAULT);
}

void switchkey(int y, string key, string value, bool good) {
    gotoxy(1,y);
    setTextColor(COLOR_PURPLE);
    cout << key;
    cout << ": ";
    if (good) {
        setTextColor(COLOR_LIME);
    } else {
        setTextColor(COLOR_RED);
    }
    cout << value;
    setTextColor(COLOR_DEFAULT);
}

void menuop(string message) {
    setTextColor(COLOR_WHITE);
    cout << "[";
    setTextColor(COLOR_YELLOW);
    cout << message.at(0);
    setTextColor(COLOR_WHITE);
    cout << "]";
    setTextColor(COLOR_LIME);
    cout << message.substr(1);
    setTextColor(COLOR_DEFAULT);
}

void about() {
    clearscr();
    olog("Entered About section.");
    cursor(false);
    setTextColor(COLOR_LIME);
    system("title ATM - About");
    ol(5, "This program was designed to simulate the activity of a");
    ol(6, "bank management program, but is labeled an ATM due to");
    ol(7, "regulations imposed by this requirement.");
    
    ol(9, "Users may be able to sign up and log in at their own");
    ol(10, "discretion, and can easily start depositing immediately.");
    
    ol(12, "This version (simplifed) removes the SHA256 hashing function");
    ol(13, "entirely. It also includes optimizations for newer C++");
    ol(14, "versions. Also, it's way more easy to understand this time.");
    
    ol(16, "Data is stored in the data/ folder in the directory of this");
    ol(17, "program. It is normally hidden, but is not encrypted. It");
    ol(18, "may be deleted anytime. The data location can also be changed");
    ol(19, "in the provided options menu, along with enabling the log.");
    
    ol(21, "In partial fulfillment of requirements in Computer Science 2");
    getch();
    olog("Exited About section.");
    cursor(true);
    clearscr();
}

void credits() {
    clearscr();
    cursor(false);
    setTextColor(COLOR_YELLOW);
    system("title ATM - Credits");
    ol(5, "Written by Chlod Aidan Alejandro from scratch.");
    
    ol(7, "Created with the assistance of: ");
    ol(8, "Stack Overflow");
    ol(9, "C++ Forums");
    ol(10, "Gil P. De Luna, Computer Science 2 Teacher");
    ol(11, "Microsoft Developer Network (MSDN)");
    
    ol(13, "Written in C++ and compiled using MinGW64 compilers.");
    ol(14, "File created on April 4, 2018. Development paused on April 5.");
    ol(15, "Development continued May 1, 2018. Finished May 8, 2018.");
    ol(16, "Total working time: 20 hours");
    
    ol(18, "This is an improved version of the original. All documented.");
    ol(19, "This also replaces the password and hashing with a PIN.");
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
    ol(21, "Modify Setting: ");
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
                    setTextColor(COLOR_PURPLE);
                    ollc(7, "Past Location: "); cout << DATA_LOC;
                    string dataloc;
                    ollc(7, " New Location: "); getline(cin, dataloc);
                    if (MessageBoxW(NULL, L"Change data location?", L"Configuration", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDNO) {
                        MessageBoxW(NULL, L"Cancelled.", L"Configuration", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_OK);
                    } else {
                        olog("Switching data locations...");
                        DATA_LOC = dataloc + "/";
                        ACC_LOC = DATA_LOC + "accounts/";
                        LOG_LOC = DATA_LOC + "log.txt";
                        if (!dirExists(DATA_LOC)) CreateDirectoryW(s2ws(DATA_LOC), NULL);
                        if (!dirExists(ACC_LOC)) CreateDirectoryW(s2ws(ACC_LOC), NULL);
                        SetFileAttributesW(s2ws(DATA_LOC), FILE_ATTRIBUTE_HIDDEN);
                        olog("New data location set.");
                        MessageBoxW(NULL, L"New Data Location saved successfully.", L"Configuration", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_OK);
                    }
                    break;
                }
            case 'A':
                {
                    clearscr();
                    setTextColor(COLOR_PURPLE);
                    ollc(7, "Past Location: "); cout << ACC_LOC;
                    string accloc;
                    ollc(7, " New Location: "); cout << DATA_LOC; getline(cin, accloc);
                    if (MessageBoxW(NULL, L"Change accounts location?", L"Configuration", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDNO) {
                        MessageBoxW(NULL, L"Cancelled.", L"Configuration", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_OK);
                    } else {
                        olog("Switching accounts location.");
                        ACC_LOC = DATA_LOC + accloc + "/";
                        if (!dirExists(ACC_LOC)) CreateDirectoryW(s2ws(ACC_LOC), NULL);
                        olog("Accounts location switched.");
                        MessageBoxW(NULL, L"New Data Location saved successfully.", L"Configuration", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_OK);
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
                        setTextColor(COLOR_PURPLE);
                        ollc(7, "Past Location: "); cout << LOG_LOC;
                        string logloc;
                        ollc(7, " New Location: "); cout << DATA_LOC; getline(cin, logloc);
                        if (MessageBoxW(NULL, L"Change log location?", L"Configuration", MB_ICONWARNING | MB_SYSTEMMODAL | MB_YESNO) == IDNO) {
                            olog("Cancelled log location switch.");
                            MessageBoxW(NULL, L"Cancelled.", L"Configuration", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_OK);
                        } else {
                            olog("Switching logging location.");
                            LOG_LOC = DATA_LOC + logloc;
                            olog("New logging location established.");
                            MessageBoxW(NULL, L"New Data Location saved successfully.", L"Configuration", MB_SYSTEMMODAL | MB_ICONINFORMATION | MB_OK);
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

main() {
    clearscr();
    ifstream config("atm.cfg");
    if (config.good()) {
        ifstream config("atm.cfg");
        getline(config, DATA_LOC);
        getline(config, ACC_LOC);
        getline(config, LOG_LOC);
        config >> LOG;
        if (!dirExists(DATA_LOC)) CreateDirectoryW(s2ws(DATA_LOC), NULL);
        if (!dirExists(ACC_LOC)) CreateDirectoryW(s2ws(ACC_LOC), NULL);
        SetFileAttributesW(s2ws(DATA_LOC), FILE_ATTRIBUTE_HIDDEN);
        SetFileAttributesW(L"atm.cfg", FILE_ATTRIBUTE_HIDDEN);
    } else {
        DATA_LOC = "data/";
        ACC_LOC = DATA_LOC + "accounts/";
        LOG_LOC = DATA_LOC + "log.txt";
        LOG = true;
        if (!dirExists(DATA_LOC)) CreateDirectoryW(s2ws(DATA_LOC), NULL);
        if (!dirExists(ACC_LOC)) CreateDirectoryW(s2ws(ACC_LOC), NULL);
        ofstream config("atm.cfg");
        config << DATA_LOC << endl;
        config << ACC_LOC << endl;
        config << LOG_LOC << endl;
        config << LOG << endl;\
        config.close();
        olog("Config initialized.");
        SetFileAttributesW(s2ws(DATA_LOC), FILE_ATTRIBUTE_HIDDEN);
        SetFileAttributesW(L"atm.cfg", FILE_ATTRIBUTE_HIDDEN);
    }
    system("title ATM");
    do {	
        olog("Main Menu.");
        clearscr();
        ol(5, "[ === Account Management === ]");
        showMenuOptions(9, 5, 6, "Sign Up  ", "Login");
        ol(8, "[ === Information === ]");
        showMenuOptions(9, 7, 9, "About    ", "Credits");
        ol(11, "[ === Program === ]");
        showMenuOptions(9, 4, 12, "Options   ", "Exit");
        
        if (MAINMENUCODE != 0) {
            switch(MAINMENUCODE) {
                case 1: {
                    setTextColor(COLOR_LIME);
                    ol(15, "Successfully logged out.");
                    setTextColor(COLOR_DEFAULT);
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
                    if (MessageBoxW(NULL, L"Exit?", L"Exit", MB_SYSTEMMODAL | MB_ICONQUESTION | MB_YESNO) == IDYES) {
                        EXIT = true;
                        olog("Exiting...");
                        clearscr();
                        setTextColor(COLOR_RED);
                        system("title ATM Simulation Program");
                        system("cls");
                        ol(5, "Program terminated.");
                        ol(7, "Press any key to close the window.");
                        
                        setTextColor(COLOR_YELLOW);
                        ol(9, "ATM Simulation Program");
                        setTextColor(COLOR_TEAL);
                        ol(10, "Philippine Science High School - Cagayan Valley Campus");
                        ol(11, "Masoc, Bayombong, Nueva Vizcaya");
                        setTextColor(COLOR_LIME);
                        ol(15, "Apache License 2.0");
                        setTextColor(COLOR_YELLOW);
                        ol(17, "by Chlod Aidan Alejandro, Batch 2022");
                        setTextColor(COLOR_DEFAULT);
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
