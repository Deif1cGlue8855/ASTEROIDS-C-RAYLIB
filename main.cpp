#include <raylib.h>
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string>
#include <list>
#include <random>
#define numShoot 50
#define maxAst 48
//12 MAX BIG ASTEROIDS

using namespace std;

double radians(double digrees){
    return digrees * (3.14159265358979323846 / 180.0);
}
struct Ship{
    Vector2 position = {700 / 2, 700/ 2};
    Vector2 velocity = {0,0};
    double angle = 270;
    float maxVelocity = 2;
};

struct Bullet{
    Vector2 position = {0,0};
    double angle = 0;
    bool active = false;
    
};

struct Asteroid{
    Vector2 position = {0,0};
    int angle = 0;
    bool active = false;
    int size = 0;
    int speed = 0;
    int rotation = 0;
};

int square(int num){
    return num * num;
}

void shipControl(Ship &ship);
void respawn(Ship &ship);

static Bullet bullet[numShoot];
static int ShootRate = 0;

static Asteroid asteroid [50];

int rnd = 0;
int score = 0;
int legitAst = 0;
int size [3] = {12, 25, 50};
int corners[3] = {3,5,5};
int lives = 3;
int wait = 1;

int gamephase = 1;

void newRound(){
    rnd += 1;
    int RNG = rand() % 4 + 1;
    legitAst = RNG;
    for(int i = 0; i < RNG; i++){
        asteroid[i].active = true;

        for(int j = 0; j < 2; j++){
            int NUM = rand() % 2 + 1;
            if(j == 0){
                if(NUM == 1){
                    asteroid[i].position.x = 0;
                    int num = rand()% 290 + 430;
                    if(num > 360){num -= 360;}
                    asteroid[i].angle = num;
                }
                else{
                    asteroid[i].position.x = 700;
                    asteroid[i].angle = rand()% 200 + 340;
                }
            }
            if(j == 1){
                int NUm = rand() % 900 + 0;
                asteroid[i].position.y = NUm;
            }
        }
        asteroid[i].size = rand()%3+1;
        asteroid[i].speed = rand()% 4 + 2;
        asteroid[i].rotation = rand()% 360 + 1;
    }

    cout << "RNG IS: " << RNG << endl;

}


int main()
{
    Ship ship;
    const int screenWidth = 700;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "Asteroids");
    SetTargetFPS(60);

    //INTRO TITLE SCREEN
    while(gamephase == 1){
        DrawText("ASTEROIDS", 55, 100, 100, WHITE);

        if(wait > 0){
            DrawText("PRESS SPACE TO START", 220, 500, 20, WHITE);
            wait++;
            if(wait == 60){
                wait = -1;
            }
        }
        else if(wait < 0){
            wait--;
            if(wait == -60){
                wait = 1;
            }
        }
        if(IsKeyPressed(KEY_SPACE)){
            gamephase = 2;
        }


        BeginDrawing();
        ClearBackground(BLACK);
        EndDrawing();
    }

    newRound();

    wait = 0;

    while (!WindowShouldClose())
    {
        //std::string ean = std::to_string(static_cast<int>(playerXVelocity));DrawText(ean.c_str(), 1, 60, 10,PINK);std::string an = std::to_string(static_cast<int>(playerYVelocity));DrawText(an.c_str(), 1, 70, 10,GREEN);*/
        BeginDrawing();
        ClearBackground(BLACK);

        //DRAW ASTEROIDS
        for(int i = 0; i < maxAst; i++){
            if(asteroid[i].active == true){
                asteroid[i].position.x += cos(asteroid[i].angle) * asteroid[i].speed;
                asteroid[i].position.y += sin(asteroid[i].angle) * asteroid[i].speed;
                asteroid[i].rotation += 1;

                if(asteroid[i].position.x > 710){
                    asteroid[i].position.x = -5;
                }
                else if(asteroid[i].position.x < -10){
                    asteroid[i].position.x = 705;
                }
                if(asteroid[i].position.y > 710){
                    asteroid[i].position.y = -5;
                }
                else if(asteroid[i].position.y < -10){
                    asteroid[i].position.y = 705;
                }

                DrawPolyLinesEx({asteroid[i].position.x,asteroid[i].position.y}, corners[asteroid[i].size], size[asteroid[i].size], asteroid[i].rotation,2.0, WHITE);
                legitAst += 1;

            }
        }

        
        //Measure for how long player is dead for
        if(wait > 0){
            DrawText("ROUND: ", 280, 60, 30,WHITE);
            std::string ean = std::to_string(static_cast<int>(rnd));
            DrawText(ean.c_str(), 390 + ((rnd % 10) * 10), 60, 30,WHITE);
            ship.position.x = -100000;
            ship.position.y = -100000;

            wait += 1;
            if(wait == 240){
                respawn(ship);
                wait = 0;
            }
        }
        else{
            if(lives != 0){
                shipControl(ship);

                //fire PEW PEW
                if(IsKeyDown(KEY_SPACE)){
                ShootRate += 5;
                    for(int i = 0; i < numShoot; i++){
                        if(!bullet[i].active && ShootRate % 40 == 0){
                            bullet[i].position.x = ship.position.x + (cos(ship.angle) * 20);
                            bullet[i].position.y = ship.position.y + (sin(ship.angle) * 20);
                            bullet[i].active = true;
                            bullet[i].angle = ship.angle;
                            break;
                        }
                    }  
                }
            }
        
            else{gamephase = 3;}
        }
        //MOVE BULLET AND CHECK COLISSION WITH ASTEROID
        for(int i = 0; i < numShoot; i++){
            if(bullet[i].active){
                bullet[i].position.x += cos(bullet[i].angle) * 5;
                bullet[i].position.y += sin(bullet[i].angle) * 5;

                for(int j = 0; j < maxAst; j++){
                    if(asteroid[j].active == true){
                        int dist = abs(sqrt(square(abs(bullet[i].position.x - asteroid[j].position.x)) + square(abs(bullet[i].position.y - asteroid[j].position.y))));
                        if(dist < size[asteroid[j].size]){
                            bullet[i].active = false;
                            score += 100;
                            if(asteroid[j].size - 1 == -1){
                                asteroid[j].active = false;
                            }
                            else{
                                asteroid[legitAst] = asteroid[j];

                                asteroid[j].size -= 1;
                                asteroid[j].angle  = rand()% 360 + -360;
                                asteroid[j].speed += rand()% 1 + -1;
                                if(asteroid[j].speed == 0){
                                    asteroid[j].speed = 1;
                                }
                                asteroid[legitAst].size -= 1;
                                asteroid[legitAst].angle  = rand()% 360 + -360;
                                asteroid[legitAst].speed += rand()% 1 + -1;
                                if(asteroid[legitAst].speed == 0){
                                    asteroid[legitAst].speed = 1;
                                }

                                legitAst += 1;
                            }
                        }
                    }
                }
        

                DrawCircle(bullet[i].position.x, bullet[i].position.y, 2, WHITE);

                if(bullet[i].position.x > 700 || bullet[i].position.y > 700 || bullet[i].position.x < 0 || bullet[i].position.y < 0){
                    bullet[i].active = false; 
                }
            }
        }

        //DrawPolyLines({700/2,700/2}, 2, 100, 0, WHITE);
        
        //check if asteroid hits plauer
        for(int i = 0; i < legitAst; i++){
            int dist = abs(sqrt(square(abs(ship.position.x - asteroid[i].position.x)) + square(abs(ship.position.y - asteroid[i].position.y))));
            if(dist < size[asteroid[i].size]+ 15 && wait == 0){
                lives -= 1;
                wait = 1;
            }
        }

        std::string scor = std::to_string(static_cast<int>(score));DrawText(scor.c_str(), 0, 0, 30,WHITE);
        std::string live = std::to_string(static_cast<int>(lives));DrawText(live.c_str(), 670, 0, 30,WHITE);
        
        if(legitAst == 0 && wait == 0){
            wait = 1;
            newRound();
        }
        while(gamephase == 3){
            std::string scor = std::to_string(static_cast<int>(score));DrawText(scor.c_str(), 350 , 220, 40,WHITE);
            DrawText("PRESS SPACE TO START", 220, 500, 20, WHITE);

            if(IsKeyPressed(KEY_SPACE)){
            gamephase = 2;
            }
            BeginDrawing();
            ClearBackground(BLACK);
            EndDrawing();
        }

        
        EndDrawing();

        legitAst = 0;

    }

    CloseWindow();
    return 0;
}

void shipControl(Ship &ship){

    if(IsKeyDown(KEY_LEFT)){
        ship.angle -= 0.1;
    }
    else if(IsKeyDown(KEY_RIGHT)){
        ship.angle += 0.1;
    }

    if(IsKeyDown(KEY_UP)){
        ship.velocity.x += cos(ship.angle) / 10;
        ship.velocity.y += sin(ship.angle) / 10;
    }
    else if(IsKeyDown(KEY_DOWN)){
        ship.velocity.x -= cos(ship.angle) / 10;
        ship.velocity.y -= sin(ship.angle) / 10;
    }
    if(abs(ship.velocity.x) > ship.maxVelocity){
        if(ship.velocity.x > ship.maxVelocity){
            ship.velocity.x = ship.maxVelocity;
        }
        else if(ship.velocity.x < -ship.maxVelocity){
            ship.velocity.x = -ship.maxVelocity;
        }
    }
    if(abs(ship.velocity.y) > ship.maxVelocity){
        if(ship.velocity.y > ship.maxVelocity){
            ship.velocity.y = ship.maxVelocity;
        }
        else if(ship.velocity.y < -ship.maxVelocity){
            ship.velocity.y = -ship.maxVelocity;
        }
    }

    ship.position.x += ship.velocity.x;
    ship.position.y += ship.velocity.y;

    if(ship.position.x > 710){
        ship.position.x = -5;
    }
    else if(ship.position.x < -10){
        ship.position.x = 705;
    }
    if(ship.position.y > 710){
        ship.position.y = -5;
    }
    else if(ship.position.y < -10){
        ship.position.y = 705;
    }

    Vector2 point = {ship.position.x + (cos(ship.angle) * 20), ship.position.y + (sin(ship.angle) * 20)};
    Vector2 backLWing = {ship.position.x + (cos(ship.angle + 120) * -18), ship.position.y + (sin(ship.angle + 120) * -18)};
    Vector2 backRWing = {ship.position.x + (cos(ship.angle - 120) * -18), ship.position.y + (sin(ship.angle - 120) * -18)};

    DrawTriangleLines(point,backRWing,backLWing, WHITE);

    //DrawCircle(ship.position.x, ship.position.y, 15, RED);

}

void respawn(Ship &ship){
    ship.position.x = 700 /2;
    ship.position.y = 700 /2;
    ship.angle = 270;
    ship.velocity = {0,0};
}