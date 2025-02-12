#pragma once
#ifndef INTERFACES_ITASK_H_
#define INTERFACES_ITASK_H_

#include <stdint.h>

/// @brief Task is a finite process interface.
/// 'work' is a metric of task progress/remaining work (jobs/bytes/something other)
class ITask
{
public:
	virtual ~ITask() {};

	virtual bool isActive() const = 0;
	virtual uint32_t getWorkTotal() const = 0;
	virtual uint32_t getWorkDone() const = 0;
	virtual void update() = 0;
	virtual void waitForEnd() = 0;
	virtual void terminate() = 0;
	virtual void activate() = 0;
	virtual void deactivate() = 0;
};

#endif // INTERFACES_ITASK_H_