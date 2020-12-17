#include <iostream>
#include <vector>

class HeapSort
{
public:
  // 堆排序需要引入堆结构
  // 引入堆结构以后，就可以使用堆结构的性质进行排序
  // 堆的定义是：结点元素的值比左右子树的结点值都要大，左右子树也是一个堆结构
  // 堆可以采用数组表示法
  // 引入索引计算，通过索引可以计算关联的索引
  void operator()(std::vector<int> &a)
  {
    heapSortImpl(a);
  }

private:
  void heapSortImpl(std::vector<int> &a)
  {
    BuildHeap(a); // 构建大顶堆
    int n = a.size();
    for (int i = n - 1; i >= 0; i--)
    {
      swap(a[0], a[i]);
      AdjustHeap(a, 0, i); // 保证堆的性质
    }
  }

  void BuildHeap(std::vector<int> &a)
  {
    int n = a.size();
    if (n == 0)
    {
      return;
    }
    for (int i = n / 2; i >= 0; i -= 1)
    {
      // 自底向上构建堆
      AdjustHeap(a, i, n);
    }
  }

  // 采用自顶向下调整堆
  void AdjustHeap(std::vector<int> &a, int p, int n)
  {
    auto l = left(p);
    auto r = right(p);
    auto m = p;
    if (l < n && a[m] < a[l])
    {
      m = l;
    }
    if (r < n && a[m] < a[r])
    {
      m = r;
    }

    swap(a[m], a[p]);

    if (m != p)
    {
      AdjustHeap(a, m, n);
    }
  }

  // 左孩子
  int left(int p)
  {
    return (p + 1) * 2 - 1;
  }

  // 右孩子
  int right(int p)
  {
    return (p + 1) * 2;
  }

  void swap(int &a, int &b)
  {
    auto tmp = a;
    a = b;
    b = tmp;
  }
};

auto heapSort = HeapSort();

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
  heapSort(a);
  printArray(a);
  return 0;
}
