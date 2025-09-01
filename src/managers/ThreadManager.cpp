//
// Created by svjat on 8/31/2025.
//

#include "ThreadManager.h"

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

void ThreadManager::QueueChunkLoad(glm::ivec3 chunk_coords, int priority) {
	tasks_.emplace(Task(TaskType::CHUNK_LOAD,priority,chunk_coords));
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
				std::unique_lock lock(completed_chunk_mutex_);
				ChunkData data = world_generation.GenerateChunkData(task.chunk_coords);
				auto chunk = std::make_unique<Chunk>(data, world);
				completed_chunks_.push_back(std::move(chunk));
				lock.unlock();
			}
				break;
			default: {
			}
				break;
		}
	}
}