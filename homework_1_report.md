# 41143263

作業一：問題一（阿克曼函數 Ackermann's Function）

## 解題說明

本題要求實現遞迴與非遞迴函式，計算阿克曼函數（Ackermann's function）的值。

### 解題策略

1. **遞迴版本**：
   根據題目定義，將問題拆解為三種情況：
   * 當 $m = 0$ 時，回傳 $n + 1$。
   * 當 $m > 0$ 且 $n = 0$ 時，呼叫 $A(m-1, 1)$。
   * 其他情況，呼叫 $A(m-1, A(m, n-1))$。主程式只需準備 $m, n$ 並呼叫函式完成所有遞迴計算。
2. **非遞迴版本**：
   在禁用 `<stack>` 的情況下，使用動態陣列 `int* s = new int[capacity]` 與指標 `top` 來自行模擬 Stack 行為。
   透過 `while(top >= 0)` 不斷從 Stack 取出未完成的 $m$，並依據 $m=0, n=0$ 或其他情況改變 $n$ 並推入新的狀態，直到 Stack 清空。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>

using namespace std;

// 遞迴版本
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

// 非遞迴版本
int ackermann_nonrecursive(int m, int n) {
    int capacity = 16; // 初始容量
    int top = -1;
    int* s = new int[capacity];

    push_stack(s, top, capacity, m);

    while (top >= 0) {
        m = pop_stack(s, top);

        if (m == 0) {
            n++;
        } else if (n == 0) {
            n = 1;
            push_stack(s, top, capacity, m - 1);
        } else {
            n--;
            push_stack(s, top, capacity, m - 1);
            push_stack(s, top, capacity, m);
        }
    }
    
    delete[] s;
    return n;
}

int main() {
    int m = 2, n = 2;
    cout << "Ackermann Recursive A(" << m << ", " << n << ") = " 
         << ackermann_recursive(m, n) << '\n';
    cout << "Ackermann Non-recursive A(" << m << ", " << n << ") = " 
         << ackermann_nonrecursive(m, n) << '\n';
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

1. 遞迴與非遞迴版本皆能正確計算阿克曼函數的值。
2. 透過動態陣列自製 Stack，成功避開了 C++ 標準模板庫的限制，並以擴充容量機制解決了預設 $capacity=16$ 可能不足的問題。
3. 測試案例涵蓋了不同層級的增長幅度，驗證了演算法在小數值下的正確性。

## 申論及開發報告

### 遞迴與非遞迴實作的設計取捨

在本程式中，探討遞迴與非遞迴計算阿克曼函數的主要心得如下：

1. **遞迴程式邏輯簡單直觀**
   遞迴版本完美貼合了數學定義，程式碼簡潔易懂。但由於阿克曼函數是非原始遞迴函數，其巢狀呼叫會迅速消耗系統 Call Stack，極易導致 Stack Overflow。
2. **非遞迴陣列模擬的必要性**
   為了在不能使用 `<stack>` 的情況下解決遞迴深度的限制，採用動態配置 `int* s = new int[capacity]` 來將記憶體需求轉移至 Heap 區段。
   透過 `push_stack` 與 `pop_stack` 集中管理未處理的 $m$ 值，雖然程式碼較為冗長，但能更安全地掌控記憶體的運用，是學習底層資料結構操作的極佳實踐。

---

# 41143263

作業一：問題二（冪集 Power Set）

## 解題說明

本題要求實現一個遞迴函式，找出給定集合的所有可能子集（Powerset），並按照特定格式輸出。

### 解題策略

1. 使用布林陣列 `chosen[]` 記錄每個元素是否被選入子集，透過遞迴函式處理「選」與「不選」兩條分支，並不斷前進到下一個 `index`。
2. 當 `index == m`（集合大小）時作為 Base Case 觸發輸出，表示所有元素已決定完畢。
3. 輸出時使用全域變數 `is_first_subset` 與區域變數 `first_element` 精準控制子集與元素之間的逗號排版。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

// 全域變數
int m;
bool is_first_subset = true;

void powerset(int index, bool* chosen, char* p) {
    // Base Case：所有元素都已走訪
    if (index == m) {
        if (!is_first_subset)
            cout << ", ";
        is_first_subset = false;

        cout << "(";
        bool first_element = true;
        for (int i = 0; i < m; i++) {
            if (chosen[i]) {
                if (!first_element)
                    cout << ",";
                cout << p[i];
                first_element = false;
            }
        }
        cout << ")";
        return;
    }

    // 分支 1：不放入目前元素
    chosen[index] = false;
    powerset(index + 1, chosen, p);

    // 分支 2：放入目前元素
    chosen[index] = true;
    powerset(index + 1, chosen, p);
}

int main() {
    string input = "abc";
    m = input.length();
    
    char* p = new char[m];
    for (int i = 0; i < m; i++) {
        p[i] = input[i];
    }
    sort(p, p + m); // 確保集合元素有序
    
    bool* chosen = new bool[m];
    
    cout << "powerset(S) = {";
    powerset(0, chosen, p);
    cout << "}\n";
    
    delete[] p;
    delete[] chosen;
    
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

1. 程式能正確計算並展開集合 $S$ 的所有子集。
2. 透過 `is_first_subset` 與 `first_element` 的布林狀態控制，成功實作了符合題目嚴格要求的括號與逗號排版。
3. 主程式與遞迴函式權責分明，主程式處理完字元排序與記憶體配置後，將搜尋工作完美交給了遞迴核心。

## 申論及開發報告

### 選擇 Backtracking 遞迴探勘的原因

在本程式中，使用遞迴計算 Powerset 的主要原因如下：

1. **二元樹結構的完美對應**
   子集的生成本質上是一個深度優先搜尋（DFS）的過程。對於每一個位置的元素，我們設定 `chosen[index] = false` 與 `chosen[index] = true` 來模擬走訪二元樹的左右分支，直到到達葉節點（`index == m`）才將累積的結果印出。這比起使用位元運算（Bit Manipulation）來得更加直觀且符合資料結構的教學邏輯。
2. **免去複雜的字串疊加操作**
   受限於可用標頭檔，我們無法輕易使用 `<vector><string>` 收集所有結果再統一處理。透過傳遞一個全域的狀態陣列 `chosen` 搭配即時輸出的策略（On-the-fly printing），大幅降低了記憶體的消耗，也避開了繁瑣的字串切割與重組。