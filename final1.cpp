#include <iostream>
#include <vector>
#include <algorithm> // for std::max
#include <fstream>
#include <omp.h>
using namespace std;

// Counting Sort 函數
void countingSort(vector<int> &arr, int max) {
    int min = 0; // 假設最小值為 0
    int range = max - min + 1; // 計數數組的範圍

    // 建立計數數組，並初始化為 0
    vector<int> count(range, 0);

    // 計算每個元素的出現次數
    #pragma omp parallel for
    for (int x=0;x<arr.size();x++) {
        count[arr[x]]++;
    }

    // 累加計數數組
    
    for (int i = 1; i < range; i++) {
        count[i] += count[i - 1];
    }
    // 建立輸出數組
    vector<int> output(arr.size());

    // 將元素放入正確位置，從後往前確保穩定性
    for (int i = arr.size() - 1; i >= 0; i--) {
        output[count[arr[i] - min] - 1] = arr[i];
        count[arr[i] - min]--;
    }

    // 將排序結果複製回原數組
    arr = output;
}

// 主函數
int main() {
    string filename;
    cin >> filename;
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Could not open the file " << filename << endl;
        return 1;
    }

    int n=0, max = 0;
    inputFile >> n >> max;
    vector<int> arr;
    
    
    for(int x=0;x<n;x++){
      int tmp;
       inputFile >> tmp;
       arr.push_back(tmp);
    }
    
    countingSort(arr, max);

    
    for (int num : arr) {
        cout << num << " ";
    }
    

    return 0;
}
