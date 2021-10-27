#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

class Topo
{
public:
	vector<string> operator()(vector<vector<string>> &a) {
		vector<string> topoxu; // 拓扑序列
		set<string> T; // 查找表
		map<string, vector<string>> graph; // 偏序图集

		if (a.empty()) {
			return topoxu;
		}

		buildGraph(graph, a);

		for (auto itr = graph.begin(); itr!=graph.end(); itr++ ) {
			buildTopoxu(topoxu, T, graph, itr->first);
		}

		return topoxu;
	}

	void buildGraph(map<string, vector<string>> & graph, vector<vector<string>> &a) {
		int n = a.size();
		for (int i=0; i<n; i++) {
			initKey(graph, a[i][1]);
			graph[a[i][1]].push_back(a[i][0]);
		}
	}

	void initKey(map<string, vector<string>> & graph, string key) {
		if (graph.find(key) != graph.end()) {
			return;
		}
		graph.insert(pair<string, vector<string>>(key, vector<string>{}));
	}

	void buildTopoxu(vector<string> & topoxu, set<string> &T, map<string, vector<string>> &graph, string key) {
		if (T.find(key) != T.end() || graph.find(key) == graph.end()) {
			return;
		}

		vector<string> & qianxuji = graph.find(key)->second;
		for (auto itr = qianxuji.begin(); itr!=qianxuji.end(); itr++) {
			buildTopoxu(topoxu, T, graph, *itr);
		}

		topoxu.push_back(key);
		T.insert(key);
	}
};

class Topo2 {
public:
	vector<string> operator()(vector<vector<string>> &a) {
		vector<string> topoxu;
		vector<TopoNode> HT;

		return topoxu;
	}
private:
	class TopoNode {
		string key;
		int score;

	public:
		bool operator< (const TopoNode & oth) const {
			return this->score < oth.score;
		}
	}
}

void print(vector<string> & a) {
	for (auto itr=a.begin(); itr != a.end(); itr++) {
		cout << *itr << " ";
	}
	cout << endl;
}

int main()
{
	return 0;
}