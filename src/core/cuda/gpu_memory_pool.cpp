#include "mandelbrot/core/cuda/gpu_memory_pool.h"

#include "mandelbrot/core/cuda/defines.h"

#include <cuda_runtime.h>
#include <stdexcept>

namespace mandelbrot {

GPUMemoryPool::GPUMemoryPool(size_t pool_size)
  : pool_size_{pool_size},
    pool_{nullptr} {
  CUDA_CHECK(cudaMalloc(&pool_, pool_size_));
}

GPUMemoryPool::~GPUMemoryPool() {
  CUDA_CHECK(cudaFree(pool_));
}

void* GPUMemoryPool::Alloc(size_t size) {
  // GPUMemoryPool isn't properly implemented yet
  if (size > pool_size_) {
    throw std::runtime_error{ "Not enough memory in pool" };
  }
  return pool_;
}

void GPUMemoryPool::Free(void* ptr) {
  // GPUMemoryPool isn't properly implemented yet
}

}