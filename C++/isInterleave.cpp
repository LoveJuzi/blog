class Solution
{
public:
    vector<string> findItinerary(vector<vector<string>> &tickets)
    {
        vector<string> rt;

        map<string, map<string, int>> graph;

        int N = tickets.size() + 1;

        buildGraph(graph, tickets);

        rt.push_back("JFK");
        bool success = true;
        find(graph, success, rt, "JFK", N);

        return rt;
    };

    void buildGraph(map<string, map<string, int>> &graph, vector<vector<string>> &tickets)
    {
        int m = tickets.size();

        for (int i = 0; i < m; i++)
        {
            if (graph.find(tickets[i][0]) == graph.end())
            {
                graph[tickets[i][0]] = map<string, int>{};
            }
            graph[tickets[i][0]][tickets[i][1]]++;
        }
    }

    // graph 当前的图
    // success 表示找到一条路径能够遍历所有的
    // path 解的结果
    // start 当前的起点
    // N 总的飞机票数 + 1
    void find(map<string, map<string, int>> &graph, bool &success, vector<string> &path, string start, int N)
    {
        map<string, map<string, int>>::iterator sp = graph.find(start);
        if (sp == graph.end() || !hasNext(sp->second))
        { // 表示没有下一个路径了
            print(path);
            if (path.size() == N)
            {
                success = true;
            }
            else
            {
                success = false;
            }
            return;
        }

        map<string, int> &nexts = sp->second;

        for (map<string, int>::iterator itr = nexts.begin(); itr != nexts.end(); itr++)
        {
            if (itr->second == 0)
            {
                continue;
            }
            path.push_back(itr->first);
            itr->second--;
            success = true;
            find(graph, success, path, itr->first, N);
            if (!success)
            {
                path.pop_back();
                itr->second++;
            }
        }
    }

    bool hasNext(map<string, int> &nexts)
    {
        bool rt = false;
        for (map<string, int>::iterator itr = nexts.begin(); itr != nexts.end(); itr++)
        {
            if (itr->second != 0)
            {
                rt = true;
                break;
            }
        }
        return rt;
    }

    void print(vector<string> &a)
    {
        for (auto itr = a.begin(); itr != a.end(); itr++)
        {
            cout << *itr << " ";
        }
        cout << endl;
    }
};