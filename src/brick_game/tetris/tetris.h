#ifndef TETRIS_H
#define TETRIS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>  //дл работы с динамической памятью
#include <time.h>

//физические кнопки управления программой
typedef enum {
  Start = '\n',
  Pause = 'p',
  Terminate = 27,
  Left = 'a',
  Right = 'd',
  Up = 'w',
  Down = 's'
  //    Action
} UserAction_t;

//функция принимает на вход действие пользователя action и доп параметр hold,
//отвечающий за зажатие клавиши
void userInput(UserAction_t action, bool hold);

//структура, описывающая состояние поля для отрисовки...
typedef struct {
  int** field;
  int** next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
} GameInfo_t;

//...заполняется новыми данными в функции
GameInfo_t updateCurrentState();

#define HEIGHT 22
#define WIDTH 12

enum errors { ok, fail, collision };

typedef struct {
  int rows;
  int columns;
  int** matrix;
} matrix_t;

typedef struct {
  int color;
  int y;  //координата расположения на поле по y
  int x;  //координата расположения на поле по x
  matrix_t figure;  //сама фигура
} figure;

extern figure next;

int initializingGame();
int drawingFigureOnField(matrix_t* field, matrix_t* figure, int y, int x);
int creatingFigure(figure* newFigure, int num);
void drawingShape(figure* f, int n);
int generatingRandomNumber(int from, int to);
int creatingMatrix(matrix_t* matrix, int rows, int columns);
int removeMatrix(matrix_t* matrix);
int bordersMatrix(matrix_t* matrix);
void removeGame();

// GameInfo_t updateCurrentState();
void settingSpeed(GameInfo_t* allInfo);
void raisingLevel(GameInfo_t* allInfo);
void scoringPoints(GameInfo_t* field, int line);
int deletingLine(matrix_t* field);
void offsetPerLine(matrix_t* field, int i);

int endOfTime(int* time);

void movingLeft();
void movingRight();
void movingDown();
int clockwiseRotation();
int checkingCollision(int y, int x, matrix_t* f);
void clearing(matrix_t* field, matrix_t* figure, int y, int x);
int copyMatrix(matrix_t* a, matrix_t* b);
int replacingFigure();

#endif
