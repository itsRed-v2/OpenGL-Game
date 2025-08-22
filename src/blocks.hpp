#ifndef VOXELS_BLOCKS_HPP
#define VOXELS_BLOCKS_HPP

#include "block.hpp"

namespace Blocks {
    const Block& fromId(block_id id);

    const Block AIR(0, "air", "air");
    const Block TEST(1, "test", "test");
    const Block STONE(2, "stone", "stone");
    const Block GRASS(3, "grass_top", "grass_sides");

}

void ensureCorrectBlockIDs();

#endif //VOXELS_BLOCKS_HPP