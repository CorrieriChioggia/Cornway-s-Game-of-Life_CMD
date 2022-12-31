#include<iostream>
#include<windows.h>
#include<cstdlib>
#include<conio.h>
using namespace std;

#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_F 102
#define KEY_G 103

const int n=42;

void goToConsole(int x, int y){
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

bool moveCursor(int &xCursor, int &yCursor, int xMin, int yMin, int xMax, int yMax, bool m[n][n], int &i, int &j){
    goToConsole(xCursor, yCursor);
    char key;
    int value;
    key = getch();
    value=key;
    switch(value){
        case KEY_W:{
            yCursor--;
            i--;
            break;
        }
        case KEY_A:{
            xCursor=xCursor-2;
            j--;
            break;
        }
        case KEY_S:{
            yCursor++;
            i++;
            break;
        }
        case KEY_D:{
            xCursor=xCursor+2;
            j++;
            break;
        }
        case KEY_F:{
            m[i][j]=!m[i][j];
            break;
        }
        case KEY_G:{
            return false;
            break;
        }
    }
    if(xCursor<xMin){
        xCursor=xMin;
        j++;
    }
    if(yCursor<yMin){
        yCursor=yMin;
        i++;
    }
    if(xCursor>xMax*2-xMin-1){
        xCursor=xMax*2-xMin-1;
        j--;
    }
    if(yCursor>yMax-2){
        yCursor=yMax-2;
        i--;
    }
    goToConsole(xCursor,yCursor);
    return true;
}

void cursorVisibility(bool a, int b){
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO ci;
    ci.dwSize = b;
    if(a){
        ci.bVisible = TRUE;
    }else{
        ci.bVisible = FALSE;
    }
    SetConsoleCursorInfo(output, &ci);
}

void setMaximized(){
    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd,SW_SHOWMAXIMIZED);
}

void printMat(bool m[n][n], int xMin, int yMin, int &a, int &b){
    a=0;
    goToConsole(xMin,yMin);
    for(int i=1; i<n-1; i++){
        for(int j=1; j<n-1; j++){
            switch(m[i][j]){
                case false:{
                    cout<<char(255)<<char(255);
                    break;
                }
                case true:{
                    cout<<char(219)<<char(219);
                    a++;
                    break;
                }
            }
        }
        goToConsole(xMin, i+yMin);
        //cout<<endl;
    }
    b++;
}

void printASCIIFrame(int xMin, int yMin, int xMax, int yMax, int sideSize){
    goToConsole(xMin-1, yMin-1);
    cout<<char(201);
    for(int i=0; i<sideSize*2; i++){
        cout<<char(205);
    }
    cout<<char(187);
    for(int i=0; i<sideSize-1; i++){
        goToConsole(xMin-1, yMin+i);
        cout<<char(186);
        goToConsole(xMax*2-xMin, yMin+i);
        cout<<char(186);
    }
    goToConsole(xMin-1, yMax-1);
    cout<<char(200);
    for(int i=0; i<sideSize*2; i++){
        cout<<char(205);
    }
    cout<<char(188);
    goToConsole(xMax*2+10, yMin+1);
    cout<<"Colore cella bianco: cella viva; colore cella nero: cella morta";
    goToConsole(xMax*2+10, yMin+2);
    cout<<"[w] Muovi cella su";
    goToConsole(xMax*2+10, yMin+3);
    cout<<"[s] Muovi cella gi"<<char(151);
    goToConsole(xMax*2+10, yMin+4);
    cout<<"[d] Muovi cella destra";
    goToConsole(xMax*2+10, yMin+5);
    cout<<"[a] Muovi cella sinistra";
    goToConsole(xMax*2+10, yMin+6);
    cout<<"[f] Cambia stato (viva/morta)";
    goToConsole(xMax*2+10, yMin+7);
    cout<<"[g] Inizia l'evoluzione";
}

void copyMat(bool m1[n][n], bool m2[n][n]){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            m1[i][j]=m2[i][j];
        }
    }
}

//uncomment this function if you want a pattern to be preloaded

/*void setMat(bool m[n][n]){
    m[5][1]=true;
    m[6][1]=true;
    m[5][2]=true;
    m[6][2]=true;
    m[5][11]=true;
    m[6][11]=true;
    m[7][11]=true;
    m[4][12]=true;
    m[8][12]=true;
    m[3][13]=true;
    m[3][13]=true;
    m[9][13]=true;
    m[3][14]=true;
    m[9][14]=true;
    m[6][15]=true;
    m[4][16]=true;
    m[8][16]=true;
    m[5][17]=true;
    m[6][17]=true;
    m[7][17]=true;
    m[6][18]=true;
    m[3][21]=true;
    m[4][21]=true;
    m[5][21]=true;
    m[3][22]=true;
    m[4][22]=true;
    m[5][22]=true;
    m[2][23]=true;
    m[6][23]=true;
    m[1][25]=true;
    m[2][25]=true;
    m[6][25]=true;
    m[7][25]=true;
    m[3][35]=true;
    m[4][35]=true;
    m[3][36]=true;
    m[4][36]=true;
    //m[21][21]=true;
    //m[0][0]=true;
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            m[i][j]=true;
        }
    }
}*/

void fillFalse(bool m[n][n]){
    for(int i=0; i<n; i++){
        for(int j=0; j<n; j++){
            m[i][j]=false;
        }
    }
}

void nextState(bool m1[n][n], bool m2[n][n], int a, int b, int &nearCount){
    nearCount=0;
    for(int i=a-1; i<=a+1; i++){
        for(int j=b-1; j<=b+1; j++){
            if(m1[i][j]==true){
                nearCount++;
            }
        }
    }
    if(m1[a][b]==true&&nearCount>0){
        nearCount--;
    }

    if((nearCount==2||nearCount==3)&&m1[a][b]==true){
        m2[a][b]=true;
    }
    if((nearCount>3||nearCount<2)&&m1[a][b]==true){
        m2[a][b]=false;
    }
    if(m1[a][b]==false&&nearCount==3){
        m2[a][b]=true;
    }
}

void setFalseBorder(bool m[n][n]){
    for(int i=0; i<n; i++){
        m[0][i]=false;
    }
    for(int i=1; i<n-1; i++){
        m[i][0]=false;
        m[i][n-1]=false;
    }
    for(int i=0; i<n; i++){
        m[n-1][i]=false;
    }
}

int main(){
    int nearCount=0;;
    bool m1[n][n], m2[n][n];
    int xMin=10, xMax=n+xMin, yMin=5, yMax=n+yMin;
    int xCursor=xMin, yCursor=yMin;
    int Yi=1, Xj=1;
    int a, b=0;
    fillFalse(m1);
    fillFalse(m2);
    setMaximized();
    cursorVisibility(true, 100);
    //setMat(m1);
    printASCIIFrame(xMin, yMin, xMax, yMax, n);
    while(moveCursor(xCursor, yCursor, xMin, yMin, xMax, yMax, m1, Xj, Yi)){
        cursorVisibility(false, 25);
        printMat(m1, xMin, yMin, a,b);
        cursorVisibility(true, 100);
    }
    cursorVisibility(false, 25);
    //è necessario mettere cursorVisibility falsa dopo setMaximized, altrimenti cursorVisibility si resetta
    printMat(m1, xMin, yMin, a,b);
    b=0;
    while(true){
        for(int i=1; i<n-1; i++){
            for(int j=1; j<n-1; j++){
                nextState(m1, m2, i, j, nearCount);
                setFalseBorder(m1);
            }
        }
        copyMat(m1, m2);
        //Sleep(200);
        //goToConsole(0,0);
        printMat(m1, xMin, yMin, a, b);
        goToConsole(xMax*2+10, yMin+9);
        cout<<"Numero evoluzioni: "<<b;
        goToConsole(xMax*2+29, yMin+10);
        cout<<"    ";
        goToConsole(xMax*2+10, yMin+10);
        cout<<"Numero celle vive: "<<a;
    }
    
    return 0;
}
