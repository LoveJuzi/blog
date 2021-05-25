class Solution
{
public:
  // 条件：
  // null 表示当前目录
  //  .   表示当前目录
  // ..   表示上一级目录
  // 根目录是 /
  // 路径末尾不能有 /，除根目录外
  // // 需要优化城 /
  // 输入是一个绝对路径

  // 切分计算单元
  // 针对计算单元进行组合
  string simplifyPath(string path)
  {
    vector<string> a;
    vector<string> c;

    split(a, path);

    for (auto i=0; i<a.size(); i++) {
      if (isDotDot(a[i])) {
        dealDotDot(c);
      } else if (isDot(a[i])) {
        dealDot(c);
      } else if (isNullPath(a[i])) {
        dealNullPath(c);
      } else {
        dealPath(c, a[i]);
      }
    }

    return join(c);
  }

  void print(vector<string> & a) {
    for (auto i=0; i<a.size(); i++) {
      cout << a[i] << " ";
    }
    cout << endl;
  }

private:
  void split(vector<string> & a, string path) {
    string tmp;
    for (auto i=0; i<path.size(); i++) {
      if (path[i] != '/') {
        tmp.push_back(path[i]);
      } else {
        a.push_back(tmp);
        tmp = "";
      }
    }
    a.push_back(tmp);
  }

  string join(vector<string> & c) {
    string rt;

    for (auto i=0; i<c.size(); i++) {
      rt += "/";
      rt += c[i];
    }

    if (rt == "") {
      rt = "/";
    }

    return rt;
  }
  
  void dealDot(vector<string> & c) {
    return;
  }

  void dealDotDot(vector<string> & c) {
    if (c.empty()) {
      return;
    }
    c.pop_back();
  }

  void dealNullPath(vector<string> & c) {
    return;
  }

  void dealPath(vector<string> & c, string e) {
    c.push_back(e);
  }

  bool isDot(string e) {
    return e == ".";
  }

  bool isDotDot(string e) {
    return e == "..";
  }

  bool isNullPath(string e) {
    return e == "";
  }
};