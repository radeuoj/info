#include <iostream>
#include <vector>
 
const int MAX_N = 2e5;
 
struct Node {
    std::vector<int> adj;
    int val;
    int parent;
    int heavy;
    int head;
    int time_in;
};
 
unsigned next_pow_of_2(unsigned x) {
    return 1 << (32 - __builtin_clz(x - 1));
}
 
struct SegmentTree {
    int n, v[4 * MAX_N];
 
    void init(int n) {
        this->n = next_pow_of_2(n);
    }
 
    void unsafe_set(int pos, int val) {
        v[n + pos] = val;
    }
 
    void build() {
        for (int i = n - 1; i > 0; i--) {
            v[i] = std::max(v[2 * i], v[2 * i + 1]);
        }
    }
 
    void update(int pos, int val) {
        pos += n;
        v[pos] = val;
 
        while (pos /= 2) {
            v[pos] = std::max(v[2 * pos], v[2 * pos + 1]);
        }
    }
 
    int range_max(int l, int r) {
        l += n;
        r += n;
        int res = 0;
 
        while (l <= r) {
            if (l % 2 == 1) 
                res = std::max(res, v[l++]);
            l /= 2;
 
            if (r % 2 == 0)
                res = std::max(res, v[r--]);
            r /= 2;
        }
 
        return res;
    }
};
 
int n, q;
Node a[MAX_N + 1];
SegmentTree seg_tree;
 
void read_values() {
    for (int i = 1; i <= n; i++) {
        std::cin >> a[i].val;
    }
}
 
void read_tree() {
    for (int i = 1; i < n; i++) {
        int u, v;
        std::cin >> u >> v;
 
        a[u].adj.push_back(v);
        a[v].adj.push_back(u);
    }
}
 
int heavy_dfs(int node) {
    int node_size = 1;
    int heavy_size = 0;
 
    for (int child : a[node].adj) {
        if (child == a[node].parent) continue;
 
        a[child].parent = node;
        int child_size = heavy_dfs(child);
        node_size += child_size;
 
        if (child_size > heavy_size) {
            a[node].heavy = child;
            heavy_size = child_size;
        }
    }
 
    return node_size;
}
 
void decompose_dfs(int node, int head) {
    static int time = 0;
    a[node].time_in = time++;
    a[node].head = head;
 
    if (a[node].heavy != 0) {
        decompose_dfs(a[node].heavy, head);
    }
 
    for (int child : a[node].adj) {
        if (child == a[node].parent || child == a[node].heavy)
            continue;
 
        decompose_dfs(child, child);
    }
}
 
void init_seg_tree() {
    seg_tree.init(n);
 
    for (int i = 1; i <= n; i++) {
        seg_tree.unsafe_set(a[i].time_in, a[i].val);
    }
 
    seg_tree.build();
}
 
int query(int u, int v) {
    int res = 0;
 
    while (a[u].head != a[v].head) {
        if (a[u].time_in > a[v].time_in)
            std::swap(u, v);
 
        res = std::max(res, seg_tree.range_max(a[a[v].head].time_in, a[v].time_in));
        v = a[a[v].head].parent;
    }
 
    if (a[u].time_in > a[v].time_in)
        std::swap(u, v);
 
    res = std::max(res, seg_tree.range_max(a[u].time_in, a[v].time_in));
 
    return res;
}
 
void solve_queries() {
    while (q--) {
        int t;
        std::cin >> t;
 
        if (t == 1) {
            int s, x;
            std::cin >> s >> x;
            seg_tree.update(a[s].time_in, x);
        } else {
            int a, b;
            std::cin >> a >> b;
            std::cout << query(a, b) << '\n';
        }
    }
}
 
int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
 
    std::cin >> n >> q;
    read_values();
    read_tree();
    heavy_dfs(1);
    decompose_dfs(1, 1);
    init_seg_tree();
    solve_queries();
}