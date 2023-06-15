#include <iostream>
#include "minesweeper.h"
/**
 * @brief Главная функция программы.
 *
 * @return Возвращаемое значение (cтатус завершения программы).
 */
int main() {
    Minesweeper minesweeper(10, 10, 15);

    while (minesweeper.GetGameStatus() != Minesweeper::GameStatus::DEFEAT && minesweeper.GetGameStatus() != Minesweeper::GameStatus::VICTORY) {
        for (const auto& line : minesweeper.RenderField()) {
            std::cout << line << '\n';
        }
        std::cout << "Mark or Open?" << std::endl;
        char action;
        std::cin >> action;

        size_t y;
        size_t x;
        std::cin >> y >> x;
        std::cout << std::endl;
        --y, --x;

        if (action == 'M') {
            minesweeper.MarkCell({x, y});
        } else if (action == 'O') {
            minesweeper.OpenCell({x, y});
        } else {
            continue;
        }
    }

    std::cout << minesweeper.GetGameTime() << std::endl;

}
