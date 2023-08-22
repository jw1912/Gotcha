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

        [[nodiscard]] auto isGameOver() const { return passes >= 2; }
        [[nodiscard]] auto sizeOf() const { return size * size; }
        [[nodiscard]] auto getHash() const { return hash; }
        [[nodiscard]] auto moveHead() const { return empty; }
        [[nodiscard]] auto operator[](Tile tile) const { return tiles[tile.index()]; }

    private:
        LinkHead empty;
        std::uint16_t passes;
        std::uint16_t size;
        std::vector<LinkNode> tiles;
        std::vector<Group> groups;
        Zobrist hash;
};

class Board
{
    public:
        BoardState board;

        Board(const std::uint16_t withSize)
        {
            board = BoardState(withSize);
            setStm(Colour::Black);
        }

        bool tryMakeMove(const Tile tile);

        void setStm(Colour colour) { stm = colour; }

        void undoMove()
        {
            stm = flipColour(stm);
            board = history.back();
            history.pop_back();
        }

        void display(const bool showGroups) const;

    private:
        Colour stm;
        std::vector<BoardState> history;
};