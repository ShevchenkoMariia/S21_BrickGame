#include <stdlib.h>
#include <string.h>
#include <ncurses.h> // Подключаем библиотеку

typedef struct window {
        const unsigned int rows;
        const unsigned int columns;
        const unsigned int y;
        const unsigned int x;
	WINDOW* window;
} win;


int main() {

	int arr[6][6] = {
		{0, 0, 0, 0, 0, 0},
		{1, 1, 0, 0, 0, 0},
		{1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0}
	};
	
	int arr5[6][6];

	for(int i = 0; i < 6; i++) {
		for(int j = 0; j < 6; j++) {
			printf("%d ", arr[i][j]);
		}
		printf("\n");
	}

	printf("\n");
		
	for(int i = 0, jj = 0; i < 6; i++, jj++) {
                for(int j = 0, ii = 5; j < 6; j++, ii--) {
			arr5[i][j] = arr[ii][jj];
			printf("%d ", arr5[i][j]);
		} 
		printf("\n");
	}

	int arr6[6][6];
	printf("\n");

	for(int i = 0, jj = 0; i < 6; i++, jj++) {
                for(int j = 0, ii = 5; j < 6; j++, ii--) {
                        arr6[i][j] = arr5[ii][jj];
                        printf("%d ", arr6[i][j]);
                }
                printf("\n");
        }


	return 0;
}
