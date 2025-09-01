//
// Created by svjat on 8/30/2025.
//

#include "WorldGeneration.h"
#include "Tilemap.h"
#include "Chunk.h"

void WorldGeneration::setSeed(int seed) {
	seed_ = seed;

	continent_noise_.SetSeed(seed_);
	erosion_noise_.SetSeed(seed_);
	peak_and_valley_noise_.SetSeed(seed_);
}
WorldGeneration::WorldGeneration() {
	// Large scale continent shapes
	continent_noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
	continent_noise_.SetFrequency(0.002f);

	// Erosion noise
	erosion_noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	erosion_noise_.SetFrequency(0.002f);

	// Peaks and valley noise
	peak_and_valley_noise_.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	peak_and_valley_noise_.SetFrequency(0.004f);
	peak_and_valley_noise_.SetFractalType(FastNoiseLite::FractalType_Ridged);
	peak_and_valley_noise_.SetFractalOctaves(1);

	continent_spline_.insert({
		{-1,0},{-0.8,20},{-0.75,50},{-0.5,100},{-0.3,130},
		{0.1,160},{0.15,180},{0.5,240},{0.7,250},{0.8,270},{1,300}
	});
	erosion_spline_.insert({
		{-1,0.01},{-0.5,0.15},{-.3,0.25},{-.2,0.3},{-0.1,0.35},
		{0.1,0.4},{0.3,0.5},{0.6,0.75},{0.6,0.8},{0.8,0.9},{1,0.95}
	});
	peak_and_valley_spline_.insert({
		{-1,-160},{-0.8,-130},{-0.7,-140},{-0.1,-80},
		{0.1,-20},{0.3,30},{0.6,50},{0.9,90},{1,100}
	});
}


ChunkData WorldGeneration::GenerateChunkData(glm::ivec3 chunk_pos) {
	ChunkMetaData meta_data;
	std::vector<CubeData> cube_data_vec;
	cube_data_vec.reserve(k_chunk_size_x_*k_chunk_size_y_*k_chunk_size_z_);

    auto chunk_coords = glm::fvec3(chunk_pos);
	int y_offset = (int)chunk_coords.y * (int)k_chunk_size_y_;

    for (int x = 0; x < k_chunk_size_x_; x++) {
    	for (int z = 0; z < k_chunk_size_z_; z++) {
    		float world_x = chunk_coords.x * k_chunk_size_x_ + x;
    		float world_z = chunk_coords.z * k_chunk_size_z_ + z;

    		// Combine multiple noise octaves
    		float continent = continent_noise_.GetNoise(world_x, world_z);
    		float erosion = erosion_noise_.GetNoise(world_x, world_z);
    		float peak_and_valley = peak_and_valley_noise_.GetNoise(world_x, world_z) * .5f;

    		const int continent_height = (int)getSplineValue(continent, CONTINENT, continent);
    		const float erosion_mult = getSplineValue(erosion, EROSION, erosion);
    		const int peak_and_valley_height = (int)getSplineValue(peak_and_valley, PEAK_AND_VALLEY, peak_and_valley);

    		// Combine all noise layers
    		int base_height = sea_level_ + (int)(continent_height * erosion_mult);
    		int additional_height = (int)(peak_and_valley_height * erosion_mult);
    		int combined_noise = base_height + additional_height;

    		for (int y = 0; y < k_chunk_size_y_; y++) {
    			int true_y = y + y_offset;
    			CubeData cube_data{};
    			cube_data.position = glm::vec3(x, y, z);

    			if (true_y <= combined_noise) {
    				if (continent_height > 190 && erosion_mult > 0.6 && peak_and_valley_height > -15 && true_y >= combined_noise - 4) {cube_data.type = SNOW_BLOCK; }
    				else if (continent_height > 110 && erosion_mult > .4) {cube_data.type = STONE_BLOCK; }
    				else {
    					if (true_y == combined_noise) {cube_data.type = GRASS_BLOCK;}
    					else if (true_y >= combined_noise - 3) {cube_data.type = DIRT_BLOCK;}
    					else {cube_data.type = STONE_BLOCK;}
    				}

    				meta_data.solid_block_count++;
    			}
    			else if (true_y > combined_noise && true_y < sea_level_) {
    				cube_data.type = SAND_BLOCK;

    				meta_data.solid_block_count++;
    			}
    			else {
    				cube_data.is_air = true;
    				cube_data.type = DIRT_BLOCK; // Air blocks

    				meta_data.air_count++;
    			}

    			cube_data_vec.emplace_back(cube_data);
    		}

    	}
    }

	meta_data.CalculateFlags();

	auto chunk_data = ChunkData(chunk_coords,meta_data,cube_data_vec);
	return chunk_data;
}

float WorldGeneration::getSplineValue(float noise, SplineType type, float height_noise) {
	std::map<float, float> spline_map;
	InterpolationType interp;

	switch (type) {
		case CONTINENT:
			spline_map = continent_spline_;
		interp = CUBIC;
		break;
		case EROSION:
			spline_map = erosion_spline_;
		interp = SMOOTH;
		break;
		case PEAK_AND_VALLEY:
			spline_map = peak_and_valley_spline_;
		interp = height_noise < 0 ? SMOOTH : LINEAR;
		break;
	}

	auto first = spline_map.begin();
	auto last = spline_map.rbegin();

	// If noise is at or below the first point
	if (noise <= first->first) return first->second;
	// If noise is at or above the last point
	if (noise >= last->first) return last->second;

	for (auto it = spline_map.begin(); it != spline_map.end(); ++it) {
		if (noise == it->first) {
			return it->second;
		}
		if (noise < it->first) {
			// We found the upper bound, get the lower bound
			auto upper = it;
			auto lower = --it; // Move back to previous element

			// Get interpolation points
			float x0 = lower->first, y0 = lower->second;
			float x1 = upper->first, y1 = upper->second;
			float t = (noise - x0) / (x1 - x0);

			// Apply different interpolation methods
			float calculated_value;
			switch (interp) {
				case LINEAR:
					calculated_value = y0 + t * (y1 - y0);
				break;

				case SMOOTH:
					t = t * t * (3.0f - 2.0f * t);
				calculated_value = y0 + t * (y1 - y0);
				break;

				case CUBIC:
					t = t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
				calculated_value = y0 + t * (y1 - y0);
				break;

				default:
					calculated_value = y0 + t * (y1 - y0);
				break;
			}

			return calculated_value;
		}
	}

	// Should never reach here due to edge case handling above
	return 0.0f;
}
