class Solution
{
public:
  vector<string> findItinerary(vector<vector<string>> &tickets)
  {
    // Each node (airport) contains a set of outgoing edges (destination).
    map<string, multiset<string>> graph;
    // We are always appending the deepest node to the itinerary,
    // so will need to reverse the itinerary in the end.
    vector<string> itinerary;
    if (tickets.size() == 0)
    {
      return itinerary;
    }
    // Construct the node and assign outgoing edges
    for (vector<string> eachTicket : tickets)
    {
      graph[eachTicket[0]].insert(eachTicket[1]);
    }
    stack<string> dfs;
    dfs.push("JFK");
    while (!dfs.empty())
    {
      string topAirport = dfs.top();
      if (graph[topAirport].empty())
      {
        // If there is no more outgoing edges, append to itinerary
        // Two cases:
        // 1. If it searchs the terminal end first, it will simply get
        //    added to the itinerary first as it should, and the proper route
        //    will still be traversed since its entry is still on the stack.
        // 2. If it search the proper route first, the dead end route will also
        //    get added to the itinerary first.
        itinerary.push_back(topAirport);
        dfs.pop();
      }
      else
      {
        // Otherwise push the outgoing edge to the dfs stack and
        // remove it from the node.
        dfs.push(*(graph[topAirport].begin()));
        graph[topAirport].erase(graph[topAirport].begin());
      }
    }
    // Reverse the itinerary.
    reverse(itinerary.begin(), itinerary.end());
    return itinerary;
  }
};