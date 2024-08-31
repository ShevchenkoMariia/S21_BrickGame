#include <stdlib.h>
#include <string.h>
#include <ncurses.h> // Подключаем библиотеку
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

typedef unsigned short size_sh;

#define CORRECT 0
#define INCORRECT_MATRIX 1
#define CALCULATION_ERROR 2

typedef struct window {
        size_sh rows;
        size_sh columns;
        size_sh y;
        size_sh x;
        WINDOW* window;
} win;

typedef enum {
        red = 1,
        orange,
        yellow,
        green,
        blue,
        ping,
        violet,
        w_black,
        b_y,
	y_b,
	w_blue,
	b_b
} color;

//из матриц

typedef struct matrix_struct {
  double **matrix;
  int rows;
  int columns;
} matrix_t;

typedef struct turning_figure {
	size_sh color;
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


void s21_remove_matrix(matrix_t *A) {
  if (A != NULL && A->matrix != NULL) {
    for (int i = 0; i < A->rows; i++) free(A->matrix[i]);
    free(A->matrix);
  }
  if (A != NULL) {
    A->rows = 0;
    A->columns = 0;
    A->matrix = NULL;
  }
}

///////

void initializing_color() {
        start_color();
        init_color(COLOR_RED, 700, 0 , 0);
        init_pair(1, COLOR_WHITE, COLOR_RED);
        init_color(COLOR_CYAN, 999, 450, 0);
        init_pair(2, COLOR_WHITE, COLOR_CYAN); //оранжевый
        init_color(COLOR_YELLOW, 900, 800, 0);
        init_pair(3, COLOR_WHITE, COLOR_YELLOW);
        init_color(COLOR_GREEN, 0, 550, 0);
        init_pair(4, COLOR_WHITE, COLOR_GREEN);
        init_color(COLOR_BLUE, 200, 0, 900);
        init_pair(5, COLOR_BLUE, COLOR_BLUE);
        init_color(COLOR_MAGENTA, 900, 0, 500);
        init_pair(6, COLOR_WHITE, COLOR_MAGENTA); //розовый
        init_color(COLOR_WHITE, 500, 0, 700);
        init_pair(7, COLOR_WHITE, COLOR_WHITE); //фиолетовый
        init_pair(8, COLOR_WHITE, COLOR_BLACK);
        init_pair(9, COLOR_BLACK, COLOR_YELLOW);
        init_pair(10, COLOR_YELLOW, COLOR_BLACK);
        init_pair(11, COLOR_WHITE, COLOR_BLUE);
	init_color(COLOR_BLACK, 0, 0, 0);
        init_pair(12, COLOR_BLACK, COLOR_BLACK);
}

void rendering_str(win window, size_sh num_str, const char str[], int color) {
	size_sh len = (size_sh)strlen(str);
	wattron(window.window,COLOR_PAIR(color));
	mvwprintw(window.window, num_str, (window.columns-len)/2, "%s", str);
	wattroff(window.window,COLOR_PAIR(color));
}

void rendering_bold_str(win window, size_sh num_str, const char str[], int color) {
	for (int i = 0, x = (int)((window.columns - (int)strlen(str))/2); i < (int)strlen(str); i++, x++) {
                mvwaddch(window.window, num_str, x, COLOR_PAIR(color) | A_BOLD | str[i]);
        }
}


void filling(win window, const char ch, int color) { // заливка
	for(size_sh i = 1; i < window.rows - 1; i++) {
                for(size_sh j = 1; j < window.columns - 1; j++) {
                        mvwaddch(window.window, i, j, COLOR_PAIR(color) | A_INVIS | ch);
                }
        }	
}

void initializing_win(win* window, size_sh rows, size_sh columns, size_sh y, size_sh x) {
	window->rows = rows;
        window->columns = columns;
        window->y = y;
        window->x = x;
        window->window = newwin(window->rows, window->columns, window->y, window->x);
}

// можно попробовать адаптировать для создания любых окон
void initializing_windows(win* background, win* game_space, win* score, win* level, win* preview, win* record) {
//	win* all_win[5] = {background, game_space, score, level, preview, record};
	initializing_win(background, 24, 44, 1, 3);
	initializing_win(game_space, 20, 22, 3, 5);
	initializing_win(score, 5, 13, 3, 30);
	initializing_win(level, 3, 13, 9, 30);
	initializing_win(preview, 5, 14, 13, 30);
	initializing_win(record, 4, 14, 19, 30);
}

void drawing_design(win* background, win* game_space, win* score, win* level, win* preview, win* record) { //прорисовка дизайна
	win *all_win[6] = {background, game_space, score, level, preview, record};
        for(int i = 0; i < 6; i++) {
                box(all_win[i]->window, 0, 0);
		if(i == 0) {
			filling(*all_win[i], ' ', violet);
		} else if (i == 1) {
			filling(*all_win[i], '0', y_b);
		} else if (i == 2) {
			rendering_str(*all_win[i], 0, "score", y_b);
		} else if (i == 3) {
			rendering_str(*all_win[i], 0, "level", y_b);
		} else if (i == 4) {
			rendering_str(*all_win[i], 0, "next", y_b);
		} else if(i == 5) {
			rendering_str(*all_win[i], 0, "record", y_b);
		} 
                wrefresh(all_win[i]->window);
        }
}

void game_over(win game_space) {
	win game_over;
        initializing_win(&game_over, 11, 16, (game_space.rows - 12)/2 + game_space.y, (game_space.columns - 16)/2 + game_space.x);
        box(game_over.window, 0, 0);

	size_sh art[7][14] = {
		{0,0,0,0,2,2,2,2,2,2,0,0,0,0},
		{0,0,2,2,2,2,2,2,2,2,2,2,0,0},
		{2,2,5,5,5,5,2,2,5,5,5,5,2,2},
		{2,2,5,5,0,0,2,2,0,0,5,5,2,2},
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2},
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2},
		{2,2,0,0,2,2,0,0,2,2,0,0,2,2}
	};

	for(size_sh i = 0, y = 3; i < 7; i++, y++) {
		for(size_sh j = 0, x = 1; j < 14; j++, x++) {
			if(art[i][j] > 0) {mvwaddch(game_over.window, y, x, COLOR_PAIR(art[i][j])| ' ');}
		}
	}
        rendering_bold_str(game_over, 1, " GAME OVER! ", w_blue); 
        rendering_bold_str(game_over, 2, " YOU'RE THE BEST! ", w_blue); 
        wrefresh(game_over.window);
	delwin(game_over.window);

		

/*	char nums[3][5][6] = {
		{{1, 1, 1, 1, 1, 1}, {0, 0, 0, 0, 1, 1}, {0, 0, 1, 1, 1, 1}, {0, 0, 0, 0, 1, 1}, {1, 1, 1, 1, 1, 1}},
		{{1, 1, 1, 1, 1, 1}, {0, 0, 0, 0, 1, 1}, {1, 1, 1, 1, 1, 1}, {1, 1, 0, 0, 0, 0}, {1, 1, 1, 1, 1, 1}},
		{{0, 1, 1, 1, 1, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 1, 1, 0}, {0, 0, 0, 1, 1, 0}}
	};

	for(int count = 0; count < 3; count++) {
		box(start.window, 0, 0);
		rendering_str(start, start.rows-3, " ARE YOU READY? ", 9); 
		for(size_sh i = 0, y = 2; i < 5; i++, y++) {
			for(size_sh j = 0, x = (start.columns - 6)/2; j < 6; j++, x++) {
				if(nums[count][i][j] == 1) mvwaddch(start.window, y, x, COLOR_PAIR(3) | A_BOLD | ' ');
			}
		}

		wrefresh(start.window);
		sleep(1);
		wclear(start.window);
	}
		wrefresh(start.window);
		sleep(1);*/
}

//фигуры
size_sh choosing_figure(matrix_t* figure) {
	srand(time(NULL));
	size_sh number_figure = rand() % 7 + 1;
	switch(number_figure) {
		case 1: {
			
				s21_create_matrix(2, 4, figure);
                                for(int i = 0; i < figure->rows; i++) {
                                        for(int j = 0; j < figure->columns; j++) {
                                                figure->matrix[i][j] = 1;
                                        }
                                }
				break;
			}
		case 2: {
				//попробовать заменить на size_sh
				s21_create_matrix(1, 8, figure);
				for (int i = 0; i < figure->columns; i++) {
					figure->matrix[0][i] = 1;
				}
				break;
			}
		case 3: {
				s21_create_matrix(2, 6, figure);
                                for(int i = 0; i < figure->rows; i++) {
                                        for(int j = 0; j < figure->columns; j++) {
                                                if((i == 0 && j < 2) || (i == 0 && j > 3)) figure->matrix[i][j] = 0;
                                                else figure->matrix[i][j] = 1;
                                        }
                                }
				break;
			}
		case 4: {
				s21_create_matrix(2, 6, figure);
				for(int i = 0; i < figure->rows; i++) {
					for(int j = 0; j < figure->columns; j++) {
						if(i == 0 && j > 1) figure->matrix[i][j] = 0;
						else figure->matrix[i][j] = 1;
					}
				}
				break;
			}
		case 5: {
				s21_create_matrix(2, 6, figure);
                                for(int i = 0; i < figure->rows; i++) {
                                        for(int j = 0; j < figure->columns; j++) {
                                                if(i == 0 && j < 4) figure->matrix[i][j] = 0;
                                                else figure->matrix[i][j] = 1;
                                        }
                                }
				break;
			}
		case 6: {
				s21_create_matrix(2, 6, figure);
                                for(int i = 0; i < figure->rows; i++) {
                                        for(int j = 0; j < figure->columns; j++) {
                                                if((i == 0 && j < 2) || (i == 1 && j > 3)) figure->matrix[i][j] = 0;
                                                else figure->matrix[i][j] = 1;
                                        }
                                }
				break;
			}
		case 7: {
				s21_create_matrix(2, 6, figure);
                                for(int i = 0; i < figure->rows; i++) {
                                        for(int j = 0; j < figure->columns; j++) {
                                                if((i == 1 && j < 2) || (i == 0 && j > 3)) figure->matrix[i][j] = 0;
                                                else figure->matrix[i][j] = 1;
                                        }
                                }
				break;
			}
	}
	return number_figure;
}

void rendering_figure(win window, size_sh win_y, size_sh win_x, matrix_t figure, size_sh color) {
	//перевести цыфру в символ
	char ch = 48+color;
        for(int i = 0, y = win_y; i < figure.rows; i++, y++) {
                for(int j = 0, x = win_x; j < figure.columns; j++, x++) {
                        if(figure.matrix[i][j]) {
                                mvwaddch(window.window, y, x, COLOR_PAIR(color)| A_INVIS | ch);
                        }
                }
        }
}


//очистка окна
void cleaning_window(win window) {
	for(size_sh i = 1; i < window.rows - 1; i++) {
		for(size_sh j = 1; j < window.columns - 1; j++) {
			mvwaddch(window.window, i, j, A_INVIS|'0');
		}
	}
}

//очистка фигуры
void cleaning_figure(win window, size_sh win_y, size_sh win_x, matrix_t figure) {
        for(int i = 0, y = win_y; i < figure.rows; i++, y++) {
                for(int j = 0, x = win_x; j < figure.columns; j++, x++) {
                        if(figure.matrix[i][j]) {
                                mvwaddch(window.window, y, x, A_INVIS |'0');
                        }
                }
        }
}

void copy_matrix(matrix_t A, matrix_t *B) {
	//проверка что матрицы равны
	for(int i = 0; i < A.rows; i++) {
		for(int j = 0; j < A.columns; j++) {
			B->matrix[i][j] = A.matrix[i][j];
		}
	}
}

void states_figure(matrix_t figure, position *shape, size_sh color) {
	shape->color = color;
	//сохранение нулевого состояния
	s21_create_matrix(figure.rows, figure.columns, &shape->f0);	
	copy_matrix(figure, &shape->f0);

	//поворот на 90 градусов вправо 1
	s21_create_matrix(shape->f0.columns/2, shape->f0.rows*2, &shape->f1);
	for(int i = 0, jj = 0; i < shape->f1.rows; i++, jj+=2) {
                for(int j = 0, ii = shape->f1.columns/2 - 1; j < shape->f1.columns; j++, ii--) {
                        shape->f1.matrix[i][j] = shape->f0.matrix[ii][jj];
                        shape->f1.matrix[i][++j] = shape->f0.matrix[ii][jj];

                } 
        }
	
	//поворот на 90 градусов вправо 2/
	s21_create_matrix(shape->f0.rows, shape->f0.columns, &shape->f2);
	for(int i = 0, ii = shape->f2.rows - 1; i < shape->f2.rows; i++,ii--) {
                for(int j = 0, jj = shape->f2.columns - 1; j < shape->f2.columns; j++, jj--) {
                        shape->f2.matrix[i][j] = shape->f0.matrix[ii][jj];
                } 
        }	

	//поворот на 90 градусов вправо 3
	s21_create_matrix(shape->f1.rows, shape->f1.columns, &shape->f3);
	for(int i = 0, ii = shape->f3.rows - 1; i < shape->f3.rows; i++, ii--) {
                for(int j = 0, jj = shape->f3.columns - 1; j < shape->f3.columns; j++, jj--) {
                        shape->f3.matrix[i][j] = shape->f1.matrix[ii][jj];
                } 
        }
}

void cleaning_states_figure(position* shape) {
	shape->color = 0;
	s21_remove_matrix(&shape->f0);
	s21_remove_matrix(&shape->f1);
	s21_remove_matrix(&shape->f2);
	s21_remove_matrix(&shape->f3);
}

void delay(int ms) {
	int c = clock() + ms;
	while(clock()<c);
}

//анализ поля
//bool collision_analysis(win window, size_sh win_y, size_sh win_x, matrix_t figure) {

bool checking_before_moving_down(win window, size_sh win_y, size_sh win_x, matrix_t figure) {
	bool result = false;
	//проверка перед падением
	for(int i = 0; i < figure.rows; i++) {
		for(int j = 0; j < figure.columns; j++) {
			char c = mvwinch(window.window, win_y + i + 1, win_x + j);
			if((figure.matrix[i][j] >= 1 && figure.matrix[i][j] <=7) && !(i < figure.rows - 1 && figure.matrix[i+1][j] >= 1 && figure.matrix[i][j] <=7) && c != '0') {
				result = true;
		        	break;
			}
		}
	}
	return result;
}



typedef struct state_game {
	size_sh level; //уровень
	size_t speed; //скорость перемещения фигур
	size_t score; //очки
} changes; 

void starting_state_game(changes* state) {
	state -> level = 1;
	state -> speed = 1500000;
	state -> score = 0;
}

//разбить на две функци
void drawing_state_game(win* level, win* score, changes* state) {
	wattron(score->window, COLOR_PAIR(w_blue));
        wattron(level->window, COLOR_PAIR(w_blue));
	char str_score[8] = {0};
	snprintf(str_score, sizeof(str_score), "%lu", state->score);
	size_t len_score = (strlen(str_score));
        mvwprintw(score->window, score->rows/2, (score->columns-2-len_score)/2, " %lu ", state->score);

	char str_level[3] = {0};
	snprintf(str_level, sizeof(str_level), "%u", state->level);
	size_t len_level = (strlen(str_level));
        mvwprintw(level->window, level->rows/2, (level->columns-2-len_level)/2, " %u ", state->level);
        wattroff(score->window, COLOR_PAIR(w_blue));
        wattroff(level->window, COLOR_PAIR(w_blue));
        wrefresh(score->window);
        wrefresh(level->window);
}

//подготовка игрового пространства
void preparing_game_space(win* background, win* game_space, win* score, win* level, win* preview, changes* state, win* record) {
	//создание окон
	initializing_windows(background, game_space, score, level, preview, record);
	//отрисовка окон
	drawing_design(background, game_space, score, level, preview, record);
	//определение стартового состояния игры 
	starting_state_game(state);
	//отрисовка состояния игры
	drawing_state_game(level, score, state);
}

// номер фигуры = номер цвета

//подготовка к работе с библиотекой ncurses
void preparing_to_work_with_ncurses() {
	initscr();
	curs_set(0);
	noecho();
//	raw();
	initializing_color();
}


void printing_shape_and_preview(position shape, size_sh* y, size_sh*x, win* game_space, win* preview, matrix_t* figure, size_sh* num_figure) {
	//отрисовка фигуры для игры
	rendering_figure(*game_space, *y, *x, shape.f0, shape.color);
	//очистка окна превью
	cleaning_window(*preview);
	//очистка матрицы с фигурой
	s21_remove_matrix(figure);
	//выбор новой фигуры
	*num_figure = choosing_figure(figure);
	//отрисовка нового превью
	rendering_figure(*preview, (preview->rows-figure->rows)/2 + 1, (preview->columns-figure->columns)/2, *figure, *num_figure);
	
	//вывод на экран
	wrefresh(preview->window);
	wrefresh(game_space->window);
}



//скорость падения фигур
void speed_of_falling_figures(changes* state) {

	/*for(size_sh i = 1, j = 2; i <= 9 && j <= 10; i++, j++) {
		if(state->score >= 600*1 && state->score < 600*j) {
			state->level = j;
			state->speed -= 100000; 
		}
	}*/

	if(state->score >= 600 && state->score < 600*2) {
		state->level = 2;
		state->speed = 1400000;	
	} else if(state->score >= 600*2 && state->score < 600*3) {
                state->level = 3;
                state->speed = 1300000;
	} else if(state->score >= 600*3 && state->score < 600*4) {
                state->level = 4;
                state->speed = 1200000;
	} else if(state->score >= 600*4 && state->score < 600*5) {
                state->level = 5;
                state->speed = 1100000;
	} else if(state->score >= 600*5 && state->score < 600*6) {
                state->level = 6;
                state->speed = 1000000;
	} else if(state->score >= 600*6 && state->score < 600*7) {
                state->level = 7;
                state->speed = 900000;
	} else if(state->score >= 600*7 && state->score < 600*8) {
                state->level = 8;
                state->speed = 800000;
	} else if(state->score >= 600*8 && state->score < 600*9) {
                state->level = 9;
                state->speed = 700000;
	} else if(state->score >= 600*9) {
                state->level = 10;
                state->speed = 600000;
	}
}

//смещение на строку
void offset_per_line(int row, win* game_space) {
	for(int i = row; i > 1 ; i--) {
                for(int j = 1; j < game_space->columns - 1; j++) {
			char c = mvwinch(game_space->window, i-1, j);
			int cc = c-48;
			mvwaddch(game_space->window, i,j, COLOR_PAIR(cc)|A_INVIS|c);
                }
	}
	wrefresh(game_space->window);
	delay(1000);
}

void deleting_line(win* game_space, size_sh* line) {
	for(int i = game_space->rows - 1; i > 0 ; i--) {
		int count = 0;
		for(int j = 1; j < game_space->columns - 1; j++) {
			char c = mvwinch(game_space->window, i, j);
			if(c >= '1' && c<='7') {
				count ++;
			}
		}
		if(count == game_space->columns - 2) {
			(*line)++;
			//смещение всего на строку
			offset_per_line(i, game_space);
			i++;
		}
	}
}

void scoring_points(changes* state, size_sh* line) {
	switch(*line) {
		case 1: state->score += 100;
			break;
		case 2: state->score += 300;
			break;
		case 3: state->score += 700;
			break;
		case 4: state->score += 1500;
			break;
	}
}

bool checking_on_the_right(win* game_space, size_sh y, size_sh x, matrix_t figure) {
	bool check = false;
	for(int i = 0; i< figure.rows; i++) {
		for(int j = 0; j < figure.columns; j++) {
			char ch = mvwinch(game_space->window, y+i, x+j+1);
			if(ch != '0' && figure.matrix[i][j] == 1 && !(figure.matrix[i][j+1] == 1 && j < figure.columns - 1)) {
				check = true;
				break;	
			}
		}
	}
	return check;
}

bool checking_on_the_left(win* game_space, size_sh y, size_sh x, matrix_t figure) {
	bool check = false;
	for(int i = 0; i< figure.rows; i++) {
		for(int j = 0; j < figure.columns; j++) {
			if(figure.matrix[i][j] == 1) {
            			char ch = mvwinch(game_space->window, y+i, x+j-1);
				if(ch != '0') {
					check = true;
				}
				break;
			}
		}
	}
	return check;
}

//определяем координаты перед поворотом
void coordinates_before_turning(size_sh color, int *number_matrix, size_sh* y, size_sh* x) {
	int num_matrix = (*number_matrix) + 1;
	if(num_matrix == 4) {
		num_matrix = 0;
	}
	size_sh help_y = *y;
       	size_sh help_x = *x;
	if(color != 1) {
		if(num_matrix == 1) help_x+=2;
		if(num_matrix == 2) {help_x-=2; if (color != 2) help_y++;}
		if(num_matrix == 3 && color != 2) {help_y--;}
		if(num_matrix == 3 && color == 2) {help_x+=2;}
		if(num_matrix == 0 && color == 2) {help_x-=2;}
	}
	*y = help_y;
	*x = help_x;
	*number_matrix = num_matrix;
}

bool checking_the_coordinates(win* game_space, size_sh y, size_sh x, matrix_t figure) {
	bool check = false;
	if(y <= 0) check = true; //проверка на верхнюю границу
	else if (y + figure.rows > game_space->rows-1) check = true; // проверка на нижнюю границу
	else if(x+figure.columns > game_space->columns-1 || x<=0) check = true; //проверка на правую и левую границы
	else {
		for(int i = 0; i < figure.rows; i++) {
			for(int j = 0; j < figure.columns; j++) {
            			char ch = mvwinch(game_space->window, y+i, x+j);
				if(ch != '0' && figure.matrix[i][j] == 1) {
					check = true;
					break;
				}
			}
		}
	}
	return check;
}

void char_tetris (win* menu) {
        int rows_tetris = 5;
        int columns_tetris = 36;

	bool tetris[5][36] = {
		{1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1},
		{0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0},
		{0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 1},
		{0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 1, 1},
		{0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1}
	};

	for(int i = 0, y = 2; i < rows_tetris; i++, y++) {
		for(int j = 0, x = (menu->columns - columns_tetris)/2; j < columns_tetris; j++, x++) {
			if(tetris[i][j]) {
				mvwaddch(menu->window, y, x, COLOR_PAIR(yellow) | ' ');
			}
		}
	}
	
}

void drawing (win* menu) {
	int rows_dr = 6;
        int columns_dr = 42;
	size_sh drawing[6][42] = {
		{3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2},
		{3, 3, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 2, 2},
		{3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 3, 3, 2, 2},
		{2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 5, 5, 0, 0, 0, 0, 7, 7, 7, 7, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 7, 7, 7, 7, 3, 3, 2, 2},
		{1, 1, 1, 1, 4, 4, 5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 3, 3, 7, 7, 7, 7, 4, 4, 1, 1, 1, 1, 0, 0, 4, 4, 4, 4, 4, 4, 7, 7, 7, 7, 5, 5},
		{1, 1, 1, 1, 4, 4, 4, 4, 4, 4, 6, 6, 6, 6, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 5, 5, 5, 5, 5, 5}
	};
	
	for(int i = 0, y = 17; i < rows_dr; i++, y++) {
                for(int j = 0, x = 1; j < columns_dr; j++, x++) {
                        if(drawing[i][j] > 0) {
                                mvwaddch(menu->window, y, x, COLOR_PAIR(drawing[i][j]) | ' ');
                        }
                }
        }

}

bool menu(){
	bool exit = false;
	const unsigned int rows = 24;
        const unsigned int columns = 44;
	win menu;
	initializing_win(&menu, rows, columns, 1, 3);
	keypad(menu.window, TRUE);
	box(menu.window, 0, 0);

	char_tetris(&menu);
	drawing(&menu);
	rendering_bold_str(menu, 8, " Welcome to the game! ", violet);

	size_sh color_game = yellow;
	size_sh color_go = w_black;
	while(1) {
		rendering_bold_str(menu, 12, " NEW GAME ", color_game);
		rendering_bold_str(menu, 13, " EXIT ", color_go);

		wrefresh(menu.window);
		int ch = wgetch(menu.window);
		if(color_go == yellow && color_game == w_black && ch == '\n') {
			exit = true;
			break;
		} else if(color_go == w_black && color_game == yellow  && ch == KEY_DOWN) {
			color_go = yellow;
			color_game = w_black;
		} else if(color_game == w_black && color_go == yellow && ch == KEY_UP) {
			color_go = w_black;
                        color_game = yellow;
		} else if(color_go == w_black && color_game == yellow  && ch == '\n') {
			break;
		}
	}
			
        delwin(menu.window);
	return exit;
}

void mini_minu(bool* exit, bool* restart, int* ch, win game_space) {
	win mini_menu;
	initializing_win(&mini_menu, 4, 14, 19, 30);
	box(mini_menu.window, 0, 0);
	keypad(mini_menu.window, TRUE);
	nodelay(mini_menu.window, TRUE);
	rendering_str(mini_menu, 0, "stop", y_b);
	size_sh color_new = violet;
	size_sh color_out = w_black;
	while (1) {
		rendering_bold_str(mini_menu, 1, " NEW GAME ", color_new);
		rendering_bold_str(mini_menu, 2, " MENU ", color_out);
		wrefresh(mini_menu.window);
		*ch = wgetch(mini_menu.window);
		if(color_out == violet && color_new == w_black && *ch == '\n') {
			*exit = true;
			break;
		} else if(color_out == w_black && color_new == violet && *ch == KEY_DOWN) {
			color_out = violet;
			color_new = w_black;
		} else if(color_new == w_black && color_out == violet && *ch == KEY_UP) {
			color_out = w_black;
	                color_new = violet;
		} else if(color_out == w_black && color_new == violet && *ch == '\n') {
			*restart = true;
			cleaning_window(game_space);
			break;
		} else if(*ch == 32) {
			break;
		}
	}
	delwin(mini_menu.window);
}

int main() {

	//подготовка к работе с библиотекой ncurses
	preparing_to_work_with_ncurses();
	
	if(menu()) {
		endwin();
		return 0;
	}
	//подготовка игрового пространства
	do {
		win background, game_space, score, level, preview, record;
		changes state;
		preparing_game_space(&background, &game_space, &score, &level, &preview, &state, &record);
		keypad(game_space.window, TRUE);

		matrix_t figure;
		size_sh num_figure = choosing_figure(&figure); //выбор фигуры
		//отрисовка превью
		rendering_figure(preview, (preview.rows-figure.rows)/2 + 1, (preview.columns-figure.columns)/2, figure, num_figure); // отрисовка фигуры
		wrefresh(preview.window);
		sleep(1);

		//1-3 перед игрой
		//добавить правила игры
	
		bool exit = false;

		for(int i = 0; exit == false; i++) {
			//сохранение фгуры из превью и формирования ёё состояний
			bool restart = false;
			position shape;
			states_figure(figure, &shape, num_figure);	
	
			//отрисовка фигуры и нового превью
			size_sh y = 1, x = (game_space.columns-figure.columns)/2;
			if(x%2 == 0 ) x--;
	
			matrix_t all[] = {shape.f0, shape.f1, shape.f2, shape.f3};
			int num_matrix = 0;
			if(checking_the_coordinates(&game_space, y, x, all[num_matrix])) {
				nodelay(game_space.window, FALSE);
				game_over(game_space);
				int ch = 0;
				mini_minu(&exit, &restart, &ch, game_space);
				if(exit == false) {
					initializing_win(&record, 4, 14, 19, 30);
					box(record.window, 0, 0);
					rendering_str(record, 0, "record", y_b);
					wrefresh(record.window);
				}
				if(restart){
					cleaning_window(level);
					cleaning_window(score);
				       	starting_state_game(&state);
					drawing_state_game(&level, &score, &state);
				}
			} else { 
				printing_shape_and_preview(shape, &y, &x, &game_space, &preview, &figure, &num_figure);
			}
			while((y < game_space.rows - shape.f0.rows - 1) && !checking_before_moving_down(game_space, y, x, all[num_matrix]) && exit == false && restart == false) {
				nodelay(game_space.window, TRUE);
				int start_time = clock();
				while(clock() - start_time < (long int)state.speed) {
					int ch = wgetch(game_space.window);
					if(ch == KEY_LEFT && !checking_on_the_left(&game_space, y, x, all[num_matrix])) {
						cleaning_figure(game_space, y, x, all[num_matrix]);
						x-=2;
				                rendering_figure(game_space, y, x, all[num_matrix], shape.color);
						wrefresh(game_space.window);
					}
					if(ch == KEY_RIGHT && !checking_on_the_right(&game_space, y, x, all[num_matrix])) {
						cleaning_figure(game_space, y, x, all[num_matrix]);
						x+=2;
				                rendering_figure(game_space, y, x, all[num_matrix], shape.color);
						wrefresh(game_space.window);
					}
					if(ch == KEY_UP) {
						size_sh help_y = y, help_x = x;
						coordinates_before_turning(shape.color, &num_matrix, &help_y, &help_x);
						
						num_matrix--;
						if(num_matrix == -1) num_matrix = 3;
						cleaning_figure(game_space, y, x, all[num_matrix]);
						num_matrix++;
               	        	                if(num_matrix == 4) {
       	        	                                num_matrix = 0;
        	                                }
						if(!checking_the_coordinates(&game_space, help_y, help_x, all[num_matrix])) {
							y = help_y;
							x = help_x;
							rendering_figure(game_space, y, x, all[num_matrix], shape.color);
        	        	                        wrefresh(game_space.window);
						} else {
							num_matrix--;
							if(num_matrix == -1) num_matrix = 3;
							rendering_figure(game_space, y, x, all[num_matrix], shape.color);
                		                        wrefresh(game_space.window);
						}
					}
					if(ch == 32) { // 32 - пробел
						mini_minu(&exit, &restart, &ch, game_space);
						if(exit == false) {
							initializing_win(&record, 4, 14, 19, 30);
							box(record.window, 0, 0);
							rendering_str(record, 0, "record", y_b);
							wrefresh(record.window);
						}
						if(restart){
						       	starting_state_game(&state);
							drawing_state_game(&level, &score, &state);
						}
					}
					if(ch == KEY_DOWN) {
						while(!checking_before_moving_down(game_space, y, x, all[num_matrix])) {
								cleaning_figure(game_space, y, x, all[num_matrix]);
								rendering_figure(game_space, ++y, x, all[num_matrix], shape.color);
 					                        wrefresh(game_space.window);
								delay(1000);
						}
					}
				}
				if(!checking_before_moving_down(game_space, y, x, all[num_matrix]) && exit == false && restart == false) {
					cleaning_figure(game_space, y, x, all[num_matrix]);
		        	        rendering_figure(game_space, ++y, x, all[num_matrix], shape.color);
					wrefresh(game_space.window);
				}
			}
			cleaning_states_figure(&shape);
			//сжигаем строки
			if(exit == false && restart == false) {
				size_sh line = 0;
				deleting_line(&game_space, &line);
				//подсчет очков
				if(line != 0) {
					scoring_points(&state, &line);
					//устанавливаем скорость падения фигур
					if(state.level < 10) {
						speed_of_falling_figures(&state);
					}
					drawing_state_game(&level, &score, &state);
				}
			}		
		}
		s21_remove_matrix(&figure);
	
		
		win all_win[6] = {background, game_space, score, level, preview, record};
		for(int i = 0; i < 6; i++) {
        	        delwin(all_win[i].window);
	        }

	} while(!menu());	

	endwin();

	return 0;
}
