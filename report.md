# 41443136

問題一（阿克曼函數 Ackermann's Function）

## 解題說明

本題要求實現遞迴與非遞迴函式，計算阿克曼函數（Ackermann's function）的值。

### 解題策略

主要概念：遞迴 (Recursion)

這題的解題邏輯很像在做「是非題」。我們針對集合裡面的每一個字母，都去問「要」還是「不要」，把所有可能的組合都走過一遍。

具體步驟：

整理資料：先把使用者輸入的字母排好順序，並且把重複的字母剔除掉，確保資料是乾淨的。

遞迴做選擇：寫一個遞迴函式，每一次面對一個字母時，程式都會走兩條路：

第一條路：不選這個字母，直接前往下一個字母。

第二條路：選這個字母，把它記錄下來，然後前往下一個字母。

印出結果：當所有的字母都決定好「選或不選」之後，就把有被選到的字母加上括號印出來。接著程式會自動退回上一部，繼續嘗試其他的選擇，直到所有組合都印完。

## 程式實作

以下為主要程式碼：

```cpp
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
```

## 效能分析

1. 時間複雜度：時間複雜度為 $O(A(m, n))$，因為阿克曼函數成長極快，執行時間與最終計算出的數值成正比。
2. 空間複雜度：空間複雜度為 $O(m \times A(m, n))$，遞迴版本受限於 Call Stack 深度，非遞迴版本則取決於動態陣列的擴充容量，均會隨 $m, n$ 增大而急遽增加。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $m, n$ | 預期輸出 | 實際輸出 |
|----------|-----------------|----------|----------|
| 測試一   | $m = 0, n = 0$  | 1        | 1        |
| 測試二   | $m = 1, n = 1$  | 3        | 3        |
| 測試三   | $m = 2, n = 2$  | 7        | 7        |
| 測試四   | $m = 3, n = 2$  | 29       | 29       |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o ackermann ackermann.cpp
$ ./ackermann
Ackermann Recursive A(2, 2) = 7
Ackermann Non-recursive A(2, 2) = 7
```

### 結論

1. 程式能準確計算出 Ackermann 函數的結果。
2. 具備防呆機制，若使用者輸入負數（不符合函數規定），會要求重新輸入。
3. 成功實作了「遞迴」與「非遞迴（自建堆疊）」兩種版本，兩種算出來的答案完全一致。

## 申論及開發報告

### 遞迴與非遞迴實作的設計取捨

遞迴與自建堆疊的選擇與比較
在實作這題的過程中，我們對比了兩種寫法的優缺點：

1. 遞迴版的優點：最直觀的數學翻譯
   遞迴版的寫法非常簡單，基本是把數學公式翻譯成程式碼：
      if (m == 0) return n + 1;
   這種寫法完全不需要動腦筋去設計額外的變數，對於把數學公式轉換成程式來說，是最好懂的方法。

2. 遞迴版的致命缺點：容易當機 (Stack Overflow)
   Ackermann 是一個計算層數極度誇張的函數。如果只依賴電腦系統來做遞迴，當數字稍微大一點（例如 m=4, n=1），電腦用來記錄計算過程的記憶體空間很快就會塞爆，導致程式直接當機崩潰。

3. 非遞迴版（自建堆疊）的突破
   為了解決遞迴會當機的問題，我們改用一個陣列來自己做「堆疊 (Stack)」。我們把還沒算完的數字先塞進陣列裡，慢慢拿出來算。
   雖然這種寫法比較複雜、程式碼也變長了，但因為我們使用的是電腦裡空間比較大的記憶體區域（Heap），所以成功突破了系統限制，讓程式算得更安全、不會當機。這展現了為了「系統穩定性」而犧牲一點「程式易    讀性」的實際開發考量。

---

問題二（冪集 Power Set）

## 解題說明

主要概念：遞迴 vs 自建堆疊 (Stack)這題的目的是實作一個數學函數，並且比較「遞迴寫法」與「非遞迴寫法」的差異。具體步驟：遞迴版 (Recursive)：最直覺的寫法，就是直接把題目給的數學公式翻譯成 if-else 程式碼。雖然寫起來很短很簡單，但因為這個函數的計算層數非常深，數字只要稍微大一點，電腦的系統記憶體就會爆炸，造成程式當機 (Stack Overflow)。非遞迴版 (Non-recursive)：為了解決遞迴會當機的問題，我們改用 while 迴圈來做。我們自己建立一個陣列當作「堆疊 (Stack)」，用來代替電腦記住「還沒算完的數字 $m$」。程式在迴圈裡，會不斷把數字從堆疊拿出來看，依照公式規則更新數字 $n$，或是把新的數字推回堆疊裡。等到堆疊完全清空，沒有數字需要算的時候，留下來的 $n$ 就是最後的答案。這個方法比較複雜，但執行起來更安全。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <algorithm>

using namespace std;

// 全域變數
char p[100];
bool chosen[100];
int m = 0;
bool is_first = true;

void powerset(int index) 
{
    if (index == m) 
    {
        if (is_first == false) 
        {
            cout << ", ";
        }
        is_first = false;

        cout << "(";
        bool first_char = true;
        for (int i = 0; i < m; i++) 
        {
            if (chosen[i] == true) 
            {
                if (first_char == false) 
                {
                    cout << ",";
                }
                cout << p[i];
                first_char = false;
            }
        }
        cout << ")";
        return; 
    }

    chosen[index] = false;
    powerset(index + 1); 

    chosen[index] = true;
    powerset(index + 1); 
}

int main() 
{
    int n;
    
    cout << "Enter the number of characters: ";
    
    while (cin >> n) 
    {

        if (n < 0) 
        {
            cout << "Please enter a non-negative integer.\n";
   
            cout << "Enter the number of characters: ";
            continue; 
        }

        // 要把狀態重置
        m = 0;
        is_first = true;

 
        cout << "Enter the characters (for example: a b c): ";
        
        char temp[100];
        for (int i = 0; i < n; i++) 
        {
            cin >> temp[i];
        }

        //排序
        sort(temp, temp + n);

        //土法煉鋼去重複
        for (int i = 0; i < n; i++) 
        {
            if (i == 0 || temp[i] != temp[i - 1]) 
            {
                p[m] = temp[i];
                m++; 
            }
        }

        cout << "powerset(S) = {";
        powerset(0);
        cout << "}\n\n"; 

  
        cout << "Enter the number of characters: ";
    }

    return 0;
}
```

## 效能分析

1. 時間複雜度：程式的時間複雜度為 $O(2^n)$，其中 $n$ 為集合的大小（程式碼中的 $m$），因為每個元素皆有選與不選兩種狀態。
2. 空間複雜度：空間複雜度為 $O(n)$，主要消耗在函式遞迴呼叫的深度與動態陣列 `chosen[]` 和 `p[]` 的儲存空間。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入集合 `S` | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | `a`          | `{(), (a)}` | `{(), (a)}` |
| 測試二   | `ab`         | `{(), (b), (a), (a,b)}` | `{(), (b), (a), (a,b)}` |
| 測試三   | `abc`        | `{(), (c), (b), (b,c), (a), (a,c), (a,b), (a,b,c)}` | `{(), (c), (b), (b,c), (a), (a,c), (a,b), (a,b,c)}` |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o powerset powerset.cpp
$ ./powerset
powerset(S) = {(), (c), (b), (b,c), (a), (a,c), (a,b), (a,b,c)}
```

### 結論

1. 程式能正確找出輸入字元的所有可能組合（子集）。
2. 具備防呆機制，若使用者輸入負數，程式會擋下來並引導重新輸入。
3. 程式能自動過濾掉重複的字母，確保印出來的集合符合數學定義，測試結果皆正確。

## 申論及開發報告

### 選擇遞迴的原因

在本程式中，選擇使用遞迴來尋找子集的原因如下：

邏輯像「是非題」一樣簡單
遞迴的寫法能夠很直覺地表達「選擇」的過程。針對每一個字母，我們只需要決定兩條路：「要選」或「不要選」。

程式碼乾淨好懂
如果不使用遞迴，要找出所有子集通常需要寫很複雜的多層迴圈或是二進位運算。使用遞迴，程式碼只需要呼叫自己兩次（一條路選、一條路不選），非常的適合用來學習與理解。

內建自動「退回」的功能 (回溯)
遞迴最大的好處是，當我們走到最後、印出一組答案後，程式會自動「退回」上一層，繼續來嘗試剛剛還沒走過的另一條路。這樣就不需用額外寫程式去記住剛剛走到哪裡。

