#include<iostream>
#include<string>
#include<stack>

using namespace std;

int evaluate(string A){
    int n = A.size();
    stack<int> stack;
    int temp;
    for(int i = 0; i < n; i++){
        char now = A[i];
        if(now >= '0' && now <= '9'){//now为数字
            temp = int(now) - int('0');
            stack.push(temp);
        }else if(now == '+'){
            int x1 = stack.top();
            stack.pop();
            int x2 = stack.top();
            stack.pop();
            stack.push(x1 + x2);
        }else if(now == '-'){
            int x1 = stack.top();
            stack.pop();
            int x2 = stack.top();
            stack.pop();
            stack.push(x2 - x1);
        }else if(now == '/'){
            int x1 = stack.top();
            stack.pop();
            int x2 = stack.top();
            stack.pop();
            stack.push(x2 / x1);
        }
        else if(now == '*'){
            int x1 = stack.top();
            stack.pop();
            int x2 = stack.top();
            stack.pop();
            stack.push(x1 * x2);
        }else if(now == 'm'){
            int x1 = stack.top();
            stack.pop();
            int x2 = stack.top();
            stack.pop();
            stack.push(x2 % x1);
        }
    }
    return stack.top();
}

int main(){
    string A;
    cin >> A;
    cout << evaluate(A);
}