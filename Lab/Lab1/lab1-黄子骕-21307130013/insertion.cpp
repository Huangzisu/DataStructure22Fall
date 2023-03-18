#include<iostream>
#include<vector>
#include<limits.h>

using namespace std;

void insertion_sort(){
    int n;//数据个数
    cin >> n;//得到数据个数
    int unsorted[n];//定义待排序数组
    for(int i = 0; i < n; i++){
        cin >> unsorted[i];//输入数据
    }
    //开始排序
    for(int j = 1; j < n; j++){
        int key = unsorted[j];
        int i = j - 1;
        while(i >= 0 && unsorted[i] > key){
            unsorted[i+1] = unsorted[i];
            --i;
            unsorted[i+1] = key;
        }
    }
    //排序结束
    //输出结果
    for(int i = 0; i < n; i++){
        cout << unsorted[i] << " ";
    }
}


int main(){
    insertion_sort();
    return 0;
}
