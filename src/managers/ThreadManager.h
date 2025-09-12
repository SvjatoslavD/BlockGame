//
// Created by svjat on 8/31/2025.
//

#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <glm\glm.hpp>
#include <mutex>
#include <queue>
#include <thread>

#include "world/Chunk.h"

class WorldGeneration;
class World;

enum class TaskType {
	CHUNK_LOAD = 0,
};

struct Task {
	TaskType type;
	int priority;

	glm::ivec2 chunk_pos;
	int height_start;
	int height_end;
};


class ThreadManager {
public:
	explicit ThreadManager(size_t num_threads = 1); // keep one thread for general OS use and one for the main program
	~ThreadManager() = default;

	void Shutdown();
	void SetupThreads(WorldGeneration& world_generation, World& world);
	void QueueChunkLoad(glm::ivec2 chunk_pos,int height_start, int height_end, int priority);
	std::vector<std::unique_ptr<Chunk>> getCompletedChunks();

private:
	void Worker(WorldGeneration& world_generation, World& world);

	std::vector<std::jthread> threads_;
	size_t num_threads_;

	// worker related members
	std::queue<Task> tasks_;
	std::mutex task_queue_mutex_;
	std::condition_variable task_queue_condition_;
	std::atomic_bool should_shutdown_ = false;

	// output members
	std::vector<std::unique_ptr<Chunk>> completed_chunks_;
	std::mutex completed_chunk_mutex_;
};



#endif //THREADMANAGER_H
