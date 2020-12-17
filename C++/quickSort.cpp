#include <iostream>
#include <vector>

class QuickSort
{
public:
  // 快速排序
  // 输入：
  //    a: 一个无序序列
  // 输出:
  //    a: 一个有序序列
  // 条件:
  //    采用快速排序的算法
  void operator()(std::vector<int> &a)
  {
    // 快速排序流程如下：
    // 1. 选择一个元素做为基准元素，使用基准元素，对数组进行划分
    // 2. 使用快速排序排序左划分，使用快速排序排序右划分
    quickSortImpl(a, 0, a.size() - 1);
  }

private:
  // 采用左闭右闭区间计算
  void quickSortImpl(std::vector<int> &a, int p, int q)
  {
    // 如果序列是空，或者只有一个元素，直接返回
    if (p >= q)
    {
      return;
    }

    auto base = a[q]; // 采用区间的末尾元素为基准元素
    auto k = p;       // 基准元素的位置
    // 划分
    for (auto i = p; i < q; i++)
    {
      if (a[i] < base)
      {
        swap(a[i], a[k]);
        k++;
      }
    }

    swap(a[k], a[q]);

    quickSortImpl(a, p, k - 1);
    quickSortImpl(a, k + 1, q);
  }

  void swap(int &a, int &b)
  {
    auto tmp = a;
    a = b;
    b = tmp;
  }
};

auto quickSort = QuickSort();

// 打印数组
void printArray(std::vector<int> &a)
{
  for (auto i = 0; i < a.size(); i++)
  {
    std::cout << a[i] << " ";
  }
  std::cout << std::endl;
}

int main()
{
  std::vector<int> a = {1, 3, 2, 5, 4, 9, 8, 7, 6, 0};
  printArray(a);
  quickSort(a);
  printArray(a);
  return 0;
}
