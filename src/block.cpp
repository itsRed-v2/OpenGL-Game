#include "block.hpp"

Block::Block(const block_id id, const string &topTexture, const string &sidesTexture): id(id), topTexture(topTexture), sidesTexture(sidesTexture) {}

bool operator==(const Block& block, const block_id id) {
    return block.id == id;
}

bool operator==(const block_id id, const Block& block) {
    return block.id == id;
}

bool operator!=(const Block& block, const block_id id) {
    return block.id != id;
}

bool operator!=(const block_id id, const Block& block) {
    return block.id != id;
}