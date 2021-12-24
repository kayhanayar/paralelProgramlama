
#include<cuda_runtime.h>
#include <iostream>
#include <math.h>
#include "yardimci.hpp"


__global__ 
void toplaCihaz(int n, float *x, float *y,float* sonuc)
{
    
    int i = blockIdx.x*blockDim.x+threadIdx.x;
    if(i<n)
        sonuc[i] = pow(2,x[i]) + y[i]*x[i]+pow(2,y[i])+y[i]*x[i]+pow(2,y[i]);;
    
    
}

void toplaSunucu(int n,float *x, float *y,float *sonuc)
{
    for(int i=0;i<n;i++)
    {
        
        sonuc[i] = pow(2,x[i]) +y[i]*x[i]+pow(2,y[i])+y[i]*x[i]+pow(2,y[i]);
    }
}

void rastgeleDegerata(float *p,int size)
{
    for(int i=0;i<size;i++)
    {
        p[i] = (float)(rand()%0xFF)/10.0f;
    }
}

void sonuclariKontrolEt(float* host,float*device,int N)
{
    float epsilon = 1.0E-8;
    bool eslesme = 1;

    for(int i=0;i<N;i++)
    {
        if(abs(host[i]-device[i])>epsilon)
        {
            eslesme = 0;
            printf("Diziler esit degil\n");
            printf("%d indeksteki sunucu %5.4f gpu %5.4f",i,host[i],device[i]);
            break;
        }
    }
    if(eslesme)
        printf("Diziler esit");
}
int main(void)
{
    int N = 1UL<<24; 
    
    
    float *h_x = new float[N];
    float *h_y = new float[N];
    float *h_sonuc = new float[N];
    float *g_sonuc = new float[N];
   
    
    rastgeleDegerata(h_x,N);
    rastgeleDegerata(h_y,N);

    float *d_x,*d_y,*d_sonuc;
    
    cudaSetDevice(0);
    cudaMalloc(&d_x,sizeof(float)*N);
    cudaMalloc(&d_y,sizeof(float)*N);
    cudaMalloc(&d_sonuc,sizeof(float)*N);

    cudaMemcpy(d_x,h_x,sizeof(float)*N,cudaMemcpyHostToDevice);
    cudaMemcpy(d_y,h_y,sizeof(float)*N,cudaMemcpyHostToDevice);

    int iplikSayisi = 1024;
    dim3 block(iplikSayisi);
    dim3 grid((N+block.x-1)/block.x);
    printf("iplik Sayisi:%d\n",block.x);
    printf("block Sayisi:%d\n",grid.x);

    TIMERSTART(device);
    toplaCihaz<<<grid,block>>>(N,d_x,d_y,d_sonuc);
    cudaDeviceSynchronize();
    TIMERSTOP(device)
    
    
    auto err=cudaMemcpy(g_sonuc,d_sonuc,sizeof(float)*N,cudaMemcpyDeviceToHost);
    if(err!=cudaSuccess)
        printf("hata:%s",cudaGetErrorString(err));
    
    TIMERSTART(host);
    toplaSunucu(N,h_x,h_y,h_sonuc);
    TIMERSTOP(host);
    sonuclariKontrolEt(h_sonuc,g_sonuc,N);
    


    delete [] h_x;
    delete [] h_y;
    delete [] h_sonuc;
    delete [] g_sonuc;

    cudaFree(d_x);
    cudaFree(d_y);
    cudaFree(d_sonuc);
    return 0;
}