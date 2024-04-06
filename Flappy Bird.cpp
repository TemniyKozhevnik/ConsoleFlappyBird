#include <iostream>
#include <time.h>
#include <cstdlib>
#include <conio.h>
#include <windows.h>
#include <limits>
#include <fstream>
using namespace std;
#ifdef max
#undef max
#endif

bool gameover = false;
enum Direction { stop, up };

ifstream fin;
ofstream fout;
int mas_of_maxresult[3];

int maxresult = 0;

Direction dir;

class Bird {
public:
    int jump_height;
    int x;
    int y;
    Bird(int c, int a, int b) {
        jump_height = c;
        x = a;
        y = b;
    }
};

class Field {
public:
    int width;
    int height;
    Field(int a, int b) {
        width = a;
        height = b;
    }
};

class Wall {
public:
    int height;
    int x;
    int y[5] = { 0,0,0,0,0 };

};

bool check() {
    char nextchar;
    if (cin.fail() || cin.get(nextchar) && nextchar != '\n') {
        cout << "Error of input\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }
    return true;
}

bool checkch(char ch, bool F1) {
    if (F1) {
        if (ch != '1' && ch != '2' && ch != '3') {
            cout << "Incorrect symbol\n";
            return false;
        }
    }
    return true;
}

void Draw(Field field, Bird& bird, Wall wallup[3], Wall walldown[3], int result, int mode) {
    system("cls");
    for (int y = 0; y <= field.height + 1; y++) {
        for (int x = 0; x <= field.width; x++) {
            if (y == 0 || y == field.height + 1)
                cout << "#";
            else if (x == 0)
                cout << "#";
            else if (x == bird.x && y == bird.y) {
                cout << "a";
            }
            else {
                bool print1[3], print2[3];
                for (int i = 0; i < 3; i++) {
                    print1[i] = 0; print2[i] = 0;
                    if (x == wallup[i].x && y > 0 && y <= wallup[i].height) {
                        cout << "#";
                        print1[i] = 1;
                    }
                    if (x == walldown[i].x && y >= field.height - walldown[i].height + 1 && y <= field.height) {
                        cout << "#";
                        print2[i] = 1;
                    }
                }
                if (!print1[0] && !print1[1] && !print1[2] && !print2[0] && !print2[1] && !print2[2])
                    cout << " ";
            }
            if (x == field.width) {
                cout << "#\n";
            }
        }
    }
    cout << "Result: " << result<<'\n';
    cout << "MaxResult: " << maxresult;
}

char RusToEng(char ch) {
    if (int(toupper(ch)) == -26) {
        ch = 'W';
    }
    return ch;
}

void Input() {
    char ch;
    if (_kbhit()) {
        ch = _getch();
        ch = RusToEng(ch);
        switch (ch) {
        case 'W':
        case 'w':
            dir = up;
            break;
        }
    }
}

int random(int a, int b) {
    return(a + rand() % (b - a + 1));
}

void Logic(Field field, Bird& bird, Wall wallup[3], Wall walldown[3], int& result, int mode, int size1, int size2) {
    if (bird.y <= 0 || bird.y >= field.height + 1)
        gameover = 1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < wallup[i].height; j++)
            if (bird.x == wallup[i].x && bird.y == wallup[i].y[j])
                gameover = 1;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < walldown[i].height; j++)
            if (bird.x == walldown[i].x && bird.y == walldown[i].y[j])
                gameover = 1;
    switch (dir) {
    case up:
        bird.y -= bird.jump_height;
        dir = stop;
        break;
    default:
        bird.y++;
    }

    for (int i = 0; i < 3; i++) {
        wallup[i].x--;
        walldown[i].x--;
    }
    for (int i = 0; i < 3; i++) {
        if (wallup[i].x == 0) {
            wallup[i].x = 30;
            wallup[i].height = random(size1, size2);
            for (int j = 1; j <= wallup[i].height; j++)
                wallup[i].y[j - 1] = 0 + j;
        }
        if (walldown[i].x == 0) {
            walldown[i].x = 30;
            walldown[i].height = random(size1, size2);
            for (int j = 1; j <= walldown[j].height; j++)
                walldown[i].y[j - 1] = 9 + 2 - j;
        }
    }
    for (int i = 0; i < 3; i++)
        if (bird.x - wallup[i].x == 1 && !gameover)
            result += 1;
    if (result > maxresult)
        maxresult = result;
}

void Game(int mode, int sleep, int jump, int width, int height, int size1, int size2) {
    int result=0;

    fin.open("score.txt");
    if (fin)
    {
        for (int i = 0; i < 3; i++)
            fin >> mas_of_maxresult[i];
        fin.close();
        maxresult = mas_of_maxresult[mode];
    }
    else
    {
        fout.open("score.txt");
        if (fout)
        {
            for (int i = 0; i < 3; i++)
            {
                fout << 0 << '\n';
                mas_of_maxresult[i] = 0;
            }
            fout.close();
        }
    }
  
    srand(time(NULL));
    Field field(width, height);
    Bird bird(jump, width / 3 + 1, height / 2 + 1);
    Wall wallup[3];
    Wall walldown[3];
    for (int i = 0; i < 3; i++) {
        wallup[i].height = random(size1, size2);
        wallup[i].x = (width - 3) + i * 10;
    }
    for (int j = 0; j < 3; j++) {
        walldown[j].height = random(size1, size2);
        walldown[j].x = (width - 3) + j * 10;
    }
    for (int i = 0; i < 3; i++)
        for (int j = 1; j <= wallup[i].height; j++)
            wallup[i].y[j - 1] = 0 + j;
    for (int i = 0; i < 3; i++)
        for (int j = 1; j <= walldown[j].height; j++)
            walldown[i].y[j - 1] = height + 2 - j;


    while (gameover == 0) {
        Sleep(sleep);
        Draw(field, bird, wallup, walldown, result, mode);
        Input();
        Logic(field, bird, wallup, walldown, result, mode, size1, size2);
    }
    if (gameover) 
    {
        mas_of_maxresult[mode] = maxresult;
        fout.open("score.txt");
        if (fout)
        {
            for (int i = 0; i < 3; i++)
                fout << mas_of_maxresult[i] << '\n';
            fout.close();
        }
    }

}

int main() {
    SetConsoleOutputCP(1251);
    char ch;
    bool F1, F2;

    cout << "Choose difficulty:\n";
    cout << "1 - Easy \n2 - Medium \n3 - Hard\n";
    do {
        cout << "Write symbol: ";
        cin >> ch;
        F1 = check();
        F2 = checkch(ch, F1);
    } while (F1 == 0 || F2 == 0);

    if (ch == '1')
        Game(0, 200, 3, 21, 9, 1, 2);
    if (ch == '2')
        Game(1, 150, 3, 21, 9, 2, 3);
    if (ch == '3')
        Game(2, 100, 3, 21, 9, 2, 4);
}
