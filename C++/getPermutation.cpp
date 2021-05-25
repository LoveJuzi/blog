class Solution
{
public:
  string getPermutation(int n, int k)
  {
    std::vector<std::string> a;
    init(a, n);
    kth(a, k, 0, n);
    return join(a);
  }

private:
  void init(std::vector<std::string> &a, int n)
  {
    for (int i = 1; i <= n; i++)
    {
      a.push_back(std::to_string(i));
    }
  }

  void kth(std::vector<std::string> &a, int &k, int s, int n)
  {
    if (s == n)
    { // 说明找到一个序列了
      k = k - 1;
      return;
    }
    if (k == 0)
    {
      return;
    }
    for (int i = s; i < n; i++)
    {
      next(a, s, i);
      kth(a, k, s + 1, n);
      if (k == 0)
      {
        return;
      }
      pre(a, s, i);
    }
  }

  std::string join(std::vector<std::string> &a)
  {
    std::string rt;

    for (int i = 0; i < a.size(); i++)
    {
      rt += a[i];
    }

    return rt;
  }

  void next(std::vector<std::string> &a, int p, int q)
  {
    auto tmp = a[q];
    for (int i = q; i > p; i--)
    {
      a[i] = a[i - 1];
    }
    a[p] = tmp;
  }

  void pre(std::vector<std::string> &a, int p, int q)
  {
    auto tmp = a[p];
    for (int i = p; i < q; i++)
    {
      a[i] = a[i + 1];
    }
    a[q] = tmp;
  }

  void print(std::vector<std::string> a)
  {
    for (int i = 0; i < a.size(); i++)
    {
      std::cout << a[i] << " ";
    }
    std::cout << std::endl;
  }
};