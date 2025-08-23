//
// Created by svjat on 8/10/2025.
//

#ifndef TILEMAP_H
#define TILEMAP_H
#include <string>

enum BlockFace;
enum SolidBlockType;

class Tilemap {
public:
	Tilemap();
	~Tilemap();

private:
	int tile_size_ = 16;
	std::string tile_map_ = "../assets/images/tile-map.png";

};

struct Block {
	BlockFace faces[6]; // Order: top, bottom, left, right, front, back

	Block(BlockFace single_face) {
		for (int i = 0; i < 6; ++i) {
			faces[i] = single_face;
		}
	}
	Block(BlockFace top_face, BlockFace bottom_face, BlockFace side_face) {
		faces[0] = top_face;
		faces[1] = bottom_face;
		for (int i = 2; i < 6; ++i)
			faces[i] = side_face;
	}
	Block(BlockFace top_face, BlockFace bottom_face, BlockFace left_face, BlockFace right_face, BlockFace front_face, BlockFace back_face) {
		faces[0] = top_face;
		faces[1] = bottom_face;
		faces[2] = left_face;
		faces[3] = right_face;
		faces[4] = front_face;
		faces[5] = back_face;
	}
};

// the index of "SolidBlocks" should match the index of the "SolidBlockType" enum
extern Block SolidBlocks[];

// For all enums, the first item is defaulted to 0, and incremented subsequently

// Corresponds to block tile-map; index is (x + y*x_size)
enum BlockFace : int {
	GRASS_TOP,
	GRASS_SIDE,
	DIRT_SIDE,
	SAND_SIDE,
	GRAVEL_SIDE,
	STONE_SIDE,
	STONE_DIAMOND_ORE_SIDE,
	STONE_COPPER_ORE_SIDE,
	STONE_IRON_ORE_SIDE,
	STONE_GOLD_ORE_SIDE,
	OAK_LOG_SIDE,
	OAK_LOG_TOP,
	OAK_PLANK_SIDE,
	OAK_PLANK_SLAB_SIDE,
	OAK_PLANK_STAIR_SIDE,
	LEAVES_1_SIDE,
	LEAVES_2_SIDE,
	WATER_TOP,
	COBBLESTONE_SIDE,
	COBBLESTONE_SLAB_SIDE,
	ROSE_SIDE,
	DANDELION_SIDE,
	SMALL_GRASS_SIDE,
	SNOW_SIDE,
	TEST_SIDE
};

enum SolidBlockType : int {
	GRASS_BLOCK ,
	DIRT_BLOCK,
	SAND_BLOCK,
	GRAVEL_BLOCK,
	STONE_BLOCK,
	STONE_COPPER_ORE_BLOCK,
	STONE_IRON_ORE_BLOCK,
	STONE_GOLD_ORE_BLOCK,
	STONE_DIAMOND_ORE_BLOCK,
	OAK_LOG_BLOCK,
	OAK_PLANK_BLOCK,
	COBBLESTONE_BLOCK,
	SNOW_BLOCK,
};


#endif //TILEMAP_H
