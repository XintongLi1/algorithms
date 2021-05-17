// Created Time: Mon 09 Nov 2020 10:37:48 AM EST
#include <iostream>
#include <vector>

using namespace std;
vector<int> primeList {11, 23, 47, 97, 197, 397, 797, 1597, 3203, 6421, 12853, 25717};
const float A = 0.618; // golden ratio


struct Hashtable {
    int numberOfItems;
    int totalProbes, tempProbes;
    int sizeOfHashTable;
    Hashtable(): numberOfItems{0}, totalProbes{0}, tempProbes{0}, sizeOfHashTable{11}{} 
    void refreshTemp(){tempProbes = 0;}
    virtual void insert(int key) = 0;
    virtual int search(int key) = 0;
    virtual void deleteKey(int k) = 0;
    virtual void reHash() = 0;
};

struct Node {
    int key;
    Node * next;
    Node(int k, Node * n = nullptr): key{k}, next{n}{}
};

struct SeparateChaning : public Hashtable{
    vector<Node *> table;
    SeparateChaning(): Hashtable(){
        for (int i = 0; i < 11; ++i) table.push_back(nullptr);
    }
    void insert(int key) override;
    int search(int key) override;
    void deleteKey(int key) override;
    void reHash() override;
    ~SeparateChaning(){
        for (auto & n : table){
            while (n != nullptr){
                Node * t = n;
                n = n->next;
                delete t;
            }
        }
        table.clear();
    }
};

void SeparateChaning::insert(int k){
    int pos = k % sizeOfHashTable;
    Node * node = new Node{k, table[pos]};
    table[pos] = node;
    tempProbes = 1; totalProbes += tempProbes;
    numberOfItems++;
}

int SeparateChaning::search(int k){
    int pos = k % sizeOfHashTable;
    Node * cur = table[pos];    
    tempProbes++; totalProbes++;
    while (cur != nullptr){
        if (cur->key == k) return 1;
        else {
            cur = cur->next;
            tempProbes++; totalProbes++;
        }
    }
    return -1;
}

void SeparateChaning::deleteKey(int k){
    int pos = k % sizeOfHashTable;
    Node * cur = table[pos];    
    tempProbes++; totalProbes++;
    if (cur == nullptr) return;
    else if (cur->key == k) {
        table[pos] = cur->next;
        delete cur;
        numberOfItems--;
        return;
    }
    tempProbes++; totalProbes++;
    while (cur->next != nullptr){
        if (cur->next->key == k){
            Node * d = cur->next; 
            cur->next = d->next;
            delete d;
            numberOfItems--;
            return;
        }
        else {
            cur = cur->next;
            tempProbes++; totalProbes++;
        }
    }
}

void SeparateChaning::reHash(){
    vector<Node *> newTable;
    for(auto & r : primeList){
        if (r > sizeOfHashTable){
            sizeOfHashTable = r; break;
        }
    }
    for (int i = 0; i < sizeOfHashTable; ++i){
        newTable.push_back(nullptr);
    }
    for (auto & list : table){
        while (list != nullptr){
            int pos = list->key % sizeOfHashTable;
            Node * node = new Node(list->key, newTable[pos]);
            newTable[pos] = node;
            Node * t = list;
            list = list->next;
            delete t;
        }
    }
    table.clear();
    table = newTable;
}

struct LinearProbing : public Hashtable{
    vector<int> table;
    LinearProbing(): Hashtable(){
        for (int i = 0; i < 11; ++i) table.push_back(-2); // -2: empty, -1: deleted
    }
    void insert(int key) override;
    int search(int key) override;
    void deleteKey(int key) override;
    void reHash() override;
    ~LinearProbing(){
        table.clear();
    }
};

void LinearProbing::insert(int k){
    for (int i = 0; i < sizeOfHashTable; ++i){
        tempProbes++; totalProbes++;
        int pos = (k % sizeOfHashTable +i) % sizeOfHashTable;
        if (table[pos] == -1 || table[pos] == -2){
            table[pos] = k;
            numberOfItems++;
            return;
        }
    }
    reHash();
    insert(k);
}

int LinearProbing::search(int k){
    for (int i = 0; i < sizeOfHashTable; ++i){
        tempProbes++; totalProbes++;
        int pos = (k % sizeOfHashTable +i) % sizeOfHashTable;
        if (table[pos] == -2) return -1;
        else if (table[pos] == k) return pos;
    }
    return -1;
}

void LinearProbing::deleteKey(int k){
    int index = search(k);
    if (index != -1) table[index] = -1;
    else return;
    numberOfItems--;
}

void LinearProbing::reHash(){
    vector<int> newTable;
    for(auto & r : primeList){
        if (r > sizeOfHashTable){
            sizeOfHashTable = r; break;
        }
    }
    for (int i = 0; i < sizeOfHashTable; ++i){
        newTable.push_back(-2);
    }
    for (auto & k : table) if (k != -1 && k != -2) {
        for (int i = 0; i < sizeOfHashTable; ++i){
            int pos = (k % sizeOfHashTable +i) % sizeOfHashTable;
            if (newTable[pos] == -1 || newTable[pos] == -2){
                newTable[pos] = k;
                break;
            }
        }
    }
    table.clear();
    table = newTable;
}

struct DoubleHashing : public Hashtable{
    vector<int> table;
    DoubleHashing (): Hashtable(){
        for (int i = 0; i < 11; ++i) table.push_back(-2); // -2: empty, -1: deleted
    }
    void insert(int key) override;
    int search(int key) override;
    void deleteKey(int key) override;
    void reHash() override;
    ~DoubleHashing(){
        table.clear();
    }
};

void DoubleHashing::insert(int k){
    for (int i = 0; i < sizeOfHashTable; ++i){
        tempProbes++; totalProbes++;
        int pos = static_cast<int>(k % sizeOfHashTable + i * (1 + (sizeOfHashTable-1) * (k*A - static_cast<int>(k*A)))) % sizeOfHashTable; 
        if (table[pos] == -1 || table[pos] == -2){
            table[pos] = k;
            numberOfItems++;
            return;
        }
    }
    reHash();
    insert(k);
}

int DoubleHashing::search(int k){
    for (int i = 0; i < sizeOfHashTable; ++i){
        tempProbes++; totalProbes++;
        int pos = static_cast<int>(k % sizeOfHashTable + i * (1 + (sizeOfHashTable-1) * (k*A - static_cast<int>(k*A)))) % sizeOfHashTable; 
        if (table[pos] == -2) return -1;
        else if (table[pos] == k) return pos;
    }
    return -1;
}

void DoubleHashing::deleteKey(int k){
    int index = search(k);
    if (index != -1) table[index] = -1;
    else return;
    numberOfItems--;
}

void DoubleHashing::reHash(){
    vector<int> newTable;
    for(auto & r : primeList){
        if (r > sizeOfHashTable){
            sizeOfHashTable = r; break;
        }
    }
    for (int i = 0; i < sizeOfHashTable; ++i){
        newTable.push_back(-2);
    }
    for (auto & k : table) if (k != -1 && k != -2){
        for (int i = 0; i < sizeOfHashTable; ++i){
            int pos = static_cast<int>(k % sizeOfHashTable + i * (1 + (sizeOfHashTable-1) * (k*A - static_cast<int>(k*A)))) % sizeOfHashTable; 
            if (newTable[pos] == -1 || newTable[pos] == -2){
                newTable[pos] = k;
                break;
            }
        }
    }
    table.clear();
    table = newTable;
}


struct CuckooHashing : public Hashtable{
    vector<int> table1;
    vector<int> table2;
    CuckooHashing(): Hashtable(){
        for (int i = 0; i < 11; ++i) table1.push_back(-1); // -1: empty
        for (int i = 0; i < 11; ++i) table2.push_back(-1); // -1: empty
    }
    void insert(int key) override;
    int search(int key) override;
    void deleteKey(int key) override;
    void insert2(vector<int> & t1, vector<int> & t2, int k);
    void reHash() override;
    ~CuckooHashing(){
        table1.clear(); table2.clear();
    }
};

void CuckooHashing::insert(int k){
    int flag = 0; // 0 for table1, 1 for table2    
    int e = k;
    int h;
    for (int i = 0; i < 20; ++i){
        tempProbes++;totalProbes++;
        if (flag == 0){
            h = e % sizeOfHashTable;
            if (table1[h] == -1) {
                table1[h] = e; numberOfItems++; return;
            } 
            int t = table1[h];
            table1[h] = e; e = t; 
        }
        else {
            h = static_cast<int>(sizeOfHashTable * (A * e - static_cast<int>(A * e)));
            if (table2[h] == -1) {
                table2[h] = e; numberOfItems++; return;
            } 
            int t = table2[h];
            table2[h] = e; e = t; 
        }
        flag = 1 - flag;
    }
    reHash();
    insert(e);
    //cout << " insert : " << e << endl;
}

void CuckooHashing::insert2(vector<int> & t1, vector<int> & t2, int k){
    int flag = 0; // 0 for table1, 1 for table2    
    int e = k;
    int h;
    for(int i = 0; i < 20; ++i){
        if (flag == 0){
            h = e % sizeOfHashTable;
            if (t1[h] == -1) {
                t1[h] = e; numberOfItems++; return;
                cout << "modify : " << t1[h] << endl;
            } 
            int t = t1[h];
            t1[h] = e;
            e = t; 
        }
        else {
            h = static_cast<int>(sizeOfHashTable * (A * e - static_cast<int>(A * e)));
            if (t2[h] == -1) {
                t2[h] = e; return;
            } 
            int t = t2[h];
            t2[h] = e;
            e = t; 
        }
        flag = 1 - flag;
    }
}

int CuckooHashing::search(int k){
    int pos = k % sizeOfHashTable;
    tempProbes++; totalProbes++;
    if (table1[pos] == k) return pos;
    tempProbes++; totalProbes++;
    pos = static_cast<int>(sizeOfHashTable * (A * k - static_cast<int>(A * k)));
    if (table2[pos] == k) return pos;
    return -1;
}

void CuckooHashing::deleteKey(int k){
    //cout << "delete key = " << k << endl;
    int index = search(k);
    //cout << "delete: " << index << endl;
    if (index == -1) return;
    if (table1[index] == k) table1[index] = -1;
    else table2[index] = -1;
    numberOfItems--;
}

void CuckooHashing::reHash(){
    vector<int> newTable1;
    vector<int> newTable2;
    for(auto & r : primeList){
        if (r > sizeOfHashTable){
            sizeOfHashTable = r; break;
        }
    }
    for (int i = 0; i < sizeOfHashTable; ++i){
        newTable1.push_back(-1);
        newTable2.push_back(-1);
    }

    for (auto & k : table1) if (k != -1){
        insert2(newTable1, newTable2, k);

    }
    for (auto & k : table2) if (k != -1){
        insert2(newTable1, newTable2, k);
    }
    table1.clear();
    table2.clear();
    table1 = newTable1; table2 = newTable2;
}

int main(){
    string command;
    SeparateChaning SC = SeparateChaning();
    LinearProbing LP = LinearProbing();
    DoubleHashing DH = DoubleHashing();
    CuckooHashing CH = CuckooHashing();

    while (cin >> command) if(!cin.eof()){
        if (command == "i"){
            int key; 
            cin >> key;
            SC.insert(key); LP.insert(key); DH.insert(key); CH.insert(key);
            cout << SC.tempProbes  << " " << LP.tempProbes << " " << DH.tempProbes  << " " << CH.tempProbes << endl;
        }
        else if(command == "d"){
            int key; 
            cin >> key;
            SC.deleteKey(key); LP.deleteKey(key); DH.deleteKey(key); CH.deleteKey(key);
            cout << SC.tempProbes  << " " << LP.tempProbes << " " << DH.tempProbes << " " << CH.tempProbes << endl;
        }
        else if(command == "s"){
            int key; 
            cin >> key;
            SC.search(key); LP.search(key); DH.search(key); CH.search(key);
            cout << SC.tempProbes  << " " << LP.tempProbes << " " << DH.tempProbes << " " << CH.tempProbes << endl;
        }
        else if (command == "stats"){
            cout << SC.totalProbes << " " << LP.totalProbes << " " << DH.totalProbes << " " << CH.totalProbes << 
                " " << DH.numberOfItems << " " << SC.sizeOfHashTable << " " << LP.sizeOfHashTable << " " << DH.sizeOfHashTable << " " << CH.sizeOfHashTable << endl; 

        }
        else if (command == "r"){
            int h;
            cin >> h;
            switch(h){
                case 1: SC.reHash(); cout << "REHASH " << SC.sizeOfHashTable << endl; break;
                case 2: LP.reHash(); cout << "REHASH " << LP.sizeOfHashTable << endl; break;
                case 3: DH.reHash(); cout << "REHASH " << DH.sizeOfHashTable << endl; break;
                case 4: CH.reHash(); cout << "REHASH " << CH.sizeOfHashTable << endl; break;
            }
        }
        else{
            cerr << "invalid command" << endl;
        }
        command.clear();
        SC.refreshTemp(); LP.refreshTemp(); DH.refreshTemp(); CH.refreshTemp();
    }

}

