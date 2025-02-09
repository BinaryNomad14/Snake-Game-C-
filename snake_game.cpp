#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#include<conio.h>
#include<windows.h>

#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif
using namespace std;

const int GRID_SIZE = 50;  
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define WHITE   "\033[97m"
#define BG_YELLOW "\033[43m"
#define BG_GREEN  "\033[42m"
#define BG_BLACK  "\033[40m"
#define BG_BLUE    "\033[94m"
#define BG_RED    "\033[41m"

#ifdef _WIN32
bool kbhit(){
    return _kbhit();
}

bool getKey(){
    return _getch();
}

void sleep_ms(int milliseconds){
    Sleep(milliseconds);
}

void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

#else
bool kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return true;
    }

    return false;
}

int getkey() {
    int key;
    key = getchar();
    return key;
}

void sleep_ms(int milliseconds){
    usleep(milliseconds*1000);
}

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define WHITE   "\033[97m"
#define BG_YELLOW "\033[43m"
#define BG_GREEN  "\033[42m"
#define BG_BLACK  "\033[40m"
#define BG_BLUE    "\033[94m"
#define BG_RED    "\033[41m"
#endif

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    cout << "\033[H\033[J";
    #endif
}

struct Node {
    int x, y;
    Node* next;

    Node(int x, int y) : x(x), y(y), next(nullptr) {}
};

class Snake {
public:
    Node* head;
    Node* tail;
    char direction;
    int snake_size = 3;
    int print;
    
    Snake() {
        head = new Node(GRID_SIZE / 2, GRID_SIZE / 2);
        tail = head;
        addNode(head->x, head->y - 1);
        addNode(head->x, head->y - 2);
        direction = 'R';
    }

    ~Snake() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void addNode(int x, int y) {
        Node* newNode = new Node(x, y);
        tail->next = newNode;
        tail = newNode;
    }

    void removeTail() {
        if (head == tail) return;
        Node* temp = head;
        while (temp->next != tail) {
            temp = temp->next;
        }
        delete tail;
        tail = temp;
        tail->next = nullptr;
    }
    void movement(bool size_increase) {
        int new_headX = head->x;
        int new_headY = head->y;
        
        if (direction == 'U') new_headX--;
        else if (direction == 'D') new_headX++;
        else if (direction == 'R') new_headY++;
        else if (direction == 'L') new_headY--;

        
        Node* newHead = new Node(new_headX, new_headY);
        newHead->next = head;
        head = newHead;
        snake_size++;

        if (!size_increase) {
            removeTail();
            snake_size--;
        }
    }

    bool checkForCollision() {
       if (head->x < 0 || head->x >= GRID_SIZE || head->y < 0 || head->y >= GRID_SIZE) {
            print = 1;
            return true;
        }

        Node* current = head->next;
        while (current) {
            if (current->x == head->x && current->y == head->y) {
                print = 2;
                return true;
            }
            current = current->next;
        }
        return false;
    }
};

class Game {
public:
    Snake *snake;
    pair<int, int> food;
    pair<int, int> special_food;
    int score;
    int high_score;
    char difficulty;
    int base_speed;
    bool special_food_active;
    int special_food_timer;
    int updateCount;
    bool game_over;

    Game() {
        snake =  new Snake();
        score = 0;
        high_score = 0;
        game_over = false;
        special_food_active = false;
        special_food_timer = 0;
        updateCount = 0;
        generateFood();
    }

    ~Game() {
        delete snake;
    }

    void generateFood(){
        srand(time(0));
        bool valid_pos = false;
        while (!valid_pos){
            food.first = rand() % GRID_SIZE;
            food.second = rand() % GRID_SIZE;
            valid_pos = true;
            for (Node* part = snake->head; part != NULL; part = part -> next) {
                if (part->x == food.first && part->y == food.second) {
                    valid_pos = false;
                    break;
                }
            }
        }
    }

    void generateSpecialFood(){
        srand(time(0) + rand());
        bool valid_pos = false;
        while(!valid_pos){
            special_food.first = rand() % GRID_SIZE;
            special_food.second = rand() % GRID_SIZE;
            valid_pos = true;
            for(Node* part = snake->head; part != NULL; part = part -> next){
                if((part->x == special_food.first && part->y == special_food.second) || special_food == food){
                    valid_pos = false;
                    break;
                }
            }
        }
        special_food_active = true;
        special_food_timer = 50;
    }

    void drawCell(int i, int j, bool isSnake, bool isFood, bool isSpecialFood) {
    #ifdef _WIN32
        if (isSnake) {
            setConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            cout << "O";
        } else if (isFood) {
            setConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
            cout << "@";
        } else {
            setConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            cout << " ";
        }
        setConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    #else
    if(isSnake) {
        cout << BG_GREEN << RED << "O" << RESET;
    } else if(isFood) {
        cout << BG_RED << WHITE << "@" << RESET;
    } else if(isSpecialFood){
        cout << BG_BLUE << WHITE << "$" << RESET;
    } else {
        cout << BG_GREEN << " " << RESET;
    }
    #endif
}

void board() {

    cout << "\033[H"; 

    for (int i = 0; i <= GRID_SIZE + 1; i++) {
        for (int j = 0; j <= GRID_SIZE + 1; j++) {
            if (i == 0 || i == GRID_SIZE + 1) {
                cout << "-";
            } else if (j == 0 || j == GRID_SIZE + 1) {
                cout << "|";
            } else {
                bool isSnake = false;
                bool isFood = (food.first == i - 1 && food.second == j - 1);
                bool isSpecialFood = (special_food_active && special_food.first == i-1 && special_food.second == j-1);
                for (Node* part = snake->head; part != NULL; part = part->next) {
                    if (part->x == i - 1 && part->y == j - 1) {
                        isSnake = true;
                        break;
                    }
                }
                drawCell(i, j, isSnake, isFood, isSpecialFood);
            }
        }
        cout << endl;
    }
    cout << " Current Score: " << score << "  High Score: " << high_score << endl;

}


    void keyboardInput() {
        if (kbhit()) {
            char key = getkey();

            if ((key == 'w' || key == 'W') && snake->direction != 'D') snake->direction = 'U';
            else if ((key == 's' || key == 'S') && snake->direction != 'U') snake->direction = 'D';
            else if ((key == 'd' || key == 'D') && snake->direction != 'L') snake->direction = 'R';
            else if ((key == 'a' || key == 'A') && snake->direction != 'R') snake->direction = 'L';
        }
    }

    void updateGame() {
        keyboardInput();

        bool size_increase = (snake->head->x == food.first && snake->head->y == food.second);
        if (size_increase) {
            score += 10;
            high_score = max(score, high_score);
            generateFood();
        }
        
        if(special_food_active && (snake->head->x == special_food.first && snake->head->y == special_food.second)){
            score += 50;
            high_score = max(score, high_score);
            special_food_active = false;
        }
        snake->movement(size_increase);
        board();

        if (snake->checkForCollision()) {
            if (gameOver()) {
                restartGame();
            } else {
                game_over = true;
            }
        }

        if(special_food_active){
            special_food_timer--;
            if(special_food_timer <= 0){
                special_food_active = false;
            }
        }

        updateCount++;
        if(!special_food_active && updateCount%200 == 0){
            generateSpecialFood();
        }
        
        if(difficulty == 'E') base_speed = 100;
        else if(difficulty == 'M') base_speed = 80;
        else if(difficulty == 'H') base_speed = 65;

        int speed = base_speed;

        if(snake->snake_size >=7 ){
            speed = base_speed/1.5;
        }
        if(snake->snake_size >=11){
            speed = base_speed/2.25;
        }
        sleep_ms(speed); 
    }

    bool gameOver() {
        clearScreen();
        if(snake->print == 1) cout << "Uh Oh! The snake collided with the grid!" << endl << endl;
        else if(snake->print == 2) cout << "Uh Oh! The snake collided with itself!" << endl << endl;
        cout << " * * * * * * * * * * * * * * * * " << endl << endl;
        cout << "     - - - GAME OVER - - -" << endl << endl;
        cout << " * * * * * * * * * * * * * * * * " << endl << endl;
        cout << "Final Score: " << score << "    High Score: " << high_score << endl << endl;
        cout << "Press Q to quit the game or R to restart the game..." << endl;

        while (true) {
            char choice = getkey();

            if (choice == 'r' || choice == 'R') {
                return true;
            } else if (choice == 'Q' || choice == 'q') {
                return false;
            }
        }
    }

    void restartGame() {
        delete snake;
        snake = new Snake();
        score = 0;
        special_food_active = false;
        special_food_timer = 0;
        updateCount = 0;
        game_over = false;
        generateFood();
    }
};

void startScreen(Game &game) {
    cout << " - - - - Welcome to Snake Game - - - - " << endl << endl;
    cout << "The controls are: W = Up, S = Down, D = Right, A = Left" << endl;
    cout << "If the snake collides with the grid or itself, the game is over." << endl << endl;
    cout << "When the snake eats the food you get 10 points and the snake grows in size!" << endl;
    cout << "At certain amount of time a special food spawns for a limited time which gets you 50 points!" << endl << endl;
    cout << "There are three modes to play: 1. Easy 2. Medium 3. Hard" << endl;
    cout << "In each level the snake moves faster than the previous level... " << endl;
    cout << "Enter 1 for Easy, 2 for Medium, and 3 for Hard... " << endl;

    char key = getkey();
    if(key == '1') game.difficulty = 'E';
    else if(key == '2') game.difficulty = 'M';
    else if(key == '3') game.difficulty = 'H';

    clearScreen();
}

int main() {
    Game game;
    startScreen(game);

    while (true) {
        while (!game.game_over) {
            game.updateGame();
        }

        if (!game.gameOver()) {
            break;
        }

        game.restartGame();
    }

    return 0;
}