#include <curses.h>
#include <time.h>
#include <tuple>

void print_sudoku(int puzzle[9][9]) {
	std::string puzzle_string;
	for (int a = 0; a < 9; a++) {
		for (int b = 0; b < 9; b++) {
			puzzle_string.append(std::to_string(puzzle[a][b])).append("  ");
		}
		puzzle_string.append("\n");
	}
	mvaddstr(0, 0, puzzle_string.c_str());
	refresh();
}

bool is_valid_move(int puzzle[9][9], int i, int j, int num) {
	// num in row?
	for (int a = 0; a < 9; a++) {
		if (puzzle[a][j] == num) {
			return false;
		}
	}
	// num in col?
	for (int b = 0; b < 9; b++) {
		if (puzzle[i][b] == num) {
			return false;
		}
	}
	// num in 3x3 subsection
	int sub_i = i - i % 3;
	int sub_j = j - j % 3;
	for (int a = sub_i; a < sub_i + 3; a++) {
		for (int b = sub_j; b < sub_j + 3; b++) {
			if (puzzle[a][b] == num) {
				return false;
			}
		}
	}
	return true;
}

std::tuple<int, int> find_empty_field(int puzzle[9][9]) {
	for (int a = 0; a < 9; a++) {
		for (int b = 0; b < 9; b++) {
			if (puzzle[a][b] == 0) {
				return std::make_tuple(a, b);
			}
		}
	}
	return std::make_tuple(-1, -1);
}

bool solve_sudoku(int puzzle[9][9]) {
	std::tuple<int, int> coords = find_empty_field(puzzle);
	int i = std::get<0>(coords);
	int j = std::get<1>(coords);
	if (i == -1) {
		return true;
	}
	for (int num = 1; num < 10; num++) {
		if (is_valid_move(puzzle, i, j, num)) {
			puzzle[i][j] = num;
			print_sudoku(puzzle);
			if (solve_sudoku(puzzle)) {
				return true;
			} else {
				puzzle[i][j] = 0;
			}
		}
	}
	return false;
}

int main() {
	int puzzle[9][9] = {{0, 1, 5, 0, 9, 4, 0, 0, 0},
						{0, 0, 0, 6, 0, 0, 0, 3, 0},
						{0, 7, 0, 0, 5, 0, 0, 0, 0},
						{4, 0, 0, 5, 0, 0, 0, 8, 2},
						{0, 0, 1, 0, 0, 0, 9, 0, 0},
						{5, 2, 0, 0, 0, 9, 0, 0, 1},
						{0, 0, 0, 0, 8, 0, 0, 5, 0},
						{0, 5, 0, 0, 0, 7, 0, 0, 0},
						{0, 0, 0, 3, 6, 0, 8, 2, 0}};

	initscr();
	noecho();
	cbreak();

	clock_t t1, t2;
	t1 = clock();
	solve_sudoku(puzzle);
	t2 = clock();
	float diff((float)t2 - (float)t1);
	addstr(std::to_string(diff / CLOCKS_PER_SEC).c_str());
	if (getch() == 'q') {
		endwin();
	}
}
