#include <stdio.h>
#include <stdlib.h>


#define CORRECT 0
#define INCORRECT_MATRIX 1
#define CALCULATION_ERROR 2

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

typedef struct turning_figure {
        matrix_t f0;
        matrix_t f1;
        matrix_t f2;
        matrix_t f3;
} position;

int s21_create_matrix(int rows, int columns, matrix_t *result) {
  int error = CORRECT;
  if (rows <= 0 || columns <= 0 || result == NULL) {
    error = INCORRECT_MATRIX;
  } else {
    result->rows = rows;
    result->columns = columns;
    result->matrix = (double **)calloc(rows, sizeof(double *));
    for (int i = 0; i < rows; i++)
      result->matrix[i] = (double *)calloc(columns, sizeof(double));
    if (result->matrix == NULL) error = INCORRECT_MATRIX;
  }
  return error;
}

void n(position* num) {
	s21_create_matrix(1, 8, &num->f0);
	for(int i = 0; i < num->f0.rows; i++) {
		for(int j = 0; j < num->f0.columns; j++) {
			num->f0.matrix[i][j] = 1;
			printf("%f ", num->f0.matrix[i][j]);
		}
		printf("\n");
	}

	s21_create_matrix(num->f0.columns/2, num->f0.rows*2, &num->f1);
	printf("%d %d\n", num->f1.rows, num->f1.columns);
	
	for(int i = 0; i < num->f1.rows; i++) {
                for(int j = 0, ii = num->f1.columns - 1; j < num->f1.columns; j++, ii--) {
                        num->f1.matrix[i][j] = num->f0.matrix[ii][i];
                }
        }
}

int main() {
	position num;
	n(&num);
	
        for(int i = 0; i < num.f1.rows; i++) {
                for(int j = 0; j < num.f1.columns; j++) {
                        printf("%f ", num.f1.matrix[i][j]);
                }
                printf("\n");
        }
	
	
	return 0;
}
