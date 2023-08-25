#pragma once

#include <array>
#include <cstdint>
#include <vector>

enum struct Colour : std::uint8_t
{
    Black = 0,
    White = 1,
};

[[nodiscard]] constexpr auto flipColour(Colour colour)
{
    return static_cast<Colour>(!static_cast<std::uint8_t>(colour));
}

enum struct State : std::int8_t
{
    Loss = -1,
    Ongoing = 0,
    Win = 1,
};

[[nodiscard]] constexpr auto flipState(State state)
{
    return static_cast<State>(-static_cast<std::int8_t>(state));
}

enum struct Territory : std::uint8_t
{
    Neither = 0,
    Black = 1,
    White = 2,
    Both = 3,
};

[[nodiscard]] constexpr auto territoryFrom(bool black, bool white)
{
    const auto blackVal = static_cast<std::uint8_t>(black);
    const auto whiteVal = static_cast<std::uint8_t>(white);
    return static_cast<Territory>(blackVal + 2 * whiteVal);
}

[[nodiscard]] constexpr auto territoryMerge(Territory t1, Territory t2)
{
    return static_cast<Territory>(
        static_cast<std::uint8_t>(t1) | static_cast<std::uint8_t>(t2)
    );
}

struct Vec4
{
    std::array<uint16_t, 4> elements;
    std::uint16_t length;

    constexpr void push(std::uint16_t element)
    {
        elements[length] = element;
        length++;
    }
};

class Tile
{
    public:
        constexpr Tile(std::uint16_t t) { tile = t; }

        constexpr Tile() { tile = 1024; }

        constexpr Tile(std::uint16_t x, std::uint16_t y, std::uint16_t size)
        {
            tile = size * y + x;
        }

        [[nodiscard]] constexpr auto operator==(const Tile other) const { return tile == other.tile; }

        [[nodiscard]] constexpr auto index() const { return tile; }
        [[nodiscard]] constexpr auto isNull() const { return tile == 1024; }

        [[nodiscard]] constexpr auto getAdjacent(std::uint16_t size) const
        {
            auto adj = Vec4{};

            if ((tile % size) > 0)
                adj.push(static_cast<std::uint16_t>(-1));

            if ((tile % size) < (size - 1))
                adj.push(static_cast<std::uint16_t>(1));

            if ((tile / size) > 0)
                adj.push(static_cast<std::uint16_t>(-size));

            if ((tile / size) < (size - 1))
                adj.push(static_cast<std::uint16_t>(size));

            return adj;
        }

    private:
        std::uint16_t tile = 1024;
};

struct LinkNode
{
    Tile prev;
    Tile next;
    std::uint16_t group;

    LinkNode(std::uint16_t groupId) { group = groupId; }

    LinkNode() { group = 1024; }
};

class LinkHead
{
    public:
        Tile first;
        Tile last;

        LinkHead(const Tile x, const Tile y, const std::uint32_t z)
        {
            first = x;
            last = y;
            length = z;
        }

        LinkHead()
        {
            first = Tile{};
            last = Tile{};
            length = 0;
        }

        [[nodiscard]] constexpr auto len() const { return length; }
        [[nodiscard]] constexpr auto isEmpty() const { return length == 0; }

        void join(LinkHead& other, std::vector<LinkNode>& tiles);

        void remove(const Tile tile, std::vector<LinkNode>& tiles);

    private:
        std::uint32_t length;
};
