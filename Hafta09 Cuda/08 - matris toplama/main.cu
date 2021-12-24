#include "yardimci.hpp"
#include <cuda_runtime.h>
#include <stdio.h>



void ilkDegerleriAta(float *ip, const int size)
{
    int i;

    for(i = 0; i < size; i++)
    {
        ip[i] = (float)(rand() & 0xFF) / 10.0f;
    }

    return;
}

void cpuMatrisHesaplama(float *A, float *B, float *C, const int nx,
                     const int ny)
{
    for (int iy = 0; iy < ny; iy++)
    {
        for (int ix = 0; ix < nx; ix++)
        {
            C[ix] = A[ix] + B[ix];

        }

        A += nx;
        B += nx;
        C += nx;
    }
    return;
}


void sonuclariKontrolEt(float *hostRef, float *gpuRef, const int N)
{
    double epsilon = 1.0E-8;
    bool match = 1;

    for (int i = 0; i < N; i++)
    {
        if (abs(hostRef[i] - gpuRef[i]) > epsilon)
        {
            match = 0;
            printf("host %f gpu %f\n", hostRef[i], gpuRef[i]);
            break;
        }
    }

    if (match)
        printf("Matrisler esit.\n\n");
    else
        printf("matrisler esit degil.\n\n");
}


__global__ void 
gpuMatrisToplama2D(float *MatA, float *MatB, float *MatC, 
                    int nx,
                    int ny)
{
    unsigned int sutun = threadIdx.x + blockIdx.x * blockDim.x;
    unsigned int satir = threadIdx.y + blockIdx.y * blockDim.y;
    unsigned int index = satir * nx + sutun;

    if (sutun < nx && satir < ny)
        MatC[index] = MatA[index] + MatB[index];
}

int main(int argc, char **argv)
{
    

    

    cudaSetDevice(0);

    // set up data size of matrix
    int nx = 1 << 14;
    int ny = 1 << 14;

    int nxy = nx * ny;
    int nBytes = nxy * sizeof(float);
    printf("Matris boyutu: nx %d ny %d\n", nx, ny);

    // malloc host memory
    float *h_A, *h_B, *hostRef, *gpuRef;

    h_A = (float *)malloc(nBytes);
    h_B = (float *)malloc(nBytes);

    hostRef = (float *)malloc(nBytes);
    gpuRef = (float *)malloc(nBytes);

    TIMERSTART(ilkDegerAtama)
    ilkDegerleriAta(h_A, nxy);
    ilkDegerleriAta(h_B, nxy);
    TIMERSTOP(ilkDegerAtama)

    memset(hostRef, 0, nBytes);
    memset(gpuRef, 0, nBytes);
    TIMERSTART(cpuileHesaplama)
    cpuMatrisHesaplama(h_A, h_B, hostRef, nx, ny);
    TIMERSTOP(cpuileHesaplama)

   
    float *d_MatA, *d_MatB, *d_MatC;
    cudaMalloc((void **)&d_MatA, nBytes);
    cudaMalloc((void **)&d_MatB, nBytes);
    cudaMalloc((void **)&d_MatC, nBytes);

    
    cudaMemcpy(d_MatA, h_A, nBytes, cudaMemcpyHostToDevice);
    cudaMemcpy(d_MatB, h_B, nBytes, cudaMemcpyHostToDevice);

    
    int dimx = 16;
    int dimy = 16;
    dim3 block(dimx, dimy);
    dim3 grid((nx + block.x - 1) / block.x, (ny + block.y - 1) / block.y);
    gpuMatrisToplama2D<<<grid, block>>>(d_MatA, d_MatB, d_MatC, nx, ny);
    TIMERSTART(GPU_ileHesaplama)
    gpuMatrisToplama2D<<<grid, block>>>(d_MatA, d_MatB, d_MatC, nx, ny);
    cudaDeviceSynchronize();
    TIMERSTOP(GPU_ileHesaplama)
    printf("gpuMatrisToplama2D <<<(%d,%d), (%d,%d)>>> ", grid.x,
           grid.y,
           block.x, block.y);
   
    cudaMemcpy(gpuRef, d_MatC, nBytes, cudaMemcpyDeviceToHost);

    // check device results
    sonuclariKontrolEt(hostRef, gpuRef, nxy);

   
    cudaFree(d_MatA);
    cudaFree(d_MatB);
    cudaFree(d_MatC);

    // free host memory
    free(h_A);
    free(h_B);
    free(hostRef);
    free(gpuRef);

    // reset device
    return (0);
}