# 41143263

作業一

---

# 第一題：Ackermann's Function（阿克曼函數）

## 解題說明

阿克曼函數（Ackermann's Function）是一個著名的超重度成長函數，其數學定義如下：

$$
A(m, n) = \begin{cases} 
n + 1 & \text{if } m = 0 \\ 
A(m - 1, 1) & \text{if } n = 0 \\ 
A(m - 1, A(m, n - 1)) & \text{otherwise} 
\end{cases}
$$

本題要求分別實作「遞迴版本」與「非遞迴（迭代）版本」來計算 $A(m, n)$。

### 解題策略

1. **遞迴版本**：
   - 嚴格依照數學分段定義直接對應條件判斷：
     - 若 $m = 0$，直接回傳 $n + 1$（基礎終止條件）。
     - 若 $n = 0$，遞迴呼叫 $A(m - 1, 1)$。
     - 其他情況，遞迴呼叫 $A(m - 1, A(m, n - 1))$。

2. **非遞迴版本**：
   - 由於阿克曼函數涉及多層與嵌套的遞迴呼叫，無法單純用簡單計數迴圈完成，需要利用自建的**堆疊（Stack）**結構來模擬系統的呼叫堆疊（Call Stack）。
   - 堆疊內存放當前待計算的 $m$ 值，並以變數維護 $n$ 的數值：
     - 當堆疊頂端為 $m = 0$ 時，出棧並更新 $n = n + 1$。
     - 當 $n = 0$ 時，將頂端的 $m$ 替換為 $m - 1$，並令 $n = 1$。
     - 其餘情況（$m > 0$ 且 $n > 0$），需要先計算內層 $A(m, n - 1)$，再傳入外層 $A(m - 1, \cdot)$，因此在棧中依序壓入 $m - 1$ 與 $m$，並將 $n$ 遞減為 $n - 1$。
     - 當堆疊為空時，$n$ 即為最終結果。

## 程式實作

以下為第一題之主要程式碼：

```cpp
#include <iostream>
#include <stack>
#include <stdexcept>
using namespace std;

// 遞迴版本
int ackermann_recursive(int m, int n) {
    if (m < 0 || n < 0) {
        throw invalid_argument("m and n must be non-negative");
    }
    if (m == 0) {
        return n + 1;
    } else if (n == 0) {
        return ackermann_recursive(m - 1, 1);
    } else {
        return ackermann_recursive(m - 1, ackermann_recursive(m, n - 1));
    }
}

// 非遞迴版本（利用 Stack 模擬呼叫堆疊）
int ackermann_nonrecursive(int m, int n) {
    if (m < 0 || n < 0) {
        throw invalid_argument("m and n must be non-negative");
    }

    stack<int> s;
    s.push(m);

    while (!s.empty()) {
        m = s.top();
        s.pop();

        if (m == 0) {
            n = n + 1;
        } else if (n == 0) {
            s.push(m - 1);
            n = 1;
        } else {
            s.push(m - 1);
            s.push(m);
            n = n - 1;
        }
    }
    return n;
}

int main() {
    int m = 2, n = 2;
    cout << "Ackermann Recursive (" << m << ", " << n << ") = " 
         << ackermann_recursive(m, n) << '\n';
    cout << "Ackermann Non-recursive (" << m << ", " << n << ") = " 
         << ackermann_nonrecursive(m, n) << '\n';
    return 0;
}
```

## 效能分析

1. **時間複雜度**：
   阿克曼函數是非原始遞迴（non-primitive recursive）函數，其運算步數與輸出值呈超指數成長（包含階乘冪與塔式指數運算）。
   - 對於固定的小值：
     - 當 $m = 1$ 時，時間複雜度為 $O(n)$。
     - 當 $m = 2$ 時，時間複雜度為 $O(n)$。
     - 當 $m = 3$ 時，值為 $2^{n+3} - 3$，時間複雜度為 $O(2^n)$。
     - 當 $m = 4$ 時，成長速度已為超重度塔式指數級 $O(2^{2^{\dots}} )$。
   - 總體時間複雜度取決於函數值本身：$O(A(m, n))$。

2. **空間複雜度**：
   - **遞迴版本**：最大系統呼叫堆疊深度為 $O(A(m, n))$，極易在 $m \ge 4$ 時造成 Stack Overflow。
   - **非遞迴版本**：使用自訂堆疊模擬，堆疊空間大小同為 $O(A(m, n))$，但因配置在 Heap/自由記憶體區段，容量上限遠大於執行緒的系統調用堆疊。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $(m, n)$ | 預期輸出 | 遞迴輸出 | 非遞迴輸出 |
|----------|-------------------|----------|----------|------------|
| 測試一   | $(0, 0)$          | 1        | 1        | 1          |
| 測試二   | $(1, 2)$          | 4        | 4        | 4          |
| 測試三   | $(2, 2)$          | 7        | 7        | 7          |
| 測試四   | $(3, 2)$          | 29       | 29       | 29         |
| 測試五   | $(3, 3)$          | 61       | 61       | 61         |
| 測試六   | $(-1, 2)$         | 異常拋出 | 異常拋出 | 異常拋出   |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -O2 -o ackermann ackermann.cpp
$ ./ackermann
Ackermann Recursive (2, 2) = 7
Ackermann Non-recursive (2, 2) = 7
```

### 結論

1. 遞迴與非遞迴版本均能精確算出符合數學定義的阿克曼函數值。
2. 兩版本在各項邊界值（如 $m=0$ 或 $n=0$）皆能正確運算；遇到負數參數均能正常拋出例外阻斷。
3. 非遞迴版本透過顯式堆疊（`std::stack`）完全消除函數遞迴呼叫限制，展示了將深層雙遞迴轉化為迭代模擬的完整流程。

## 申論及開發報告

### 遞迴與非遞迴之對比分析

1. **實作複雜度與可讀性**：
   - 遞迴版本程式碼極為簡練，幾乎是數學式定義的一對一映射，邏輯清晰且易於除錯與驗證。
   - 非遞迴版本需要以資料結構手動維護狀態轉換順序，特別是雙層呼叫 $A(m - 1, A(m, n - 1))$ 時需要逆向思考壓棧順序，實作門檻較高。

2. **堆疊溢位（Stack Overflow）之考量**：
   - 系統分配給個別執行緒的 Call Stack 容量通常僅數個 MB（例如 Linux 預設 8MB，Windows 預設 1MB）。阿克曼函數在 $m \ge 4$ 時展開層數極其龐大，遞迴版本會瞬間觸發 Segment Fault。
   - 非遞迴版本將狀態節點儲存於標準庫容器中（位於 Heap 記憶體），能承受更深的運算步數。

---

# 第二題：Powerset（冪集）

## 解題說明

給定一個包含 $n$ 個相異元素的集合 $S$，其冪集 $\mathcal{P}(S)$ 定義為由 $S$ 的所有可能子集所組成的集合（包含空集合與自身）。
例如：若 $S = \{a, b, c\}$，則其冪集為：
$$\mathcal{P}(S) = \{\emptyset, \{a\}, \{b\}, \{c\}, \{a, b\}, \{a, c\}, \{b, c\}, \{a, b, c\}\}$$

本題要求撰寫一個遞迴函式來計算並輸出給定集合的冪集。

### 解題策略

1. **遞迴狀態設計（包含 / 不包含決策）**：
   - 對於集合中的每一個元素 $S[i]$，在建構子集合時只有兩種選擇：**選入** 或 **不選入**。
   - 定義遞迴函式 `generatePowerset(index, currentSubset)`：
     - **終止條件**：當 `index == n`（所有元素皆決策完畢），將當前子集加入結果集。
     - **遞迴分支一（不選）**：直接呼叫 `generatePowerset(index + 1, currentSubset)`。
     - **遞迴分支二（選入）**：將 $S[index]$ 加入子集，呼叫 `generatePowerset(index + 1, currentSubset)`，呼叫完畢後執行回溯（Pop）。
2. 主程式以空子集合及索引 0 啟動遞迴，依序完成整個二元決策樹的走訪。

## 程式實作

以下為第二題之主要程式碼：

```cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

// 遞迴計算冪集
void getPowersetRecursive(const vector<char>& S, size_t index, 
                         vector<char>& current, vector<vector<char>>& result) {
    // 終止條件：已對集合中所有元素完成決策
    if (index == S.size()) {
        result.push_back(current);
        return;
    }

    // 決策一：不選入目前元素
    getPowersetRecursive(S, index + 1, current, result);

    // 決策二：選入目前元素
    current.push_back(S[index]);
    getPowersetRecursive(S, index + 1, current, result);

    // 回溯恢復狀態
    current.pop_back();
}

// 輔助函式：排版列印冪集
void printPowerset(const vector<vector<char>>& pset) {
    cout << "{ ";
    for (size_t i = 0; i < pset.size(); ++i) {
        cout << "(";
        for (size_t j = 0; j < pset[i].size(); ++j) {
            cout << pset[i][j];
            if (j + 1 < pset[i].size()) cout << ", ";
        }
        cout << ")";
        if (i + 1 < pset.size()) cout << ", ";
    }
    cout << " }\n";
}

int main() {
    vector<char> S = {'a', 'b', 'c'};
    vector<vector<char>> result;
    vector<char> current;

    getPowersetRecursive(S, 0, current, result);

    cout << "Original Set S: {a, b, c}\n";
    cout << "Powerset(S) count: " << result.size() << "\n";
    cout << "Powerset(S) = ";
    printPowerset(result);

    return 0;
}
```

## 效能分析

1. **時間複雜度**：
   - 大小為 $n$ 的集合其冪集共有 $2^n$ 個子集合。
   - 決策樹為深度為 $n$ 的完全二元樹，葉節點共有 $2^n$ 個，每次葉節點加入結果需要複製子集合長度（平均長度為 $\frac{n}{2}$）。
   - 總時間複雜度為 $O(n \cdot 2^n)$。

2. **空間複雜度**：
   - 呼叫堆疊的最大深度等於集合元素數量 $n$，即堆疊空間為 $O(n)$。
   - 若計入儲存全數輸出結果的空間，共需儲存 $2^n$ 個子集，空間複雜度為 $O(n \cdot 2^n)$；若不計輸出容器，暫存回溯陣列空間僅為 $O(n)$。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入集合 $S$ | 子集總數 ($2^n$) | 輸出內容驗證 |
|----------|--------------|------------------|--------------|
| 測試一   | $\{\}$ (空集合) | $2^0 = 1$        | $\{()\}$ |
| 測試二   | $\{a\}$      | $2^1 = 2$        | $\{(), (a)\}$ |
| 測試三   | $\{a, b\}$   | $2^2 = 4$        | $\{(), (b), (a), (a, b)\}$ |
| 測試四   | $\{a, b, c\}$| $2^3 = 8$        | $\{(), (c), (b), (b, c), (a), (a, c), (a, b), (a, b, c)\}$ |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -O2 -o powerset powerset.cpp
$ ./powerset
Original Set S: {a, b, c}
Powerset(S) count: 8
Powerset(S) = { (), (c), (b), (b, c), (a), (a, c), (a, b), (a, b, c) }
```

### 結論

1. 透過二元決策樹模型，遞迴結構能嚴密地涵蓋所有 $2^n$ 個子集組合，沒有遺漏或重複。
2. 搭配回溯（Backtracking）技巧，空間消耗控制在 $O(n)$，避免了重複拷貝子集的資源浪費。

## 申論及開發報告

### 選擇遞迴實作冪集的核心優勢

1. **概念抽象與分治契機**：
   冪集本質上符合分治原則：一個大小為 $n$ 的集合 $S$ 之子集，可以分為「包含最後一個元素」與「不包含最後一個元素」兩大群體，每群恰好為前 $n-1$ 個元素的子集結構。遞迴自然對應了這一遞推關係。

2. **避免位元運算的長度受限**：
   非遞迴常見的做法是利用「位元遮罩（Bitmasking）」，以整數的每一位元（Bit）代表元素是否存在。然而整數類型有位元上限（如 `uint64_t` 最大僅支援 64 個元素），遞迴回溯法不受硬體暫存器長度限制，擴展性更佳。