// Copyright (c) 2018, The Util Authors.
// All rights reserved.
//
// Author: He Lei <heleicp3@163.com>

#ifndef BASE_MEMORY_BARRIER_H_
#define BASE_MEMORY_BARRIER_H_

namespace chris {

inline void CompilerBarrier() { __asm__ __volatile__("": : :"memory"); }
#if defined __i386__ || defined __x86_64__
inline void MemoryBarrier() { __asm__ __volatile__("mfence": : :"memory"); }
inline void MemoryReadBarrier() { __asm__ __volatile__("lfence" ::: "memory"); }
inline void MemoryWriteBarrier() { __asm__ __volatile__("sfence" ::: "memory"); }
#else
#error Unsupportted platform.
#endif

} // namespace chris

#endif // BASE_MEMORY_BARRIER_H_