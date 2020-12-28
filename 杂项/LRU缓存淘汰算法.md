# LRU缓存淘汰算法

```golang
package lru

/*
最近最久未使用原则
缓存淘汰策略

算法过程：
    1. 申请一个缓存列表
    2. 添加一个新的元素：
        a. 如果元素不在缓存列表中，那么将该元素提升到列表的头部
        b. 如果元素不在缓存列表中，且有足够空间，那么将元素直接插入到列表头部
        c. 如果空间不足，首先淘汰列表尾部元素，然后重新添加元素
    3. 还需要提供一个根据key查询数据的接口，注意，需要更新优先级
*/

type data interface{}

type key int

type lruCacheNode struct {
    k    key
    pre  *lruCacheNode
    next *lruCacheNode
}

// LRUCache 缓存定义
// 此处的 ht1，ht2 的实现有问题，会造成内存泄漏
type LRUCache struct {
    size  int
    ht1   map[key]data
    ht2   map[key]*lruCacheNode
    lhead *lruCacheNode // 缓存队列头
    ltail *lruCacheNode // 缓存队列尾
    lsize int           // 队列长度
}

// CreateLRUCache 创建一个LRUCache
func CreateLRUCache(size int) *LRUCache {
    c := &LRUCache{
        size:  size,
        ht1:   make(map[key]data),
        ht2:   make(map[key]*lruCacheNode),
        ltail: &lruCacheNode{},
        lhead: &lruCacheNode{},
        lsize: 0,
    }
    c.lhead.next = c.ltail
    c.ltail.pre = c.lhead
    return c
}

// AddElement 更新缓存
func (c *LRUCache) AddElement(k key, v data) {
    // 淘汰元素
    if !c.hasKey(k) && c.isFull() {
        c.removeTail()
    }

    // 插入一个新元素
    if !c.hasKey(k) {
        c.addNewKey(k, v)
    }

    // 更新key对应的数据
    c.ht1[k] = v

    // 更新元素的淘汰优先级
    c.updateKey(k)
}

// GetElement 查找元素
func (c *LRUCache) GetElement(k key) (data, bool) {
    if c.hasKey(k) {
        // 更新元素的淘汰优先级
        c.updateKey(k)
        return c.ht1[k], true
    }
    return nil, false
}

func (c *LRUCache) hasKey(k key) bool {
    var rt bool
    _, rt = c.ht1[k]
    return rt
}

func (c *LRUCache) isFull() bool {
    return c.lsize == c.size
}

func (c *LRUCache) isEmpty() bool {
    return c.lsize == 0
}

func (c *LRUCache) addNewKey(k key, v data) {
    node := &lruCacheNode{
        k:    k,
        pre:  nil,
        next: nil,
    }
    c.insertHead(node)

    c.ht1[k] = v
    c.ht2[k] = node
    c.lsize++
}

func (c *LRUCache) insertHead(node *lruCacheNode) {
    headNext := c.lhead.next
    node.next = headNext
    headNext.pre = node
    c.lhead.next = node
    node.pre = c.lhead
}

func (c *LRUCache) updateKey(k key) {
    node := c.ht2[k]
    nodePre := node.pre
    nodeNext := node.next

    nodePre.next = nodeNext
    nodeNext.pre = nodePre

    c.insertHead(node)
}

func (c *LRUCache) removeTail() {
    if c.isEmpty() {
        return
    }

    node := c.ltail.pre
    nodePre := node.pre
    nodePre.next = c.ltail
    c.ltail.pre = nodePre

    delete(c.ht1, node.k)
    delete(c.ht2, node.k)
    c.lsize--
}
```
