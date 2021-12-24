#include <iostream>
#include <math.h>


__global__ 
void topla(int n, float *x, float *y)
{

  for (int i = 0; i < n; i++)
      y[i] = x[i] + y[i];
}

int main(void)
{
    int N = 1<<20; 
    
    
    float *h_x = new float[N];
    float *h_y = new float[N];
    float *h_sonuc = new float[N];
   
    for (int i = 0; i < N; i++) {
        h_x[i] = 1.0f;
        h_y[i] = 2.0f;
    }

    float *d_x,*d_y;
    cudaMalloc(&d_x,sizeof(float)*N);
    cudaMalloc(&d_y,sizeof(float)*N);

    cudaMemcpy(d_x,h_x,sizeof(float)*N,cudaMemcpyHostToDevice);
    cudaMemcpy(d_y,h_y,sizeof(float)*N,cudaMemcpyHostToDevice);

    topla<<<1,1>>>(N,d_x,d_y);
    cudaDeviceSynchronize();

    cudaMemcpy(h_sonuc,d_y,sizeof(float)*N,cudaMemcpyDeviceToHost);

    for(int i=0;i<N;i++)
    {
        if(i%10==0)
            printf("\n");

        printf("%5.4f ",h_sonuc[i]);
    }


    delete [] h_x;
    delete [] h_y;

    cudaFree(d_x);
    cudaFree(d_y);
    return 0;
}