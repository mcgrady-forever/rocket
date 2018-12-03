// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#ifndef ROCKET_CONTAINER_RING_BUFFER_H_
#define ROCKET_CONTAINER_RING_BUFFER_H_

#include <stdint.h>

namespace rocket {
namespace container {

/* This is not thread-safe*/
class RingBuffer {
public:
	RingBuffer(uint32_t size);
	~RingBuffer();
	
	uint32_t Read(void* buf, uint32_t size);

	uint32_t Write(void* buf, uint32_t size);

	uint32_t capacity() const {
		return _capacity;
	}

	uint32_t size() const {
		return _in - _out;
	}

	uint32_t remain() const {
		return _capacity + _out -_in;
	}

private:
	uint32_t roundup_pow_of_two(uint32_t n);

	bool is_pow_of_two(uint32_t n);

private:
	uint32_t	_capacity;
	char*       _data;
	uint32_t    _in;
	uint32_t    _out;
};

} // namespace container
} // namespace rocket

#endif // ROCKET_CONTAINER_RING_BUFFER_H_
