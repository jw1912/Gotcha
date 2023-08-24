#pragma once

#include "core.hpp"
#include "hash.hpp"

struct Group
{
    Group(Tile tile, Colour colour)
    {
        belongsTo = colour;
        stones = LinkHead(tile, tile, 1);
        liberties = 0;
        hash = Zobrist::hashFor(tile, colour);
    }

    void join(Group& other, std::vector<LinkNode>& tiles)
    {
        liberties += other.liberties - 1;
        stones.join(other.stones, tiles);
        hash ^= other.hash;
    }

    Colour belongsTo;
    LinkHead stones;
    std::uint16_t liberties;
    Zobrist hash;
};

class BoardState
{
    public:
        BoardState(const std::uint16_t withSize);

        BoardState() { BoardState(5); };

        bool placeStone(const Tile tile, Colour colour);

        void killGroup(const std::uint16_t groupId);

        void display(const bool showGroups) const;

        void passMove() { passes++; }

        State gameState(Colour stm, float komi) const;

        [[nodiscard]] auto isGameOver() const { return passes >= 2; }
        [[nodiscard]] auto sizeOf() const { return size * size; }
        [[nodiscard]] auto getHash() const { return hash; }
        [[nodiscard]] auto moveHead() const { return empty; }
        [[nodiscard]] auto numStones() const { return stones; }
        [[nodiscard]] auto operator[](Tile tile) const { return tiles[tile.index()]; }

    private:
        LinkHead empty;
        std::uint16_t passes;
        std::uint16_t size;
        std::vector<LinkNode> tiles;
        std::vector<Group> groups;
        std::array<std::uint16_t, 2> stones;
        Zobrist hash;
};

class Board
{
    public:
        Board(const std::uint16_t withSize)
        {
            board = BoardState(withSize);
            setStm(Colour::Black);
        }

        bool tryMakeMove(const Tile tile);

        void setStm(Colour colour) { stm = colour; }

        void setKomi(float val) { komi = val; }

        void undoMove()
        {
            stm = flipColour(stm);
            board = history.back();
            history.pop_back();
        }

        std::vector<Tile> moveList(std::vector<Tile> used) const;

        void display(const bool showGroups) const;

        std::uint64_t runPerft(uint8_t depth);

        [[nodiscard]] auto stones() const { return board.numStones(); }
        [[nodiscard]] auto gameState() const { return board.gameState(stm, komi); }

    private:
        BoardState board;
        Colour stm;
        float komi;
        std::vector<BoardState> history;
};
