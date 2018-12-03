#include "ring_buffer.h"

#include <cassert>
#include <string.h>

#include <algorithm>

namespace rocket {
namespace container {

RingBuffer::RingBuffer(uint32_t size) 
	: _data(NULL)
	, _in(0)
	, _out(0) {
	if (!is_pow_of_two(size)) {
		_capacity = roundup_pow_of_two(size);
	}
	_data = new char[_capacity];	
}

RingBuffer::~RingBuffer() {
	if (_data != NULL) {
		delete[] _data;
		_data = NULL;
	}
}

uint32_t RingBuffer::Read(void* buf, uint32_t n) {
	n = std::min(n, size());

	uint32_t len = std::min(n, _capacity - (_out & (_capacity - 1)));

	/* first get the data starting from _out to buffer end */
	memcpy(buf, _data + (_out & (_capacity - 1)), len);

	/* then get the rest (if any) at the beginning of the buffer */
	memcpy((char*)buf + len, _data, n - len);

	_out += n;

	return n;
}

uint32_t RingBuffer::Write(void* buf, uint32_t n) {
	n = std::min(n, remain());
	
	uint32_t len = std::min(n, _capacity - (_in & (_capacity - 1)));

	/* first put the data starting from _in to buffer end */
	memcpy(_data + (_in & (_capacity - 1)), buf, len);

	/* then put the rest (if any) at the beginning of the buffer */
	memcpy(_data, (char*)buf + len, n -len);

	_in += n;

	return n;
}

// 提升为2的次幂
uint32_t RingBuffer::roundup_pow_of_two(uint32_t n) {
	for (int i = 0; i < sizeof(uint32_t) * 8; ++i) {
		int tmp = (1 << i);
		if (tmp > n) {
			return tmp;
		}
	}
	return 0;
}

bool RingBuffer::is_pow_of_two(uint32_t n) {
	return (n != 0  && (n & (n - 1)) == 0);
}

} // namespace container
} // namespace rocket