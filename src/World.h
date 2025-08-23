//
// Created by svjat on 8/1/2025.
//

#ifndef WORLD_H
#define WORLD_H

#include <map>
#include "FastNoiseLite.h"
#include "Tilemap.h"
#include "Camera.h"
#include "Chunk.h"

struct CubeData;
enum SplineType;

struct Vec2Less {
	bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
		if (a.x < b.x) return true;
		if (a.x > b.x) return false;
		return a.y < b.y;
	}
};

class World {
public:
	World() = default;
	~World();

	void Setup(int seed);
	bool ChunkExists(glm::ivec2 key);
	std::vector<CubeData>& getChunkData(glm::ivec2 key);
	void Update(glm::ivec2 player_chunk_coords);
	void RenderChunks(Shader& shader, Camera& camera);

private:
	int seed_;
	int render_distance_ = 14;
	bool pause_chunk_loading = true;

	unsigned int k_chunk_size_x_ = 16;
	unsigned int k_chunk_size_y_ = 512;
	unsigned int k_chunk_size_z_ = 16;

	glm::ivec2 center_chunk_coords = glm::ivec2(0, 0);

	// the position of the chunk is written in x,z coordinates
	std::map<glm::ivec2, std::unique_ptr<Chunk>, Vec2Less> chunks_;

	std::vector<CubeData> GenerateChunkData(glm::ivec2 key);
	void GenerateChunk(glm::ivec2 key);

	FastNoiseLite continent_noise_, erosion_noise_, peak_and_valley_noise_;
	int sea_level_ = 100;
	// continent spline turns noise (-1 to 1) to terrain height (0 to 130)
	std::map<float, float> continent_spline_;
	// erosion spline turns noise (-1 to 1) to terrain addition multiplier (0.1 to 1.7)
	std::map<float, float> erosion_spline_;
	// peak and valley spline turns noise (-1 to 1) to terrain height change (-80 to 70)
	std::map<float, float> peak_and_valley_spline_;

	float getSplineValue(float noise, SplineType type);
};

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

#endif //WORLD_H
