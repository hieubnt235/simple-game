#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <vector>

using std::size_t;

constexpr size_t elem_rc(size_t row, size_t col, size_t cols)
{
	return row * cols + col;
}

constexpr size_t try_wrap(std::int64_t index, std::int64_t dim)
{
	return ((index % dim) + dim) % dim;
}


size_t alive_neighbors(
	size_t row,
	size_t col,
	size_t rows,
	size_t cols,
	const std::vector<bool> & board)
{
	struct Adjust
	{
		std::int64_t r;
		std::int64_t c;
	};
	constexpr std::array<Adjust, 8> adjusts{
		Adjust{0, -1}, Adjust{-1, -1}, Adjust{-1, 0}, Adjust{-1, 1},
		Adjust{0, 1}, Adjust{1, 1}, Adjust{1, 0}, Adjust{1, -1}
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
	size_t row,
	size_t col,
	size_t rows,
	size_t cols,
	const std::vector<bool> & board
	)
{
	return board[elem_rc(row, col, cols)] && alive_neighbors(
		       row, col, rows, cols, board) < 2;
}

bool rule_2(
	size_t row,
	size_t col,
	size_t rows,
	size_t cols,
	const std::vector<bool> & board)
{
	auto neighbors{alive_neighbors(row, col, rows, cols, board)};
	return board[elem_rc(row, col, cols)] && (neighbors == 2 || neighbors == 3);
}

bool rule_3(
	size_t row,
	size_t col,
	size_t rows,
	size_t cols,
	const std::vector<bool> & board)
{
	return board[elem_rc(row, col, cols)] && alive_neighbors(
		       row, col, rows, cols, board) > 3;
}

bool rule_4(
	size_t row,
	size_t col,
	size_t rows,
	size_t cols,
	const std::vector<bool> & board)
{
	return !board[elem_rc(row, col, cols)] && alive_neighbors(
		       row, col, rows, cols, board) == 3;
}

void update_board(size_t rows, size_t cols, std::vector<bool> & board)
{
	auto original_board{board};
	for (size_t row{0}; row < rows; ++row) {
		for (size_t col{0}; col < cols; ++col) {
			if (rule_1(row, col, rows, cols, original_board) ||
			    rule_3(row, col, rows, cols, original_board)) {
				board[elem_rc(row, col, cols)] = false;
			} else if (rule_2(row, col, rows, cols, original_board) ||
			           rule_4(row, col, rows, cols, original_board)) {
				board[elem_rc(row, col, cols)] = true;
			}
		}
	}
}

void print_board(size_t rows, size_t cols, const std::vector<bool> & board)
{
	constexpr std::array<char, 2> alive_dead_display{'*', '@'};
	for (size_t row{0}; row < rows; ++row) {
		for (size_t col{0}; col < cols; ++col) {
			std::cout << alive_dead_display[board[elem_rc(row, col, cols)]];
		}
		std::cout << '\n';
	}
}

[[noreturn]] int main()
{
	constexpr size_t cols{40};
	constexpr size_t rows{20};

	std::vector<bool> board(rows * cols, false);

	// glider
	board[elem_rc(10, 7, cols)] = true;
	board[elem_rc(11, 8, cols)] = true;
	board[elem_rc(11, 9, cols)] = true;
	board[elem_rc(10, 9, cols)] = true;
	board[elem_rc(9, 9, cols)] = true;

	for (;;) {
		std::cout << "\33[H\33[2J\33[3J";
		print_board(rows, cols, board);
		update_board(rows, cols, board);
		std::cin.get();
	}
}