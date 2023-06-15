#include "minesweeper.h"

#include <ctime>
#include <deque>
#include <random>
/**
 * @brief Функция для перезапуска игрового поля.
 */
void Minesweeper::RestartField() {
    field_.assign(height_, std::vector<char>(width_, '0'));
    opened_.assign(height_, std::vector<CellStatus>(width_, CellStatus::Closed));
}
/**
* @brief Функция для проверки окружения ячейки на наличие соседних ячеек.
* @param x Координата X ячейки.
* @param y Координата Y ячейки.
* @return Вектор соседних ячеек.
*/
std::vector<Minesweeper::Cell> Minesweeper::CheckEnvironment(size_t x, size_t y) {
    std::vector<Cell> result;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (y + dy < height_ && x + dx < width_ && !(dy == 0 && dx == 0)) {
                result.push_back({x + dx, y + dy});
            }
        }
    }
    return result;
}
/**
 * @brief Функция для завершения игры.
 * @param game_status Статус игры (поражение или победа).
 */
void Minesweeper::StopGame(GameStatus game_status) {
    game_status_ = game_status;
    time_ = std::time(nullptr) - time_;

    if (game_status == GameStatus::DEFEAT) {
        opened_.assign(height_, std::vector<CellStatus>(width_, CellStatus::Opened));
    }
}
/**
 * @brief Функция для начала новой игры.
 * @param width Ширина игрового поля.
 * @param height Высота игрового поля.
 * @param mines_count Количество мин на поле.
 */
void Minesweeper::NewGame(size_t width, size_t height, size_t mines_count) {
    std::vector<size_t> random_vector(width * height);
    for (size_t i = 0; i < width * height; ++i) {
        random_vector[i] = i;
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(random_vector.begin(), random_vector.end(), g);
    std::vector<Cell> cells_with_mines;

    for (size_t i = 0; i < mines_count; ++i) {
        size_t y = random_vector[i] % height;
        size_t x = (random_vector[i] / height) % width;
        cells_with_mines.push_back({x, y});
    }
    Minesweeper::NewGame(width, height, cells_with_mines);
}
/**
 * @brief Функция для начала новой игры.
 * @param width Ширина игрового поля.
 * @param height Высота игрового поля.
 * @param cells_with_mines Вектор ячеек с минами.
 */
void Minesweeper::NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    game_status_ = GameStatus::NOT_STARTED;
    width_ = width;
    height_ = height;
    cells_count_ = width * height - cells_with_mines.size();
    mines_ = cells_with_mines;
    RestartField();

    for (const auto& [x, y] : cells_with_mines) {
        field_[y][x] = '*';
        for (const auto& cell : CheckEnvironment(x, y)) {
            if (field_[cell.y][cell.x] != '*') {
                ++field_[cell.y][cell.x];
            }
        }
    }
}
/**
 * @brief Конструктор класса Minesweeper.
 * @param width Ширина игрового поля.
 * @param height Высота игрового поля.
 * @param mines_count Количество мин на поле.
 */
Minesweeper::Minesweeper(size_t width, size_t height, size_t mines_count) {
    NewGame(width, height, mines_count);
}
/**
 * @brief Конструктор класса Minesweeper.
 * @param width Ширина игрового поля.
 * @param height Высота игрового поля.
 * @param cells_with_mines Вектор ячеек с минами.
 */
Minesweeper::Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines) {
    NewGame(width, height, cells_with_mines);
}
/**
 * @brief Функция для открытия ячейки.
 * @param cell Координаты ячейки для открытия.
 */
void Minesweeper::OpenCell(const Cell& cell) {
    if (game_status_ == GameStatus::DEFEAT || game_status_ == GameStatus::VICTORY) {
        return;
    }
    if (game_status_ == GameStatus::NOT_STARTED) {
        game_status_ = GameStatus::IN_PROGRESS;
        time_ = std::time(nullptr);
    }

    if (opened_[cell.y][cell.x] == CellStatus::Marked || opened_[cell.y][cell.x] == CellStatus::Opened) {
        return;
    }
    if (field_[cell.y][cell.x] == '*') {
        StopGame(GameStatus::DEFEAT);
        return;
    }

    std::deque<Cell> queue;
    queue.push_back({cell});
    while (!queue.empty()) {
        size_t x = queue[0].x;
        size_t y = queue[0].y;
        if (opened_[y][x] == CellStatus::Closed) {
            --cells_count_;
        }
        opened_[y][x] = CellStatus::Opened;
        queue.pop_front();
        if (field_[y][x] == '0') {
            for (const auto& cell_temp : CheckEnvironment(x, y)) {
                if (opened_[cell_temp.y][cell_temp.x] == CellStatus::Closed) {
                    queue.push_back({cell_temp.x, cell_temp.y});
                }
            }
        }
    }

    if (cells_count_ == 0) {
        StopGame(GameStatus::VICTORY);
    }
}
/**
 * @brief Функция для пометки ячейки флажком.
 * @param cell Координаты ячейки для пометки.
 */
void Minesweeper::MarkCell(const Cell& cell) {
    if (game_status_ == GameStatus::DEFEAT || game_status_ == GameStatus::VICTORY) {
        return;
    }
    if (game_status_ == GameStatus::NOT_STARTED) {
        game_status_ = GameStatus::IN_PROGRESS;
        time_ = std::time(nullptr);
    }

    if (opened_[cell.y][cell.x] == CellStatus::Closed) {
        opened_[cell.y][cell.x] = CellStatus::Marked;
    } else if (opened_[cell.y][cell.x] == CellStatus::Marked) {
        opened_[cell.y][cell.x] = CellStatus::Closed;
    }
}
/**
 * @brief Функция для получения текущего статуса игры.
 * @return Текущий статус игры.
 */
Minesweeper::GameStatus Minesweeper::GetGameStatus() const {
    return game_status_;
}
/**
 * @brief Функция для получения времени игры.
 * @return Время игры.
 */
time_t Minesweeper::GetGameTime() const {
    if (game_status_ == GameStatus::IN_PROGRESS) {
        return std::time(nullptr) - time_;
    } else if (game_status_ == GameStatus::NOT_STARTED) {
        return 0;
    } else {
        return time_;
    }
}
/**
 * @brief Функция для отображения игрового поля.
 * @return Вектор строк, представляющих игровое поле.
 */
Minesweeper::RenderedField Minesweeper::RenderField() const {
    Minesweeper::RenderedField rendered_field;
    for (size_t row = 0; row < height_; ++row) {
        rendered_field.push_back({});
        for (size_t col = 0; col < width_; ++col) {
            if (opened_[row][col] == CellStatus::Closed) {
                rendered_field[row] += '-';
            } else if (opened_[row][col] == CellStatus::Marked) {
                rendered_field[row] += '?';
            } else if (field_[row][col] == '0') {
                rendered_field[row] += '.';
            } else {
                rendered_field[row] += field_[row][col];
            }
        }
    }
    return rendered_field;
}
/**
 * @brief Функция для получения координат мин.
 * @return Вектор ячейк, возвращаемых координаты мин.
 */
const std::vector<Minesweeper::Cell>& Minesweeper::GetMines() const {
    return mines_;
}
