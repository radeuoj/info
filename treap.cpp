#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <cassert>

const int N = 500'000;
const int NUM_PASSES = 1;

struct Treap {
    struct Node {
        int key, prior;
        Node *l, *r;
        int cnt;
    };

    Node* root = nullptr;

    void deinit(Node*& node) {
        if (node == nullptr) return;
        deinit(node->l);
        deinit(node->r);
        delete node;
        node = nullptr;
    }

    void deinit() {
        deinit(root);
    }

    Node* search(int key) {
        Node* t = root;

        while (t != nullptr && key != t->key) {
            t = key < t->key ? t->l : t->r;
        }

        return t;
    }

    bool contains(int key) {
        return search(key) != nullptr;
    }

    int node_count(Node* t) {
        return t != nullptr ? t->cnt : 0;
    }

    void update_count(Node* t) {
        if (t == nullptr) return;
        t->cnt = 1 + node_count(t->l) + node_count(t->r);
    }

    void split(Node* t, int key, Node*& l, Node*& r) {
        if (t == nullptr) {
            l = r = nullptr;
        } else if (key < t->key) {
            split(t->l, key, l, t->l);
            r = t;
        } else {
            split(t->r, key, t->r, r);
            l = t;
        }

        update_count(t);
    }

    void insert(Node*& t, Node* node) {
        if (t == nullptr) {
            t = node;
        } else if (node->prior > t->prior) {
            split(t, node->key, node->l, node->r);
            t = node;
        } else {
            insert(node->key < t->key ? t->l : t->r, node);
        }

        update_count(t);
    }

    void insert(int key) {
        Node* node = new Node { key, rand(), nullptr, nullptr, 0 };
        insert(root, node);
    }

    void merge(Node*& t, Node* l, Node* r) {
        if (l == nullptr) {
            t = r;
        } else if (r == nullptr) {
            t = l;
        } else if (l->prior > r->prior) {
            merge(l->r, l->r, r);
            t = l;
        } else {
            merge(r->l, l, r->l);
            t = r;
        }

        update_count(t);
    }

    void erase(Node*& t, int key) {
        if (t == nullptr) return;

        if (key == t->key) {
            Node* old_ptr = t;
            merge(t, t->l, t->r);
            delete old_ptr;
        } else {
            erase(key < t->key ? t->l : t->r, key);
        }

        update_count(t);
    }

    void erase(int key) {
        erase(root, key);
    }

    int order_of(int key) {
        int res = 0;
        Node* t = root;

        while (t != nullptr) {
            if (key > t->key) {
                res += 1 + node_count(t->l);
                t = t->r;
            } else {
                t = t->l;
            }
        }

        return res;
    }

    int kth_element(int k) {
        if (k >= node_count(root)) return -1;
        Node* t = root;

        while (k != node_count(t->l)) {
            if (k < node_count(t->l)) {
                t = t->l;
            } else {
                k -= 1 + node_count(t->l);
                t = t->r;
            }
        }

        return t->key;
    }
};

Treap treap;
int perm[N];

void gen_perm() {
    for (int i = 0; i < N; i++) {
        int j = rand() % (i + 1);
        perm[i] = perm[j];
        perm[j] = i;
    }
}

void verify() {
    for (int i = 0; i < N; i++) {
        treap.insert(perm[i]);
        int j = rand() % N;
        assert(treap.contains(perm[j]) == (j <= i));
    }

    for (int i = 0; i < N; i++) {
        assert(treap.order_of(i) == i);
        assert(treap.kth_element(i) == i);
    }

    for (int i = 0; i < N; i++) {
        treap.erase(perm[i]);
        int j = rand() % N;
        assert(treap.contains(perm[j]) == (j > i));
    }

    treap.deinit();
}

int main() {
    srand(time(0));

    for (int pass = 0; pass < NUM_PASSES; pass++) {
        gen_perm();
        printf("Pass %d/%d\t", pass + 1, NUM_PASSES);
        verify();
        printf("OK!\n");
    }
}