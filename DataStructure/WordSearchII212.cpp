#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

class TrieNode {
public:
    unordered_map<char, TrieNode *> chars;
    string word = "";
    ~TrieNode(){
        for (auto & p: chars) delete p.second;
    }
};


class Solution {
    TrieNode * root;
    vector<string> res;

    void insertWord (string word) {
        TrieNode * cur = root;
        for (char c: word){
            if (cur->chars.find(c) == cur->chars.end()){
                cur->chars[c] = new TrieNode();
            }
            cur = cur->chars[c];
        }
        cur->word = word;
    }

    void dfs(int i, int j, TrieNode* cur, vector<vector<char>>& board){
        if (board[i][j] == '*' || cur->chars.find(board[i][j]) == cur->chars.end()) return;
        char ch = board[i][j];
        cur = cur->chars[ch];
        if (!cur->word.empty()) {
            res.push_back(cur->word);
            cur->word = "";  // avoid duplicate word being added to result
        }
        board[i][j] = '*';
        if (i > 0) dfs(i-1, j, cur, board);
        if (j > 0) dfs(i, j - 1, cur, board);
        if (i < board.size() - 1) dfs(i + 1, j, cur, board);
        if (j < board[0].size() - 1) dfs(i, j + 1, cur, board);
        board[i][j] = ch; // reset
    }

public:
    vector<string> findWords(vector<vector<char>>& board, vector<string>& words) {
        root = new TrieNode();
        res.clear();
        for (auto & word: words) insertWord(word); // build trie
        for (int i = 0; i < board.size(); ++i){
            for (int j = 0; j < board[0].size(); ++j){
                dfs(i, j, root, board);
            }
        }
        return res;
    }

    ~Solution(){
        delete root;
        res.clear();
    }
};
