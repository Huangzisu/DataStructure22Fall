#include<iostream>
#include<vector>
#include<cstdlib>
#include<time.h>
#include<cmath>
#include<stdexcept>
#include<limits.h>

using namespace std;

void max_heapify_d(vector<int> &A, int i, int d){
    int largest = i;
    for(int j = 1; j <= d; ++j){
        int child = d * i + j;//确定子节点
        if(child < A.size()){//若存在此子节点则与保存的最大节点比较，不存在则break
            if(A[child] > A[largest]){
                largest = child;
            }
        }else{
            break;
        }          
    }
    if(largest != i){//若最大子节点比此父节点大，则交换
        int temp = A[largest];
        A[largest] = A[i];
        A[i] = temp;
        max_heapify_d(A, largest, d);//递归调用max_heapify_d
    }
    
}

void build_max_heap_d(vector<int> &A, int d){
    int heap_size = A.size();
    for(int i = (heap_size - 1) / d; i >= 0; --i){//对所有父节点进行max_heapify_d
        max_heapify_d(A, i, d);
    }
}

int extract_max_d(vector<int> &A, int d){
    try{
        if(A.size() < 1){
            throw runtime_error("heap underflow");
        }
    }catch(runtime_error &e){
        cerr << e.what() << endl;
        return -1;//若A中无元素则抛出异常并return-1
    }
    int max = A[0];//取出最大元素
    A[0] = A[A.size() - 1];//将最后一个元素放到堆顶
    A.pop_back();//删除最后一个位置的元素并且使得size-1
    max_heapify_d(A, 0, d);//对堆顶元素进行heapify
    return max;
}

void heap_increaseKey_d(vector<int> &A, int i, int k, int d){
    try{
        if(k < A[i]){
            throw runtime_error("new key is smaller than current key");
        }
    }catch(runtime_error &e){
        cerr << e.what() << endl;
        return;//若新key小于原来值则抛出异常并return
    }
    A[i] = k;//若k>=A[i]，则将A[i]增加到k
    while(i > 0 && A[(i - 1) / d] < A[i]){//若A[i]比父节点大则交换他们，知道它到达合适的位置
        int temp = A[i];
        A[i] = A[(i - 1) / d];
        A[(i - 1) / d] = temp;
        i = (i - 1) / d;//更新i
    }
    return;
}

void heap_insert_d(vector<int> &A, int x, int d){
    A.push_back(INT_MIN);//使堆的大小加一并将最后一个元素赋值为负无穷
    heap_increaseKey_d(A, A.size()-1, x, d);//调用heap_increaseKey_d
}

int main(){
    vector<int> A;        
    srand(time(NULL));
    int n = rand() % 100 + 1;//随机生成一个大小为1~100的n作为堆的大小
    int d = rand() % 50 + 1;//随机生成一个大小为1~50的d作为堆的分叉数
    for(int i = 0; i < n; i++){//随机生成堆内元素（1`100）
        A.push_back(rand() % 100 + 1);
    }
    build_max_heap_d(A, d);//构造最大堆
    //输出
    // for(int i = 0; i < A.size(); i++){
    //     cout << A[i] << " ";
    // }
    // cout << endl;
}