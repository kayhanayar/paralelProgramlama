#include "yardimci.hpp"
#include <cuda_runtime.h>
#include <stdio.h>

/*
 * simpleDivergence demonstrates divergent code on the GPU and its impact on
 * performance and CUDA metrics.
 */

__global__ void mathKernel1(float *c)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    float ia, ib;
    ia = ib = 0.0f;

    if (tid % 2 == 0)
    {
        ia = 100.0f;
    }
    else
    {
        ib = 200.0f;
    }

    c[tid] = ia + ib;
}

__global__ void mathKernel2(float *c)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    float ia, ib;
    ia = ib = 0.0f;

    if ((tid / warpSize) % 2 == 0)
    {
        ia = 100.0f;
    }
    else
    {
        ib = 200.0f;
    }

    c[tid] = ia + ib;
}

__global__ void mathKernel3(float *c)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    float ia, ib;
    ia = ib = 0.0f;

    bool ipred = (tid % 2 == 0);

    if (ipred)
    {
        ia = 100.0f;
    }

    if (!ipred)
    {
        ib = 200.0f;
    }

    c[tid] = ia + ib;
}

__global__ void mathKernel4(float *c)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    float ia, ib;
    ia = ib = 0.0f;

    int itid = tid >> 5;

    if (itid & 0x01 == 0)
    {
        ia = 100.0f;
    }
    else
    {
        ib = 200.0f;
    }

    c[tid] = ia + ib;
}

__global__ void warmingup(float *c)
{
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    float ia, ib;
    ia = ib = 0.0f;

    if ((tid / warpSize) % 2 == 0)
    {
        ia = 100.0f;
    }
    else
    {
        ib = 200.0f;
    }

    c[tid] = ia + ib;
}


int main(int argc, char **argv)
{

    cudaSetDevice(0);
    
    int size = 1<<12;
    int blocksize = 64;

    if(argc > 1) blocksize = atoi(argv[1]);

    if(argc > 2) size      = atoi(argv[2]);

    printf("Data size %d ", size);

   
    dim3 block (blocksize, 1);
    dim3 grid  ((size + block.x - 1) / block.x, 1);
    printf("Execution Configure (block %d grid %d)\n", block.x, grid.x);

    // allocate gpu memory
    float *d_C;
    size_t nBytes = size * sizeof(float);
    cudaMalloc((float**)&d_C, nBytes);


    cudaDeviceSynchronize();
    TIMERSTART(warmup1)
    warmingup<<<grid, block>>>(d_C);
    cudaDeviceSynchronize();
    TIMERSTOP(warmup1)



    TIMERSTART(mathKernel1)
    mathKernel1<<<grid, block>>>(d_C);
    cudaDeviceSynchronize();
    TIMERSTOP(mathKernel1)
    
   
    TIMERSTART(mathKernel2)
    mathKernel2<<<grid, block>>>(d_C);
    cudaDeviceSynchronize();
    TIMERSTOP(mathKernel2)
   
    TIMERSTART(mathKernel3)
    mathKernel3<<<grid, block>>>(d_C);
    cudaDeviceSynchronize();
    TIMERSTOP(mathKernel3)
   

    TIMERSTART(mathKernel4)
    mathKernel4<<<grid, block>>>(d_C);
    cudaDeviceSynchronize();
    TIMERSTOP(mathKernel4)

    // free gpu memory and reset divece
    cudaFree(d_C);

    return EXIT_SUCCESS;
}