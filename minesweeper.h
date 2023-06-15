#pragma once

#include <string>
#include <vector>
/**
 * @brief Класс Minesweeper представляет игру Сапёр.
 */
class Minesweeper {
public:
    /**
 * @brief Структура, представляющая координаты ячейки.
 */
    struct Cell {
        size_t x = 0;
        size_t y = 0;
    };
    /**
     * @brief Перечисление, представляющее возможные статусы игры.
     */

    enum class GameStatus {
        NOT_STARTED,
        IN_PROGRESS,
        VICTORY,
        DEFEAT,
    };

    using RenderedField = std::vector<std::string>;
    /**
     * @brief Конструктор класса Minesweeper.
     * @param width Ширина игрового поля.
     * @param height Высота игрового поля.
     * @param mines_count Количество мин на поле.
     */
    Minesweeper(size_t width, size_t height, size_t mines_count);
    /**
     * @brief Конструктор класса Minesweeper.
     * @param width Ширина игрового поля.
     * @param height Высота игрового поля.
     * @param cells_with_mines Вектор ячеек с минами.
     */
    Minesweeper(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);
    /**
     * @brief Функция для начала новой игры.
     * @param width Ширина игрового поля.
     * @param height Высота игрового поля.
     * @param mines_count Количество мин на поле.
     */
    void NewGame(size_t width, size_t height, size_t mines_count);
    /**
     * @brief Функция для начала новой игры.
     * @param width Ширина игрового поля.
     * @param height Высота игрового поля.
     * @param cells_with_mines Вектор ячеек с минами.
     */
    void NewGame(size_t width, size_t height, const std::vector<Cell>& cells_with_mines);
    /**
     * @brief Функция для открытия ячейки.
     * @param cell Координаты ячейки для открытия.
     */
    void OpenCell(const Cell& cell);
    /**
     * @brief Функция для пометки ячейки флажком.
     * @param cell Координаты ячейки для пометки.
     */
    void MarkCell(const Cell& cell);
    /**
     * @brief Функция для получения текущего статуса игры.
     * @return Текущий статус игры.
     */
    GameStatus GetGameStatus() const;
    /**
     * @brief Функция для получения времени игры.
     * @return Время игры в секундах.
     */
    time_t GetGameTime() const;
    /**
     * @brief Функция для отрисовки игрового поля.
     * @return Вектор строк, представляющих отрисованное игровое поле.
     */
    RenderedField RenderField() const;
    const std::vector<Cell>& GetMines() const;

private:
    enum class CellStatus {
        Opened,
        Closed,
        Marked,
    };
    /**
     * @brief Функция для перезапуска игрового поля.
     */
    void RestartField();
    /**
 * @brief Функция для проверки окружения ячейки на наличие соседних ячеек.
 * @param x Координата X ячейки.
 * @param y Координата Y ячейки.
 * @return Вектор соседних ячеек.
 */
    std::vector<Cell> CheckEnvironment(size_t x, size_t y);
    /**
     * @brief Функция для завершения игры.
     * @param game_status Статус игры (победа или поражение).
     */
    void StopGame(GameStatus game_status);

    GameStatus game_status_ = GameStatus::NOT_STARTED; /**< Текущий статус игры. */
    time_t time_ = 0; /**< Время игры в секундах. */
    size_t width_; /**< Ширина игрового поля. */
    size_t height_; /**< Высота игрового поля. */
    size_t cells_count_; /**< Количество оставшихся закрытых ячеек. */

    std::vector<std::vector<char>> field_; /**< Матрица игрового поля. */
    std::vector<std::vector<CellStatus>> opened_; /**< Матрица статусов ячеек. */
    std::vector<Cell> mines_; /**< Вектор координат мин. */
};
