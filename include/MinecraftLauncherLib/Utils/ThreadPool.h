#pragma once
#ifndef UTILS_THREAD_POOL_HPP_
#define UTILS_THREAD_POOL_HPP_

#include "MinecraftLauncherLib/MCLLib-api.h"

#include <queue>
#include <atomic>
#include <thread>
#include <chrono>
#include <iostream>
#include <functional>
#include <condition_variable>
#include <utility>
#include <map>

#include "MinecraftLauncherLib/Utils/Delegates.h"
#include "MinecraftLauncherLib/Utils/Interfaces/ITask.h"

namespace MCLCPPLIB_NAMESPACE
{
	namespace utils
	{
		namespace threads
		{
			using namespace delegates;

			template<class J, class T>
			struct ThreadPoolResult
			{
				std::shared_ptr<J> job;
				std::condition_variable& variable;
				int workerIndex;
				bool& locked;
				T entry;
			};

			/// <summary>
			/// Abstract class for worker class in threadpool
			/// </summary>
			/// <typeparam name="T">param</typeparam>
			/// <typeparam name="R">result</typeparam>
			template<class T, class R>
			class Worker
			{
			public:
				Worker() {}
				virtual ~Worker() {}

				virtual R operator()(const std::shared_ptr<T>&) = 0;
			};

			template<class T, class R>
			class ThreadPool : public ITask
			{
			private:
				void threadLoop(int index, std::shared_ptr<Worker<T, R>> worker) {
					std::condition_variable variable;
					std::mutex mutex;
					bool locked = false;

					while (true) {
						std::shared_ptr<T> job;
						std::string jobName;

						{
							std::unique_lock<std::mutex> lock(jobsMutex);
							jobsMutexCondition.wait(lock, [this] {
								return !working || !priorityJobs.empty() || !jobs.empty();
								});

							if (!working && priorityJobs.empty()) {
								break; 
							}

							if (!priorityJobs.empty()) {
								auto it = priorityJobs.begin();
								jobName = it->first;
								job = it->second;
								priorityJobs.erase(it); // ������� �� ������ �����
							}
							else if (!jobs.empty()) {
								job = jobs.front();
								jobs.pop();
							}
							else {
								continue;
							}

							busyWorkers++;
						}

						try {
							R result = (*worker)(job);
							{
								std::lock_guard<std::mutex> lock(resultsMutex);
								results.push(ThreadPoolResult<T, R>{
									job, variable, index, locked, result});
							}
						}
						catch (const std::exception& err) {
							busyWorkers--;
							if (onJobFailed) {
								onJobFailed(job);
							}
							if (stopOnFail) {
								std::lock_guard<std::mutex> lock(jobsMutex);
								failed = true;
							}
						}

						if (!jobName.empty()) {
							std::lock_guard<std::mutex> lock(priorityJobMutex);
							priorityJobCompleted[jobName] = true;
							priorityJobCondition.notify_all();
						}

						busyWorkers--;

						if (failed) {
							break;
						}
					}
				}
			public:
				ThreadPool(
					std::string name,
					supplier<std::shared_ptr<Worker<T, R>>> workersSupplier,
					consumer<R&> resultConsumer
				)
					: /*logger(std::move(name)), */resultConsumer(resultConsumer), worker_supplier(workersSupplier) {
					const uint32_t num_threads = std::thread::hardware_concurrency();
					for (uint32_t i = 0; i < num_threads; i++) {
						threads.emplace_back(
							&ThreadPool<T, R>::threadLoop, this, i, workersSupplier()
						);
						workersBlocked.emplace_back();
					}
				}
				~ThreadPool() {
					terminate();
				}

				bool isActive() const override {
					return working;
				}

				void activate() override
				{
					if (working) 
					{
						return;
					}
					{
						std::lock_guard<std::mutex> lock(jobsMutex);
						working = true;
					}
					jobsMutexCondition.notify_all();
				}

				void deactivate() override
				{
					if (!working)
					{
						return;
					}
					{
						std::lock_guard<std::mutex> lock(jobsMutex);
						working = false;
					}
				}

				void terminate() override {
					if (!working) {
						return;
					}
					{
						std::lock_guard<std::mutex> lock(jobsMutex);
						working = false;
					}
					{
						std::lock_guard<std::mutex> lock(resultsMutex);
						while (!results.empty()) {
							ThreadPoolResult<T, R> entry = results.front();
							results.pop();
							if (!standaloneResults) {
								entry.locked = false;
								entry.variable.notify_all();
							}
						}
					}

					jobsMutexCondition.notify_all();
					for (auto& thread : threads) {
						thread.join();
					}
				}

				void join()
				{
					/*{
						std::lock_guard<std::mutex> lock(resultsMutex);
						while (!results.empty()) {
							ThreadPoolResult<T, R> entry = results.front();
							results.pop();
							if (!standaloneResults) {
								entry.locked = false;
								entry.variable.notify_all();
							}
						}
					}*/

					bool complete = false;
					while(!complete)
					{
						std::lock_guard<std::mutex> lock(resultsMutex);
						while (!results.empty()) {
							ThreadPoolResult<T, R> entry = results.front();
							results.pop();

							try {
								resultConsumer(entry.entry);
							}
							catch (std::exception& err) {
								//logger.error() << err.what();
								if (onJobFailed) {
									onJobFailed(entry.job);
								}
								if (stopOnFail) {
									std::lock_guard<std::mutex> jobsLock(jobsMutex);
									failed = true;
									complete = false;
								}
								break;
							}

							if (!standaloneResults) {
								entry.locked = false;
								entry.variable.notify_all();
							}
						}

						if (onComplete && busyWorkers == 0) {
							std::lock_guard<std::mutex> jobsLock(jobsMutex);
							if (jobs.empty()) {
								onComplete();
								complete = true;
							}
						}
					}
					if (failed) {
						throw std::runtime_error("some job failed");
					}
				}

				void update() override {
					if (!working) {
						return;
					}
					if (failed) {
						throw std::runtime_error("some job failed");
					}

					bool complete = false;
					{
						std::lock_guard<std::mutex> lock(resultsMutex);
						while (!results.empty()) {
							ThreadPoolResult<T, R> entry = results.front();
							results.pop();

							try {
								resultConsumer(entry.entry);
							}
							catch (std::exception& err) {
								//logger.error() << err.what();
								if (onJobFailed) {
									onJobFailed(entry.job);
								}
								if (stopOnFail) {
									std::lock_guard<std::mutex> jobsLock(jobsMutex);
									failed = true;
									complete = false;
								}
								break;
							}

							if (!standaloneResults) {
								entry.locked = false;
								entry.variable.notify_all();
							}
						}

						if (onComplete && busyWorkers == 0) {
							std::lock_guard<std::mutex> jobsLock(jobsMutex);
							if (jobs.empty()) {
								onComplete();
								complete = true;
							}
						}
					}
					if (failed) {
						throw std::runtime_error("some job failed");
					}
					if (complete) {
						terminate();
					}
				}

				void enqueueJob(const std::shared_ptr<T>& job) {
					{
						std::lock_guard<std::mutex> lock(jobsMutex);
						jobs.push(job);
					}
					jobsMutexCondition.notify_one();
				}

				void enqueueJobWithPriority(const std::shared_ptr<T>& job, const std::string& job_name = std::string()) {
					{
						std::lock_guard<std::mutex> lock(priorityJobMutex);
						priorityJobs[job_name] = job;
						priorityJobCompleted[job_name] = false;
					}
					jobsMutexCondition.notify_one();
				}

				void waitForPriorityJob(const std::string& job_name = std::string()) {
					std::unique_lock<std::mutex> lock(priorityJobMutex);
					priorityJobCondition.wait(lock, [this, &job_name]
						{
							return priorityJobCompleted[job_name];
						});
				}

				/// @brief If false: worker will be blocked until it's result performed
				void setStandaloneResults(bool flag) {
					standaloneResults = flag;
				}

				void setStopOnFail(bool flag) {
					stopOnFail = flag;
				}

				/// @brief onJobFailed called on exception thrown in worker thread.
				/// Use engine.postRunnable when calling terminate()
				void setOnJobFailed(consumer<T&> callback) {
					this->onJobFailed = callback;
				}

				/// @brief onComplete called in ThreadPool.update() when all jobs done
				/// if ThreadPool was not terminated
				void setOnComplete(runnable callback) {
					this->onComplete = callback;
				}

				uint32_t getWorkTotal() const override {
					return jobs.size() + jobsDone + busyWorkers;
				}

				uint32_t getWorkDone() const override {
					return jobsDone;
				}

				virtual void waitForEnd() override {
					using namespace std::chrono_literals;
					while (working) {
						std::this_thread::sleep_for(2ms);
						update();
					}
				}

				uint32_t getWorkersCount() const {
					return threads.size();
				}

			private:
				//debug::Logger logger;
				std::queue<std::shared_ptr<T>> jobs;
				std::queue<ThreadPoolResult<T, R>> results;
				std::mutex resultsMutex;
				std::vector<std::thread> threads;
				std::condition_variable jobsMutexCondition;
				std::mutex jobsMutex;
				std::vector<std::unique_lock<std::mutex>> workersBlocked;
				consumer<R&> resultConsumer;
				consumer<std::shared_ptr<T>&> onJobFailed = nullptr;
				runnable onComplete = nullptr;
				supplier<std::shared_ptr<Worker<T, R>>> worker_supplier = nullptr;
				std::atomic<int> busyWorkers = 0;
				std::atomic<uint32_t> jobsDone = 0;
				std::atomic<bool> working = true;
				bool failed = false;
				bool standaloneResults = true;
				bool stopOnFail = true;

			private:
				std::map<std::string, std::shared_ptr<T>> priorityJobs;
				std::map<std::string, bool> priorityJobCompleted;
				std::condition_variable priorityJobCondition;
				std::mutex priorityJobMutex;
			};
		} // namespace threads
	} // namespace utils
}

#endif // UTILS_THREAD_POOL_HPP_