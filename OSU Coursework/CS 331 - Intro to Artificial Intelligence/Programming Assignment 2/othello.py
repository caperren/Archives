import argparse
import math
from copy import deepcopy


class Player:
    def __init__(self, player_number, board_symbol):
        self._player_number = player_number
        self._board_symbol = board_symbol

    @property
    def board_symbol(self):
        return self._board_symbol

    @property
    def player_number(self):
        return self._player_number

    def get_move(self, board):
        raise NotImplementedError


class HumanPlayer(Player):
    def __init__(self, player_number, board_symbol):
        super(HumanPlayer, self).__init__(player_number, board_symbol)

    def get_move(self, _):
        valid_input = False
        column = None
        row = None

        while not valid_input:
            try:
                column = int(input("Enter Column: "))
                row = int(input("Enter Row: "))
                valid_input = True
            except ValueError:
                print("Value entered was not an integer, please input column and row again...")

        return column, row


class MinimaxPlayer(Player):
    def __init__(self, player_number, board_symbol):
        super(MinimaxPlayer, self).__init__(player_number, board_symbol)

    def utility(self, board, player_symbol):
        if player_symbol == "X":
            return board.count_score("O") - board.count_score("X")
        else:
            return board.count_score("X") - board.count_score("O")

    def successors(self, board, player_symbol):
        successors = []

        for row in range(board.num_rows):
            for column in range(board.num_columns):
                if board.cell_empty(column, row):
                    if board.is_legal_move(column, row, player_symbol):
                        successors.append((column, row))

        return successors

    def minimax_max_value(self, board, current_player_symbol):
        if not board.has_legal_moves_remaining(current_player_symbol):
            return self.utility(board, current_player_symbol), (None, None)

        best_found_value = -math.inf, (None, None)

        for successor in self.successors(board, current_player_symbol):
            new_board = deepcopy(board)
            new_board.play_move(*successor, current_player_symbol)

            next_player_symbol = "O" if current_player_symbol == "X" else "X"

            new_board_minimax = self.minimax_min_value(new_board, next_player_symbol)

            if new_board_minimax[0] > best_found_value[0]:
                best_found_value = new_board_minimax[0], successor

        return best_found_value

    def minimax_min_value(self, board, current_player_symbol):
        if not board.has_legal_moves_remaining(current_player_symbol):
            return self.utility(board, current_player_symbol), (None, None)

        best_found_value = math.inf, (None, None)

        for successor in self.successors(board, current_player_symbol):
            new_board = deepcopy(board)
            new_board.play_move(*successor, current_player_symbol)

            next_player_symbol = "O" if current_player_symbol == "X" else "X"

            new_board_minimax = self.minimax_max_value(new_board, next_player_symbol)

            if new_board_minimax[0] < best_found_value[0]:
                best_found_value = new_board_minimax[0], successor

        return best_found_value

    def minimax_decision(self, board, current_player_symbol):
        minimax_result = self.minimax_max_value(board, current_player_symbol)

        return minimax_result[1]

    def get_move(self, board):
        return self.minimax_decision(board, self.board_symbol)


class OthelloBoard:
    EMPTY = "."
    DIRECTIONS = {
        "N": {"column": 0, "row": 1},
        "NE": {"column": 1, "row": 1},
        "E": {"column": 1, "row": 0},
        "SE": {"column": 1, "row": -1},
        "S": {"column": 0, "row": -1},
        "SW": {"column": -1, "row": -1},
        "W": {"column": -1, "row": 0},
        "NW": {"column": -1, "row": 1},
    }

    def __init__(self, num_columns, num_rows, player_1_board_symbol, player_2_board_symbol):
        self._columns = num_columns
        self._rows = num_rows

        self.grid = [[self.EMPTY for _ in range(self.num_rows)] for _ in range(self.num_columns)]

        self.set_cell(self.num_columns // 2, self.num_rows // 2, player_1_board_symbol)
        self.set_cell(self.num_columns // 2 - 1, self.num_rows // 2 - 1, player_1_board_symbol)
        self.set_cell(self.num_columns // 2 - 1, self.num_rows // 2, player_2_board_symbol)
        self.set_cell(self.num_columns // 2, self.num_rows // 2 - 1, player_2_board_symbol)

        self.display()

    @property
    def num_columns(self):
        return self._columns

    @property
    def num_rows(self):
        return self._rows

    def in_bounds(self, column, row):
        return (0 <= column < self.num_columns) and (0 <= row < self.num_rows)

    def get_cell(self, column, row):
        assert self.in_bounds(column, row)
        return self.grid[row][column]

    def set_cell(self, column, row, value):
        assert self.in_bounds(column, row)
        self.grid[row][column] = value

    def cell_empty(self, column, row):
        assert self.in_bounds(column, row)
        return self.grid[row][column] == self.EMPTY

    def display(self):
        for row_index in reversed(range(self.num_rows)):
            print("{}:| ".format(row_index), end="")
            for column_index in range(self.num_rows):
                print("{} ".format(self.get_cell(column_index, row_index)), end="")
            print()

        print("   -", end="")
        for _ in range(self.num_columns):
            print("--", end="")
        print()

        print("    ", end="")
        for column_index in range(self.num_columns):
            print("{} ".format(column_index), end="")
        print("\n")

    def set_coordinates_in_direction(self, column, row, direction):
        direction_offsets = self.DIRECTIONS[direction]

        return column + direction_offsets["column"], row + direction_offsets["row"]

    def check_endpoint(self, column, row, player_symbol, direction, match_symbol):
        if not self.in_bounds(column, row) or self.cell_empty(column, row):
            return False

        if match_symbol:
            if self.get_cell(column, row) == player_symbol:
                return True
            else:
                next_column, next_row = self.set_coordinates_in_direction(column, row, direction)
                return self.check_endpoint(next_column, next_row, player_symbol, direction, match_symbol)

        else:
            if self.get_cell(column, row) == player_symbol:
                return False
            else:
                next_column, next_row = self.set_coordinates_in_direction(column, row, direction)
                return self.check_endpoint(next_column, next_row, player_symbol, direction, not match_symbol)

    def flip_pieces_helper(self, column, row, player_symbol, direction):
        if self.get_cell(column, row) == player_symbol:
            return 0

        self.set_cell(column, row, player_symbol)
        next_column, next_row = self.set_coordinates_in_direction(column, row, direction)

        return 1 + self.flip_pieces_helper(next_column, next_row, player_symbol, direction)

    def flip_pieces(self, column, row, player_symbol):
        assert self.in_bounds(column, row)

        pieces_flipped = 0

        for direction in self.DIRECTIONS.keys():
            next_column, next_row = self.set_coordinates_in_direction(column, row, direction)
            if self.check_endpoint(next_column, next_row, player_symbol, direction, False):
                pieces_flipped += self.flip_pieces_helper(next_column, next_row, player_symbol, direction)

        return pieces_flipped

    def has_legal_moves_remaining(self, player_symbol):
        for row in range(self.num_rows):
            for column in range(self.num_columns):
                if self.cell_empty(column, row) and self.is_legal_move(column, row, player_symbol):
                    return True

        return False

    def is_legal_move(self, column, row, player_symbol):
        if not self.in_bounds(column, row) or not self.cell_empty(column, row):
            return False

        for direction in self.DIRECTIONS.keys():
            next_column, next_row = self.set_coordinates_in_direction(column, row, direction)
            if self.check_endpoint(next_column, next_row, player_symbol, direction, False):
                return True

        return False

    def play_move(self, column, row, player_symbol):
        self.set_cell(column, row, player_symbol)
        self.flip_pieces(column, row, player_symbol)

    def count_score(self, player_symbol):
        score = 0

        for row in self.grid:
            for current_column_value in row:
                if current_column_value == player_symbol:
                    score += 1

        return score


class GameDriver:
    VALID_PLAYER_TYPES = ["human", "minimax"]

    def __init__(self, player_1_type, player_2_type, num_columns, num_rows):
        self.player_1 = (HumanPlayer(1, "X") if player_1_type == "human" else MinimaxPlayer(1, "X"))
        self.player_2 = (HumanPlayer(2, "O") if player_2_type == "human" else MinimaxPlayer(2, "O"))

        self.board = OthelloBoard(num_columns, num_rows, self.player_1.board_symbol, self.player_2.board_symbol)

    def display(self):
        print()
        print("Player 1 ({}) score: {}".format(self.player_1.board_symbol,
                                               self.board.count_score(self.player_1.board_symbol)))
        print("Player 2 ({}) score: {}".format(self.player_2.board_symbol,
                                               self.board.count_score(self.player_2.board_symbol)))
        self.board.display()

    def process_move(self, current_player):
        invalid_move = True

        while invalid_move:
            column, row = current_player.get_move(self.board)

            if not self.board.is_legal_move(column, row, current_player.board_symbol):
                print("Invalid move")
            else:
                print("Selected move: col = {}, row = {}".format(column, row))
                self.board.play_move(column, row, current_player.board_symbol)
                invalid_move = False

    def run(self):
        game_running = True

        current_player = self.player_1
        cant_move_counter = 0

        while game_running:
            print("Player {} ({}) move:".format(current_player.player_number, current_player.board_symbol))

            if self.board.has_legal_moves_remaining(current_player.board_symbol):
                cant_move_counter = 0
                self.process_move(current_player)
                self.display()
            else:
                print("No moves available for player {}".format(current_player.player_number))
                cant_move_counter += 1

                if cant_move_counter == 2:
                    game_running = False

            current_player = (self.player_1 if current_player == self.player_2 else self.player_2)

        player_1_score = self.board.count_score(self.player_1.board_symbol)
        player_2_score = self.board.count_score(self.player_2.board_symbol)

        print()

        if player_1_score == player_2_score:
            print("Tie Game")
        elif player_1_score > player_2_score:
            print("Player 1 Wins")
        else:
            print("Player 2 Wins")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="A command line version of the Othello board game")

    parser.add_argument("player_1_type", choices=["human", "minimax"], help="Type for player 1")
    parser.add_argument("player_2_type", choices=["human", "minimax"], help="Type for player 2")
    args = parser.parse_args()

    game = GameDriver(args.player_1_type, args.player_2_type, 4, 4)
    game.run()
