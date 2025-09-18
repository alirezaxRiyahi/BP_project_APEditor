#include <bits/stdc++.h>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfx.h>
#include <string>
#include <SDL2/SDL_mixer.h>
#include <cstdlib>
#include <fstream>
using namespace std;
struct b{bool file=false,edit=false,view=false,compile=false,run=false,save=false;
};
struct point{int x,y;
};
void checklibrary(bool &iostream, bool &cmath ,bool &bits,vector<string>lines){
    regex bit ( "#include[ ]+<bits/stdc\\+\\+.h>");
    regex io ( "#include[ ]+<iostream>");
    regex cm ( "#include[ ]+<cmath>");
    iostream= false;
    cmath= false;
    bits= false;
    for(string i:lines){
        if(regex_search(i,bit)){
            bits= true;
            iostream= true;
            cmath= true;
            break;
        }
        if(regex_search(i,io)&&!iostream){
            iostream= true;
        }
        if(regex_search(i,cm)&&!cmath)
            cmath=true ;
    }
}
void checkfunctions(bool iostream, bool cmath ,bool bits,string &lastword,int &lastwordb){
    if(bits){
        lastwordb=0;
        return;}
    if(!iostream){
        regex io("(std::cout)|(std::cin)|(std::getline)");
        bool ios=regex_search(lastword,io);
        if(ios){
            lastwordb= 1;
        }
    } if(!cmath){
        regex cm("(std[ ]*::[ ]*sqrt)|(std[ ]*::[ ]*pow)|(std[ ]*::[ ]*sin)|(std[ ]*::[ ]*cos)|(std[ ]*::[ ]*tan)|(std[ ]*::[ ]*abs)|(std[ ]*::[ ]*exp)|(std[ ]*::[ ]*log)|(std[ ]*::[ ]*log10)|(std[ ]*::[ ]*floor)|(std[ ]*::[ ]*ceil)");
        bool cma= regex_search(lastword,cm);
        if(cma){
            lastwordb= 2;
        }
    }
}
void finalword(bool iostream, bool cmath ,bool bits,vector<string>lines,map<string,int>&includeerror){
    for (string i:lines) {
        istringstream straem(i);
        string word,lastword;
        while (straem>>word){
            int lastwordb=-1;
            lastword=word;
            checkfunctions(iostream,cmath,bits,lastword,lastwordb);
            if(lastwordb!=-1){
                includeerror[lastword]=lastwordb;}
        }
    }

}
void addimg(SDL_Renderer* renderer,int x, int y,int w,int h,string path){
    //int imgFalgs=IMG_INIT_PNG;
    SDL_Surface *imagsurface=IMG_Load(path.c_str());
    SDL_Texture* texture= SDL_CreateTextureFromSurface(renderer,imagsurface);
    SDL_FreeSurface(imagsurface);
    if(texture){
        SDL_Rect destRect;
        destRect.x=x;
        destRect.y=y;
        destRect.w=w;
        destRect.h=h;
        SDL_RenderCopy(renderer,texture, nullptr,&destRect);
        SDL_DestroyTexture(texture);
    }
}
void textshow(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color, const std::string& temp, int x, int y) {
    SDL_Surface* textsurface = TTF_RenderText_Blended(font, temp.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textsurface);
    SDL_FreeSurface(textsurface);
    int textw, texth;
    SDL_QueryTexture(texture, nullptr, nullptr, &textw, &texth);
    SDL_Rect textr = { x, y, textw, texth };
    SDL_RenderCopy(renderer, texture, nullptr, &textr);
    SDL_DestroyTexture(texture);
}
void ensureLastLineVisible(int currentLine, int &scrollOffset, int SCREEN_HEIGHT, int LINE_HEIGHT, int totalLines);
void renderTextWithColor(SDL_Renderer* renderer, TTF_Font* font, const string& text, SDL_Color defaultColor, int y, int cursorPos,bool mode);
vector<string>Treemenu(string r){
    vector<string>x;
    string t="";
    for (int i = 0; i < r.length(); ++i) {
        if(r[i]!='\n'){
            t+=r[i];
        }
        else{
            x.push_back(t);
            t="";
        }
    }
    x.erase(x.begin(),x.begin()+2);
    x[0]="data:\\Tree_project";
    return x;
}
vector<string>splitString(string r) {
    vector<string> x;
    string t = "";
    for (int i = 0; i < r.length(); ++i) {
        if (r[i] != '\n') {
            t += r[i];
        } else {
            x.push_back(t);
            t = "";
        }
    }
    if(r[r.length()-1]!='\n')//{
        x.push_back(t);//}
    return x;
}
void Tree(SDL_Renderer *renderer,TTF_Font *font,SDL_Color color,map<string,vector<int>>&xx){
    FILE* pipe = popen("cd data/Tree_project && tree /f", "r");
    stringstream st;
    string r="";
    vector<string>x;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        string str(buffer);
        string s = buffer;
        r += s;
    }
    pclose(pipe);
    x = Treemenu(r);
    bool T=false;
    regex p("(\\.)cpp");
    for(string i:x){
        if(regex_search(i,p)){
            T= true;
            break;
        }
    }
    if(T){
        int j = 0;
        for (string i: x) {
            textshow(renderer, font, color, i, 5, 60 + j * 18);
            int len,h;
            TTF_SizeText(font,i.c_str(),&len,&h);
            xx[i]={5,60+j*15,len,h};
            j++;
        }}
    else{
        textshow(renderer, font, color,"No project", 5, 60 + 0 * 18);
    }
}
bool click(int x,int y,int len,int h,point c){
    if((x<=c.x&&c.x<=x+len)&&(y<=c.y&&c.y<=y+h)){
        return true;
    }
    else{
        return false;}
}
vector<string> open_file(string i){
    vector<string>s;
    string r;
    fstream fin("data/Tree_project/"+i,ios::in);
    while (!fin.eof()){
        getline(fin,r);
        s.push_back(r);
    }return s;
}
void save(string temp,vector<string>lines){
    ofstream fout("data/Tree_project/"+temp+".cpp",ios::out);
    for (string i:lines) {
        fout<<i<<endl;
    }
}
void save_as(string temp,vector<string>lines){
    regex duc("Documents");
    regex des("Desktop");
    regex pro("project");
    if(regex_search(temp,duc)){
        if(regex_search(temp,pro)){
            string e="cd C:\\Users\\Asus\\Documents\\project ";
            string n=temp.substr(18);
            int r= system((e+"&& copy nul "+n+".cpp").c_str());
            ofstream fout(e+n+".cpp",ios::out);
            for (string i:lines) {
                fout << i << endl;
            }
        }
        else{
            string e="cd C:\\Users\\Asus\\Documents ";
            string n=temp.substr(10);
            int r= system((e+"&& copy nul "+n+".cpp").c_str());
            ofstream fout(e+n+".cpp",ios::out);
            for (string i:lines) {
                fout << i << endl;
            }
        }
    }
    else if(regex_search(temp,des)){
        if(regex_search(temp,pro)){
            string e="cd C:\\Users\\Asus\\Desktop\\project ";
            string n=temp.substr(16);
            int r= system((e+"&& copy nul "+n+".cpp").c_str());
            ofstream fout(e+n+".cpp",ios::out);
            for (string i:lines) {
                fout << i << endl;
            }
        }
        else{
            string e="cd C:\\Users\\Asus\\Desktop ";
            string n=temp.substr(8);
            int r= system((e+"&& copy nul "+n+".cpp").c_str());
            ofstream fout(e+n+".cpp",ios::out);
            for (string i:lines) {
                fout << i << endl;
            }
        }
    }
    else{
        int r= system(("cd data/Tree_project && copy nul "+temp+".cpp").c_str());
        ofstream fout("data/Tree_project/"+temp+".cpp",ios::out);
        for (string i:lines) {
            fout<<i<<endl;
        }
    }
}
void vtof(vector<string>&lines){
    ofstream fout("data/Tree_project/New folder/text_handling.txt",ios::out);
    for(string i:lines){
        fout<<i<<endl;
    }
}
void copystr(){
    bool f = system(("cd data/Tree_project/New folder && type text_handling.txt | clip"));
}
vector<string> paste(){
    FILE* pipe = popen("powershell -command \"Get-Clipboard\"", "r");
    string r="";
    vector<string>x;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        string str(buffer);
        string s = buffer;
        r += s;
    }
    pclose(pipe);
    string t = "";
    for (int i = 0; i < r.length(); ++i) {
        if (r[i] != '\n') {
            t += r[i];
        } else {
            x.push_back(t);
            t = "";
        }
    }
    if(r[r.length()-1]!='\n')//{
        x.push_back(t);//}
    return x;
}
struct edit{
    string don;
    int line;
    int cursor;
    int type;//0=text,1=bakspace,2=enter
};
struct comment_color {
    int cursor;
    int line;
    int type;
};
vector<string> run(string filename){
    FILE* pipe = popen(("data/Tree_project/" + filename + ".exe").c_str(), "r");
    stringstream st;
    string r="";
    vector<string>x;
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        string str(buffer);
        string s = buffer;
        r += s;
    }
    pclose(pipe);
    x = splitString(r);
    return x;
}
vector<string> compile(string filename){
    std::string compileCommand = "g++ data/Tree_project/" + filename + ".cpp -o data/Tree_project/" + filename + ".exe 2>&1";
    FILE* pipe = popen(compileCommand.c_str(), "r");
    stringstream st;
    string r="";
    vector<string>x;
    char buffer[255];
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        string str(buffer);
        string s = buffer;
        r += s;
    }
    pclose(pipe);
    x = splitString(r);
    return x;
}
int checkcount(vector<string>s,char r,char rr){
    int x=0;
    int xx=0;
    for (string i:s) {
        for (int j = 0; j < i.length(); ++j) {
            if(i[j]==r)
                x++;
            else if(i[j]==rr)
                xx++;
        }
    }
    return x-xx;
}
vector<int> cemicalom(vector<string>s){
    regex p("(\\s*if)|(\\s*else)|(\\s*while)|(\\s*do)|(\\s*switch)|(\\s*default)|(\\s*case)|(^\\s*})|(\\s*class)|(\\s*#)|(\\s*\\w+\\s+\\w+\\s*\\(.*\\)\\s*\\{)|(^\\s*//)");
    regex pp("//");
    regex ppp(";\\s*//");
    regex f("\\bfor\\b");
    regex ss("\\bfor\\s*\\(.+;.+;.+\\)");
    regex sss("^\\s*[^//]+;\\s*}*\\s*(//.*)?$");
    regex ff("^\\s*$");
    regex str("(\"[^\"]*\")");
    int x=0;
    vector<int>error;
    for (string ii:s) {
        bool T= regex_search(ii,str);
        string i= regex_replace(ii,str," ");
        ++x;
        if(regex_search(i,p)|| (regex_match(i,ff)&&!T)){
            continue;
        }//Without semicolon(key word+function+line comment)
        else if(regex_search(i,pp)){
            if(regex_search(i,ppp)){
                continue;
            }
            error.push_back(x);
        }//Comment(code+comment)
        else if(regex_search(i,f)){
            if(regex_search(i,ss)){
                continue;
            }
            error.push_back(x);
        }//for
        else if(regex_search(i,sss)){
            continue;
        }//rows
        else{
            error.push_back(x);
        }//another pattern that they have error
    }
    return error;
}
double jaro(string s1,string s2){
    regex p("(\\=.*)|(\\{.*)|(\\(.*)|(<<.*)");
    //regex p("(\\=)|(\\{)|(\\()|(<<)");
    s1= regex_replace(s1,p,"");
    int len1=s1.length();
    int len2=s2.length();
    if(len1==0||len2==0||len1!=len2)
        return 0;
    vector<bool>m1(len1,false);
    vector<bool>m2(len2,false);
    double m=0,t=0;
    for (int i = 0; i < len1; ++i) {
        for (int j = 0; j < len2; ++j) {
            if(m2[j])
                continue;
            if(s1[i]!=s2[j])
                continue;
            m1[i]=true;
            m2[j]=true;
            m++;
            break;
        }
    }
    if(m==0)
        return 0;
    int k=0;
    for (int i = 0; i < len1; ++i) {
        if(!m1[i])
            continue;
        while(!m2[k])
            k++;
        if (s1[i] != s2[k])
            t++;
        k++;
    }
    t=floor(t/2);
    double jaroo=1.0/3.0*(m/len1+m/len2+(m- t)/m);
    return jaroo;
}
string suggestion(string s){
    //vector<string>keywords = {"int", "float", "double", "char", "bool","void","if", "else", "for", "while", "switch", "case","break", "continue", "return","class", "struct", "static", "const", "new", "delete","namespace", "using", "typedef","default","main"};
    vector<string>keywords = {"if","while","else","return","break","main"};
    double m=0;
    string d;
    for (string i:keywords ) {
        if(jaro(s,i)>m){
            m=jaro(s,i);
            d=i;
        }
    }
    if(m>0.75&&m<1.00)
        return d;
    return "";
}
map<string,int> key_error(vector<string>s){
    map<string,int>x;
    int j=0;
    regex p("(;.*)|(\\(.*)|(<<.*)|(\\{.*)");
    regex str("(\".*\")");
    for(string ii:s){
        j++;
        string i= regex_replace(ii,str," ");
        istringstream straem(i);
        string word,sugg;
        while (straem>>word) {
            word= regex_replace(word,p,"");
            sugg= suggestion(word);
            if(sugg!=""){
                x[word+" : "+sugg]=j;
            }
        }
    }
    return x;
}
map<string,int> val_errorf(vector<string>s){
    map<string,int>x;
    regex str("(\".*\")");
    regex p("(int|string|bool|float|double|char)(\\s+)(\\w+)(\\s*)(;|=)");
    vector<string>keywords = {"int", "float", "double", "char", "bool","void","if", "else", "for", "while", "switch", "case","break", "continue", "return","class", "struct", "static", "const", "new", "delete","namespace", "using", "typedef","default","main"};
    smatch mch;
    int j=0;
    for (string iii:s) {
        j++;
        string i= regex_replace(iii,str," ");
        if(regex_search(i,mch,p)){
            string s=mch[3].str();
            for(string ii:keywords){
                if(s==ii){
                    x[s]=j;
                    break;
                }
            }
        }
    }
    return x;
}
vector<int> op_error( vector<string>& s) {
    vector<int> x;
    // رشته‌ها
    regex str("\"[^\"]*\"");
    // کامنت‌ها
    regex com("//.*");
    // اپراتورهای غیرمجاز (به صورت دقیق و escape‌شده)
    regex op(R"((\+\+\+|--\+|==\+|!=\+|<=\+|>=\+|\+=\+|-=\+|\*=/|/= \*|%= \*|&&&|\|\|\||\^\^\^|<<=<<|>>=>>|\?\?|\*\*\*|///|%%%|<<<|>>>))");

    int j = 0;
    for (string iii : s) {
        j++;
        // رشته و کامنت رو حذف کن
        iii = regex_replace(iii, str, " ");
        iii = regex_replace(iii, com, " ");
        // اگر اپراتور نامعتبر پیدا شد
        if (regex_search(iii, op)) {
            x.push_back(j);
        }
    }
    return x;
}
vector<int> str_error(vector<string>s){
    regex ss("\"");
    regex sss("\".*\"");
    regex com("//.*");
    vector<int>x;
    int j=0;
    for (string i:s) {
        j++;
        i= regex_replace(i,com," ");
        if(regex_search(i,ss)){
            if(regex_search(i,sss))
                continue;
            else{
                x.push_back(j);
            }
        }
    }
    return x;
}
// Screen dimensions
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 816;


int main(int argc, char* argv[]) {
    //vector<vector<string>>projects;
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return -1;
    }

    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
        return -1;
    }
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "TTF could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow("AP EDITOR",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH,
                                          SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }
    SDL_Surface* icon = IMG_Load("assets/photo16646248983.jpg");
    SDL_SetWindowIcon(window, icon);


    Mix_Music* music = Mix_LoadMUS("assets/New Recording.mp3");
    Mix_PlayMusic(music, 1);
    addimg(renderer,0,0,1000,816,"assets/photo16646248983.jpg");
    SDL_RenderPresent(renderer);
    SDL_Delay(2000);
    Mix_HaltMusic();
    // Load font
    TTF_Font* font = TTF_OpenFont("assets/consola.ttf", 24); // Replace with the path to your .ttf font
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return -1;
    }

    std::vector<std::string> lines = {""}; // Holds multiple lines of text
    int currentLine = 0; // Track the current line being edited
    int cursorPos = 0; // Track the cursor position within the current line
    const int LINE_HEIGHT = TTF_FontHeight(font); // Height of each line
    int i1 = -1;
    int scrollOffset = i1 * LINE_HEIGHT; // Keeps track of scrolling
    int y;

    // Timer for cursor blinking
    Uint32 lastCursorToggle = SDL_GetTicks();
    bool cursorVisible = true;
    const Uint32 CURSOR_BLINK_INTERVAL = 500; // 500 ms for blinking

    SDL_Event e;
    b bol;
    map<int,edit> edits;
    map<int,edit> edits2;
    map<int,comment_color> com;
    string name="";
    map<string,vector<int>>xx;
    map<string ,int>includeerror;
    map<string ,int>keyworderror;
    vector<int>operror;
    map<string,int>val_error;
    int iii=0,k=0,a=0,cc=0,com1=0,com2=0;
    bool bp=false;
    bool quit = false;
    bool mode= true;
    bool redo=false;
    bool undo=false;
    bool new_p=false;
    bool save_p=false;
    bool exit=false;
    bool save_ass=false;
    bool gotoline=false;
    bool select_all=false;
    bool select_part=false;
    bool T=true;
    int paran,acco,bera;
    vector<int> cemi;
    point start;
    point finish;
    vector<string>run_p;
    vector<string>compile_p;
    vector<int>strerror;
    TTF_Font* font1 = TTF_OpenFont("assets/consola.ttf", 12);
    TTF_Font *font3=TTF_OpenFont("assets/consola.ttf", 13);
    TTF_Font *font4=TTF_OpenFont("assets/consola.ttf", 10);
    TTF_Font *font5=TTF_OpenFont("assets/consola.ttf", 15);
    SDL_Color text_l = {0, 0, 0, 255};
    SDL_Color text_d={255, 255, 255, 255};
    SDL_Color color3;

    while (!quit) {
        if(mode) {
            color3= {30, 30, 30, 255};
        }
        else {
            color3= {255, 255, 255, 255};
        }
        // Handle cursor blinking
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastCursorToggle + CURSOR_BLINK_INTERVAL) {
            cursorVisible = !cursorVisible;
            lastCursorToggle = currentTime;
        }

        // Event loop
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
                break;
            }
            else if (e.type == SDL_MOUSEWHEEL) {
                // Handle scroll
                if (e.wheel.y > 0) { // Scroll up
                    scrollOffset = std::max(i1*LINE_HEIGHT, scrollOffset - LINE_HEIGHT);
                } else if (e.wheel.y < 0) { // Scroll down
                    scrollOffset += LINE_HEIGHT;
                }
            }
            else if (e.type == SDL_KEYDOWN) {
                SDL_Keymod mod = SDL_GetModState();
                if (e.key.keysym.sym == SDLK_BACKSPACE) {
                    // Ensure cursorPos is within the valid range
                    if (cursorPos > 0 && cursorPos <= lines[currentLine].size()) {
                        iii=k;
                        if(k<10){k++;}
                        if(iii <= 10){iii++;}
                        if(iii <= 10) {
                            edits[iii].cursor=cursorPos;
                            edits[iii].line=currentLine;
                            edits[iii].don=lines[currentLine][cursorPos - 1];
                            edits[iii].type=1;
                        }
                        if(iii == 11) {
                            for(int j=1;j<10;j++) {
                                edits[j].cursor=edits[j+1].cursor;
                                edits[j].line=edits[j+1].line;
                                edits[j].don=edits[j+1].don;
                                edits[j].type=edits[j+1].type;
                            }
                            edits[10].cursor=cursorPos;
                            edits[10].line=currentLine;
                            edits[10].don=lines[currentLine][cursorPos-1];
                            edits[10].type=1;
                        }
                        if(lines[currentLine][cursorPos - 1]=='('&&lines[currentLine][cursorPos - 0]==')'){
                            lines[currentLine].erase(cursorPos - 1, 2);
                            cursorPos--;}
                        if(lines[currentLine][cursorPos - 1]=='{'&&lines[currentLine][cursorPos - 0]=='}'){
                            lines[currentLine].erase(cursorPos - 1, 2);
                            cursorPos--;}
                        if(lines[currentLine][cursorPos - 1]=='['&&lines[currentLine][cursorPos - 0]==']'){
                            lines[currentLine].erase(cursorPos - 1, 2);
                            cursorPos--;}
                        else{
                            lines[currentLine].erase(cursorPos - 1, 1);
                            cursorPos--;
                        }
                    } else if (currentLine > 0) {
                        // Merge with previous line
                        cursorPos = lines[currentLine - 1].size();
                        lines[currentLine - 1] += lines[currentLine].substr(0);
                        lines.erase(lines.begin() + currentLine);
                        currentLine--;
                    }

                    // Ensure there's always at least one line
                    if (lines.empty()) {
                        lines.push_back("");
                        currentLine = 0;
                        cursorPos = 0;
                    }
                    bool bits,cmath,iostream;
                    includeerror.clear();
                    checklibrary(iostream,cmath,bits,lines);
                    finalword(iostream,cmath,bits,lines,includeerror);
                }
                else if (e.key.keysym.sym == SDLK_RETURN) {
                    if (cursorPos <= lines[currentLine].size()) {
                        std::string remainder = lines[currentLine].substr(cursorPos);
                        iii=k;
                        if(k<10){k++;}
                        if(iii <= 10){iii++;}
                        if(iii <= 10) {
                            edits[iii].cursor=cursorPos;
                            edits[iii].line=currentLine;
                            edits[iii].don=remainder;
                            edits[iii].type=2;
                        }
                        if(iii == 11) {
                            for(int j=1;j<10;j++) {
                                edits[j].cursor=edits[j+1].cursor;
                                edits[j].line=edits[j+1].line;
                                edits[j].don=edits[j+1].don;
                                edits[j].type=edits[j+1].type;
                            }
                            edits[10].cursor=cursorPos;
                            edits[10].line=currentLine;
                            edits[10].don=remainder;
                            edits[10].type=2;
                        }
                        lines[currentLine] = lines[currentLine].substr(0, cursorPos);
                        lines.insert(lines.begin() + currentLine + 1, remainder);
                        currentLine++;
                        cursorPos = 0;
                        ensureLastLineVisible(currentLine, scrollOffset, SCREEN_HEIGHT, LINE_HEIGHT, lines.size());
                    }
                    bool bits,cmath,iostream;
                    includeerror.clear();
                    checklibrary(iostream,cmath,bits,lines);
                    finalword(iostream,cmath,bits,lines,includeerror);
                }
                else if (e.key.keysym.sym == SDLK_TAB) {
                    // Add spaces for tab

                    lines[currentLine].insert(cursorPos, "    ");
                    cursorPos += 4;
                    bool bits,cmath,iostream;
                    includeerror.clear();
                    checklibrary(iostream,cmath,bits,lines);
                    finalword(iostream,cmath,bits,lines,includeerror);
                }
                else if (e.key.keysym.sym == SDLK_LEFT) {
                    select_all= false;
                    // Move cursor left
                    if (cursorPos > 0) {
                        cursorPos--;
                    } else if (currentLine > 0) {
                        currentLine--;
                        cursorPos = lines[currentLine].size();
                    }
                }
                else if (e.key.keysym.sym == SDLK_RIGHT) {
                    select_all= false;
                    // Move cursor right
                    if (cursorPos < lines[currentLine].size()) {
                        cursorPos++;
                    } else if (currentLine < lines.size() - 1) {
                        currentLine++;
                        cursorPos = 0;
                    }
                }
                else if (e.key.keysym.sym == SDLK_UP) {
                    select_all= false;
                    // Move cursor up
                    if (currentLine > 0) {
                        currentLine--;
                        cursorPos = std::min(cursorPos, (int)lines[currentLine].size());
                        ensureLastLineVisible(currentLine, scrollOffset, SCREEN_HEIGHT, LINE_HEIGHT, lines.size());
                    }
                }
                else if (e.key.keysym.sym == SDLK_DOWN) {
                    select_all=false;
                    if (currentLine < lines.size() - 1) {
                        currentLine++;
                        cursorPos = std::min(cursorPos, (int)lines[currentLine].size());
                        ensureLastLineVisible(currentLine, scrollOffset, SCREEN_HEIGHT, LINE_HEIGHT, lines.size());
                    }
                }
                else if (e.key.keysym.sym == SDLK_RALT||e.key.keysym.sym == SDLK_LALT) {
                    if((lines[currentLine][cursorPos-1]=='r')&&(lines[currentLine][cursorPos-2]=='o')&&(lines[currentLine][cursorPos-3]=='f')&&((lines[currentLine][cursorPos-4]==' ')||cursorPos==3)){lines[currentLine].insert(cursorPos, "(int i=0;i< ;i++){}");cursorPos+=11;}
                    if((lines[currentLine][cursorPos-1]=='f')&&(lines[currentLine][cursorPos-2]=='i')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "(){}");cursorPos+=1;}
                    if((lines[currentLine][cursorPos]==')')&&(lines[currentLine][cursorPos-1]=='(')&&(lines[currentLine][cursorPos-2]=='e')&&(lines[currentLine][cursorPos-3]=='l')&&(lines[currentLine][cursorPos-4]=='i')&&(lines[currentLine][cursorPos-5]=='h')&&(lines[currentLine][cursorPos-6]=='w')){lines[currentLine].insert(cursorPos+1, "{}");cursorPos+=2;}
                    if((lines[currentLine][cursorPos-1]=='e')&&(lines[currentLine][cursorPos-2]=='l')&&(lines[currentLine][cursorPos-3]=='i')&&(lines[currentLine][cursorPos-4]=='h')&&(lines[currentLine][cursorPos-5]=='w')){lines[currentLine].insert(cursorPos, "()");cursorPos+=1;}
                    if((lines[currentLine][cursorPos-1]=='n')&&(lines[currentLine][cursorPos-2]=='i')&&(lines[currentLine][cursorPos-3]=='a')&&(lines[currentLine][cursorPos-4]=='m')&&(lines[currentLine][cursorPos-5]==' ')&&(lines[currentLine][cursorPos-6]=='t')&&(lines[currentLine][cursorPos-7]=='n')&&(lines[currentLine][cursorPos-8]=='i')){lines[currentLine].insert(cursorPos, "(){}");cursorPos+=3;}
                    if((lines[currentLine][cursorPos-1]=='o')&&(lines[currentLine][cursorPos-2]=='d')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "()");cursorPos+=1;}
                    if((lines[currentLine][cursorPos-1]=='e')&&(lines[currentLine][cursorPos-2]=='s')&&(lines[currentLine][cursorPos-3]=='l')&&(lines[currentLine][cursorPos-4]=='e')&&((lines[currentLine][cursorPos-5]==' ')||cursorPos==4)){lines[currentLine].insert(cursorPos, "{}");cursorPos+=1;}
                    if((lines[currentLine][cursorPos-1]=='t')&&(lines[currentLine][cursorPos-2]=='r')&&(lines[currentLine][cursorPos-3]=='q')&&(lines[currentLine][cursorPos-4]=='s')&&((lines[currentLine][cursorPos-5]==' ')||cursorPos==4)){lines[currentLine].insert(cursorPos, "()");cursorPos+=1;}
                    if((lines[currentLine][cursorPos-1]=='s')&&(lines[currentLine][cursorPos-2]=='b')&&(lines[currentLine][cursorPos-3]=='a')&&((lines[currentLine][cursorPos-4]==' ')||cursorPos==3)){lines[currentLine].insert(cursorPos, "()");cursorPos+=1;}
                    if((lines[currentLine][cursorPos-1]=='n')&&(lines[currentLine][cursorPos-2]=='e')&&(lines[currentLine][cursorPos-3]=='l')&&(lines[currentLine][cursorPos-4]=='r')&&(lines[currentLine][cursorPos-5]=='t')&&(lines[currentLine][cursorPos-6]=='s')&&((lines[currentLine][cursorPos-7]==' ')||cursorPos==6)){lines[currentLine].insert(cursorPos, "()");cursorPos+=1;}
                    if((lines[currentLine][cursorPos-1]=='f')&&(lines[currentLine][cursorPos-2]=='t')&&(lines[currentLine][cursorPos-3]=='n')&&(lines[currentLine][cursorPos-4]=='i')&&(lines[currentLine][cursorPos-5]=='r')&&(lines[currentLine][cursorPos-6]=='p')&&((lines[currentLine][cursorPos-7]==' ')||cursorPos==6)){lines[currentLine].insert(cursorPos, "()");cursorPos+=1;}
                    if((lines[currentLine][cursorPos-1]=='f')&&(lines[currentLine][cursorPos-2]=='n')&&(lines[currentLine][cursorPos-3]=='a')&&(lines[currentLine][cursorPos-4]=='c')&&(lines[currentLine][cursorPos-5]=='s')&&((lines[currentLine][cursorPos-6]==' ')||cursorPos==5)){lines[currentLine].insert(cursorPos, "()");cursorPos+=1;}
                    if((lines[currentLine][cursorPos-1]=='h')&&(lines[currentLine][cursorPos-2]=='c')&&(lines[currentLine][cursorPos-3]=='t')&&(lines[currentLine][cursorPos-4]=='i')&&(lines[currentLine][cursorPos-5]=='w')&&(lines[currentLine][cursorPos-6]=='s')&&((lines[currentLine][cursorPos-7]==' ')||cursorPos==6)){lines[currentLine].insert(cursorPos, "(){}");cursorPos+=3;}
                    if((lines[currentLine][cursorPos-1]=='s')&&(lines[currentLine][cursorPos-2]=='s')&&(lines[currentLine][cursorPos-3]=='a')&&(lines[currentLine][cursorPos-4]=='l')&&(lines[currentLine][cursorPos-5]=='c')&&((lines[currentLine][cursorPos-6]==' ')||cursorPos==5)){lines[currentLine].insert(cursorPos, "{}");cursorPos+=1;}
                    if((lines[currentLine][cursorPos-1]=='e')&&(lines[currentLine][cursorPos-2]=='d')&&(lines[currentLine][cursorPos-3]=='u')&&(lines[currentLine][cursorPos-4]=='l')&&(lines[currentLine][cursorPos-5]=='c')&&(lines[currentLine][cursorPos-6]=='n')&&(lines[currentLine][cursorPos-7]=='i')&&(lines[currentLine][cursorPos-8]=='#')&&((lines[currentLine][cursorPos-9]==' ')||cursorPos==8)){lines[currentLine].insert(cursorPos, " <>");cursorPos+=2;}
                    if((lines[currentLine][cursorPos-1]=='l')&&(lines[currentLine][cursorPos-2]=='e')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "se");cursorPos+=2;}
                    if((lines[currentLine][cursorPos-1]=='o')&&(lines[currentLine][cursorPos-2]=='f')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "r");cursorPos+=1;}
                    if((lines[currentLine][cursorPos-1]=='n')&&(lines[currentLine][cursorPos-2]=='i')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "t");cursorPos+=1;}
                    if((lines[currentLine][cursorPos-1]=='i')&&((lines[currentLine][cursorPos-2]==' ')||cursorPos==1)){lines[currentLine].insert(cursorPos, "f");cursorPos+=1;}
                    if((lines[currentLine][cursorPos-1]=='n')&&(lines[currentLine][cursorPos-2]=='i')&&(lines[currentLine][cursorPos-3]=='#')&&((lines[currentLine][cursorPos-4]==' ')||cursorPos==3)){lines[currentLine].insert(cursorPos, "clude");cursorPos+=5;}
                    if((lines[currentLine][cursorPos-1]=='h')&&(lines[currentLine][cursorPos-2]=='w')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "ile");cursorPos+=3;}
                    if((lines[currentLine][cursorPos-1]=='w')&&(lines[currentLine][cursorPos-2]=='s')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "itch");cursorPos+=4;}
                    if((lines[currentLine][cursorPos-1]=='l')&&(lines[currentLine][cursorPos-2]=='c')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "ass");cursorPos+=3;}
                    if((lines[currentLine][cursorPos-1]=='a')&&(lines[currentLine][cursorPos-2]=='c')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "se");cursorPos+=2;}
                    if((lines[currentLine][cursorPos-1]=='r')&&(lines[currentLine][cursorPos-2]=='b')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "eak");cursorPos+=3;}
                    if((lines[currentLine][cursorPos-1]=='o')&&(lines[currentLine][cursorPos-2]=='c')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "ntinue");cursorPos+=6;}
                    if((lines[currentLine][cursorPos-1]=='e')&&(lines[currentLine][cursorPos-2]=='r')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "turn");cursorPos+=4;}
                    if((lines[currentLine][cursorPos-1]=='e')&&(lines[currentLine][cursorPos-2]=='d')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "fault:");cursorPos+=5;}
                    if((lines[currentLine][cursorPos-1]=='a')&&(lines[currentLine][cursorPos-2]=='m')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "in");cursorPos+=2;}
                    if((lines[currentLine][cursorPos-1]=='l')&&(lines[currentLine][cursorPos-2]=='f')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "oat");cursorPos+=3;}
                    if((lines[currentLine][cursorPos-1]=='o')&&(lines[currentLine][cursorPos-2]=='d')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "uble");cursorPos+=4;}
                    if((lines[currentLine][cursorPos-1]=='o')&&(lines[currentLine][cursorPos-2]=='l')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "ng");cursorPos+=2;}
                    if((lines[currentLine][cursorPos-1]=='c')&&(lines[currentLine][cursorPos-2]=='s')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "anf");cursorPos+=3;}
                    if((lines[currentLine][cursorPos-1]=='r')&&(lines[currentLine][cursorPos-2]=='p')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "intf");cursorPos+=4;}
                    if((lines[currentLine][cursorPos-1]=='q')&&(lines[currentLine][cursorPos-2]=='s')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "rt");cursorPos+=2;}
                    if((lines[currentLine][cursorPos-1]=='b')&&(lines[currentLine][cursorPos-2]=='a')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "s");cursorPos+=1;}
                    if((lines[currentLine][cursorPos-1]=='t')&&(lines[currentLine][cursorPos-2]=='s')&&((lines[currentLine][cursorPos-3]==' ')||cursorPos==2)){lines[currentLine].insert(cursorPos, "rlen");cursorPos+=4;}
                }
                else if(e.key.keysym.sym==SDLK_d && (mod & KMOD_CTRL)){
                    save_ass= true;
                }
                else if(e.key.keysym.sym==SDLK_s && (mod & KMOD_CTRL)){
                    save_p= true;
                }
                else if(e.key.keysym.sym==SDLK_g && (mod & KMOD_CTRL)){
                    gotoline= true;
                }
                else if(e.key.keysym.sym==SDLK_a && (mod & KMOD_CTRL)){
                    select_all= true;
                    select_part= false;
                    vtof(lines);
                }
                else if(e.key.keysym.sym==SDLK_c && (mod & KMOD_CTRL)){
                    if(select_all||select_part)
                        copystr();
                }
                else if(e.key.keysym.sym == SDLK_LSHIFT){
                    select_part= true;
                    select_all=false;
                    //select_all=false;
                    start.x=cursorPos;
                    start.y=currentLine;}
                else if(e.key.keysym.sym==SDLK_x && (mod & KMOD_CTRL)){
                    if(select_part){
                        if (start.y > finish.y) {
                            swap(start, finish);
                        }
                        else if(start.x > finish.x) {
                            swap(start, finish);
                        }
                        if(start.y < finish.y){
                            string r=lines[finish.y].substr(finish.x);
                            lines[start.y].erase(start.x);
                            lines[start.y].insert(lines[start.y].length(),r);
                            lines.erase(lines.begin()+start.y+1,lines.begin()+finish.y+1);
                            cursorPos= lines[start.y].length();
                            currentLine=start.y;
                        }
                        else if(start.y == finish.y){
                            lines[start.y].erase(start.x,(finish.x-start.x));
                            cursorPos=start.x;
                        }}
                    if(select_all){
                        currentLine = 0;
                        lines={""};
                        cursorPos=0;
                        scrollOffset=i1*LINE_HEIGHT;
                    }
                    if(select_all||select_part)
                        copystr();
                }
                else if(e.key.keysym.sym==SDLK_v && (mod & KMOD_CTRL)){
                    vector<string>Tpaste;
                    Tpaste=paste();
                    int n=Tpaste.size();
                    string m=lines[currentLine].substr(cursorPos);
                    lines[currentLine].erase(cursorPos);
                    lines[currentLine].insert(cursorPos,Tpaste[0]);
                    lines.insert(lines.begin()+currentLine+1,Tpaste.begin()+1,Tpaste.end());
                    cursorPos=lines[currentLine+n-1].length();
                    lines[currentLine+n-1].insert(lines[currentLine+n-1].length(),m);
                    currentLine=currentLine+n-1;

                }
                else if(e.key.keysym.sym==SDLK_z && (mod & KMOD_CTRL)){
                    undo= true;
                }
            }
            else if(e.type== SDL_KEYUP){
                if(e.key.keysym.sym == SDLK_LSHIFT) {
                    finish.x = cursorPos;
                    finish.y = currentLine;
                    if (start.y > finish.y) {
                        swap(start, finish);
                    }
                    else if(start.x > finish.x) {
                        swap(start, finish);
                    }
                    vector<string>sel;
                    if(start.y < finish.y){
                        sel.push_back(lines[start.y].substr(start.x));
                        for (int i = start.y+1; i <finish.y ; ++i) {
                            sel.push_back(lines[i]);
                        }
                        sel.push_back(lines[finish.y].substr(0,finish.x));
                    }
                    else if(start.y == finish.y){
                        sel.push_back(lines[start.y].substr(start.x,(finish.x-start.x)));
                    }

                    vtof(sel);
                }
            }
            else if (e.type == SDL_TEXTINPUT) {
                if (e.text.text) {
                    iii=k;
                    if(k<10){k++;}
                    if(iii <= 10){iii++;}
                    select_all= false;
                    select_part= false;
                    lines[currentLine].insert(cursorPos, e.text.text);
                    if(lines[currentLine].size() > 61) {
                        if(currentLine+1 >= lines.size())
                            lines.push_back("");
                        currentLine++;
                        cursorPos = 0;
                    }
                    if(iii <= 10) {
                        edits[iii].cursor=cursorPos;
                        edits[iii].line=currentLine;
                        edits[iii].don=e.text.text;
                        edits[iii].type=0;
                    }
                    if(iii == 11) {
                        for(int j=1;j<10;j++) {
                            edits[j].cursor=edits[j+1].cursor;
                            edits[j].line=edits[j+1].line;
                            edits[j].don=edits[j+1].don;
                            edits[j].type=0;
                        }
                        edits[10].cursor=cursorPos;
                        edits[10].line=currentLine;
                        edits[10].don=e.text.text;
                        edits[10].type=0;
                    }
                    if(lines[currentLine][cursorPos-2]=='/'&&lines[currentLine][cursorPos-1]=='*') {
                        com1++;
                        com[com1].line=currentLine;
                        com[com1].cursor=cursorPos-2;
                        com[com1].type=1;
                    }
                    if(lines[currentLine][cursorPos-2]=='*'&&lines[currentLine][cursorPos-1]=='/') {
                        com2++;
                        com[com2].line=currentLine;
                        com[com2].cursor=cursorPos-1;
                        com[com2].type=0;
                        bp=true;
                    }
                    cursorPos += strlen(e.text.text);
                    if(strcmp(e.text.text,"(")==0){lines[currentLine].insert(cursorPos, ")");}
                    else if(strcmp(e.text.text,"{")==0){lines[currentLine].insert(cursorPos, "}");}
                    else if(strcmp(e.text.text,"[")==0){lines[currentLine].insert(cursorPos, "]");}

                    bool bits,cmath,iostream;
                    includeerror.clear();
                    checklibrary(iostream,cmath,bits,lines);
                    finalword(iostream,cmath,bits,lines,includeerror);

                    ensureLastLineVisible(currentLine, scrollOffset, SCREEN_HEIGHT, LINE_HEIGHT, lines.size());
                }
            }
            else if(e.type==SDL_MOUSEBUTTONDOWN){
                if(e.button.x>=104&&e.button.x<=141&&e.button.y>=0&&e.button.y<=24){
                    bol.view=true;
                    //bol.compile= false;
                    bol.edit=false;
                    bol.file= false;
                    //bol.run=false;
                    bol.save=false;
                }
                if(e.button.x>=104&&e.button.x<=158&&e.button.y>=24&&e.button.y<=52){
                    if(bol.view&&!mode){mode=true;}
                    else if(bol.view&&mode){mode=false;}
                    bol.view= false;
                }
                if(e.button.x>=67&&e.button.x<=104&&e.button.y>=0&&e.button.y<=24){
                    bol.view=false;
                    //bol.compile= false;
                    bol.edit=true;
                    bol.file= false;
                    //bol.run=false;
                    bol.save=false;
                }
                if(e.button.x>=67&&e.button.x<=121&&e.button.y>=24&&e.button.y<=52){
                    if(bol.edit==true){undo=true;}
                    bol.edit= false;
                }
                if(e.button.x>=67&&e.button.x<=121&&e.button.y>=52&&e.button.y<=80){
                    if(bol.edit==true){redo=true;}
                    bol.edit= false;
                }
                if(e.button.x>=30&&e.button.x<=67&&e.button.y>=0&&e.button.y<=24){
                    bol.view=false;
                    //bol.compile= false;
                    bol.edit=false;
                    bol.file= true;
                    //bol.run=false;
                    bol.save=false;
                }
                if(e.button.x>=30&&e.button.x<=160&&e.button.y>=24&&e.button.y<=52){
                    if(bol.file==true){new_p=true;}
                    bol.file= false;
                }
                if(e.button.x>=30&&e.button.x<=160&&e.button.y>=52&&e.button.y<=80){
                    if(bol.file==true){save_p=true;}
                    bol.file= false;
                }
                if(e.button.x>=30&&e.button.x<=160&&e.button.y>=108&&e.button.y<=136){
                    if(bol.file==true){exit=true;}
                    bol.file= false;
                }
                if(e.button.x>=30&&e.button.x<=160&&e.button.y>=80&&e.button.y<=108){
                    if(bol.file==true){save_ass=true;}
                    bol.file= false;
                }
                if(e.button.x>=141&&e.button.x<=178&&e.button.y>=0&&e.button.y<=24){
                    bol.view=false;
                    //bol.compile= false;
                    bol.edit=false;
                    bol.file= false;
                    //bol.run=false;
                    bol.save=true;
                }
                if(e.button.x>=178&&e.button.x<=235&&e.button.y>=0&&e.button.y<=24) {
                    bol.view=false;
                    bol.file= false;
                    bol.run=false;
                    bol.edit= false;
                    bol.save= false;
                    bol.compile= true;
                    save(name,lines);
                    compile_p=compile(name);
                    if(compile_p.size()==1){
                        compile_p.push_back("Compilation was successful.");
                    }
                }//compile &compile
                if(e.button.x>=235&&e.button.x<=265&&e.button.y>=0&&e.button.y<=24) {
                    bol.view=false;
                    bol.file= false;
                    bol.run=true;
                    bol.edit= false;
                    bol.save= false;
                    bol.compile=false;
                    if(name==""){
                        save("temp",lines);
                        compile_p=compile("temp");
                        run_p=run("temp");
                    }
                    else{save(name,lines);
                        compile_p=compile(name);
                        run_p=run(name);
                    }
                }//run
                if(e.button.x>160||e.button.x<30||e.button.y>108) {
                    bol.file= false;
                }
                if(e.button.x>121||e.button.x<67||e.button.y>80) {
                    bol.edit= false;
                }
                if(e.button.x>158||e.button.x<104||e.button.y>52) {
                    bol.view= false;
                }
                if(!redo&&!save_p&&!save_ass&&!bol.edit&&!bol.file&&e.button.x>=0&&e.button.x<=207&&e.button.y>=24&&e.button.y<=500){
                    point c;
                    c.x=e.button.x;
                    c.y=e.button.y;
                    regex p("\\.cpp");
                    for (auto i:xx) {
                        bool R= regex_search(i.first,p);
                        bool T=click(i.second[0],i.second[1],i.second[2],i.second[3],c);
                        if(T&&R){
                            string open_name=i.first.substr(4);
                            vector<string>lineq=open_file(open_name);
                            lines.clear();
                            lines=lineq;
                            cursorPos=0;
                            scrollOffset=i1*LINE_HEIGHT;
                            currentLine=0;
                            string ss=i.first.substr(4);
                            int rr=ss.find(".cpp");
                            name=ss.substr(0,rr);
                            break;
                        }
                    }
                }
                if(e.button.x>=207&&e.button.x<=1000&&e.button.y>=24&&e.button.y<=816){
                    select_all= false;select_part=false;
                }
                if((bol.run||bol.compile)&&e.button.x>=975&&e.button.x<=995&&e.button.y>=610&&e.button.y<=630){
                    if(bol.run)
                        bol.run=false;
                    if(bol.compile)
                        bol.compile=false;
                }
            }

        }
        // Clear screen
        if(mode==true) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        }//light mode
        else {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }//dark mode
        SDL_RenderClear(renderer);

        // Render text
        y = -scrollOffset; // Start rendering based on the scroll offset

        for (size_t i = 0; i < lines.size(); ++i) {
            if (y + LINE_HEIGHT > LINE_HEIGHT && y < SCREEN_HEIGHT) { // Render only visible lines
                if (lines[i].empty()) {
                    lines[i] = " "; // Show cursor on the current line
                }
                if(mode==true) {
                    renderTextWithColor(renderer, font, lines[i], text_l, y, cursorPos,mode);
                }
                else {
                    renderTextWithColor(renderer, font, lines[i], text_d, y, cursorPos,mode);
                }

                // Render cursor if this is the current line
                if (i == currentLine && cursorVisible) {
                    int cursorX = 200;
                    int cursorX1;
                    if (cursorPos > 0) {
                        TTF_SizeText(font, lines[i].substr(0, cursorPos).c_str(), &cursorX1, nullptr);
                        cursorX+=cursorX1;
                    }
                    cursorX += 10; // Add padding for the left margin
                    if(mode==true) {
                        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    }
                    else {
                        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    }
                    SDL_RenderDrawLine(renderer, cursorX, y, cursorX, y + LINE_HEIGHT);
                }
            }
            y += LINE_HEIGHT; // Move to the next line
        }
        addimg(renderer,0,0,30,25,"assets/photo16646248983.jpg");
        if(mode){
            boxRGBA(renderer, 30, 0, 1000, 24, 245, 245, 245, 255 );//menu
            boxRGBA(renderer, 0, 400, 207, 816, 255, 159, 67, 255 );//error
            boxRGBA(renderer, 0, 25, 207, 400, 200, 255, 200, 255 );//Tree
        }
        else {
            boxRGBA(renderer, 30, 0, 1000, 24, 45, 45, 45, 255 );//menu
            boxRGBA(renderer, 0, 400, 207, 816, 255, 99, 71, 255 );//error
            boxRGBA(renderer, 0, 25, 207, 400, 0, 51, 51, 255 );//Tree
        }
        textshow(renderer,font1,color3,"File",30+37*0+5,6);
        textshow(renderer,font1,color3,"Edit",30+37*1+5,6);
        textshow(renderer,font1,color3,"View",30+37*2+5,6);
        textshow(renderer,font1,color3,"Save",30+37*3+5,6);
        textshow(renderer,font1,color3,"compile",30+37*4+5,6);
        textshow(renderer,font1,color3,"Run",30+37*5+25,6);

        int j=0;
        SDL_Color eror,tree,open_menu,ok;
        if(mode) {
            eror={255,255,255,255};
            tree={0,102,0,255};
            open_menu={30,30,30,255};
            ok={255,255,255,255};
        }
        else {
            eror={255,255,255,255};
            tree={153,255,153,255};
            open_menu={255,255,255,255};
            ok={30,30,30,255};
        }
        if(select_all){
            textshow(renderer,font1,eror,"select all text is on",30,410+10*j);
            j++;
        }
        if(select_part){
            textshow(renderer,font1,eror,"select a part of text is on",10,410+10*j);
            j++;
        }
        xx.clear();
        Tree(renderer,font5,tree,xx);
        paran=checkcount(lines,'(',')');
        acco=checkcount(lines,'{','}');
        bera=checkcount(lines,'[',']');
        cemi= cemicalom(lines);
        keyworderror= key_error(lines);
        val_error= val_errorf(lines);
        operror= op_error(lines);
        strerror= str_error(lines);
        if(!includeerror.empty()){
            bool T=true,TT=true;
            for (auto ii:includeerror) {
                if (ii.second == 1&&T){
                    textshow(renderer, font1, eror, "iostream error", 10, 410+10*j);T=false;j++;}
                else if (ii.second == 2&&TT){
                    textshow(renderer, font1, eror, "cmath error", 10, 410+10*j);TT=false;j++;}
            }
        }//show library error
        if(paran!=0){
            if(paran>0){
                string e= to_string(paran);
                textshow(renderer,font1,eror,"() Parenthesis error need "+e+" )",10,410+10*j);j++;}
            else if(paran<0){
                string e= to_string(-paran);
                textshow(renderer,font1,eror,"() Parenthesis error need "+e+" (",10,410+10*j);j++;}
        }//() error
        if(acco!=0){
            if(acco>0){
                string e= to_string(acco);
                textshow(renderer,font1,eror,"{} accolade error need "+e+" }",10,410+10*j);j++;}
            else if(acco<0){
                string e= to_string(-acco);
                textshow(renderer,font1,eror,"{} accolade error need "+e+" {",10,410+10*j);j++;}
        }//{} error
        if(bera!=0){
            if(bera>0){
                string e= to_string(bera);
                textshow(renderer,font1,eror,"[] bracket error need "+e+" ]",10,410+10*j);j++;}
            else if(bera<0){
                string e= to_string(-bera);
                textshow(renderer,font1,eror,"[] bracket error need "+e+" [",10,410+10*j);j++;}
        }//[] error
        if(com1!=com2){
            textshow(renderer,font1,eror,"comment error",10,410+10*j);j++;}//comment error
        if(!cemi.empty()){
            for (auto ii:cemi) {
                string s= "Semicolon error in line: "+to_string(ii);
                textshow(renderer,font1,eror,s,10,410+10*j);
                j++;
            }
        }// ; error
        if(!keyworderror.empty()){
            for (auto i:keyworderror) {
                string e= to_string(i.second);
                textshow(renderer,font1,eror,"in line "+e+"-->"+i.first,10,410+10*j);j++;
            }
        }//key word error
        if(!val_error.empty()){
            for (auto i:val_error) {
                string s= to_string(i.second);
                textshow(renderer,font1,eror,"Invalid name in line:"+s,10,410+10*j);j++;
            }
        }//invalid name for variable
        if(!operror.empty()){
            for (int i:operror) {
                string s= to_string(i);
                textshow(renderer,font1,eror,"Invalid opreator in line:"+s,10,410+10*j);j++;
            }
        }//operator error
        if(!strerror.empty()){
            for (int i:strerror) {
                string s= to_string(i);
                textshow(renderer,font1,eror,"open string in line:"+s,10,410+10*j);j++;
            }
        }//str error
        if(bol.view){
            if(mode)
                boxRGBA(renderer,104,24,158,52,255,255,255,255);
            else
                boxRGBA(renderer,104,24,158,52,60,60,60,255);
            TTF_Font* font2 = TTF_OpenFont("assets/consola.ttf", 18);
            textshow(renderer,font2,open_menu,"Mode",112,29);
        }//show view menu
        if(bol.edit){
            if(mode)
                boxRGBA(renderer,67,24,121,80,255,255,255,255);
            else
                boxRGBA(renderer,67,24,121,80,60,60,60,255);
            TTF_Font* font2 = TTF_OpenFont("assets/consola.ttf", 18);
            textshow(renderer,font2,open_menu,"Undo",75,29);
            textshow(renderer,font2,open_menu,"Redo",75,57);
        }//show edit menu
        if(bol.file){
            if(mode)
                boxRGBA(renderer,30,24,160,136,255,255,255,255);
            else
                boxRGBA(renderer,30,24,160,136,60,60,60,255);
            TTF_Font* font2 = TTF_OpenFont("assets/consola.ttf", 18);
            textshow(renderer,font2,open_menu,"New Project",36,29);
            textshow(renderer,font2,open_menu,"Save Project",36,57);
            textshow(renderer,font2,open_menu,"Save As",36,85);
            textshow(renderer,font2,open_menu,"Exit",36,113);
        }//show file menu
        if(undo&&k>0) {
            if(edits[k].type==0) {
                lines[edits[k].line].erase(edits[k].cursor,1);
                a=cc;
                if(cc < 10){cc++;}
                if(a<=10){a++;}
                if(a<=10) {
                    edits2[a].cursor=edits[k].cursor;
                    edits2[a].line=edits[k].line;
                    edits2[a].don=edits[k].don;
                    edits2[a].type=edits[k].type;
                }
                if(a==11) {
                    for(int j=1;j<10;j++) {
                        edits2[j].cursor=edits2[j+1].cursor;
                        edits2[j].line=edits2[j+1].line;
                        edits2[j].don=edits2[j+1].don;
                        edits2[j].type=edits2[j+1].type;
                    }
                    edits2[10].cursor=edits[k].cursor;
                    edits2[10].line=edits[k].line;
                    edits2[10].don=edits[k].don;
                    edits2[10].type=edits[k].type;
                }
            }
            else if(edits[k].type==1) {
                lines[edits[k].line].insert(edits[k].cursor-1,edits[k].don);
                a=cc;
                if(cc < 10){cc++;}
                if(a<=10){a++;}
                if(a<=10) {
                    edits2[a].cursor=edits[k].cursor;
                    edits2[a].line=edits[k].line;
                    edits2[a].don=edits[k].don;
                    edits2[a].type=edits[k].type;
                }
                if(a==11) {
                    for(int j=1;j<10;j++) {
                        edits2[j].cursor=edits2[j+1].cursor;
                        edits2[j].line=edits2[j+1].line;
                        edits2[j].don=edits2[j+1].don;
                        edits2[j].type=edits2[j+1].type;
                    }
                    edits2[10].cursor=edits[k].cursor;
                    edits2[10].line=edits[k].line;
                    edits2[10].don=edits[k].don;
                    edits2[10].type=edits[k].type;
                }
            }
            else if(edits[k].type==2) {
                lines[edits[k].line+1].erase(0,edits[k].don.size());
                lines[edits[k].line].insert(edits[k].cursor,edits[k].don);
                a=cc;
                if(cc < 10){cc++;}
                if(a<=10){a++;}
                if(a<=10) {
                    edits2[a].cursor=edits[k].cursor;
                    edits2[a].line=edits[k].line;
                    edits2[a].don=edits[k].don;
                    edits2[a].type=edits[k].type;
                }
                if(a==11) {
                    for(int j=1;j<10;j++) {
                        edits2[j].cursor=edits2[j+1].cursor;
                        edits2[j].line=edits2[j+1].line;
                        edits2[j].don=edits2[j+1].don;
                        edits2[j].type=edits2[j+1].type;
                    }
                    edits2[10].cursor=edits[k].cursor;
                    edits2[10].line=edits[k].line;
                    edits2[10].don=edits[k].don;
                    edits2[10].type=edits[k].type;
                }
            }
            cursorPos=edits[k].cursor;
            currentLine=edits[k].line;
            edits.erase(k);
            k--;
            undo=false;
        }
        if(redo && cc > 0) {
            if(edits2[cc].type == 0) {
                lines[edits2[cc].line].insert(edits2[cc].cursor, edits2[cc].don);
                cursorPos= edits2[cc].cursor + 1;
                currentLine=edits2[cc].line;
            }
            else if(edits2[cc].type == 1) {
                lines[edits2[cc].line].erase(edits2[cc].cursor - 1, 1);
                cursorPos= edits2[cc].cursor - 1;
                currentLine=edits2[cc].line;
            }
            else if(edits2[cc].type == 2) {
                lines[edits2[cc].line]=lines[edits2[cc].line].substr(0, edits2[cc].cursor);
                lines[edits2[cc].line + 1].insert(0, edits2[cc].don);
                cursorPos=0;
                currentLine= edits2[cc].line + 1;
            }
            edits.erase(cc);
            cc--;
            redo=false;
        }
        if (save_p||bol.save) {
            if(name==""){
                if(mode) {
                    boxRGBA(renderer, 30, 24, 350, 94, 200, 200, 200, 255);
                    boxRGBA(renderer, 320, 74, 350, 94, 100, 100, 100, 255);
                    boxRGBA(renderer, 40, 44, 300, 64, 235, 235, 235, 255);
                    boxRGBA(renderer, 310, 44, 340, 64, 235, 235, 235, 255);
                }
                else {
                    boxRGBA(renderer, 30, 24, 350, 94, 70, 70, 70, 255);
                    boxRGBA(renderer, 320, 74, 350, 94, 155, 155, 155, 255);
                    boxRGBA(renderer, 40, 44, 300, 64, 120, 120, 120, 255);
                    boxRGBA(renderer, 310, 44, 340, 64, 120, 120, 120, 255);
                }
                textshow(renderer, font4, ok, "ok", 330, 79);
                textshow(renderer,font3, open_menu,".cpp",310,44);
                SDL_Event ee;
                bool ok = true;
                while (ok) {
                    while (SDL_PollEvent(&ee)) {
                        if ((ee.type == SDL_MOUSEBUTTONDOWN && ee.button.x > 320 && ee.button.x < 350 && ee.button.y > 74 &&ee.button.y < 94)||(ee.type==SDL_KEYDOWN&&ee.key.keysym.sym==SDLK_RETURN)) {
                            ok = false;
                            save_p = false;
                            save(name,lines);
                            break;
                        }
                        else if (ee.type == SDL_KEYDOWN && ee.key.keysym.sym == SDLK_ESCAPE) {
                            ok = false;
                            save_p = false;
                            name="";
                            break;
                        }
                        else if (ee.type == SDL_TEXTINPUT) {
                            if (ee.text.text) {
                                name += ee.text.text;
                                textshow(renderer,font5, open_menu,name,50,46);
                            }
                        }
                        else if (ee.type == SDL_KEYDOWN && ee.key.keysym.sym == SDLK_BACKSPACE){
                            if(name!=""){
                                name.pop_back();
                                if(mode)
                                    boxRGBA(renderer, 40, 44, 300, 64, 235, 235, 235, 255);
                                else
                                    boxRGBA(renderer, 40, 44, 300, 64, 120, 120, 120, 255);
                                textshow(renderer,font5, open_menu,name,50,46);}
                        }

                    }
                    SDL_RenderPresent(renderer);
                }
            }
            else if(name!=""){ save(name,lines);}
            save_p= false;
            bol.save= false;
        }//save project
        if(new_p){
            //projects.push_back(lines);
            //lines.clear();
            currentLine = 0;
            lines={""};
            cursorPos=0;
            scrollOffset=i1*LINE_HEIGHT;
            name="";
            new_p=false;
            T= true;
        }//new progect
        if(exit){
            break;
        }//exit
        if(save_ass){
            string name_as;
            if(mode) {
                boxRGBA(renderer, 30, 24, 350, 94, 200, 200, 200, 255);
                boxRGBA(renderer, 320, 74, 350, 94, 100, 100, 100, 255);
                boxRGBA(renderer, 40, 44, 300, 64, 235, 235, 235, 255);
                boxRGBA(renderer, 310, 44, 340, 64, 235, 235, 255, 255);
            }
            else {
                boxRGBA(renderer, 30, 24, 350, 94, 70, 70, 70, 255);
                boxRGBA(renderer, 320, 74, 350, 94, 155, 155, 155, 255);
                boxRGBA(renderer, 40, 44, 300, 64, 120, 120, 120, 255);
                boxRGBA(renderer, 310, 44, 340, 64, 120, 120, 120, 255);
            }
            textshow(renderer, font4, ok, "ok", 330, 79);
            textshow(renderer,font3, open_menu,".cpp",310,44);
            SDL_Event ee;
            string temp;
            bool ok = true;
            while (ok) {
                while (SDL_PollEvent(&ee)) {
                    if ((ee.type == SDL_MOUSEBUTTONDOWN && ee.button.x > 320 && ee.button.x < 350 && ee.button.y > 74 &&ee.button.y < 94)||(ee.type==SDL_KEYDOWN&&ee.key.keysym.sym==SDLK_RETURN)) {
                        ok = false;
                        save_ass= false;
                        bol.save=false;
                        save_as(temp,lines);
                        break;
                    }
                    else if (ee.type == SDL_KEYDOWN && ee.key.keysym.sym == SDLK_ESCAPE) {
                        ok = false;
                        save_ass= false;
                        bol.save=false;
                        break;
                    }
                    else if (ee.type == SDL_TEXTINPUT) {
                        if (ee.text.text) {
                            temp += ee.text.text;
                            textshow(renderer,font5, open_menu,temp,50,46);
                        }
                    }
                    else if (ee.type == SDL_KEYDOWN && ee.key.keysym.sym == SDLK_BACKSPACE){
                        if(temp!=""){
                            temp.pop_back();
                            if(mode)
                                boxRGBA(renderer, 40, 44, 300, 64, 235, 235, 235, 255);
                            else
                                boxRGBA(renderer, 40, 44, 300, 64, 120, 120, 120, 255);
                            textshow(renderer,font5, open_menu,temp,50,46);}
                    }

                }
                SDL_RenderPresent(renderer);
            }
            save_ass=false;
        }//save as
        if(gotoline){
            string linenum;
            if(mode) {
                boxRGBA(renderer, 30, 24, 100, 74, 200, 200, 200, 255);
                boxRGBA(renderer, 40, 34, 80, 44, 235, 235, 235, 255);
                boxRGBA(renderer, 80, 54, 100, 74, 100, 100, 100, 255);
            }
            else {
                boxRGBA(renderer, 30, 24, 100, 74, 70, 70, 70, 255);
                boxRGBA(renderer, 40, 34, 80, 44, 120, 120, 120, 255);
                boxRGBA(renderer, 80, 54, 100, 74, 155, 155, 155, 255);
            }
            textshow(renderer, font4, ok, "ok", 85, 59);
            SDL_Event ee;
            bool ok = true;
            while (ok) {
                while (SDL_PollEvent(&ee)) {
                    if ((ee.type == SDL_MOUSEBUTTONDOWN && ee.button.x > 80 && ee.button.x < 100 && ee.button.y > 54 &&ee.button.y < 74)||(ee.type==SDL_KEYDOWN&&ee.key.keysym.sym==SDLK_RETURN)) {
                        ok = false;
                        gotoline= false;
                        if(stoi(linenum)-1>lines.size()){
                            currentLine=lines.size()-1;
                        }
                        else{
                            currentLine = stoi(linenum)-1;
                        }
                        y=-(currentLine+2)*LINE_HEIGHT;
                        cursorPos=0;
                        break;
                    }
                    else if (ee.type == SDL_KEYDOWN && ee.key.keysym.sym == SDLK_ESCAPE) {
                        ok = false;
                        gotoline= false;
                        break;
                    }
                    else if (ee.type == SDL_TEXTINPUT) {
                        if (ee.text.text) {
                            linenum += ee.text.text;
                            textshow(renderer,font4, open_menu,linenum,45,36);
                        }
                    }
                    else if (ee.type == SDL_KEYDOWN && ee.key.keysym.sym == SDLK_BACKSPACE){
                        if(linenum!=""){
                            linenum.pop_back();
                            if(mode)
                                boxRGBA(renderer, 40, 34, 80, 44, 235, 235, 235, 255);
                            else
                                boxRGBA(renderer, 40, 34, 80, 44, 120, 120, 120, 255);
                            textshow(renderer,font4, open_menu,linenum,45,36);}
                    }

                }
                SDL_RenderPresent(renderer);
            }
            save_ass=false;

        }//go to line
        if(bol.compile){
            if(mode) {
                boxRGBA(renderer,207,600,1000,816,225,225,225,255);
                textshow(renderer,font5,{33,37,41,255},"Terminal/>",217,610);
                boxRGBA(renderer,975,610,995,630,0,123,255,255);
            }
            else {
                boxRGBA(renderer,207,600,1000,816,100,100,100,255);
                textshow(renderer,font5,{224,224,224,255},"Terminal/>",217,610);
                boxRGBA(renderer,975,610,995,630,30,136,229,255);
            }
            textshow(renderer,font5,open_menu,"x",980,610);
            int j=0;
            for (string ii:compile_p) {
                if(mode)
                    textshow(renderer,font5,{200,0,0,255},ii,230,640+j*20);
                else
                    textshow(renderer,font5,{255,65,65,255},ii,230,640+j*20);
                j++;
            }
        }//compile
        if(bol.run){
            if(mode) {
                boxRGBA(renderer,207,600,1000,816,225,225,225,255);
                textshow(renderer,font5,{0,102,204,255},"Terminal/>",217,610);
                boxRGBA(renderer,975,610,995,630,0,123,255,255);
            }
            else {
                boxRGBA(renderer,207,600,1000,816,100,100,100,255);
                textshow(renderer,font5,{100,181,246,255},"Terminal/>",217,610);
                boxRGBA(renderer,975,610,995,630,30,136,229,255);
            }
            textshow(renderer,font5,open_menu,"x",980,610);
            int j=0;
            for (string r:compile_p) {
                if(mode)
                    textshow(renderer,font5,{200,0,0,255},r,230,640+j*20);
                else
                    textshow(renderer,font5,{255,65,65,255},r,230,640+j*20);
                j++;
            }
            for (string ii:run_p) {
                textshow(renderer,font5,open_menu,ii,230,640+j*20);
                j++;
            }
            if(name==""&&T){
                bool x = system("if exist data/Tree_project/temp.cpp (del /Q data/Tree_project/temp.cpp & del /Q data/Tree_project/temp.exe)");
                T= false;
            }
        }//run
        // Update screen
        SDL_RenderPresent(renderer);
    }
    // Clean up
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    return 0;
}



void ensureLastLineVisible(int currentLine, int &scrollOffset, int SCREEN_HEIGHT, int LINE_HEIGHT, int totalLines) {
    int cursorY = currentLine * LINE_HEIGHT - scrollOffset;
    if (cursorY < 0) {
        // Scroll up
        scrollOffset = currentLine * LINE_HEIGHT;
    } else if (cursorY + LINE_HEIGHT > SCREEN_HEIGHT) {
        // Scroll down
        scrollOffset = (currentLine + 1) * LINE_HEIGHT - SCREEN_HEIGHT;
    }
    int i1=-1;
    // Ensure last line is always visible
    int contentHeight = (totalLines-i1) * LINE_HEIGHT;
    if (contentHeight > SCREEN_HEIGHT) {
        scrollOffset = std::min(scrollOffset, contentHeight - SCREEN_HEIGHT);
    } else {
        scrollOffset = (i1)*LINE_HEIGHT; // No scrolling needed if content fits
    }
}

void renderTextWithColor(SDL_Renderer* renderer, TTF_Font* font, const string& text, SDL_Color defaultColor, int y, int cursorPos,bool mode) {
    regex wordPattern("\\b(for|if|else|int|double|float|class|char|while|long|switch|case|break|continue|string|do|return|default|scanf|printf|strlen|abs|sqrt|main)\\b|\\d+|\\{|\\}|\\(|\\)|\\[|\\]|\\+|\\-|\\*|\\=|\\%|\\#include|(/(?!/))|(\\|)|\\!|\\&");
    regex quotePattern(R"(["](.*?)["]|['](.*?)['])");
    regex commentPattern(R"(//(.*))");
    regex variablePattern(R"(\b(int|double|float|long|short|char|string|bool|void|const|static)\s+([\w\s,*&]+)(?![\w\s,*&]*\s*\())");

    SDL_Color key_l = {0, 51, 102, 255};
    SDL_Color key_d = {198, 120, 221,255};
    SDL_Color data_l = {0, 128, 128, 255};
    SDL_Color data_d = {224, 108, 117, 255};
    SDL_Color include_l = {0, 139, 139, 255};
    SDL_Color include_d = {86, 182, 194, 255};
    SDL_Color bracket_l = {184, 134, 11, 255};
    SDL_Color bracket_d = {171, 178, 191, 255};
    SDL_Color number_l = {128, 0, 128, 255};
    SDL_Color number_d = {209, 154, 102, 255};
    SDL_Color operator_l = {128, 0, 0, 255};
    SDL_Color operator_d = {213, 94, 0, 255};
    SDL_Color qoute_l = {0, 100, 0, 255};
    SDL_Color qoute_d = {152, 195, 121, 255};
    SDL_Color comment_l = {128, 128, 128, 255};
    SDL_Color comment_d = {92, 99, 112, 255};
    SDL_Color function_l = {255, 140, 0, 255};
    SDL_Color function_d = {97, 175, 254, 255};
    SDL_Color variable_l = {139, 0, 0, 255};
    SDL_Color variable_d = {229, 192, 123, 255};

    int currentX = 210;
    size_t prevPos = 0;

    vector<pair<size_t, size_t>> ranges;

    for (sregex_iterator it(text.begin(), text.end(), commentPattern), end; it != end; ++it) {
        size_t start = it->position();
        size_t endPos = start + it->length();
        ranges.emplace_back(start, endPos);
    }

    for (sregex_iterator it(text.begin(), text.end(), quotePattern), end; it != end; ++it) {
        size_t start = it->position();
        size_t endPos = start + it->length();
        ranges.emplace_back(start, endPos);
    }

    for (sregex_iterator it(text.begin(), text.end(), variablePattern), end; it != end; ++it) {
        size_t start = it->position();
        size_t endPos = start + it->length();
        ranges.emplace_back(start, endPos);
    }

    for (sregex_iterator it(text.begin(), text.end(), wordPattern), end; it != end; ++it) {
        size_t start = it->position();
        size_t endPos = start + it->length();
        ranges.emplace_back(start, endPos);
    }

    sort(ranges.begin(), ranges.end());

    vector<pair<size_t, size_t>> uniqueRanges;
    size_t lastEnd = 0;
    for (const auto& range : ranges) {
        if (range.first >= lastEnd) {
            uniqueRanges.push_back(range);
            lastEnd = range.second;
        }
    }

    for (const auto& range : uniqueRanges) {
        size_t startPos = range.first;
        size_t endPos = range.second;

        if (startPos > prevPos) {
            string beforeText = text.substr(prevPos, startPos - prevPos);
            SDL_Surface* beforeSurface = TTF_RenderText_Blended(font, beforeText.c_str(), defaultColor);
            SDL_Texture* beforeTexture = SDL_CreateTextureFromSurface(renderer, beforeSurface);
            SDL_Rect beforeRect = {currentX, y, beforeSurface->w, beforeSurface->h};
            SDL_RenderCopy(renderer, beforeTexture, nullptr, &beforeRect);
            SDL_FreeSurface(beforeSurface);
            SDL_DestroyTexture(beforeTexture);
            currentX += beforeRect.w;
        }

        string word = text.substr(startPos, endPos - startPos);
        SDL_Color wordColor = defaultColor;

        if(mode== true) {
            if (regex_match(word, commentPattern)) {
                wordColor = comment_l;
            } else if (regex_match(word, quotePattern)) {
                wordColor = qoute_l;
            } else if(regex_match(word,variablePattern)) {
                wordColor = variable_l;
            } else if (regex_match(word, wordPattern)) {
                if (word == "if" || word == "else" || word == "for" || word == "class"||word=="while"||word=="switch"||word=="case"||word=="break"||word=="continue"||word=="do"||word=="return"||word=="default") {
                    wordColor = key_l;
                } else if (word == "int" || word == "double" || word == "float" || word == "char"||word=="string"||word=="long") {
                    wordColor = data_l;
                } else if (word == "#include"||word=="main") {
                    wordColor = include_l;
                } else if (word == "{" || word == "}" || word == "(" || word == ")" || word == "[" || word == "]") {
                    wordColor = bracket_l;
                } else if (isdigit(word[0])) {
                    wordColor = number_l;
                } else if (word=="="||word=="+"||word=="-"||word=="*"||word=="/"||word=="%"||word=="|"||word=="&"||word=="!"){
                    wordColor = operator_l;
                }else {
                    wordColor = function_l;
                }
            }
        }
        if(mode== false) {
            if (regex_match(word, commentPattern)) {
                wordColor = comment_d;
            } else if (regex_match(word, quotePattern)) {
                wordColor = qoute_d;
            } else if(regex_match(word,variablePattern)) {
                wordColor = variable_d;
            } else if (regex_match(word, wordPattern)) {
                if (word == "if" || word == "else" || word == "for" || word == "class"||word=="while"||word=="switch"||word=="case"||word=="break"||word=="continue"||word=="do"||word=="return"||word=="default") {
                    wordColor = key_d;
                } else if (word == "int" || word == "double" || word == "float" || word == "char"||word=="string"||word=="long") {
                    wordColor = data_d;
                } else if (word == "#include"||word=="main") {
                    wordColor = include_d;
                } else if (word == "{" || word == "}" || word == "(" || word == ")" || word == "[" || word == "]") {
                    wordColor = bracket_d;
                } else if (isdigit(word[0])) {
                    wordColor = number_d;
                } else if (word=="="||word=="+"||word=="-"||word=="*"||word=="/"||word=="%"||word=="|"||word=="&"||word=="!"){
                    wordColor = operator_d;
                }else {
                    wordColor = function_d;
                }
            }
        }

        SDL_Surface* wordSurface = TTF_RenderText_Blended(font, word.c_str(), wordColor);
        SDL_Texture* wordTexture = SDL_CreateTextureFromSurface(renderer, wordSurface);
        SDL_Rect wordRect = {currentX, y, wordSurface->w, wordSurface->h};
        SDL_RenderCopy(renderer, wordTexture, nullptr, &wordRect);
        SDL_FreeSurface(wordSurface);
        SDL_DestroyTexture(wordTexture);
        currentX += wordRect.w;

        prevPos = endPos;
    }

    if (prevPos < text.size()) {
        string remainingText = text.substr(prevPos);
        SDL_Surface* remainingSurface = TTF_RenderText_Blended(font, remainingText.c_str(), defaultColor);
        SDL_Texture* remainingTexture = SDL_CreateTextureFromSurface(renderer, remainingSurface);
        SDL_Rect remainingRect = {currentX, y, remainingSurface->w, remainingSurface->h};
        SDL_RenderCopy(renderer, remainingTexture, nullptr, &remainingRect);
        SDL_FreeSurface(remainingSurface);
        SDL_DestroyTexture(remainingTexture);
    }
}