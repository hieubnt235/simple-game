#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <vector>
# include <timer.h>

using std::size_t;

constexpr size_t elem_rc(const size_t row, const size_t col, const size_t cols)
{
	return row * cols + col;
}

constexpr size_t try_wrap(const std::int64_t index, const std::int64_t dim)
{
	return ((index % dim) + dim) % dim;
}


size_t alive_neighbors(
	const size_t row,
	const size_t col,
	const size_t rows,
	const size_t cols,
	const std::vector<bool> &board)
{
	struct Adjust {
		std::int64_t r;
		std::int64_t c;
	};
	constexpr std::array<Adjust, 8> adjusts{
		Adjust{ 0, -1 }, Adjust{ -1, -1 }, Adjust{ -1, 0 },
		Adjust{ -1, 1 },
		Adjust{ 0, 1 }, Adjust{ 1, 1 }, Adjust{ 1, 0 }, Adjust{ 1, -1 }
	};
	const auto living = std::count_if(
		std::begin(adjusts),
		std::end(adjusts),
		[&](auto adj) {
			return board[elem_rc(
				try_wrap(
					static_cast<std::int64_t>(row)
					+
					adj.r,
					static_cast<std::int64_t>(
						rows)),
				try_wrap(
					static_cast<std::int64_t>(col)
					+
					adj.c,
					static_cast<std::int64_t>(
						cols)),
				cols)];
		});
	return living;
}

bool rule_1(
	const size_t row,
	const size_t col,
	const size_t rows,
	const size_t cols,
	const std::vector<bool> &board
	)
{
	return board[elem_rc(row, col, cols)] && alive_neighbors(
		       row, col, rows, cols, board) < 2;
}

bool rule_2(
	const size_t row,
	const size_t col,
	const size_t rows,
	const size_t cols,
	const std::vector<bool> &board)
{
	const auto neighbors{ alive_neighbors(row, col, rows, cols, board) };
	return board[elem_rc(row, col, cols)] && (
		       neighbors == 2 || neighbors == 3);
}

bool rule_3(
	const size_t row,
	const size_t col,
	const size_t rows,
	const size_t cols,
	const std::vector<bool> &board)
{
	return board[elem_rc(row, col, cols)] && alive_neighbors(
		       row, col, rows, cols, board) > 3;
}

bool rule_4(
	const size_t row,
	const size_t col,
	const size_t rows,
	const size_t cols,
	const std::vector<bool> &board)
{
	return !board[elem_rc(row, col, cols)] && alive_neighbors(
		       row, col, rows, cols, board) == 3;
}

void update_board(const size_t rows, const size_t cols,
                  std::vector<bool> &board)
{
	const auto original_board{ board };
	for (size_t row{ 0 }; row < rows; ++row) {
		for (size_t col{ 0 }; col < cols; ++col) {
			if (rule_1(row, col, rows, cols, original_board) ||
			    rule_3(row, col, rows, cols, original_board)) {
				board[elem_rc(row, col, cols)] = false;
			} else if (
				rule_2(row, col, rows, cols, original_board) ||
				rule_4(row, col, rows, cols, original_board)) {
				board[elem_rc(row, col, cols)] = true;
			}
		}
	}
}

void print_board(const size_t rows, const size_t cols,
                 const std::vector<bool> &board)
{
	for (size_t row{ 0 }; row < rows; ++row) {
		constexpr std::array<char, 2> alive_dead_display{ '*', '@' };

		for (size_t col{ 0 }; col < cols; ++col) {
			std::cout << alive_dead_display[board[elem_rc(
				row, col, cols)]];
		}
		std::cout << '\n';
	}
}

[[noreturn]] int main()
{
	timer_lib_initialize();
	constexpr size_t cols{ 40 };
	constexpr size_t rows{ 27 };

	std::vector<bool> board(rows * cols, false);

	// glider
	board[elem_rc(5, 2, cols)] = true;
	board[elem_rc(6, 2, cols)] = true;
	board[elem_rc(5, 3, cols)] = true;
	board[elem_rc(6, 3, cols)] = true;

	board[elem_rc(5, 12, cols)] = true;
	board[elem_rc(6, 12, cols)] = true;
	board[elem_rc(7, 12, cols)] = true;
	board[elem_rc(4, 13, cols)] = true;
	board[elem_rc(8, 13, cols)] = true;
	board[elem_rc(3, 14, cols)] = true;
	board[elem_rc(9, 14, cols)] = true;
	board[elem_rc(3, 15, cols)] = true;
	board[elem_rc(9, 15, cols)] = true;
	board[elem_rc(6, 16, cols)] = true;
	board[elem_rc(4, 17, cols)] = true;
	board[elem_rc(8, 17, cols)] = true;
	board[elem_rc(5, 18, cols)] = true;
	board[elem_rc(6, 18, cols)] = true;
	board[elem_rc(7, 18, cols)] = true;
	board[elem_rc(6, 19, cols)] = true;

	board[elem_rc(3, 22, cols)] = true;
	board[elem_rc(4, 22, cols)] = true;
	board[elem_rc(5, 22, cols)] = true;
	board[elem_rc(3, 23, cols)] = true;
	board[elem_rc(4, 23, cols)] = true;
	board[elem_rc(5, 23, cols)] = true;
	board[elem_rc(2, 24, cols)] = true;
	board[elem_rc(6, 24, cols)] = true;
	board[elem_rc(1, 26, cols)] = true;
	board[elem_rc(2, 26, cols)] = true;
	board[elem_rc(6, 26, cols)] = true;
	board[elem_rc(7, 26, cols)] = true;

	board[elem_rc(3, 36, cols)] = true;
	board[elem_rc(4, 36, cols)] = true;
	board[elem_rc(3, 37, cols)] = true;
	board[elem_rc(4, 37, cols)] = true;

	// eater
	board[elem_rc(20, 27, cols)] = true;
	board[elem_rc(21, 27, cols)] = true;
	board[elem_rc(20, 28, cols)] = true;
	board[elem_rc(21, 28, cols)] = true;

	board[elem_rc(21, 32, cols)] = true;
	board[elem_rc(22, 31, cols)] = true;
	board[elem_rc(22, 33, cols)] = true;
	board[elem_rc(23, 32, cols)] = true;

	board[elem_rc(23, 34, cols)] = true;
	board[elem_rc(24, 34, cols)] = true;
	board[elem_rc(25, 34, cols)] = true;
	board[elem_rc(25, 35, cols)] = true;

	for (;;) {
		constexpr double delay = 0.1; // wait for 1/10th of a second
		const tick_t time = timer_current();
		printf("\33[H\33[2J\33[3J");
		print_board(rows, cols, board);
		update_board(rows, cols, board);
		while (timer_elapsed(time) < delay) {
			// busy wait (ideally cross-platform sleep for some of this...)
		}
	}
	timer_lib_shutdown();
}