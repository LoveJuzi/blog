#include <iostream>
#include <list>
#include <unordered_map>

using namespace std;

class LRUCache {
public:
    LRUCache(int capacity) : capacity(capacity), cache(capacity) {
    }
    
    int get(int key) {
        auto it = cache.find(key);
        if (it == cache.end()) {
            return -1;
        }
        touch(it);
        return it->second.first;
    }
    
    void put(int key, int value) {
        auto it = cache.find(key);
        if (it != cache.end()) {
            touch(it);
        } 
        else {
            if (cache.size() == capacity) {
                cache.erase(used.back());
                used.pop_back();
            }
            used.push_front(key);
        }
        cache[key] = { value, used.begin() };
    }

private:
    // 调整key的位置，删除当前key饿位置，然后在used的头部插入信息，修改缓存信息
    void touch(HIPII::iterator it) {
        int key = it->first;
        used.erase(it->second.second);
        used.push_front(key);
        it->second.second = used.begin();
    }

private:
    typedef list<int> LI;
    typedef pair<int, LI::iterator> PII;
    typedef unordered_map<int, PII> HIPII;

    HIPII cache; // 用来存储缓存内容
    LI used; // 缓存当前的key的信息，如果是最近已经访问的key，那么将这个key调整到开头
    int capacity; // 缓冲区的最大大小
};

/**
 * Your LRUCache object will be instantiated and called as such:
 * LRUCache* obj = new LRUCache(capacity);
 * int param_1 = obj->get(key);
 * obj->put(key,value);
 */