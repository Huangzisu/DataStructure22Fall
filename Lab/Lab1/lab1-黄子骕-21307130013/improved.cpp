#include<iostream>
#include<vector>
#include<limits.h>
#include<cstdlib>
#include<time.h>
#include<chrono>


using namespace std;
using namespace chrono;

void insertion_sort(int *A, int p, int r){
    //开始排序
    for(int j = p + 1; j <= r; j++){
        int key = A[j];
        int i = j - 1;
        while(i >= 0 && A[i] > key){
            A[i+1] = A[i];
            --i;
            A[i+1] = key;
        }
    }
    //排序结束
}

void merge(int *A, int p, int q, int r){
    int n1 = q - p + 1;
    int n2 = r - q;
    //将第p个元素到第q个元素复制到数组L，将第q+1个元素到第r个元素复制到数组R
    int L[n1+1], R[n2+1];
    for(int i = 0; i < n1; i++){
        L[i] = A[p+i];
    }
    for(int j = 0; j < n2; j++){
        R[j] = A[q+j+1];
    }
    L[n1] = R[n2] = INT_MAX;
    int i = 0, j = 0;
    //按从小到大顺序merge
    for(int k = p; k <= r; k++){
        if(L[i] <= R[j]){
            A[k] = L[i];
            ++i;
        }
        else{
            A[k] = R[j];
            ++j;
        }
    }
    
}

void improved_sort(int *A, int p, int r, int k){
    if(p < r){
        int q = (p + r) / 2;
        if(((r-p+1) / 2) > k){//当规模大于k时继续使用merge_sort
        improved_sort(A, p, q, k);
        improved_sort(A, q+1, r, k);            
        }
        else if(((r-p+1) / 2) <= k && ((r-p+1) / 2) + 1 / 2 <= k){//当规模小于等于k时使用insertion_sort
            insertion_sort(A, p, q);
            insertion_sort(A, p+1, r);
        }
        merge(A, p, q, r);
    }
}

//timer()为计时器函数，利用较高精度的chrono库计时五次取平均值
double timer(int *A, int n, int k){
    double sum = 0.0;
    for(int i = 0; i < 5; i++){
        auto start = high_resolution_clock::now();
        improved_sort(A, 0, n-1, k);
        auto end = high_resolution_clock::now();
        auto time = duration_cast<nanoseconds>(end - start);
        sum += double(time.count());
    }
    return sum / 5.0;
}

int selectK(){//需输入n以及数据以确定
    int n;
    n = 100000;//确定n
    int A[n];
    srand(time(NULL));
    for(int i = 0; i < n; i++){
        A[i] = rand() % 100000 + 1;//输入数据
    }   
    int k_min = 1, k_max = 10000;
    int  k_best;
    //下为枚举法找最佳k值，用下坡法找k值时由于程序运行时间的波动性，可能导致出现较大误差。
    // double fastest = 100000000;
    // for(int k = k_min; k <= k_max; k++){
    //     double time = timer(A, n, k);
    //     cout << " k= " << k << " , t = " << time << endl;
    //     if(time < fastest){
    //         fastest = time;
    //         k_best = k;
    //     }
    // }
    //以下为下坡法
    srand(time(NULL));
    int k2 = rand() % (k_max-k_min) + k_min, k1 = k2 - 1, k3 = k2 + 1;//随机生成一个属于区间(k_min,k2_max]的数k2
    clock_t t_sum = 0, t_start, t_end;
    double t1, t2, t3;
    //记录t1，即当k = k1 = k2 - 1时候的运行时间(5次取平均)
    t1 = timer(A, n, k1);
    //记录t2，即当k = k2时候的运行时间
    t2 = timer(A, n, k2);
    //记录t3，即当k = k3 = k2 + 1时候的运行时间
    t3 = timer(A, n, k3);
    //输出本组样本运行时间情况
    cout << " k=" << k1 << " , t=" << t1 << " ns";
    cout << " k=" << k2 << " , t=" << t2 << " ns"; 
    cout << " k=" << k3 << " , t=" << t3 << " ns" << endl;
    while(!(t1 > t2 && t3 > t2)){//以“下坡”方法得到最小t1时的k值
        if(t1 > t2 && t2 > t3){//即在k=k2处递减
            k_min = k3;//因为递减，所以继续在k>k3部分取测试样本
            k2 = rand() % (k_max-k_min) + k_min, k1 = k2 - 1, k3 = k2 + 1;//重新生成一组k1、k2、k3
        }
        else if(t1 < t2 && t2 < t3){//此时在k=k2处递增
            k_max = k1;//因为递增，所以继续在k<k1部分取测试样本
            k2 = rand() % (k_max-k_min) + k_min, k1 = k2 - 1, k3 = k2 + 1;//重新生成一组k1、k2、k3
        }
        else if(t2 > t1 && t2 > t3){//此情况可能由于计算时间的随机性造成，无效
            k2 = rand() % (k_max-k_min) + k_min, k1 = k2 - 1, k3 = k2 + 1;//重新生成一组k1、k2、k3
        }
        else if(t1 == t2 && t2 != t3){//由于计时器精度问题，不可避免的很快有至少两项出现相等情况
            if(t2 > t3){
                k_min = k3;//因为递减，所以继续在k>k3部分取测试样本
                k2 = rand() % (k_max-k_min) + k_min, k1 = k2 - 1, k3 = k2 + 1;//重新生成一组k1、k2、k3
            }
            else if(t2 < t3){
                k_max = k1;//因为递增，所以继续在k>k3部分取测试样本
                k2 = rand() % (k_max-k_min) + k_min, k1 = k2 - 1, k3 = k2 + 1;//重新生成一组k1、k2、k3                
            }
        }
        else if(t2 == t3 && t1 != t2){
            if(t2 > t1){
                k_max = k1;//因为递增，所以继续在k>k3部分取测试样本
                k2 = rand() % (k_max-k_min) + k_min, k1 = k2 - 1, k3 = k2 + 1;//重新生成一组k1、k2、k3              
            }
            else if(t2 < t1){
                k_min = k3;//因为递减，所以继续在k>k3部分取测试样本
                k2 = rand() % (k_max-k_min) + k_min, k1 = k2 - 1, k3 = k2 + 1;//重新生成一组k1、k2、k3                 
            }
        }
        //进行分别计时
        //记录t1，即当k = k1 = k2 - 1时候的运行时间(5次取平均)
        t1 = timer(A, n, k1);
        //记录t2，即当k = k2时候的运行时间
        t2 = timer(A, n, k2);
        //记录t3，即当k = k3 = k2 + 1时候的运行时间
        t3 = timer(A, n, k3);
        //计时完成，输出本次测试结果，并重新判断t1、t2、t3的大小关系判断是否继续循环
        cout << " k=" << k1 << " , t=" << t1 << " ns";
        cout << " k=" << k2 << " , t=" << t2 << " ns"; 
        cout << " k=" << k3 << " , t=" << t3 << " ns" << endl;
    }
    k_best = k2;
    cout << "final k = " << k_best;
    return k_best;
}

int main(){
    int n,k;
    cin >> n >> k;//确定n、k
    int A[n];
    for(int i = 0; i < n; i++){
        cin >> A[i];//输入数据
    }
    improved_sort(A, 0, n-1, k);//进行优化后的排序
    //输出结果
    for(int i = 0; i < n; i++){
        cout << A[i] << " ";
    }

}