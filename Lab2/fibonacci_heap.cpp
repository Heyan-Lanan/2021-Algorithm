//斐波那契最小堆
#include<bits/stdc++.h>
#include<cmath>
#include<vector>
#include <chrono>
using namespace std;

struct fibonacci_heap_node{//斐波那契堆节点类型
    int key;
    int value;//这里增加value属性是为了区分相同key的节点
    bool mark = false;//标志是否曾经失去过一个孩子，初始时均为false
    size_t degree = 0;
    fibonacci_heap_node *parent = nullptr;
    fibonacci_heap_node *prev = nullptr;
    fibonacci_heap_node *next = nullptr;
    fibonacci_heap_node *child = nullptr;
    fibonacci_heap_node(int k, int v) :key(k), value(v){}
    void print(){ printf("key: %-6d value: %-6d degree: %-6d\n", key, value, degree); }
};

class fibonacci_heap{//斐波那契堆
public:
    typedef fibonacci_heap_node	node;
private:
    node *head;//最小值节点指针
    size_t n = 0;//节点数目

    void linkNode(node *&lhs, node *&rhs){
        if (rhs == nullptr){
            rhs = lhs;
            lhs->next = lhs;
            lhs->prev = lhs;
        }
        else{
            lhs->next = rhs;
            rhs->prev->next = lhs;
            lhs->prev = rhs->prev;
            rhs->prev = lhs;
        }
    }

    void removeNode(node *p){
        if (p->next == p){
            p->next = nullptr;
            p->prev = nullptr;
        }
        else{//否则
            p->prev->next = p->next;
            p->next->prev = p->prev;
        }
    }

    void heapLink(node *big, node *small){
        removeNode(big);
        linkNode(big, small->child);
        ++small->degree;
        big->parent = small;
        big->mark = false;
    }

    void prune(node *p, node *par){
        if (par->degree == 1) par->child = nullptr;
        else if (par->child == p) par->child = p->next;
        removeNode(p);
        linkNode(p, head);
        p->parent = nullptr;
        p->mark = false;
        --par->degree;
    }

    void removeChildsToRoot(node *p){
        if (p->child != nullptr){
            node *first = p->child, *last = p->child->prev;
            while (true){
                node *curr = first;
                first = first->next;
                removeNode(curr);
                linkNode(curr, head);
                curr->parent = nullptr;
                if (curr == last) break;
            }
        }
    }

    void cascadingPrune(node *p){
        node *par = p->parent;
        if (par != nullptr){
            if (p->mark == false) p->mark = true;
            else{
                prune(p, par);
                p->mark = false;
                cascadingPrune(par);
            }
        }
    }

    void consolidate(){
        size_t max_degree = static_cast<size_t>(log(n) / log(2));
        vector<node*> temp(max_degree + 1);
        node *first = head;
        while (true){
            node *small = first;
            first = first->next;
            if (small->degree > max_degree){
                removeChildsToRoot(small);
                small->degree = 0;
            }
            size_t d = small->degree;
            if (small == temp[d]) break;
            while (temp[d] != nullptr){
                node *big = temp[d];
                if (small->key >= big->key)
                    swap(small, big);
                if (big == first) first = first->next;
                heapLink(big, small);
                temp[d] = nullptr;
                ++d;
            }
            temp[d] = small;
        }
        head = nullptr;
        for (size_t i = 0; i != temp.size(); ++i){
            if (temp[i] != nullptr && (head == nullptr || (temp[i]->key<head->key)))
                head = temp[i];
        }
    }

    void print_aux(node* p)const{
        if (p == nullptr) return;
        node *first = p->next;
        while (true){
            node *curr = first;
            first = first->next;
            print_aux(curr->child);
            curr->print();
            if (curr == p) break;
        }
    }

    void destroy(node *p){
        if (p == nullptr) return;
        node *first = p->next;
        while (true){
            node *curr = first;
            first = first->next;
            destroy(curr->child);
            --n;
            if (curr == p){
                delete curr;
                break;
            }
            else delete curr;
        }
    }
public:
    fibonacci_heap(node *h = nullptr) :head(h){}

    node* insert(int k, int v){//插入节点
        node *p = new node(k, v);
        linkNode(p, head);
        if (p->key < head->key)
            head = p;
        ++n;
        return p;
    }

    node* minimum()const{
        return head;
    }

    void FibHeapUnion(fibonacci_heap &rhs){//合并两个斐波那契堆
        if (rhs.empty())return;//若被合并堆为空
        if (empty()){//若本堆为空
            swap(head, rhs.head);
            swap(n, rhs.n);
            return;
        }
        node *head_prev = head->prev;
        head_prev->next = rhs.head->prev;
        rhs.head->prev->prev->next = head;
        head->prev = rhs.head->prev->prev;
        rhs.head->prev->prev = head_prev;
        if (rhs.head->key < head->key)
            head = rhs.head;
        n += rhs.n;
        rhs.head = nullptr;
        rhs.n = 0;
    }

    pair<int, int> extractMin(){//删除最小key节点
        node *p = head;
        removeChildsToRoot(head);
        head->child = nullptr;
        removeNode(head);
        if (head->next == nullptr)//说明堆已空
            head = nullptr;
        else{
            head = head->next;
            consolidate();//合并度数相同的根节点
        }
        --n;
        pair<int, int> tmp = pair<int, int>(p->key, p->value);
        delete p;
        return tmp;
    }

    void decreaseKey(node *p, int k){//减小节点key
        if (k>p->key){//新key更大则报错
            cerr << "error" << endl;
            return;
        }
        p->key = k;
        node *par = p->parent;
        if (par != nullptr && (p->key<par->key)){
            prune(p, par);//剪掉以p为根的树
            cascadingPrune(par);//级联剪枝父节点
        }
        if (p->key<head->key)
            head = p;
    }

    void erase(node *p){//删除节点
        node *p_min = minimum();
        decreaseKey(p, p_min->key - 1);
        extractMin();
    }

    bool empty()const{
        return head == nullptr;
    }

    size_t size()const{
        return n;
    }

    void print()const{
        print_aux(head);
    }

    node* find(int k, node* p){
        if (p == nullptr) return nullptr;
        node *first = p->next;
        while (true){
            node *curr = first;
            first = first->next;
            if(curr->key == k)
                return curr;
            node* tmp = find(k, curr->child);
            if(tmp)
                return tmp;
            if (curr == p) break;
        }
        return nullptr;
    }

    ~fibonacci_heap(){
        destroy(head);
    }

    int num(){
        if (head == nullptr) return 0;
        int num = 0;
        node *first = head->next;
        while (true){
            node *curr = first;
            first = first->next;
            num++;
            if (curr == head) break;
        }
        return num;
    }
};

int main(){

    string from = "C:/Users/ASUS/Desktop/AL/AL2/ex1/input/2_1_input.txt";
    string dest = "C:/Users/ASUS/Desktop/AL/AL2/ex1/output/result.txt";
    string time = "C:/Users/ASUS/Desktop/AL/AL2/ex1/output/time.txt";
    ifstream file_in;
    ofstream file_out, time_out;
    file_in.open(from);
    file_out.open(dest);
    time_out.open(time);
    fibonacci_heap_node* tmp;
    vector<chrono::steady_clock::time_point> timelist(10);

    fibonacci_heap fh1, fh2, fh3, fh4, fh5;
    for (auto i = 0; i < 50; i++){
        int key;
        file_in >> key;
        fh1.insert(key, i);
    }
    for (auto i = 50; i < 150; i++){
        int key;
        file_in >> key;
        fh2.insert(key, i);
    }
    for (auto i = 150; i < 300; i++){
        int key;
        file_in >> key;
        fh3.insert(key, i);
    }
    for (auto i = 300; i < 500; i++){
        int key;
        file_in >> key;
        fh4.insert(key, i);
    }
    timelist[0] = chrono::steady_clock::now();

    file_out << "H1" << endl;
    fh1.insert(249, fh1.size());
    file_out << fh1.size();
    fh1.insert(830, fh1.size());
    file_out << " " << fh1.size();
    file_out << " " << fh1.minimum()->key;
    fh1.erase(fh1.find(127, fh1.minimum()));
    file_out << " " << fh1.size();
    fh1.erase(fh1.find(141, fh1.minimum()));
    file_out << " " << fh1.size();
    file_out << " " << fh1.minimum()->key;
    fh1.decreaseKey(fh1.find(75, fh1.minimum()), 61);
    file_out << " " << fh1.minimum()->key;
    fh1.decreaseKey(fh1.find(198, fh1.minimum()), 169);
    file_out << " " << fh1.minimum()->key;
    file_out << " " << fh1.minimum()->key;
    fh1.extractMin();
    file_out << " " << fh1.minimum()->key<< endl;
    fh1.extractMin();

    timelist[1] = chrono::steady_clock::now();

    file_out << "H2" << endl;
    fh2.insert(816, fh2.size());
    file_out << fh2.size();
    file_out << " " << fh2.minimum()->key;
    fh2.insert(345, fh2.size());
    file_out << " " << fh2.size();
    file_out << " " << fh2.minimum()->key;
    fh2.extractMin();
    fh2.erase(fh2.find(504, fh2.minimum()));
    file_out << " " << fh2.size();
    fh2.erase(fh2.find(203, fh2.minimum()));
    file_out << " " << fh2.size();
    fh2.decreaseKey(fh2.find(296, fh2.minimum()), 87);
    file_out << " " << fh2.minimum()->key;
    fh2.decreaseKey(fh2.find(278, fh2.minimum()), 258);
    file_out << " " << fh2.minimum()->key;
    file_out << " " << fh2.minimum()->key;
    file_out << " " << fh2.minimum()->key << endl;
    fh2.extractMin();

    timelist[2] = chrono::steady_clock::now();

    file_out << "H3" << endl;
    file_out << fh3.minimum()->key;
    fh3.extractMin();
    file_out << " " << fh3.minimum()->key;
    fh3.insert(262, fh3.size());
    file_out << " " << fh3.size();
    file_out << " " << fh3.minimum()->key;
    fh3.extractMin();
    fh3.insert(830, fh3.size());
    file_out << " " << fh3.size();
    file_out << " " << fh3.minimum()->key;
    fh3.erase(fh3.find(134, fh3.minimum()));
    file_out << " " << fh3.size();
    fh3.erase(fh3.find(177, fh3.minimum()));
    file_out << " " << fh3.size();
    fh3.decreaseKey(fh3.find(617, fh3.minimum()), 360);
    file_out << " " << fh3.minimum()->key;
    fh3.decreaseKey(fh3.find(889, fh3.minimum()), 353);
    file_out << " " << fh3.minimum()->key << endl;

    timelist[3] = chrono::steady_clock::now();

    file_out << "H4" << endl;
    file_out << fh4.minimum()->key;
    fh4.erase(fh4.find(708, fh4.minimum()));
    file_out << " "  << fh4.size();
    fh4.insert(281, fh4.size());
    file_out << " "  << fh4.size();
    fh4.insert(347, fh4.size());
    file_out << " " << fh4.size();
    file_out << " " << fh4.minimum()->key;
    fh4.erase(fh4.find(415, fh4.minimum()));
    file_out << " "  << fh4.size();
    file_out << " "  << fh4.minimum()->key;
    fh4.extractMin();
    fh4.decreaseKey(fh4.find(620, fh4.minimum()), 354);
    file_out << " "  << fh4.minimum()->key;
    fh4.decreaseKey(fh4.find(410, fh4.minimum()), 80);
    file_out << " "  << fh4.minimum()->key;
    file_out << " "  << fh4.minimum()->key << endl;
    fh4.extractMin();

    timelist[4] = chrono::steady_clock::now();

    fh5.FibHeapUnion(fh1);
    fh5.FibHeapUnion(fh2);
    fh5.FibHeapUnion(fh3);
    fh5.FibHeapUnion(fh4);

    timelist[5] = chrono::steady_clock::now();

    file_out << "H5" << endl;
    file_out << fh5.minimum()->key;
    fh5.extractMin();
    file_out << " "  << fh5.minimum()->key;
    fh5.erase(fh5.find(800, fh5.minimum()));
    file_out << " "  << fh5.size();
    fh5.insert(267, fh5.size());
    file_out << " "  << fh5.size();
    fh5.insert(351, fh5.size());
    file_out << " "  << fh5.size();
    file_out << " "  << fh5.minimum()->key;
    fh5.extractMin();
    fh5.decreaseKey(fh5.find(478, fh5.minimum()), 444);
    file_out << " "  << fh5.minimum()->key;
    fh5.decreaseKey(fh5.find(559, fh5.minimum()), 456);
    file_out << " "  << fh5.minimum()->key;
    file_out << " "  << fh5.minimum()->key;
    fh5.erase(fh5.find(929, fh5.minimum()));
    file_out << " "  << fh5.size() << endl;

    timelist[6] = chrono::steady_clock::now();
    for(auto i = 0; i < 4; i++)
        time_out << (chrono::duration_cast<chrono::duration<double>>(timelist[i+1] - timelist[i])).count() << 's' << endl;
    time_out << (chrono::duration_cast<chrono::duration<double>>(timelist[6] - timelist[5])).count() << 's' << endl;
    return 0;
}