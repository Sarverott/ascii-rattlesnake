/*
rattlesnake console interface
Sett Sarverott @ 2022
*/
//windows release

#define PROJECTNAME "RATTLESNAKE INTERFACE"
#define VERSION "1.0.0"

#include <iostream>
#include <string>
#include <ctime>
#include <fstream>
#include <cstdarg>
#include <windows.h>
#include <cstdlib>
#include <sstream>
#include <conio.h>

#define CONFIGPATH "config.ini"

using namespace std;

bool charStringCompare(char* first, char* second){
    bool isEqual=true;
    while(*first!='\0'&&*second!='\0'&&isEqual)isEqual=*(first++)==*(second++);
    return isEqual&&*first=='\0'&&*second=='\0';
}

char* cpCharArray(char* orginal){
    int j=0;
    while(*(orginal+j)!='\0')j++;
    char* out=new char[j+3];
    for(int i=0;i<j;i++)out[i]=orginal[i];
    out[j]='\0';
    return out;
}

namespace setup{
    char* authorData="Sett Sarverott @ 2022";
    char* projectTitle=PROJECTNAME;
    char* configFilePath=CONFIGPATH;
    char* projectVersion=VERSION;
}
namespace pkInterface{
    enum optionType{
        exit=0,
        submenu=1,
        action=2,
        textdata=3
    };
    HANDLE consoleHook=GetStdHandle(STD_OUTPUT_HANDLE);
    void windowTitle(){
        SetConsoleTitleA(PROJECTNAME);
    }
    void resetColors(){
        SetConsoleTextAttribute(consoleHook, 15);
    }
    void clearConsole(int width=0, int height=0){
        for(int i=0;i<height;i++){
            if(i)cout<<"\n";
            for(int j=0;j<width;j++){
                cout<<" ";
            }
        }
    }
    void resetCursor(){
        resetColors();
        COORD pointZero;
        pointZero.X=0;
        pointZero.Y=0;
        SetConsoleCursorPosition(consoleHook, pointZero);
    }
    //int currentSelect=0;
    //int optionCount=0;
    bool loopingFlag=true;
    void printLogo(){
        SetConsoleTextAttribute(consoleHook, 8);
        cout<<setup::authorData<<"       ";
        SetConsoleTextAttribute(consoleHook, 28);
        cout
        <<(char)176
        <<(char)177
        <<(char)178;
        SetConsoleTextAttribute(consoleHook, 12);
        cout<<' '<<(char)175<<' ';
        SetConsoleTextAttribute(consoleHook, 9);
        cout<<setup::projectTitle<<" "<<setup::projectVersion;
        SetConsoleTextAttribute(consoleHook, 12);
        cout<<' '<<(char)174<<' ';
        SetConsoleTextAttribute(consoleHook, 28);
        cout
        <<(char)178
        <<(char)177
        <<(char)176
        <<"\n";
        resetColors();
    }
    int decorSlideShift=0;
    void printDecorations(int length){
        SetConsoleTextAttribute(consoleHook, 25);
        for(int i=0;i<length;i++){
            //174-254
            //char j=176+(rand()+i)%78;
            char j=176+(i+decorSlideShift)%78;
            cout<<j;
        }
        decorSlideShift++;
        if(decorSlideShift>78)decorSlideShift=0;
        resetColors();
        cout<<"\n";
    }
    void printDescription(char* text, int rowLimiter=80){
        resetColors();
        while(*text!='\0'){
            for(int i=0;i<rowLimiter&&*text!='\0';i++)
                cout<<*(text++);
            if(*(text-1)!=' '&&*text!=' '&&*text!='\0')cout<<'-';
            cout<<'\n';
        }
        SetConsoleTextAttribute(consoleHook, 4);
        while(rowLimiter--)cout<<(char)16;
        resetColors();
        cout<<'\n';
    }
    void printContent(char* path, int rowLimiter=80){
        resetColors();
        ifstream contentFile(path);
        if(contentFile.is_open()){
            while (!contentFile.eof()){
                char tmp;
                contentFile.get(tmp);
                cout<<tmp;
            }
            contentFile.close();
        }
        cout<<'\n';
        SetConsoleTextAttribute(consoleHook, 4);
        while(rowLimiter--)cout<<(char)16;
        resetColors();
        cout<<'\n';
    }
    void printTitle(char* text){
        cout<<"\n";
        SetConsoleTextAttribute(consoleHook, 25);
        cout<<(char)254<<(char)254<<(char)254;
        SetConsoleTextAttribute(consoleHook, 9);
        cout<<"  "<<text<<"  ";
        SetConsoleTextAttribute(consoleHook, 25);
        cout<<(char)254<<(char)254<<(char)254<<"\n";
        resetColors();
    }
    void printContentsPadding(){
        resetColors();
        cout<<" ";
        SetConsoleTextAttribute(consoleHook, 9);
        cout<<(char)196;
        SetConsoleTextAttribute(consoleHook, 28);
        cout<<(char)185;
        resetColors();
    }
    void printHeader(char* title, int rowLimiter=10){
        printLogo();
        printDecorations(rowLimiter);
        printTitle(title);

        cout<<"\n";
    }
    class Menu;
    struct OptionResult{
        optionType type=exit;
        char* nextmenuPath=NULL;
        Menu* nextmenu=NULL;
        char* command=NULL;
        char* textfile=NULL;
        char* plaintext="Lorem ipsum dolor sit amet";
    };
    class Option{
    public:
        OptionResult setup;

        int index=0;
        char* text="";

        Option* NEXT=NULL;
        Option(int optionCount){
            this->index=optionCount;
        }
        void print(int currentSelect, int paddingLevel=0, bool activeMenu=true){
            printContentsPadding();
            cout<<"  ";
            if(this->index==currentSelect)
                SetConsoleTextAttribute(consoleHook, 12);
            cout<<"~[";
            if(this->index==currentSelect)
                cout
                    <<(char)176
                    <<(char)177
                    <<(char)178;
            else cout<<(char)178;
            cout<<"] ";
            cout<<this->text;
            resetColors();
            cout<<"      \n";
            if(this->NEXT!=NULL)this->NEXT->print(currentSelect);
        }
        void exec(int currentSelect);
    };
    class Menu{
    public:
        Option* firstOption=NULL;
        char* title="<UNTITLED>";
        char* description;
        int currentSelect=0;
        int optionCount=0;
        Menu(){
        }
        Option* addOption(){
            if(!this->firstOption)this->firstOption=new Option(optionCount++);
            else{
                Option* hook=this->firstOption;
                while(hook->NEXT)hook=hook->NEXT;
                hook->NEXT=new Option(optionCount++);
                return hook->NEXT;
            }
            return this->firstOption;
        }
        void print(){
            printHeader(this->title, 70);
            if(this->description)printDescription(this->description, 70);
            printContentsPadding();
            cout<<'\n';
            if(this->firstOption)
                this->firstOption->print(this->currentSelect);
            printContentsPadding();
            cout<<"\n                   \n";
        }
        void moveUp(){
            currentSelect--;
            currentSelect+=optionCount;
            currentSelect%=optionCount;
        }
        void moveDown(){
            currentSelect++;
            currentSelect%=optionCount;
        }
        void select(){
            if(this->firstOption)
                this->firstOption->exec(this->currentSelect);
        }
        void loop(){
            system("cls");
            int command=0;
            loopingFlag=true;
            while(
                  loopingFlag
                  &&
                  command!=27//esc
                  &&
                  command!=120//x
            ){
                command=0;
                resetCursor();
                this->print();
                Sleep(100);
                if(kbhit()){
                    command=getch();
                }
                if(command)switch(command){
                    case 72:/*up*/case 119://w
                        this->moveUp();
                        break;
                    case 80:/*down*/case 115://s
                        this->moveDown();
                        break;
                    case 13:/*enter*/case 32:/*space*/case 101://e
                        this->select();
                        break;
                }
            }
            loopingFlag=true;
        }
    };
}

pkInterface::Menu* createInterface(char* path){
    ifstream configFile(path);
    pkInterface::Menu* currentMenu=new pkInterface::Menu();
    if(configFile.is_open()){
        pkInterface::Option* currentOption=NULL;
        bool labelFlag=true;
        char labelName[255];
        char labeledData[32768];
        int pointFlag=0;
        while (!configFile.eof()){
            char tmp;
            configFile.get(tmp);
            if(labelFlag&&(tmp=='\n'||tmp=='\r')){
                if(pointFlag>0&&labelName[0]=='#'){
                    labelName[pointFlag]='\0';
                    currentOption=currentMenu->addOption();
                    if(charStringCompare((labelName+1), "submenu")){
                        currentOption->setup.type=pkInterface::submenu;
                    }else if(charStringCompare((labelName+1), "text")){
                        currentOption->setup.type=pkInterface::textdata;
                    }else if(charStringCompare((labelName+1), "action")){
                        currentOption->setup.type=pkInterface::action;
                    }else if(charStringCompare((labelName+1), "exit")){
                        currentOption->setup.type=pkInterface::exit;
                    }
                }
                pointFlag=0;
                labeledData[pointFlag]='\0';
                labelName[pointFlag]='\0';
            }else if(tmp=='='&&labelFlag){
                labelFlag=false;
                labelName[pointFlag]='\0';
                pointFlag=0;
            }else if(labelFlag){
                labelName[pointFlag++]=tmp;
            }else if(tmp=='\n'||tmp=='\r'){
                labeledData[pointFlag]='\0';
                pointFlag=0;
                labelFlag=true;
                if(charStringCompare(labelName, "command")){
                    currentOption->setup.command=cpCharArray(labeledData);
                }else if(charStringCompare(labelName, "title")){
                    currentMenu->title=cpCharArray(labeledData);
                }else if(charStringCompare(labelName, "description")){
                    currentMenu->description=cpCharArray(labeledData);
                }else if(charStringCompare(labelName, "submenupath")){
                    currentOption->setup.nextmenuPath=cpCharArray(labeledData);
                }else if(charStringCompare(labelName, "label")){
                    currentOption->text=cpCharArray(labeledData);
                }else if(charStringCompare(labelName, "text")){
                    currentOption->setup.plaintext=cpCharArray(labeledData);
                }else if(charStringCompare(labelName, "textfile")){
                    currentOption->setup.textfile=cpCharArray(labeledData);
                }
                labeledData[pointFlag]='\0';
                labelName[pointFlag]='\0';
            }else{
                labeledData[pointFlag++]=tmp;
            }
        }
        configFile.close();
    }else{
        currentMenu->title=path;
        currentMenu->description="file read error...";
        pkInterface::Option* tmp=currentMenu->addOption();
        tmp->text="EXIT";
    }
    return currentMenu;
}

void pkInterface::Option::exec(int currentSelect){
    if(this->index==currentSelect)
        switch(this->setup.type){
            case exit:
                loopingFlag=false;
                break;
            case textdata:
                resetCursor();
                clearConsole(100,100);
                resetCursor();
                printHeader(this->text,80);
                if(this->setup.textfile)printContent(this->setup.textfile);
                else printDescription(this->setup.plaintext);
                cout<<"\npress any key...\n";
                getch();
                resetCursor();
                clearConsole(100,100);
                resetCursor();
                break;
            case submenu:
                if(this->setup.nextmenu==NULL)
                    this->setup.nextmenu=createInterface(this->setup.nextmenuPath);
                this->setup.nextmenu->loop();
                break;
            case action:
                if(this->setup.command)system(this->setup.command);
                else {
                    cout<<"command not declared";
                    getch();
                }
                break;
        }
    else if(this->NEXT)this->NEXT->exec(currentSelect);
}

int main(int argc, char** argv) {
    if(argc>2){
        setup::configFilePath=argv[2];
    }
    if(argc>5){
        setup::projectTitle=argv[3];
        setup::projectVersion=argv[4];
        setup::authorData=argv[5];

    }
    pkInterface::windowTitle();
    pkInterface::Menu* mainmenu=createInterface(setup::configFilePath);
    mainmenu->loop();
}
