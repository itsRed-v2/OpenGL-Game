#include "blocks.hpp"

#include <array>
#include <format>

#include "logger.hpp"

#define BLOCK_COUNT 4

// ReSharper disable once CppTemplateArgumentsCanBeDeduced
constexpr std::array<const Block*, BLOCK_COUNT> BLOCKS = {
    &Blocks::AIR,
    &Blocks::TEST,
    &Blocks::STONE,
    &Blocks::GRASS,
};

const Block &Blocks::fromId(const block_id id) {
    if (id < 0 || id >= BLOCK_COUNT)
        Logger::crash("Invalid block id: " + std::to_string(id));
    return *BLOCKS[id];
}

void ensureCorrectBlockIDs() {
    for (int i = 0; i < BLOCK_COUNT; i++) {
        if (BLOCKS[i]->id != i) {
            Logger::crash(std::format(
                "Block position in the BLOCKS array does not match its ID (block ID: {}, array index: {})",
                BLOCKS[i]->id, i));
        }
    }
}
