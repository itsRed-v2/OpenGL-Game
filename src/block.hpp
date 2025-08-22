#ifndef VOXELS_BLOCK_HPP
#define VOXELS_BLOCK_HPP

#include "texturemanip/texture2D.hpp"

#include <string>

using namespace std;
using block_id = uint16_t;

class Block {
public:
    const block_id id;
    const string topTexture;
    const string sidesTexture;

    Block(const Block&) = delete;
    Block& operator=(const Block&) = delete;

    Block(block_id id, const string &topTexture, const string &sidesTexture);
};

bool operator==(const Block& block, block_id id);
bool operator==(block_id id, const Block& block);
bool operator!=(const Block& block, block_id id);
bool operator!=(block_id id, const Block& block);

#endif //VOXELS_BLOCK_HPP