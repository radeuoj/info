#include <bits/stdc++.h>
using namespace std;

constexpr int MAXN = 1e5;

int next_power_of_2(int x) {
    return 1 << (32 - __builtin_clz(x - 1));
}

struct SegmentTree {
    struct Node {
        long long s, lazy;  
    };

    int n;
    Node st[MAXN * 4];

    void init(int n) {
        this->n = next_power_of_2(n);
        fill(st, st + MAXN * 4, Node { 0, 0 });
    }

    void pull(int node, int size) {
        st[node].s = 
            st[2 * node].s + st[2 * node].lazy * size / 2 +
            st[2 * node + 1].s + st[2 * node + 1].lazy * size / 2;
    }

    void pull_path(int node, int size = 1) {
        while (node /= 2) {
            size *= 2;
            pull(node, size);
        }
    }

    void add(int l, int r, int delta) {
        l += n - 1;
        r += n - 1;

        int init_l = l;
        int init_r = r;

        while (l <= r) {
            if (l % 2 == 1) {
                st[l++].lazy += delta;
            }
            l /= 2;

            if (r % 2 == 0) {
                st[r--].lazy += delta;
            }
            r /= 2;
        }

        pull_path(init_l);
        pull_path(init_r);
    }

    void push(int node, int size) {
        st[node].s += st[node].lazy * size;
        st[2 * node].lazy += st[node].lazy;
        st[2 * node + 1].lazy += st[node].lazy;
        st[node].lazy = 0;
    }

    void push_path(int node, int size) {
        if (!node) return;
        push_path(node / 2, size * 2);
        push(node, size);
    }

    long long sum(int l, int r) {
        l += n - 1;
        r += n - 1;

        long long sum = 0;
        int size = 1;

        push_path(l / 2, 2);
        push_path(r / 2, 2);

        while (l <= r) {
            if (l % 2 == 1) {
                sum += st[l].s + st[l].lazy * size;
                l++;
            }
            l /= 2;

            if (r % 2 == 0) {
                sum += st[r].s + st[r].lazy * size;
                r--;
            }
            r /= 2;

            size *= 2;
        }

        return sum;
    }
};

SegmentTree st;

void process_ops(int c) {
    while (c--) {
        int t, p, q;
        cin >> t >> p >> q;

        if (t == 0) {
            int v;
            cin >> v;

            st.add(p, q, v);
        } else {
            cout << st.sum(p, q) << '\n';
        }
    }
}

void solve() {
    int n, c;
    cin >> n >> c;

    st.init(n);
    process_ops(c);
}

int main() {
    int t;
    cin >> t;

    while (t--) {
        solve();
    }
}