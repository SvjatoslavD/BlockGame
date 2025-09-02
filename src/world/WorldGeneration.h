//
// Created by svjat on 8/30/2025.
//

#ifndef WORLDGENERATION_H
#define WORLDGENERATION_H

#include <vector>
#include <glm\glm.hpp>
#include <map>
#include "FastNoiseLite.h"
#include "Cube.h"

enum SplineType : int{
	CONTINENT = 0,
	EROSION,
	PEAK_AND_VALLEY,
};
enum InterpolationType : int{
	LINEAR = 0,
	SMOOTH,
	CUBIC,
};

struct ChunkData;

class WorldGeneration {
public:
	WorldGeneration();
	~WorldGeneration() = default;

	void setSeed(int seed);
	ChunkData GenerateChunkData(glm::ivec3 chunk_pos);

private:
	int seed_;

	unsigned int k_chunk_size_x_ = 32;
	unsigned int k_chunk_size_y_ = 32;
	unsigned int k_chunk_size_z_ = 32;

	FastNoiseLite continent_noise_, erosion_noise_, peak_and_valley_noise_, cave_noise_, cave_intersect_noise_;
	int sea_level_ = 0;
	// continent spline turns noise (-1 to 1) to terrain height (0 to 300)
	std::map<float, float> continent_spline_;
	// erosion spline turns noise (-1 to 1) to terrain addition multiplier (0.01 to .95)
	std::map<float, float> erosion_spline_;
	// peak and valley spline turns noise (-1 to 1) to terrain height change (-160 to 100)
	std::map<float, float> peak_and_valley_spline_;

	float getSplineValue(float noise, SplineType type, float height_noise);
};




#endif //WORLDGENERATION_H
