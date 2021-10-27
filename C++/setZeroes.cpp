class Solution
{
public:
  void setZeroes(vector<vector<int>> &matrix)
  {
    int m = matrix.size();
    if (m == 0) {
      return;
    }
    int n = matrix[0].size();

    print(matrix, m, n);

    bool firstRow = false;
    bool firstCol = false;

    for (int j=0; j<n; j++) {
      if (isZero(matrix, 0, j)) {
        firstRow = true;
        break;
      }
    }

    for (int i=0; i<m; i++) {
      if (isZero(matrix, i, 0)) {
        firstCol = true;
        break;
      }
    }

    print(matrix, m, n);

    for (int i=1; i<m; i++) {
      for (int j=1; j<n; j++) {
        if (isZero(matrix, i, j)) {
          setFlag(matrix, i, j);
        }
      }
    }

    print(matrix, m, n);

    for (int i=1; i<m; i++) {
      if (isZero(matrix, i, 0)) {
        setRowZero(matrix, i, n);
      }
    }

    for (int j=1; j<n; j++) {
      if (isZero(matrix, 0, j)) {
        setColZero(matrix, m, j);
      }
    }

    if (firstRow) {
      setRowZero(matrix, 0, n);
    }

    if (firstCol) {
      setColZero(matrix, m, 0);
    }
  }

private:
  // 这题考的是如何利用当前的空间
  // m行 n列
  void setFlag(vector<vector<int>> & a, int m, int n) {
    a[m][0] = 0;
    a[0][n] = 0;
  }

  bool isZero(vector<vector<int>> & a, int m, int n) {
    return a[m][n] == 0;
  }

  void setRowZero(vector<vector<int>> & a, int i, int n) {
    for (int j=0; j<n; j++) {
      a[i][j]= 0;
    }
  }

  void setColZero(vector<vector<int>> & a, int m, int j) {
    for (int i=0; i<m; i++) {
      a[i][j] = 0;
    }
  }

  void print(vector<vector<int>> & a, int m, int n) {
    for (int i=0; i<m; i++) {
      for (int j=0; j<n; j++) {
        cout << a[i][j] << " ";
      }
      cout << endl;
    }
    cout << endl;
  }
};