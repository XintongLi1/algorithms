/*
 * removeAllk first lookup for the corresponding team(s) in the priority queue, then remove it (them). The runtime is O(nlogn).  
 * The worst case running time for removeAllK() is O(nlog(n)) -> it happens when all of the teams in the priority queue have k wins
 *      1. the lookup takes linear time for each team; 2. the removal takes O(log(n)) time; 3. there are n lookups and removals(O(log(n))), so O(nlog(n)) in total.
 *  Extra notes: The worst case run time for the lookup is O(n), but this would not affect the run time of removeAllK() overall.
 */

// Created Time: Fri 25 Sep 2020 03:37:40 PM EDT
#include <iostream>
#include <vector>

using namespace std;

class findException{

} except;

class Team {
    int wins;
    int losses;
    string name;
    public:
        Team (int w, int l, string n) : wins{w}, losses{l}, name{n}{}
        bool comp(const Team & o, int type) const{ // 1 for maxWins, 0 for minLosses
            if (type) return wins > o.wins;
            else return losses < o.losses;
        }
        int getWins() const{
            return wins;
        }
        string getName() const{
            return name;
        }
};

class TeamPQ{
    vector< const Team *> maxWins; // PQ
    vector< const Team *> minLosses; // PQ

    vector<int> maxToMin, minToMax; // indicate each Team's index at the other vector -> lookup takes O(1) time in each removal()

    void fix_up(int init_pos, vector <const Team *> & vec, int type);
    void fix_down(int init_pos, vector <const Team *> & vec, int type);

    void renewIndex(int index1, int index2, int type);

    int lookupWinsK (int k, int cur = 0);
    
    public:
        void insert (const Team &t);
        const Team & findMaxWins() const;
        const Team & findMinLosses() const;
        void removeMaxWins();
        void removeMinLosses();
        void removeAllK(int k);
        ~TeamPQ(){
            for (auto & r : maxWins){
                delete r;
            }
            maxWins.clear();
            minLosses.clear();
            maxToMin.clear();
            minToMax.clear();
        }
};


void TeamPQ::renewIndex(int index1, int index2, int type){
    if (type){ // maxWins is changed
        int atLoss = maxToMin[index1];
        minToMax[atLoss] = index2;
        atLoss = maxToMin[index2];
        minToMax[atLoss] = index1;
        int tmp = maxToMin[index1];
        maxToMin[index1] = maxToMin[index2];
        maxToMin[index2] = tmp;
    }
    else{
        int atWin = minToMax[index1];
        maxToMin[atWin] = index2;
        atWin = minToMax[index2];
        maxToMin[atWin] = index1;
        int tmp = minToMax[index1];
        minToMax[index1] = minToMax[index2];
        minToMax[index2] = tmp;
    }
}

void TeamPQ::fix_up(int cur, vector<const Team *> & vec, int type){
    int parent = (cur - 1) / 2;
    const Team * t = vec[cur];
    while (parent != cur && parent >= 0 && !vec[parent]->comp(*t, type)){
        const Team * tmp = vec[parent];
        vec[parent] = t;
        vec[cur] = tmp;
        renewIndex(parent, cur, type);
        cur = parent;
        parent = (cur - 1) / 2;
    }
}
void TeamPQ::fix_down(int cur, vector <const Team *> & vec, int type){
    int s = vec.size();
    const Team * t = vec[cur];
    int left = 2 * cur + 1;
    int right = 2 * cur + 2;
    while (left < s){
        int newPos = left;
        if (left != s - 1 && vec[right]->comp(*vec[left], type))
            newPos = right;
        if (t->comp(*vec[newPos], type))
            break;
        const Team * tmp = vec[newPos];
        vec[newPos] = t; 
        vec[cur] = tmp;
        renewIndex(newPos, cur, type);
        cur = newPos;
        left = 2 * newPos + 1; right = 2 * newPos + 2;
    }
}

void TeamPQ::insert(const Team & t){
    const Team * copy_t = & t;
    maxWins.emplace_back(copy_t);
    minLosses.emplace_back(copy_t);
    int cur = maxWins.size() - 1;
    maxToMin.emplace_back(cur);
    minToMax.emplace_back(cur);
    fix_up(cur, maxWins, 1);
    fix_up(cur, minLosses, 0);
}

const Team & TeamPQ::findMaxWins() const{
    if (maxWins.empty()) throw except;
    return *maxWins[0];

}
const Team & TeamPQ::findMinLosses() const{
    if (maxWins.empty()) throw except;
    return *minLosses[0];
}

int TeamPQ::lookupWinsK (int k, int cur ){
    // return -1 if not found
    if (k > maxWins[cur]->getWins()) return -1;
    else {
        if (k == maxWins[cur]->getWins()) return cur;
        if (2 * cur + 1 >= maxWins.size()) return -1;
        int find = lookupWinsK(k, 2 * cur + 1);
        if (find != -1) return find;
        if (2 * cur + 2 < maxWins.size()){
            find = lookupWinsK(k, 2 * cur + 2);
        }
        return find;
    }
}

void TeamPQ::removeMaxWins(){
    if(maxWins.empty()) return;
    int last_pos = maxWins.size() - 1;
    renewIndex(0, last_pos, 1);
    delete maxWins[0];
    maxWins[0] = maxWins.back();
    maxWins.pop_back(); 
    fix_down(0, maxWins, 1);

    int pos = maxToMin[last_pos]; 
    renewIndex(pos, last_pos, 0);
    minLosses[pos] = minLosses.back();
    minLosses.pop_back();
    fix_down(pos, minLosses, 0);

    maxToMin.pop_back(); minToMax.pop_back();
}

void TeamPQ::removeMinLosses(){
    if(maxWins.empty()) return;
    int last_pos = maxWins.size() - 1;
    renewIndex(0, last_pos, 0);
    delete minLosses[0];
    minLosses[0] = minLosses.back();
    minLosses.pop_back(); 
    fix_down(0, minLosses, 0);

    int pos = minToMax[last_pos]; 
    renewIndex(pos, last_pos, 1);
    maxWins[pos] = maxWins.back();
    maxWins.pop_back();
    fix_down(pos, maxWins, 1);

    maxToMin.pop_back(); minToMax.pop_back();
}

void TeamPQ::removeAllK(int k){
    while (true){
        if(maxWins.empty()) return;
        int cur = lookupWinsK(k);
        int last_pos = maxWins.size() - 1;
        if (cur != -1){
            delete maxWins[cur];
            renewIndex(cur, last_pos, 1);
            maxWins[cur] = maxWins.back();
            maxWins.pop_back(); 
            fix_down(cur, maxWins, 1);

            int pos = maxToMin[last_pos]; 
            renewIndex(pos, last_pos, 0);
            minLosses[pos] = minLosses.back();
            minLosses.pop_back();
            fix_down(pos, minLosses, 0);

            maxToMin.pop_back(); minToMax.pop_back();
        }
        else return;
    }
}

int main(){
    TeamPQ PriorityQueue;
    string command;
    while (!cin.eof()){
		cin >> command; 
        if (command == "i"){
            int wins, losses;
            string name;
            cin >> wins >> losses >> name;
            if (cin.fail()) cin.clear();
            Team *t = new Team{wins, losses, name};
            PriorityQueue.insert(*t);
        }
        else if (command == "pw"){
            try{
                cout << PriorityQueue.findMaxWins().getName() << endl;
            }catch(findException &){
                cerr << "No item in PQ" << endl;
            }
        }
        else if (command == "pl"){
            try{
                cout << PriorityQueue.findMinLosses().getName() << endl;
            }catch(findException &){
                cerr << "No item in PQ" << endl;
            }
        }
        else if (command == "rw"){
            PriorityQueue.removeMaxWins();
        }
        else if (command == "rl"){
            PriorityQueue.removeMinLosses();
        }
        else if (command == "ra"){
            int k;
            cin >> k;
            if (cin.fail()) cin.clear();
            PriorityQueue.removeAllK(k);
        }
        else{
            cerr << "Invalid Command" << endl;
        }
		command.clear();
    }
}

