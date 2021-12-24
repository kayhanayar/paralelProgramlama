#include <iostream>
#include <math.h>


__global__ 
void topla(int n, float *x, float *y)
{
    int i = threadIdx.x;
    y[i] = x[i] + y[i];
    printf("id:%d \n",i);
}

int main(void)
{
    int N = 32; 
    
    
    float *h_x = new float[N];
    float *h_y = new float[N];

   
    for (int i = 0; i < N; i++) {
        h_x[i] = 1.0f;
        h_y[i] = 2.0f;
    }

    float *d_x,*d_y;
    cudaMalloc(&d_x,sizeof(float)*N);
    cudaMalloc(&d_y,sizeof(float)*N);

    cudaMemcpy(d_x,h_x,sizeof(float)*N,cudaMemcpyHostToDevice);
    cudaMemcpy(d_y,h_y,sizeof(float)*N,cudaMemcpyHostToDevice);

    topla<<<1,32>>>(N,d_x,d_y);
    cudaDeviceSynchronize();


    float maxError = 0.0f;
    for (int i = 0; i < N; i++)
        maxError = fmax(maxError, fabs(h_y[i]-3.0f));
    std::cout << "Max error: " << maxError << std::endl;    


    delete [] h_x;
    delete [] h_y;

    cudaFree(d_x);
    cudaFree(d_y);
    return 0;
}