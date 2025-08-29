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

struct Vec3Less {
	bool operator()(const glm::vec3& a, const glm::vec3& b) const {
		if (a.x != b.x) return a.x < b.x;
		if (a.y != b.y) return a.y < b.y;
		return a.z < b.z;
	}
};

class World {
public:
	World() = default;
	~World();

	void Setup(int seed);
	bool ChunkExists(glm::ivec3 key);
	std::vector<CubeData>& getChunkData(glm::ivec3 key);
	void Update(glm::ivec3 player_chunk_coords);
	void RenderChunks(Shader& shader, Camera& camera);

private:
	int seed_;
	int render_distance_ = 7;
	bool pause_chunk_loading = true;

	unsigned int k_chunk_size_x_ = 32;
	unsigned int k_chunk_size_y_ = 32;
	unsigned int k_chunk_size_z_ = 32;

	glm::ivec3 center_chunk_coords = glm::ivec3(0, 0, 0);

	std::map<glm::ivec3, std::unique_ptr<Chunk>, Vec3Less> chunks_;

	std::vector<CubeData> GenerateChunkData(glm::ivec3 key);
	void GenerateChunk(glm::ivec3 key);

	FastNoiseLite continent_noise_, erosion_noise_, peak_and_valley_noise_;
	int sea_level_ = 10;
	// continent spline turns noise (-1 to 1) to terrain height (0 to 300)
	std::map<float, float> continent_spline_;
	// erosion spline turns noise (-1 to 1) to terrain addition multiplier (0.01 to .95)
	std::map<float, float> erosion_spline_;
	// peak and valley spline turns noise (-1 to 1) to terrain height change (-160 to 100)
	std::map<float, float> peak_and_valley_spline_;

	float getSplineValue(float noise, SplineType type, float height_noise);
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
