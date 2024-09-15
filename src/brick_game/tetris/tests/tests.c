#include <check.h>

#include "../tetris.h"

// creatingMatrix(matrix_t* matrix, int rows, int columns)
START_TEST(creatingMatrix_1) {
  int res = creatingMatrix(NULL, 2, 3);
  ck_assert_int_eq(res, fail);
}
END_TEST

START_TEST(creatingMatrix_2) {
  matrix_t A;
  int res = creatingMatrix(&A, 0, 3);
  ck_assert_int_eq(res, fail);
}
END_TEST

START_TEST(creatingMatrix_3) {
  matrix_t A;
  int res = creatingMatrix(&A, 2, -1);
  ck_assert_int_eq(res, fail);
}
END_TEST

START_TEST(creatingMatrix_4) {
  matrix_t A;
  int res = creatingMatrix(&A, 2, 3);
  ck_assert_int_eq(res, ok);
  removeMatrix(&A);
}
END_TEST

// removeMatrix(matrix_t* matrix)
START_TEST(removeMatrix_1) {
  matrix_t A = {.rows = 2, .columns = 3, .matrix = NULL};
  int res = removeMatrix(&A);
  ck_assert_int_eq(res, fail);
}
END_TEST

START_TEST(removeMatrix_2) {
  matrix_t *A = NULL;
  int res = removeMatrix(A);
  ck_assert_int_eq(res, fail);
}
END_TEST

START_TEST(removeMatrix_3) {
  matrix_t A;
  creatingMatrix(&A, 1, 3);
  A.rows = -1;
  int res = removeMatrix(&A);
  A.rows = 1;
  removeMatrix(&A);
  ck_assert_int_eq(res, fail);
}
END_TEST

START_TEST(removeMatrix_4) {
  matrix_t A;
  creatingMatrix(&A, 3, 2);
  A.columns = -2;
  int res = removeMatrix(&A);
  A.columns = 2;
  removeMatrix(&A);
  ck_assert_int_eq(res, fail);
}
END_TEST

START_TEST(removeMatrix_5) {
  matrix_t A;
  creatingMatrix(&A, 2, 3);
  int res = removeMatrix(&A);
  ck_assert_int_eq(res, ok);
}
END_TEST

// int bordersMatrix(matrix_t* matrix)
START_TEST(bordersMatrix_1) {
  int res = bordersMatrix(NULL);
  ck_assert_int_eq(res, fail);
}
END_TEST

START_TEST(bordersMatrix_2) {
  matrix_t A = {.matrix = NULL, .rows = 2, .columns = 3};
  int res = bordersMatrix(&A);
  ck_assert_int_eq(res, fail);
}
END_TEST

START_TEST(bordersMatrix_3) {
  matrix_t A;
  creatingMatrix(&A, 4, 7);
  int res = bordersMatrix(&A);
  ck_assert_int_eq(res, ok);
  removeMatrix(&A);
}
END_TEST

// int creatingFigure(figure* newFigure, int num); -> drawingShape(figure* f,
// int n)
START_TEST(creatingFigure_1) {
  int res = creatingFigure(NULL, 7);
  ck_assert_int_eq(res, fail);
}
END_TEST

START_TEST(creatingFigure_2) {
  figure A;
  int res = creatingFigure(&A, 8);
  ck_assert_int_eq(res, fail);
}
END_TEST

START_TEST(creatingFigure_3) {
  figure A;
  int res = creatingFigure(&A, 7);
  ck_assert_int_eq(res, ok);
  removeMatrix(&A.figure);
}
END_TEST

// int drawingFigureOnField(matrix_t* field, matrix_t* figure, int y, int x);
START_TEST(drawingFigureOnField_1) {
  matrix_t B;
  creatingMatrix(&B, 2, 3);
  int res = drawingFigureOnField(NULL, &B, 6, 3);
  ck_assert_int_eq(res, fail);
  removeMatrix(&B);
}
END_TEST

START_TEST(drawingFigureOnField_2) {
  matrix_t B;
  creatingMatrix(&B, 2, 3);
  int res = drawingFigureOnField(&B, NULL, 6, 3);
  ck_assert_int_eq(res, fail);
  removeMatrix(&B);
}
END_TEST

START_TEST(drawingFigureOnField_3) {
  matrix_t A, B;
  creatingMatrix(&A, 2, 3);
  creatingMatrix(&B, 2, 3);
  int res = drawingFigureOnField(&A, &B, 0, 0);
  ck_assert_int_eq(res, ok);
  removeMatrix(&A);
  removeMatrix(&B);
}
END_TEST

START_TEST(drawingFigureOnField_4) {
  matrix_t A, B;
  creatingMatrix(&A, 2, 3);
  creatingMatrix(&B, 2, 3);
  int res = drawingFigureOnField(&A, &B, -1, 0);
  ck_assert_int_eq(res, fail);
  removeMatrix(&A);
  removeMatrix(&B);
}
END_TEST

START_TEST(drawingFigureOnField_5) {
  matrix_t A, B;
  creatingMatrix(&A, 2, 2);
  creatingMatrix(&B, 2, 3);
  int res = drawingFigureOnField(&A, &B, 2, 0);
  ck_assert_int_eq(res, fail);
  removeMatrix(&A);
  removeMatrix(&B);
}
END_TEST

START_TEST(drawingFigureOnField_6) {
  matrix_t A, B;
  creatingMatrix(&A, 12, 12);
  bordersMatrix(&A);
  creatingMatrix(&B, 4, 4);
  for (int i = 0; i < B.rows; i++) {
    for (int j = 0; j < B.columns; j++) {
      B.matrix[i][j] = 1;
    }
  }
  int res = drawingFigureOnField(&A, &B, 11, 4);
  ck_assert_int_eq(res, collision);
  removeMatrix(&A);
  removeMatrix(&B);
}
END_TEST

START_TEST(drawingFigureOnField_7) {
  matrix_t A, B;
  creatingMatrix(&A, 12, 12);
  bordersMatrix(&A);
  creatingMatrix(&B, 4, 4);
  for (int i = 0; i < B.rows; i++) {
    for (int j = 0; j < B.columns; j++) {
      B.matrix[i][j] = 1;
    }
  }
  int res = drawingFigureOnField(&A, &B, 8, 4);
  ck_assert_int_eq(res, collision);
  removeMatrix(&A);
  removeMatrix(&B);
}
END_TEST

START_TEST(drawingFigureOnField_8) {
  matrix_t A, B;
  creatingMatrix(&A, 12, 12);
  bordersMatrix(&A);
  creatingMatrix(&B, 4, 4);
  for (int i = 0; i < B.rows; i++) {
    for (int j = 0; j < B.columns; j++) {
      B.matrix[i][j] = 1;
    }
  }
  int res = drawingFigureOnField(&A, &B, 6, 8);
  ck_assert_int_eq(res, collision);
  removeMatrix(&A);
  removeMatrix(&B);
}
END_TEST

START_TEST(drawingFigureOnField_9) {
  matrix_t A, B;
  creatingMatrix(&A, 12, 12);
  creatingMatrix(&B, 4, 4);
  int res = drawingFigureOnField(&A, &B, 6, 6);
  ck_assert_int_eq(res, ok);
  removeMatrix(&A);
  removeMatrix(&B);
}
END_TEST

START_TEST(drawingFigureOnField_10) {
  matrix_t A, B;
  creatingMatrix(&A, 12, 12);
  bordersMatrix(&A);
  creatingMatrix(&B, 4, 4);
  for (int i = 0; i < B.rows; i++) {
    for (int j = 0; j < B.columns; j++) {
      B.matrix[i][j] = 1;
    }
  }
  int res = drawingFigureOnField(&A, &B, 12, 4);
  ck_assert_int_eq(res, fail);
  removeMatrix(&A);
  removeMatrix(&B);
}
END_TEST

// void movingLeft();
START_TEST(movingLeft_1) {
  initializingGame();
  GameInfo_t info = updateCurrentState();
  matrix_t f;
  creatingMatrix(&f, HEIGHT, WIDTH);
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      f.matrix[i][j] = info.field[i][j];
    }
  }
  movingLeft();
  info = updateCurrentState();
  for (int i = 1; i < HEIGHT - 1; i++) {
    for (int j = 1; j < WIDTH - 1; j++) {
      if (info.field[i][j] != 0)
        ck_assert_int_eq(f.matrix[i][j + 1], info.field[i][j]);
    }
  }
  removeMatrix(&f);
  removeGame();
}
END_TEST

// void movingRight
START_TEST(movingRight_1) {
  initializingGame();
  GameInfo_t info = updateCurrentState();
  matrix_t f;
  creatingMatrix(&f, HEIGHT, WIDTH);
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      f.matrix[i][j] = info.field[i][j];
    }
  }
  movingRight();
  info = updateCurrentState();
  for (int i = 1; i < HEIGHT - 1; i++) {
    for (int j = 1; j < WIDTH - 1; j++) {
      if (info.field[i][j] != 0)
        ck_assert_int_eq(f.matrix[i][j - 1], info.field[i][j]);
    }
  }
  removeMatrix(&f);
  removeGame();
}
END_TEST

// int checkingCollision(int y, int x, matrix_t* f);
START_TEST(checkingCollision_1) {
  initializingGame();
  matrix_t figure;
  creatingMatrix(&figure, 2, 3);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      figure.matrix[i][j] = 1;
    }
  }
  int res = checkingCollision(3, 2, &figure);
  ck_assert_int_eq(res, ok);
  res = checkingCollision(3, 9, &figure);
  ck_assert_int_eq(res, collision);
  res = checkingCollision(3, 7, &figure);
  ck_assert_int_eq(res, ok);
  res = checkingCollision(20, 3, &figure);
  ck_assert_int_eq(res, collision);
  res = checkingCollision(0, 0, &figure);
  ck_assert_int_eq(res, collision);
  res = checkingCollision(1, 1, &figure);
  ck_assert_int_eq(res, ok);
  removeGame();
  removeMatrix(&figure);
}
END_TEST

// int copyMatrix(matrix_t* a, matrix_t* b);
START_TEST(copyMatrix_1) {
  matrix_t A, B;
  creatingMatrix(&A, 4, 6);
  creatingMatrix(&B, 2, 3);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 6; j++) {
      A.matrix[i][j] = 1;
    }
  }
  int res = copyMatrix(&A, &B);
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 3; j++) {
      ck_assert_int_eq(A.matrix[i][j], B.matrix[i][j]);
    }
  }
  ck_assert_int_eq(res, ok);
  removeMatrix(&A);
  removeMatrix(&B);
}
END_TEST

START_TEST(copyMatrix_2) {
  matrix_t A;
  creatingMatrix(&A, 4, 6);
  int res = copyMatrix(&A, NULL);
  ck_assert_int_eq(res, fail);
  removeMatrix(&A);
}
END_TEST

START_TEST(copyMatrix_3) {
  matrix_t A;
  creatingMatrix(&A, 4, 6);
  int res = copyMatrix(NULL, &A);
  ck_assert_int_eq(res, fail);
  removeMatrix(&A);
}
END_TEST

START_TEST(copyMatrix_4) {
  matrix_t A, B;
  B.matrix = NULL;
  creatingMatrix(&A, 4, 6);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 6; j++) {
      A.matrix[i][j] = 1;
    }
  }
  int res = copyMatrix(&A, &B);
  ck_assert_int_eq(res, fail);
  removeMatrix(&A);
}
END_TEST

START_TEST(copyMatrix_5) {
  matrix_t A, B;
  creatingMatrix(&A, 4, 6);
  creatingMatrix(&B, 2, 3);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 6; j++) {
      A.matrix[i][j] = 1;
    }
  }
  A.rows = 0;
  int res = copyMatrix(&A, &B);
  A.rows = 4;
  ck_assert_int_eq(res, fail);
  removeMatrix(&A);
  removeMatrix(&B);
}
END_TEST

// int initializingGame()
START_TEST(initializingGame_1) {
  int res = initializingGame();
  figure n;
  matrix_t m;
  creatingMatrix(&m, HEIGHT, WIDTH);
  bordersMatrix(&m);
  creatingFigure(&n, generatingRandomNumber(1, 7));
  drawingFigureOnField(&m, &n.figure, n.y, n.x);
  GameInfo_t f = updateCurrentState();
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      ck_assert_int_eq(m.matrix[i][j], f.field[i][j]);
    }
  }
  ck_assert_int_eq(res, ok);
  removeMatrix(&n.figure);
  removeMatrix(&m);
  removeGame();
}
END_TEST

// int endOfTime(int* time);
START_TEST(endOfTime_1) {
  initializingGame();
  updateCurrentState();
  int t = clock();
  int res = endOfTime(&t);
  ck_assert_int_eq(res, ok);
  removeGame();
}
END_TEST

START_TEST(endOfTime_2) {
  initializingGame();
  int t = 0;
  int res = endOfTime(&t);
  ck_assert_int_eq(res, fail);
  removeGame();
}
END_TEST

// void clearing(matrix_t* field, matrix_t* figure, int y, int x);
START_TEST(clearing_1) {
  matrix_t A;
  creatingMatrix(&A, 5, 8);
  clearing(&A, NULL, 2, 3);
  removeMatrix(&A);
}
END_TEST

START_TEST(clearing_2) {
  matrix_t A;
  creatingMatrix(&A, 5, 8);
  clearing(NULL, &A, 2, 3);
  removeMatrix(&A);
}
END_TEST

START_TEST(clearing_3) {
  matrix_t A, B;
  creatingMatrix(&A, 5, 8);
  creatingMatrix(&B, 2, 3);
  clearing(&A, &B, 6, 3);
  removeMatrix(&A);
  removeMatrix(&B);
}
END_TEST

START_TEST(clearing_4) {
  matrix_t A, B;
  creatingMatrix(&A, 5, 8);
  creatingMatrix(&B, 2, 3);
  clearing(&A, &B, 4, 7);
  removeMatrix(&A);
  removeMatrix(&B);
}
END_TEST

START_TEST(clearing_5) {
  matrix_t A, B;
  creatingMatrix(&A, 5, 8);
  creatingMatrix(&B, 2, 3);
  clearing(&B, &A, 4, 7);
  removeMatrix(&A);
  removeMatrix(&B);
}
END_TEST

// int replacingFigure();
START_TEST(replacingFigure_1) {
  initializingGame();
  int res = replacingFigure();
  ck_assert_int_eq(res, ok);
  removeGame();
}
END_TEST

START_TEST(replacingFigure_2) {
  int res = replacingFigure();
  ck_assert_int_eq(res, fail);
}
END_TEST

// void settingSpeed(GameInfo_t* allInfo);
START_TEST(settingSpeed_1) {
  GameInfo_t info;
  info.level = 2;
  settingSpeed(&info);
  ck_assert_int_eq(info.speed, 1500000 - 2 * 100000);
}
END_TEST

// void raisingLevel(GameInfo_t* allInfo);
START_TEST(raisingLevel_1) {
  GameInfo_t info;
  int n = 600;
  for (int i = 1; i < 11; i++) {
    info.score = n * i;
    raisingLevel(&info);
    ck_assert_int_eq(info.level, i);
  }
}
END_TEST

// void scoringPoints(matrix_t* field, int line);
START_TEST(scoringPoints_1) {
  GameInfo_t info;
  info.score = 0;
  for (int i = 1; i < 5; i++) {
    scoringPoints(&info, i);
    switch (i) {
      case 1:
        ck_assert_int_eq(info.score, 100);
        break;
      case 2:
        ck_assert_int_eq(info.score, 400);
        break;
      case 3:
        ck_assert_int_eq(info.score, 1100);
        break;
      case 4:
        ck_assert_int_eq(info.score, 2600);
    }
  }
}
END_TEST

// void offsetPerLine(matrix_t* field, int i);
START_TEST(offsetPerLine_1) {
  matrix_t A;
  creatingMatrix(&A, 6, 6);
  for (int i = 0; i < 6; i++) A.matrix[3][i] = 1;
  offsetPerLine(&A, 4);
  for (int j = 1; j < 6 - 1; j++) {
    ck_assert_int_eq(A.matrix[4][j], 1);
    ck_assert_int_eq(A.matrix[3][j], 0);
  }
  removeMatrix(&A);
}
END_TEST

START_TEST(offsetPerLine_2) {
  matrix_t A;
  creatingMatrix(&A, 6, 6);
  for (int i = 0; i < 6; i++) A.matrix[0][i] = 1;
  offsetPerLine(&A, 0);
  for (int j = 0; j < 6; j++) {
    ck_assert_int_eq(A.matrix[0][j], 1);
  }
  removeMatrix(&A);
}
END_TEST

START_TEST(offsetPerLine_3) {
  matrix_t A;
  creatingMatrix(&A, 6, 6);
  for (int i = 0; i < 6; i++) A.matrix[1][i] = 1;
  offsetPerLine(&A, 1);
  for (int j = 0; j < 6; j++) ck_assert_int_eq(A.matrix[1][j], 1);
  removeMatrix(&A);
}
END_TEST

START_TEST(offsetPerLine_4) {
  matrix_t A;
  creatingMatrix(&A, 6, 6);
  for (int i = 0; i < 6; i++) A.matrix[2][i] = 1;
  offsetPerLine(&A, 2);
  for (int j = 1; j < 6 - 1; j++) ck_assert_int_eq(A.matrix[2][j], 0);
  removeMatrix(&A);
}
END_TEST

// int deletingLine(matrix_t* field);
START_TEST(deletingLine_1) {
  matrix_t A;
  creatingMatrix(&A, 6, 6);
  for (int i = 1; i < 5; i++) {
    A.matrix[2][i] = 1;
    A.matrix[3][i] = 1;
  }
  int res = deletingLine(&A);
  ck_assert_int_eq(res, 2);

  for (int i = 1; i < 5; i++) {
    ck_assert_int_eq(A.matrix[2][i], 0);
    ck_assert_int_eq(A.matrix[3][i], 0);
  }
  removeMatrix(&A);
}
END_TEST

START_TEST(deletingLine_2) {
  matrix_t A;
  creatingMatrix(&A, 6, 6);
  for (int i = 1; i < 5; i++) {
    A.matrix[2][i] = 1;
    A.matrix[3][i] = 1;
  }
  A.matrix[2][2] = 0;
  int res = deletingLine(&A);
  ck_assert_int_eq(res, 1);

  for (int i = 1; i < 5; i++) {
    if (i == 2)
      ck_assert_int_eq(A.matrix[3][i], 0);
    else
      ck_assert_int_eq(A.matrix[3][i], 1);
  }
  removeMatrix(&A);
}
END_TEST

// void movingDown();
START_TEST(movingDown_1) {
  initializingGame();
  GameInfo_t info = updateCurrentState();
  matrix_t f;
  creatingMatrix(&f, HEIGHT, WIDTH);
  for (int i = 0; i < HEIGHT; i++) {
    for (int j = 0; j < WIDTH; j++) {
      f.matrix[i][j] = info.field[i][j];
    }
  }
  movingDown();
  info = updateCurrentState();
  for (int i = 1; i < HEIGHT - 1; i++) {
    for (int j = 1; j < WIDTH - 1; j++) {
      if (info.field[i][j] != 0)
        ck_assert_int_eq(f.matrix[i - 1][j], info.field[i][j]);
    }
  }
  removeMatrix(&f);
  removeGame();
}
END_TEST

// void userInput(UserAction_t action, bool hold)
START_TEST(userInput_1) {
  userInput(Start, false);
  userInput(Pause, false);
  userInput(Pause, false);
  userInput(Left, false);
  userInput(Right, false);
  userInput(Down, false);
  userInput(Up, false);
  userInput(Terminate, false);
}

// int clockwiseRotation();
START_TEST(clockwiseRotation_1) {
  initializingGame();
  int res = clockwiseRotation();
  ck_assert_int_eq(res, ok);
  removeGame();
}
END_TEST

///////////////////

// Создание набора тестов
Suite *test_creatingMatrix(void) {
  Suite *s = suite_create("creatingMatrix");
  TCase *tc = tcase_create("creatingMatrix");

  tcase_add_test(tc, creatingMatrix_1);
  tcase_add_test(tc, creatingMatrix_2);
  tcase_add_test(tc, creatingMatrix_3);
  tcase_add_test(tc, creatingMatrix_4);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_removeMatrix(void) {
  Suite *s = suite_create("removeMatrix");
  TCase *tc = tcase_create("removeMatrix");

  tcase_add_test(tc, removeMatrix_1);
  tcase_add_test(tc, removeMatrix_2);
  tcase_add_test(tc, removeMatrix_3);
  tcase_add_test(tc, removeMatrix_4);
  tcase_add_test(tc, removeMatrix_5);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_bordersMatrix(void) {
  Suite *s = suite_create("bordersMatrix");
  TCase *tc = tcase_create("bordersMatrix");

  tcase_add_test(tc, bordersMatrix_1);
  tcase_add_test(tc, bordersMatrix_2);
  tcase_add_test(tc, bordersMatrix_3);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_creatingFigure(void) {
  Suite *s = suite_create("creatingFigure");
  TCase *tc = tcase_create("creatingFigure");

  tcase_add_test(tc, creatingFigure_1);
  tcase_add_test(tc, creatingFigure_2);
  tcase_add_test(tc, creatingFigure_3);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_endOfTime(void) {
  Suite *s = suite_create("endOfTime");
  TCase *tc = tcase_create("endOfTime");

  tcase_add_test(tc, endOfTime_1);
  tcase_add_test(tc, endOfTime_2);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_drawingFigureOnField(void) {
  Suite *s = suite_create("drawingFigureOnField");
  TCase *tc = tcase_create("drawingFigureOnField");

  tcase_add_test(tc, drawingFigureOnField_1);
  tcase_add_test(tc, drawingFigureOnField_2);
  tcase_add_test(tc, drawingFigureOnField_3);
  tcase_add_test(tc, drawingFigureOnField_4);
  tcase_add_test(tc, drawingFigureOnField_5);
  tcase_add_test(tc, drawingFigureOnField_6);
  tcase_add_test(tc, drawingFigureOnField_7);
  tcase_add_test(tc, drawingFigureOnField_8);
  tcase_add_test(tc, drawingFigureOnField_9);
  tcase_add_test(tc, drawingFigureOnField_10);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_initializingGame(void) {
  Suite *s = suite_create("initializingGame");
  TCase *tc = tcase_create("initializingGame");

  tcase_add_test(tc, initializingGame_1);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_checkingCollision(void) {
  Suite *s = suite_create("checkingCollision");
  TCase *tc = tcase_create("checkingCollision");

  tcase_add_test(tc, checkingCollision_1);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_copyMatrix(void) {
  Suite *s = suite_create("copyMatrix");
  TCase *tc = tcase_create("copyMatrix");

  tcase_add_test(tc, copyMatrix_1);
  tcase_add_test(tc, copyMatrix_2);
  tcase_add_test(tc, copyMatrix_3);
  tcase_add_test(tc, copyMatrix_4);
  tcase_add_test(tc, copyMatrix_5);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_clearing(void) {
  Suite *s = suite_create("clearing");
  TCase *tc = tcase_create("clearing");

  tcase_add_test(tc, clearing_1);
  tcase_add_test(tc, clearing_2);
  tcase_add_test(tc, clearing_3);
  tcase_add_test(tc, clearing_4);
  tcase_add_test(tc, clearing_5);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_replacingFigure(void) {
  Suite *s = suite_create("replacingFigure");
  TCase *tc = tcase_create("replacingFigure");

  tcase_add_test(tc, replacingFigure_1);
  tcase_add_test(tc, replacingFigure_2);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_settingSpeed(void) {
  Suite *s = suite_create("settingSpeed");
  TCase *tc = tcase_create("settingSpeed");

  tcase_add_test(tc, settingSpeed_1);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_raisingLevel(void) {
  Suite *s = suite_create("raisingLevel");
  TCase *tc = tcase_create("raisingLevel");

  tcase_add_test(tc, raisingLevel_1);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_scoringPoints(void) {
  Suite *s = suite_create("scoringPoints");
  TCase *tc = tcase_create("scoringPoints");

  tcase_add_test(tc, scoringPoints_1);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_offsetPerLine(void) {
  Suite *s = suite_create("offsetPerLine");
  TCase *tc = tcase_create("offsetPerLine");

  tcase_add_test(tc, offsetPerLine_1);
  tcase_add_test(tc, offsetPerLine_2);
  tcase_add_test(tc, offsetPerLine_3);
  tcase_add_test(tc, offsetPerLine_4);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_deletingLine(void) {
  Suite *s = suite_create("deletingLine");
  TCase *tc = tcase_create("deletingLine");

  tcase_add_test(tc, deletingLine_1);
  tcase_add_test(tc, deletingLine_2);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_movingLeftRightDown(void) {
  Suite *s = suite_create("movingLeftRight");
  TCase *tc = tcase_create("movingLeftRight");

  tcase_add_test(tc, movingLeft_1);
  tcase_add_test(tc, movingRight_1);
  tcase_add_test(tc, movingDown_1);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_clockwiseRotation(void) {
  Suite *s = suite_create("clockwiseRotation");
  TCase *tc = tcase_create("clockwiseRotation");

  tcase_add_test(tc, clockwiseRotation_1);

  suite_add_tcase(s, tc);
  return s;
}

Suite *test_userInput(void) {
  Suite *s = suite_create("userInput");
  TCase *tc = tcase_create("userInput");

  tcase_add_test(tc, userInput_1);

  suite_add_tcase(s, tc);
  return s;
}

int main() {
  int failed = 0;

  Suite *all_tests_for_tetris[] = {test_creatingMatrix(),
                                   test_removeMatrix(),
                                   test_bordersMatrix(),
                                   test_creatingFigure(),
                                   test_drawingFigureOnField(),
                                   test_initializingGame(),
                                   test_endOfTime(),
                                   test_checkingCollision(),
                                   test_copyMatrix(),
                                   test_clearing(),
                                   test_replacingFigure(),
                                   test_settingSpeed(),
                                   test_scoringPoints(),
                                   test_offsetPerLine(),
                                   test_deletingLine(),
                                   test_movingLeftRightDown(),
                                   test_raisingLevel(),
                                   test_clockwiseRotation(),
                                   test_userInput(),
                                   NULL};

  for (int i = 0; all_tests_for_tetris[i] != NULL; i++) {
    SRunner *runner = srunner_create(all_tests_for_tetris[i]);
    srunner_run_all(runner, CK_NORMAL);
    failed += srunner_ntests_failed(runner);
    srunner_free(runner);
  }

  if (failed == 0)
    printf("\n\033[0;32mTotal errors: %d\033[0m\n", failed);
  else
    printf("\n\033[0;31mTotal errors: %d\033[0m\n", failed);

  return (failed == 0) ? 0 : 1;
}
