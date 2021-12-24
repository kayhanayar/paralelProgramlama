#include<stdio.h>

__global__ void kernel()
{
    const int threadId = threadIdx.x;
    printf("iplik %d 'dan Merhaba\n",threadId);
}
int main()
{
   
    cudaSetDevice(0);

    kernel<<<1,4>>>();
    
    cudaDeviceSynchronize();
    printf("Merhaba host\n");
}