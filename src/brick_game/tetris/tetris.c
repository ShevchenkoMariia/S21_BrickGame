#include "tetris.h"

bool isRunning = false;
bool exitGame = false;
bool gameOver = false;
figure now, next;
matrix_t field;
GameInfo_t allInfo;

int startSpeed = 1500000;

void userInput(UserAction_t action, bool hold) {
  if ((!hold && !allInfo.pause) || (allInfo.pause && action == Pause) ||
      (allInfo.pause && action == Terminate)) {
    switch (action) {
      //начало игры
      case Start:
        if (!isRunning) initializingGame();
        break;
      //пауза
      case Pause:
        if (!allInfo.pause)
          allInfo.pause = 1;
        else if (allInfo.pause)
          allInfo.pause = 0;
        break;
      //выход
      case Terminate:
        exitGame = true;
        if (isRunning) removeGame();
        break;
      //падение
      case Down:
        movingDown();
        break;
      //влево
      case Left:
        movingLeft();
        break;
      //вправо
      case Right:
        movingRight();
        break;
      //вращение
      case Up:
        clockwiseRotation();
        break;
    }
  }
}

int clockwiseRotation() {
  matrix_t m;
  int error = creatingMatrix(&m, now.figure.columns, now.figure.rows);
  if (error)
    fprintf(stderr,
            "Memory allocation error in the function clockwiseRotation()\n");
  else {
    if (m.columns == 1)
      for (int i = 0; i < m.rows; i++) m.matrix[i][0] = now.figure.matrix[0][i];
    else
      for (int i = 0; i < m.rows; i++)
        for (int j = 0, x = m.columns - 1; j < m.columns; j++, x--)
          m.matrix[i][j] = now.figure.matrix[x][i];
    if (m.columns == 1)
      error = checkingCollision(now.y, now.x + 1, &m);
    else if (m.rows == 1)
      error = checkingCollision(now.y, now.x - 1, &m);
    else
      error = checkingCollision(now.y, now.x, &m);
    if (error == ok) {
      clearing(&field, &now.figure, now.y, now.x);
      error = copyMatrix(&m, &now.figure);
      if (error != ok)
        fprintf(stderr, "Сopying error in the function clockwiseRotation()\n");
      else {
        if (now.figure.columns == 1) {
          now.x++;
        }
        if (now.figure.rows == 1) {
          now.x--;
        }
        error = drawingFigureOnField(&field, &now.figure, now.y, now.x);
      }
    }
    removeMatrix(&m);
  }
  return error;
}

void removeGame() {
  removeMatrix(&now.figure);
  removeMatrix(&next.figure);
  removeMatrix(&field);
}

void movingLeft() {
  if (now.x - 1 >= 0 &&
      checkingCollision(now.y, now.x - 1, &now.figure) == ok) {
    clearing(&field, &now.figure, now.y, now.x);
    drawingFigureOnField(&field, &now.figure, now.y, --now.x);
  }
}

void movingRight() {
  if (now.x + 1 < WIDTH &&
      checkingCollision(now.y, now.x + 1, &now.figure) == ok) {
    clearing(&field, &now.figure, now.y, now.x);
    drawingFigureOnField(&field, &now.figure, now.y, ++now.x);
  }
}

void movingDown() {
  int error = checkingCollision(now.y + 1, now.x, &now.figure);
  if (!error) {
    clearing(&field, &now.figure, now.y, now.x);
    drawingFigureOnField(&field, &now.figure, ++now.y, now.x);
  } else if (error == collision) {
    int line = deletingLine(&field);
    scoringPoints(&allInfo, line);
    error = replacingFigure();
    if (!error) {
      creatingFigure(&next, generatingRandomNumber(1, 7));
      error = drawingFigureOnField(&field, &now.figure, now.y, now.x);
      // gameover
      if (error) {
        removeGame();
        isRunning = false;
        gameOver = true;
      }
    }
  }
}

int replacingFigure() {
  int result = ok;
  if (isRunning) {
    if (copyMatrix(&next.figure, &now.figure)) {
      fprintf(stderr, "Copy error in the function replacingFigure()\n");
      result = fail;
    } else {
      now.color = next.color;
      now.y = next.y;
      now.x = next.x;
      result = removeMatrix(&next.figure);
    }
  } else
    result = fail;
  return result;
}

int checkingCollision(int y, int x, matrix_t* f) {
  int error = ok;
  matrix_t helper;
  //проверка
  if ((error = creatingMatrix(&helper, field.rows, field.columns)) == ok) {
    error = copyMatrix(&field, &helper);
    clearing(&helper, &now.figure, now.y, now.x);
    if (!error) error = drawingFigureOnField(&helper, f, y, x);
    removeMatrix(&helper);
  }
  return error;
}

void clearing(matrix_t* field, matrix_t* figure, int y, int x) {
  if (field != NULL && figure != NULL && y < field->rows &&
      x < field->columns) {
    for (int i = 0; i < figure->rows; i++)
      for (int j = 0; j < figure->columns; j++)
        if (y + i < field->rows && x + j < field->columns &&
            figure->matrix[i][j] != 0)
          field->matrix[y + i][x + j] = 0;
  }
}

int copyMatrix(matrix_t* a, matrix_t* b) {
  int result = ok;
  if (a == NULL || b == NULL || a->matrix == NULL || b->matrix == NULL) {
    fprintf(stderr, "Invalid arguments in the function copyMatrix()\n");
    result = fail;
  } else {
    result = removeMatrix(b);
    if (!result) result = creatingMatrix(b, a->rows, a->columns);
    if (result) {
      fprintf(stderr, "Memory allocation error in the function copyMatrix()\n");
    } else {
      for (int i = 0; i < a->rows; i++) {
        for (int j = 0; j < a->columns; j++) {
          b->matrix[i][j] = a->matrix[i][j];
        }
      }
    }
  }
  return result;
}

int endOfTime(int* time) {
  if (clock() - *time > allInfo.speed && !allInfo.pause && !exitGame) {
    movingDown();
    *time = clock();
    return fail;
  }
  return ok;
}

GameInfo_t updateCurrentState() {
  if (!exitGame) {
    allInfo.field = field.matrix;
    allInfo.next = next.figure.matrix;
    raisingLevel(&allInfo);
    settingSpeed(&allInfo);
  }
  return allInfo;
}

void settingSpeed(GameInfo_t* allInfo) {
  allInfo->speed = startSpeed - allInfo->level * 100000;
}

void raisingLevel(GameInfo_t* allInfo) {
  if (allInfo->score <= 600)
    allInfo->level = 1;
  else if (allInfo->score > 600 && allInfo->score <= 1200)
    allInfo->level = 2;
  else if (allInfo->score > 1200 && allInfo->score <= 1800)
    allInfo->level = 3;
  else if (allInfo->score > 1800 && allInfo->score <= 2400)
    allInfo->level = 4;
  else if (allInfo->score > 2400 && allInfo->score <= 3000)
    allInfo->level = 5;
  else if (allInfo->score > 3000 && allInfo->score <= 3600)
    allInfo->level = 6;
  else if (allInfo->score > 3600 && allInfo->score <= 4200)
    allInfo->level = 7;
  else if (allInfo->score > 4200 && allInfo->score <= 4800)
    allInfo->level = 8;
  else if (allInfo->score > 4800 && allInfo->score <= 5400)
    allInfo->level = 9;
  else if (allInfo->score > 5400 && allInfo->score <= 6000)
    allInfo->level = 10;
}

void scoringPoints(GameInfo_t* allInfo, int line) {
  switch (line) {
    case 1:
      allInfo->score += 100;
      break;
    case 2:
      allInfo->score += 300;
      break;
    case 3:
      allInfo->score += 700;
      break;
    case 4:
      allInfo->score += 1500;
  }
}

int deletingLine(matrix_t* field) {
  int line = 0;
  for (int i = field->rows - 2; i > 0; i--) {
    int count = 0;
    for (int j = 1; j < field->columns - 1; j++) {
      if (field->matrix[i][j] > 0)
        count++;
      else
        break;
    }
    if (count == field->columns - 2) {
      offsetPerLine(field, i);
      line++;
      i++;
    }
  }
  return line;
}

void offsetPerLine(matrix_t* field, int i) {
  for (int a = i; a > 1; a--) {
    for (int b = 1; b < field->columns - 1; b++) {
      field->matrix[a][b] = field->matrix[a - 1][b];
    }
  }
}

//создаем все необходимое для начала игры
int initializingGame() {
  int result = ok;
  result = creatingMatrix(&field, HEIGHT, WIDTH);
  if (!result) result = bordersMatrix(&field);
  if (!result) result = creatingFigure(&now, generatingRandomNumber(1, 7));
  int n = generatingRandomNumber(1, 7);
  if (!result) result = creatingFigure(&next, n == 7 ? --n : ++n);
  if (!result) result = drawingFigureOnField(&field, &now.figure, now.y, now.x);
  if (result == ok) isRunning = true;
  return result;
}

int drawingFigureOnField(matrix_t* field, matrix_t* figure, int y, int x) {
  if (field == NULL || figure == NULL || field->matrix == NULL ||
      figure->matrix == NULL || y < 0 || x < 0 || field->rows < figure->rows ||
      field->columns < figure->columns || y >= field->rows ||
      x >= field->columns) {
    fprintf(stderr,
            "Invalid arguments in the function drawingFigureOnField()\n");
    return fail;
  }
  for (int i = 0; i < figure->rows; i++) {
    for (int j = 0; j < figure->columns; j++) {
      if (figure->matrix[i][j] != 0) {
        if (y + i > field->rows - 1 || x + j > field->columns - 1 ||
            field->matrix[y + i][x + j] != 0) {
          return collision;
        }
        field->matrix[y + i][x + j] = figure->matrix[i][j];
      }
    }
  }
  return ok;
}

int creatingFigure(figure* newFigure, int num) {
  if (num <= 0 || num >= 8 || newFigure == NULL) {
    fprintf(stderr, "Invalid arguments in the function creatingFigure()\n");
    return fail;
  }
  int rows = 0, cols = 0;
  switch (num) {
    case 1:
      rows = 1, cols = 4;
      break;
    case 4:
      rows = 2, cols = 2;
      break;
    case 2:
    case 3:
    case 5:
    case 6:
    case 7:
      rows = 3, cols = 3;
  }

  if (creatingMatrix(&newFigure->figure, rows, cols) == ok) {
    newFigure->color = num;
    newFigure->y = 1;
    newFigure->x = (WIDTH - newFigure->figure.columns) / 2;
    drawingShape(newFigure, num);
  } else {
    fprintf(stderr,
            "Memory allocation error in the function creatingFigure()\n");
    return fail;
  }
  return ok;
}

void drawingShape(figure* f, int n) {
  for (int i = 0; i < f->figure.rows; i++) {
    for (int j = 0; j < f->figure.columns; j++) {
      switch (n) {
        case 1:
        case 4:
          f->figure.matrix[i][j] = n;
          break;
        case 2:
          if ((i == 0 && j == 0) || i == 1) f->figure.matrix[i][j] = n;
          break;
        case 3:
          if ((i == 0 && j == f->figure.columns - 1) || i == 1)
            f->figure.matrix[i][j] = n;
          break;
        case 5:
          if ((i == 0 && j > 0) || (i == 1 && j < f->figure.columns - 1))
            f->figure.matrix[i][j] = n;
          break;
        case 6:
          if ((i == 0 && j == 1) || i == 1) f->figure.matrix[i][j] = n;
          break;
        case 7:
          if ((i == 0 && j < f->figure.columns - 1) || (i == 1 && j > 0))
            f->figure.matrix[i][j] = n;
      }
    }
  }
}

int generatingRandomNumber(int from, int to) {
  srand(time(NULL));  //инициализируем генератор случайных чисел случайным
                      //числом в зависимости от текущего времени
  return rand() % to + from;  //генерируем случайное
}

int creatingMatrix(matrix_t* matrix, int rows, int columns) {
  int result = ok;
  if (matrix == NULL || rows <= 0 || columns <= 0) {
    result = fail;
    fprintf(stderr, "Invalid arguments in the function creatingMatrix()\n");
  } else {
    matrix->matrix = (int**)calloc(rows, sizeof(int*));
    if (matrix->matrix == NULL) {
      result = fail;
      fprintf(
          stderr,
          "Error when allocating memory in the function creatingMatrix()\n");
    } else {
      for (int i = 0; i < rows; i++) {
        matrix->matrix[i] = (int*)calloc(columns, sizeof(int));
        if (matrix->matrix[i] == NULL) {
          result = fail;
          fprintf(stderr,
                  "Error when allocating memory in the function "
                  "creatingMatrix()\n");
          for (int j = 0; j < i; j++) {
            free(matrix->matrix[i]);
          }
          free(matrix->matrix);
          matrix->rows = 0;
          matrix->columns = 0;
          break;
        }
      }
    }
    matrix->rows = rows;
    matrix->columns = columns;
  }
  return result;
}

int bordersMatrix(matrix_t* matrix) {
  if (matrix == NULL || matrix->matrix == NULL || matrix->rows <= 0 ||
      matrix->columns <= 0) {
    fprintf(stderr, "Invalid arguments in the function bordersMatrix()\n");
    return fail;
  }
  for (int i = 0; i < matrix->rows; i++) {
    for (int j = 0; j < matrix->columns; j++) {
      if (i == 0 || j == 0 || i == matrix->rows - 1 ||
          j == matrix->columns - 1) {
        matrix->matrix[i][j] = 8;
      }
    }
  }
  return ok;
}

int removeMatrix(matrix_t* matrix) {
  if (matrix == NULL || matrix->matrix == NULL || matrix->rows <= 0 ||
      matrix->columns <= 0) {
    fprintf(stderr, "Invalid arguments in the function removeMatrix()\n");
    return fail;
  }
  for (int i = 0; i < matrix->rows; i++) {
    free(matrix->matrix[i]);
  }
  free(matrix->matrix);
  matrix->rows = 0;
  matrix->columns = 0;
  return ok;
}
