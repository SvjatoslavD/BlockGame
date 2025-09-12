//
// Created by svjat on 8/31/2025.
//

#include "ThreadManager.h"

#include <utility>

#include "World.h"
#include "WorldGeneration.h"

ThreadManager::ThreadManager(size_t num_threads): num_threads_(num_threads) {
	threads_.reserve(num_threads_);
}

void ThreadManager::SetupThreads(WorldGeneration& world_generation, World& world) {
	for (size_t i = 0; i < num_threads_; i++) {
		threads_.emplace_back(&ThreadManager::Worker, this, std::ref(world_generation), std::ref(world));
	}
}

void ThreadManager::QueueChunkLoad(glm::ivec2 chunk_pos,int height_start, int height_end, int priority) {
	std::unique_lock lock(task_queue_mutex_);
	tasks_.emplace(Task(TaskType::CHUNK_LOAD,priority,chunk_pos,height_start, height_end));
	lock.unlock();
}

std::vector<std::unique_ptr<Chunk>> ThreadManager::getCompletedChunks() {
	std::vector<std::unique_ptr<Chunk>> out;
	out.reserve(completed_chunks_.size());
	out = std::move(completed_chunks_);
	completed_chunks_.clear();
	return out;
}

void ThreadManager::Shutdown() {
	should_shutdown_ = true;
	for (auto& thread : threads_) {
		if (thread.joinable()) {
			thread.join();
		}
	}
}


void ThreadManager::Worker(WorldGeneration& world_generation, World& world) {
	while (!should_shutdown_) {
		std::unique_lock lock_queue(task_queue_mutex_);
		task_queue_condition_.wait(lock_queue, [this] { return !tasks_.empty(); });

		if (should_shutdown_) {break;}

		Task task = tasks_.front();
		tasks_.pop();
		lock_queue.unlock();

		switch (task.type) {
			case TaskType::CHUNK_LOAD: {
				std::vector<ChunkData> chunk_data = world_generation.GenerateChunkData(task.chunk_pos, task.height_start, task.height_end);

				std::unique_lock lock(completed_chunk_mutex_);
				for (auto data : chunk_data) {
					completed_chunks_.push_back(std::make_unique<Chunk>(data,world));
				}
				lock.unlock();
			}
				break;
			default: {
			}
				break;
		}
	}
}