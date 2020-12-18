class Solution
{
public:
  string fractionToDecimal(int numerator, int denominator)
  {
    int weizhi; // 当前计算的位置
    map<int, int> xunhuan; // 当前小数计算位置对应的当前位置
    vector<int> xiaoshu; // 小数部分的值
    int zhengshu; // 整数部分的值

    zhengshu = shang(numerator, denominator);
    numerator = yushu(numerator, denominator);
    weizhi = 0;

    while (numerator != 0 && xunhuan.find(numerator) != xunhuan.end()) {
      xiaoshu.push_back(shang(numerator * 10, denominator));
      xunhuan[numerator] = numerator;
      numerator = yushu(numerator * 10, denominator);
    }
    xunhuan[0] = -1;

    return join(zhengshu, xiaoshu, xunhuan[numerator]);
  }

private:
  string join(int zhengshu, vector<int> &xiaoshu, int weizhi) {
    string rt;

    rt = std::to_string(zhengshu);

    if (!xiaoshu.empty()) {
      rt.push_back('.');
    }

    for (int i=0; i<xiaoshu.size(); i++) {
      if (i == weizhi) {
        rt.push_back('(');
      }
      rt.push_back(xiaoshu[i] + '0');
    }

    if (weizhi != -1) {
      rt.push_back(')');
    }

    return rt;
  }

  int shang(int n, int d) {
    return n / d;
  }

  int yushu(int n, int d) {
    return n % d;
  }
};