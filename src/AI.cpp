/*
** EPITECH PROJECT, 2022
** AI.cpp
** File description:
** AI.cpp
*/

#include "AI.hpp"
#include "BrainCommand.hpp"
#include "define.hpp"
#include "Printer.hpp"

#include <algorithm>
#include <cmath>

void AI::turn(Board &board)
{
    std::size_t x = std::rand()%static_cast<int>(std::ceil(DEFAULT_BOARD_SIZE/3)) + static_cast<int>(std::ceil(DEFAULT_BOARD_SIZE/3));
    std::size_t y = std::rand()%static_cast<int>(std::ceil(DEFAULT_BOARD_SIZE/3)) + static_cast<int>(std::ceil(DEFAULT_BOARD_SIZE/3));
    std::srand(time(NULL));

    board.setPos(Board::CellState::SECOND_PLAYER, x, y);
    Printer::print(x, ",", y);
    // board.printBoard();
}

void AI::turn(Board &board, std::size_t playerX, std::size_t playerY)
{
    std::size_t x = 0, y = 0;
    std::pair<std::size_t, std::size_t> fieldCells = board.getFieldCell();

    if (fieldCells.first == 1 && fieldCells.second == 0) {
        _start(board, x, y, playerX, playerY);
    } else {
        if (_attack(board, x, y, playerX, playerY) == false) {
            if (_defend(board, x, y, playerX, playerY) == false) {
                x = std::rand()%DEFAULT_BOARD_SIZE;
                y = std::rand()%DEFAULT_BOARD_SIZE;
                while (!board.setPos(Board::CellState::SECOND_PLAYER, x, y)) {
                    x = std::rand()%DEFAULT_BOARD_SIZE;
                    y = std::rand()%DEFAULT_BOARD_SIZE;
                }
            }
        }
        board.setPos(Board::CellState::SECOND_PLAYER, x, y);
    }
    Printer::print(x, ",", y);
    // board.printBoard();
}

void AI::_start(Board &board, std::size_t &x, std::size_t &y, std::size_t playerX, std::size_t playerY)
{
    std::vector<int> possibleX = {-1, 0, 1}, possibleY = {-1, 0, 1};
    std::srand(time(NULL));

    if (playerX == 0)
        possibleX.erase(std::find(possibleX.begin(), possibleX.end(), -1));
    if (playerX == DEFAULT_BOARD_SIZE - 1)
        possibleX.erase(std::find(possibleX.begin(), possibleX.end(), 1));
    if (playerY == 0)
        possibleY.erase(std::find(possibleY.begin(), possibleY.end(), -1));
    if (playerY == DEFAULT_BOARD_SIZE - 1)
        possibleY.erase(std::find(possibleY.begin(), possibleY.end(), 1));
    x = playerX + possibleX.at(std::rand()%possibleX.size());
    y = playerY + possibleY.at(std::rand()%possibleY.size());
    while (!board.setPos(Board::CellState::SECOND_PLAYER, x, y)) {
        x = playerX + possibleX.at(std::rand()%possibleX.size());
        y = playerY + possibleY.at(std::rand()%possibleY.size());
    }
}

bool AI::_attack(Board &board, std::size_t &x, std::size_t &y, std::size_t playerX, std::size_t playerY)
{
    std::vector<Board::CellAttribute> line = {};
    std::vector<Board::Direction> directions = {Board::Direction::HORIZONTAL, Board::Direction::VERTICAL,
                                                Board::Direction::LEFTTORIGHT, Board::Direction::RIGHTTOLEFT};

    for (auto &dir : directions) {
        line = board.getLine(dir, playerX, playerY);
        if (line.size() < 5)
            continue;
        for (std::size_t lineIndex = 0; line.size() - lineIndex > 5; lineIndex++) {
            for (auto &pattern : Board::attackPattern) {
                for (std::size_t ptnIndex = 0; ptnIndex < 5; ptnIndex++) {
                    if (line.at(lineIndex + ptnIndex).field != pattern.at(ptnIndex))
                        break;
                    if (line.at(lineIndex + ptnIndex).field == Board::CellState::EMPTY) {
                        x = line.at(lineIndex + ptnIndex).posX;
                        y = line.at(lineIndex + ptnIndex).posY;
                    }
                    if (ptnIndex == 4)
                        return (true);
                }
            }
        }
    }
    return (false);
}

bool AI::_defend(Board &board, std::size_t &x, std::size_t &y, std::size_t playerX, std::size_t playerY)
{
    std::vector<Board::CellAttribute> line = {};
    std::vector<Board::Direction> directions = {Board::Direction::HORIZONTAL, Board::Direction::VERTICAL,
                                                Board::Direction::LEFTTORIGHT, Board::Direction::RIGHTTOLEFT};

    for (auto &dir : directions) {
        line = board.getLine(dir, playerX, playerY);
        if (line.size() < 5)
            continue;
        for (std::size_t lineIndex = 0; line.size() - lineIndex > 5; lineIndex++) {
            for (auto &pattern : Board::defensePattern) {
                for (std::size_t ptnIndex = 0; ptnIndex < 5; ptnIndex++) {
                    if (line.at(lineIndex + ptnIndex).field != pattern.at(ptnIndex))
                        break;
                    if (line.at(lineIndex + ptnIndex).field == Board::CellState::EMPTY) {
                        x = line.at(lineIndex + ptnIndex).posX;
                        y = line.at(lineIndex + ptnIndex).posY;
                    }
                    if (ptnIndex == 4)
                        return (true);
                }
            }
        }
    }
    return (false);
}
