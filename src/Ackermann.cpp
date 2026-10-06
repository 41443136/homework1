#include <iostream>
using namespace std;

// 遞迴版
int ackermann_recursive(int m, int n) {
    if (m == 0)
        return n + 1;
    if (n == 0)
        return ackermann_recursive(m - 1, 1);
    return ackermann_recursive(m - 1, ackermann_recursive(m, n - 1));
}

// 模擬 Stack 用的 push 與 pop 函式
void push_stack(int*& s, int& top, int& capacity, int value) {
    // 陣列滿時動態擴充容量
    if (top + 1 >= capacity) {
        capacity *= 2;
        int* new_s = new int[capacity];
        for (int i = 0; i <= top; i++) {
            new_s[i] = s[i];
        }
        delete[] s;
        s = new_s;
    }
    s[++top] = value;
}

int pop_stack(int* s, int& top) {
    return s[top--];
}

// 非遞迴版
int ackermann_nonrecursive(int m, int n) {
    int capacity = 16; // 初始容量
    int top = -1;
    int* s = new int[capacity];

    push_stack(s, top, capacity, m);
    while (top >= 0) {
        m = pop_stack(s, top);

        if (m == 0) {
            n++;
        }
        else if (n == 0) {
            n = 1;
            push_stack(s, top, capacity, m - 1);
        }
        else {
            n--;
            push_stack(s, top, capacity, m - 1);
            push_stack(s, top, capacity, m);
        }
    }
    delete[] s;
    return n;
}

int main() {
    int m, n;
    
    cout << "Enter m and n (for example: 2 2): ";
    

    while (cin >> m >> n) {
        
        // Ackermann 函數規定輸入必須是非負整數，加入防呆檢查
        if (m < 0 || n < 0) {
            cout << "Please enter non-negative integers.\n";
            cout << "Enter m and n (for example: 2 2): ";
            continue; 
        }

        cout << "Ackermann Recursive A(" << m << ", " << n << ") = "
             << ackermann_recursive(m, n) << '\n';
        cout << "Ackermann Non-recursive A(" << m << ", " << n << ") = "
             << ackermann_nonrecursive(m, n) << "\n\n";
             
        cout << "Enter m and n (for example: 2 2): ";
    }
    
    return 0;
}