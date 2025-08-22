#include "blocks.hpp"

#include <array>
#include <stdexcept>

#define BLOCK_COUNT 4

constexpr std::array<const Block*, BLOCK_COUNT> BLOCKS = {
    &Blocks::AIR,
    &Blocks::TEST,
    &Blocks::STONE,
    &Blocks::GRASS,
};

const Block &Blocks::fromId(const block_id id) {
    if (id < 0 || id >= BLOCK_COUNT)
        throw std::invalid_argument("invalid block id: " + std::to_string(id));
    return *BLOCKS[id];
}

void ensureCorrectBlockIDs() {
    for (int i = 0; i < BLOCK_COUNT; i++) {
        if (BLOCKS[i]->id != i) {
            throw std::runtime_error("Block position in the BLOCKS array does not match its ID (block ID: "
                + std::to_string(BLOCKS[i]->id) + ", array index: " + std::to_string(i) + ")");
        }
    }
}
