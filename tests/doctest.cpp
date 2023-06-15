#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "../minesweeper.h"
#include <thread>

TEST_CASE("Minesweeper::NewGame") {
    SUBCASE("Random mines placement") {
        Minesweeper game(3, 3, 1);
        std::vector<Minesweeper::Cell> mines = game.GetMines();
        CHECK(mines.size() == 1);
        CHECK(mines[0].x >= 0);
        CHECK(mines[0].x < 3);
        CHECK(mines[0].y >= 0);
        CHECK(mines[0].y < 3);
    }

    SUBCASE("Custom mines placement") {
        std::vector<Minesweeper::Cell> mines = {{0, 0}, {2, 2}};
        Minesweeper game(3, 3, mines);
        std::vector<Minesweeper::Cell> result = game.GetMines();
        CHECK(result.size() == 2);
        CHECK(result[0].x == mines[0].x);
        CHECK(result[0].y == mines[0].y);
        CHECK(result[1].x == mines[1].x);
        CHECK(result[1].y == mines[1].y);
    }
}

TEST_CASE("Minesweeper::OpenCell") {
    Minesweeper game(3, 3, {{0, 0}, {2, 2}});
    SUBCASE("Opened cell is marked") {
        game.MarkCell({0, 0});
        game.OpenCell({0, 0});
        CHECK(game.RenderField()[0][0] == '?');
    }

    SUBCASE("Opened cell is a number") {
        game.OpenCell({1, 1});
        CHECK(game.RenderField()[1][1] == '2');
    }

    SUBCASE("Opened cell is a zero") {
        game.OpenCell({2, 0});
        CHECK(game.RenderField()[2][0] == '.');
        CHECK(game.RenderField() == std::vector<std::string>({"-1.", "-21", "---"}));
    }
}

TEST_CASE("Minesweeper::MarkCell") {
    Minesweeper game(3, 3, {{0, 0}, {2, 2}});
    SUBCASE("Marked cell is unmarked") {
        game.MarkCell({0, 0});
        CHECK(game.RenderField()[0][0] == '?');
    }

    SUBCASE("Marked cell is opened") {
        game.OpenCell({1, 1});
        game.MarkCell({1, 1});
        CHECK(game.RenderField()[1][1] == '2');
    }

    SUBCASE("Marked cell is marked") {
        game.MarkCell({0, 0});
        CHECK(game.RenderField()[0][0] == '?');
        game.MarkCell({0, 0});
        CHECK(game.RenderField()[0][0] == '-');
    }
}

TEST_CASE("Minesweeper::GetGameStatus") {
    Minesweeper minesweeper(3, 3, {{0, 0}});
    SUBCASE("Not started") {
        CHECK(minesweeper.GetGameStatus() == Minesweeper::GameStatus::NOT_STARTED);
    }

    SUBCASE("Defeat") {
        minesweeper.OpenCell({0, 0});
        CHECK(minesweeper.GetGameStatus() == Minesweeper::GameStatus::DEFEAT);
    }

    SUBCASE("In progress") {
        minesweeper.OpenCell({1, 1});
        CHECK(minesweeper.GetGameStatus() == Minesweeper::GameStatus::IN_PROGRESS);
    }

    SUBCASE("Victory") {
        minesweeper.OpenCell({2, 2});
        CHECK(minesweeper.GetGameStatus() == Minesweeper::GameStatus::VICTORY);
    }
}

TEST_CASE("Minesweeper::GetGameTime") {
    Minesweeper game(3, 3, {{0, 0}, {2, 2}});
    SUBCASE("Game has not started") {
        CHECK(game.GetGameTime() == 0);
    }

    SUBCASE("Game is in progress") {
        game.OpenCell({1, 1});
        std::this_thread::sleep_for(std::chrono::seconds(1));
        CHECK(game.GetGameTime() == 1);
    }

    SUBCASE("Game is over") {
        game.OpenCell({0, 0});
        CHECK(game.GetGameTime() > 0);
    }
}

TEST_CASE("Minesweeper::RenderField") {
    Minesweeper game(3, 3, {{0, 0}});
    SUBCASE("Game in progress") {
        game.MarkCell({0, 0});
        game.OpenCell({1, 1});
        Minesweeper::RenderedField expected_field = {
                {"?--"},
                {"-1-"},
                {"---"},
        };
        CHECK(game.RenderField() == expected_field);
    }

    SUBCASE("Game defeated") {
        game.OpenCell({0, 0});
        Minesweeper::RenderedField expected_field = {
                {"*1."},
                {"11."},
                {"..."},
        };
        CHECK(game.RenderField() == expected_field);
    }
}