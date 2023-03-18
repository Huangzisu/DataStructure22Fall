#include<iostream>
#include<vector>
#include<limits.h>

using namespace std;

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

void merge_sort(int *A, int p, int r){
    if(p < r){
        int q = (p + r) / 2;
        merge_sort(A, p, q);
        merge_sort(A, q+1, r);
        merge(A, p, q, r);
    }
}

void Merge_sort(){
    int n;
    cin >> n;//得到数据个数n
    int A[n];
    for(int i = 0; i < n; i++){
        cin >> A[i];//输入数据
    }
    merge_sort(A, 0, n-1);//进行排序
    //输出结果
    for(int i = 0; i < n; i++){
        cout << A[i] << " ";
    }
}

int main(){
    Merge_sort();
}