
#include<cuda_runtime.h>
#include <iostream>
#include <math.h>
#include "yardimci.hpp"
void matrisYazdir(int *C, const int nx, const int ny)
{
    int *ic = C;
    printf("\nMatrix: (%d.%d)\n", nx, ny);

    for (int iy = 0; iy < ny; iy++)
    {
        for (int ix = 0; ix < nx; ix++)
        {
            printf("%3d", ic[ix]);

        }

        ic += nx;
        printf("\n");
    }

    printf("\n");
    return;
}

__global__ 
void iplikIndeksleriniYazdir(int *A, const int nx, const int ny)
{
    
    int ix = threadIdx.x + blockIdx.x * blockDim.x;
    int iy = threadIdx.y + blockIdx.y * blockDim.y;
    unsigned int idx = iy * nx + ix;

    printf("thread_id (%d,%d) block_id (%d,%d) coordinate (%d,%d) global index"
           " %2d ival %2d\n", threadIdx.x, threadIdx.y, blockIdx.x, blockIdx.y,
           ix, iy, idx, A[idx]);
    
}


int main(void)
{
    int nx = 8;
    int ny = 6;
    int elemanSayisi= nx*ny;

    int toplamBoyut = sizeof(int)*elemanSayisi;
    
    
    int *h_A = new int[toplamBoyut];
    for(int i=0;i<elemanSayisi;i++)
    {
        h_A[i]=i;
    }
    matrisYazdir(h_A,nx,ny);



    int *d_A;
    
    cudaSetDevice(0);
    cudaMalloc(&d_A,sizeof(int)*elemanSayisi);
    

    cudaMemcpy(d_A,h_A,sizeof(int)*elemanSayisi,cudaMemcpyHostToDevice);


    dim3 block(4,2);
    dim3 grid(((nx+block.x-1)/block.x),(ny+block.y-1)/block.y);
    printf("iplik Sayisi:%d\n",block.x);
    printf("block Sayisi:%d\n",grid.x);

    TIMERSTART(device);
    iplikIndeksleriniYazdir<<<grid,block>>>(d_A,nx,ny);
    cudaDeviceSynchronize();
    TIMERSTOP(device)
    
    
    delete [] h_A;
 

    cudaFree(d_A);
    return 0;
}