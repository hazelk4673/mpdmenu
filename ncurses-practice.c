
#include <ncurses.h>

int main() {
  int c;

  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();

  printw("type any character to see it in bold\n");
  c = getch();

  if (c == KEY_F(1)) printw("F1 Key pressed");
  else {
    printw("pressed key is ");
    attron(A_BOLD);
    printw("%c", c);
    attroff(A_BOLD);
  }
  refresh();
  getch();
  endwin();

  return 0;
}
