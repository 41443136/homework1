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