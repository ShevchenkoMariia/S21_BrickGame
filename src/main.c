#include "gui/cli/cli.h"

int main() {
  initializingGameWin();
  UserAction_t key;
  int startTime = 0;
  do {
    if (!isRunning) {
      infoControl();
      while (key != Start && key != Terminate) key = wgetch(feild.window);
      userInput(key, false);
      if (isRunning) startTime = clock();
    } else {
      GameInfo_t state = updateCurrentState();
      renderingGame(&state);
      if (!endOfTime(&startTime)) {
        key = wgetch(feild.window);
        userInput(key, false);
      }
    }
  } while (!exitGame);

  delWins();
  endwin();
  return 0;
}
