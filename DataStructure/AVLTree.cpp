// Created Time: Tue 13 Oct 2020 06:01:11 PM EDT
#include <iostream>
#include <vector>
#include <string>
#include <bits/stdc++.h>

using namespace std;

struct NotFound{
    string message = "ERROR";
};

class AVLNode{
    int key, value;
    AVLNode * left;
    AVLNode * right;
    AVLNode * parent;
    int height;
    AVLNode(const int k, const int v): key{k}, value{v}, left{nullptr}, right{nullptr}, parent{nullptr}, height{-1}{}
    void setHeightFromSubtrees(){
        int l = -1, r = -1;
        if (left) l = left->height;
        if (right) r = right->height;
        height = 1 + max(l, r);
    }
    public: 
        friend class AVL;
};

class AVL{
    AVLNode * root;
    int size;

    AVLNode * BSTInsert(const int & k,const int & v);
    AVLNode * restructure(AVLNode * z);
    AVLNode * BSTDelete(const int & k);

    AVLNode * rotateLeft(AVLNode * z);
    AVLNode * rotateRight(AVLNode * z);

    public:

    void AVLInsert(const int & k, const int & v);
    void AVLDelete (const int & k);
    int AVLSearch(const int & k);

    int getHeight() const {
        if (root) return root->height;
        else return -1;
    }
    int getSize() const {
        return size;
    }
    AVLNode * getRoot() const{
        return root;
    }

    AVL(): root{nullptr}, size{0}{}

    void destroy(AVLNode * r){
        if (r == nullptr) return;
        destroy(r->left);
        destroy(r->right);
        delete r;
    }

    ~AVL(){
        if (root) destroy(root);
    }
};


class SLNode{
    int key, value;
    SLNode * after;
    SLNode * below;
    SLNode(const int & k, const int & v = -1): key{k}, value{v}, after{nullptr}, below{nullptr}{}
    public:
    friend class SkipList;
};

class SkipList{
    vector<SLNode *> tower;
    int nodes;

    void increaseHeight(){ // add a floor to the tower
        SLNode * ls = new SLNode(INT_MIN); // left sentinel
        SLNode * rs = new SLNode(INT_MAX);
        ls->after = rs;
        ls->below = tower.back();
        rs->below = tower.back()->after; // points to the sentinels in the bottom layer
        tower.push_back(ls);
    }
    void deleteTower(){ // delete the top floor
        // Precondition: tower.back()->after->key = INT_MAX
        delete tower.back()->after;
        delete tower.back(); 
        tower.pop_back();
    }

    vector<SLNode *> getPredecessors(const int & k);

    public:
    int search(const int & k);
    void SLInsert(const int & k, const int & v, const int & coin); // coin is the number of heads flipped before a tail is reached when inserting into the skiplist
    void SLDelete(const int & k);

    int getNodes(){
        return nodes;
    }

    SkipList(): nodes{0}{
        SLNode * ls = new SLNode(INT_MIN); // left sentinel
        SLNode * rs = new SLNode(INT_MAX);
        ls->after = rs;
        tower.push_back(ls);
    }

    void destroy(){
        
        for (auto & n : tower){
            while (n){
                SLNode * tmp = n;
                n = n->after;
                delete tmp;
            }
        }
        tower.clear();
    }

};

int AVLComparison = 0, skipListComparison = 0;

AVLNode * AVL::BSTInsert(const int & k,const int & v){
    AVLNode * node = new AVLNode(k, v);
    AVLNode * cur = root;
    if (!cur){
        root = node;
        size = 1;
        return node;
    }
    while (cur){
        AVLComparison ++;
        if (k < cur->key){
            if (cur->left) cur = cur->left;
            else {
                cur->left = node;
                node->parent = cur;
                size ++;
                return node;
            }
        }
        else if (k > cur->key) {
            AVLComparison ++;
            if (cur->right) cur = cur->right;
            else {
                cur->right = node;
                node->parent = cur;
                size ++;
                return node;
            }
        }
        else{
            AVLComparison ++;
            delete node;
            return cur; // do not accept duplicate key
        }
    }
    return node;
}

void AVL::AVLInsert(const int & k, const int & v){
    AVLNode * z = BSTInsert(k, v);
    z->height = 0;
    while (z){
        z->setHeightFromSubtrees();
        int l = -1, r = -1;
        if(z->left) l = z->left->height;
        if(z->right) r = z->right->height;
        if (abs(l - r) == 2){
            restructure(z);
            break;
        }
        z = z->parent;
    }
}

AVLNode * AVL::restructure(AVLNode * z){
    AVLNode * x, * y;
    int l = -1, r = -1;
    if(z->left) l = z->left->height;
    if(z->right) r = z->right->height;
    if(l > r){
        y = z->left;
        l = -1; r = -1;
        if(y->left) l = y->left->height;
        if(y->right) r = y->right->height;
        if (l > r) {
            x = y->left;
            return rotateRight(z);
        }
        else {
            x = y->right;
            z->left = rotateLeft(y);
            return rotateRight(z);
        }
    }
    else{
        y = z->right;
        l = -1; r = -1;
        if(y->left) l = y->left->height;
        if(y->right) r = y->right->height;
        if (l > r){
            x = y->left;
            z->right = rotateRight(y);
            return rotateLeft(z);
        }
        else{
            x = y->right;
            return rotateLeft(z);
        }
    }
}

AVLNode * AVL::rotateLeft(AVLNode * z){
    AVLNode * y = z->right;
    z->right = y->left;
    if(y->left) y->left->parent = z;
    y->left = z;
    if (z->parent){
        if (z->parent->left == z) z->parent->left = y;
        else z->parent->right = y;
    }
    else root = y;
    y->parent = z->parent;
    z->parent = y; 
    z->setHeightFromSubtrees();
    y->setHeightFromSubtrees();
    return y;
}

AVLNode * AVL::rotateRight(AVLNode * z){
    AVLNode * y = z->left;
    z->left = y->right;
    if(y->right) y->right->parent = z;
    y->right = z;
    if (z->parent){
        if (z->parent->left == z) z->parent->left = y;
        else z->parent->right = y;
    }
    else root = y;
    y->parent = z->parent;
    z->parent = y; 
    z->setHeightFromSubtrees();
    y->setHeightFromSubtrees();
    return y;
}

int AVL::AVLSearch(const int & k){
    AVLNode * cur = root;
    while (cur){
        AVLComparison ++;
        if (k < cur->key) cur = cur->left;
        else if (k > cur->key){
            AVLComparison ++;
            cur = cur->right;
        }
       else{
           AVLComparison++;
           return cur->value;
       } 
    }
    throw NotFound();
    return -1;
}

AVLNode * AVL::BSTDelete(const int & k){
    AVLNode * cur = root;
    while (cur){
        AVLComparison++;
        if (k < cur->key){
            cur = cur->left;
        }
        else if (k > cur->key){
            AVLComparison ++;
            cur = cur->right;
        }
        else{
            AVLComparison++;
            size--;
            AVLNode * successor = nullptr;
            if (cur->right){
                successor = cur->right;
                while (successor->left)
                    successor = successor->left;
                AVLNode * p = successor->parent;
                if (cur->right == successor){
                    p = cur->parent;
                    successor->parent = cur->parent; successor->left = cur->left; 
                    if (cur->left) cur->left->parent = successor;
                } 
                else{
                    successor->parent->left = successor->right;
                    if (successor->right) successor->right->parent = successor->parent;
                    successor->parent = cur->parent; successor->left = cur->left; successor->right = cur->right;
                    if (cur->left) cur->left->parent = successor;
                    if (cur->right) cur->right->parent = successor;
                } 
                if(cur->parent){
                    if(cur->parent->left == cur) cur->parent->left = successor;
                    else cur->parent->right = successor;
                }else root = successor;
                cur->parent = nullptr; cur->left = nullptr; cur->right = nullptr; delete cur;
                return p;
            }
            else if (cur->left){
                successor = cur->left; // predecessor
                AVLNode * p = cur->parent;
                successor->parent = cur->parent; //successor->left = successor->right = nullptr;
                if(cur->parent){
                    if(cur->parent->left == cur) cur->parent->left = successor;
                    else cur->parent->right = successor;
                }else root = successor;
                cur->parent = nullptr; cur->left = nullptr; cur->right = nullptr; delete cur; 
                return p;
            }
            else{
                AVLNode * p = cur->parent;
                if (cur->parent){
                    if(cur->parent->left == cur) cur->parent->left = successor; // successor is nullptr
                    else cur->parent->right = successor;
                }
                else root = successor;
                delete cur;
                return p;
            }
        }
    }
    return nullptr;

}
void AVL::AVLDelete (const int & k){
    AVLNode * z = BSTDelete(k);
    while (z){
        z->setHeightFromSubtrees();
        int l = -1, r = -1;
        if(z->left) l = z->left->height;
        if(z->right) r = z->right->height;
        if (abs(l - r) == 2){
            restructure(z);
        }
        z = z->parent;
    }
}


vector<SLNode *> SkipList::getPredecessors(const int & k){
    vector<SLNode *>stack;
    SLNode * p = tower.back();
    stack.push_back(p);
    while (p->below){
        p = p->below;
        skipListComparison++;
        while (p->after->key < k){
            skipListComparison++;
            p = p->after;
        } 
        stack.push_back(p);
    }
    return stack;
}

int SkipList::search(const int & k){
    vector<SLNode *> stack = getPredecessors(k);
    SLNode * p = stack.back();
    skipListComparison++;
    if (p->after->key == k) return p->after->value;
    else throw NotFound();
    return -1;
}

void SkipList::SLInsert(const int & k, const int & v, const int & coin){
    vector<SLNode *> stack = getPredecessors(k);
    int floor = coin + 2 - tower.size();
    for (int i = 0; i < floor; ++i){
        increaseHeight();
        stack.insert(stack.begin(), tower.back());
    } 
    SLNode * p = stack.back(); stack.pop_back();
    SLNode * node = new SLNode(k, v); nodes ++; // memoryPool.push_back(node);
    node->after = p->after; p->after = node;
    for (int i = 0; i < coin; ++i){
        p = stack.back(); stack.pop_back();
        SLNode * newNode = new SLNode(k); nodes++; // memoryPool.push_back(newNode);
        newNode->after = p->after; p->after = newNode;
        newNode->below = node;
        node = newNode;
    }


}

void SkipList::SLDelete(const int & k){
    vector<SLNode *> stack = getPredecessors(k);
    SLNode * p = tower.back();
    while (!stack.empty()){
        p = stack.back(); stack.pop_back();
        skipListComparison++;
        if (p->after->key == k) {
            SLNode * tmp = p->after->after;
            delete p->after;
            nodes--;
            p->after = tmp;
        }
        else break;
    }
    p = tower.back();
    while (p->below && p->below->after->key == INT_MAX){
        p = p->below;
        deleteTower();
    }
}

int main(){
    AVL avl = AVL();
    SkipList sl = SkipList();
    string command;
    while (cin >> command) if(!cin.eof()){
        if (command == "i"){
            int key, value, coin;
            cin >> key >> value >> coin;
            avl.AVLInsert(key, value);
            sl.SLInsert(key, value, coin);
            cout << AVLComparison << " " << skipListComparison << endl;
            AVLComparison = 0; skipListComparison = 0;
        }
        else if(command == "d"){
            int key;
            cin >> key;
            avl.AVLDelete(key);
            sl.SLDelete(key);
            cout << AVLComparison << " " << skipListComparison << endl;
            AVLComparison = 0; skipListComparison = 0;
        }
        else if(command == "savl"){
            int key;
            cin >> key;
            int value;
            try{
                value = avl.AVLSearch(key);
                cout << value << " " << AVLComparison << endl;
            }catch(NotFound & e){
                cout << e.message << " " << AVLComparison << endl;
            }
            AVLComparison = 0;
        }
        else if (command == "ssl"){
            int key;
            cin >> key;
            int value;
            try{
                value = sl.search(key);
                cout << value << " " << skipListComparison << endl;
            }catch(NotFound & e){
                cout << e.message << " " << skipListComparison << endl;
            }
            skipListComparison = 0;
        }
        else if (command == "stats"){
            cout << avl.getSize() << " " << avl.getHeight() << " " << sl.getNodes() << endl;

        }
        else if (command == "r"){
            avl.destroy(avl.getRoot());
            avl = AVL();
            sl.destroy();
            sl = SkipList();
        }
        else{
            cerr << "invalid command" << endl;
        }

        command.clear();
    }
    sl.destroy();
}
