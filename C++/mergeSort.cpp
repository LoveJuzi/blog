#include <iostream>
#include <vector>

class MergeSort
{
  // 归并排序
  // 输入：
  //    a: 需要排序的数组
  // 输出：
  //    a: 保证数组a元素有序
  // 条件：
  //    使用归并排序手段
public:
  void operator() (std::vector<int> &a)
  {
    // 归并排序流程
    // 切分数组为两个部分
    // 左侧使用归并排序，右侧也使用归并排序
    // 合并左右两侧已经排序的序列
    mergeSortImpl(a, 0, a.size());
  }

private:
  // 归并排序的实现
  // 采用左闭右开区间计算
  void mergeSortImpl(std::vector<int> &a, int p, int q)
  {
    if (p + 1 >= q)
    { // 表明表中只有一个元素了，那么直接返回
      return;
    }

    auto r = (p + q) >> 1;

    mergeSortImpl(a, p, r);
    mergeSortImpl(a, r, q);

    merge(a, p, r, q);
  }

  // 合并
  void merge(std::vector<int> &a, int p, int r, int q)
  {
    std::vector<int> b;
    std::vector<int> c;

    copyAarry(a, b, p, r);
    copyAarry(a, c, r, q);

    auto i = 0;
    auto j = 0;
    auto k = p;
    auto n = r - p;
    auto m = q - r;
    while (i < n && j < m)
    {
      if (b[i] < c[j])
      {
        a[k] = b[i];
        k++;
        i++;
      }
      else
      {
        a[k] = c[j];
        k++;
        j++;
      }
    }

    while (i < n)
    {
      a[k] = b[i];
      k++;
      i++;
    }

    while (j < m)
    {
      a[k] = c[j];
      k++;
      j++;
    }
  }

  // 拷贝法用来复制元素
  // 采用左闭右开区间计算
  void copyAarry(std::vector<int> &a, std::vector<int> &b, int p, int q)
  {
    for (auto i = p; i < q; i++)
    {
      b.push_back(a[i]);
    }
  }
};

auto mergeSort = MergeSort();

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
  mergeSort(a);
  printArray(a);
  return 0;
}
