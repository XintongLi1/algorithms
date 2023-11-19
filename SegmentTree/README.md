# Segment Tree and Fenwick Tree
- [Segment Tree and Fenwick Tree](#segment-tree-and-fenwick-tree)
  - [Fenwick Tree](#fenwick-tree)
  - [Segment Tree](#segment-tree)
  - [Relevant Problems](#relevant-problems)
  - [Problem Solution](#problem-solution)
    - [SubarraysDistinctSum2916](#subarraysdistinctsum2916)


## Fenwick Tree 
- For Range Sum problem

Use DP to solve Prefix Sum
 * Method: `[2, 6, -1, 4]` $\Rightarrow$ `[2, 8, 7, 11]` 
 * Reduce query of sum from $i$-th element to $j$-th element to $O(1)$ time.
 * $O(n)$ time to update any item

Fenwich Tree
 * Each node store partial sum
   * $n_1 = A[1]$, $n_2 = n_1 + A[2], n_3 = A[3], n_4 = n_2 + n_3$
   * $n_5 = A[5], n_6 = n_5 + A[6], n_7 = A[7], n_8 = n_4 + n_6 + n_7$
 * Bitwise operation
   * Update: add value at `i` to `i = i + lowbit(i), i <= n`, where `lowbit(x) = x & (-x)`.
   * Query: `i -= lowbit(i)`.  
     * `7 -> 6 -> 4`
     * `3 -> 2`

```c++
class NumArray {
    private:
        vector<int> arr;
        vector<int> bit;
        
        int lower_bit(int i){
            return i & -i; // rightmost 1(0...0)
        }
        
        int query(int i){
            ++i;
            int sum = 0;
            while(i > 0){
                sum += bit[i];
                i -= lower_bit(i);
            }
            return sum;
        }
        
        void add(int i, int val){
            ++i;
            while(i < bit.size()){
                bit[i] += val;
                i += lower_bit(i);
            }
        }
        
    public:
        NumArray(vector<int> &nums) : arr(nums) {
            bit.resize(nums.size() + 1);
            for(int i = 0; i < nums.size(); ++i){
                add(i, nums[i]);
            }
        }
    
        void update(int i, int val) {
            if(val != arr[i]){
                add(i, val - arr[i]);
                arr[i] = val;
            }
        }
    
        int sumRange(int i, int j) {
            return query(j) - query(i-1);
        }
    };
```

## Segment Tree

A balanced binary tree.
Each leaf node represents an element in the array. Each non-leaf node covers the union of its children's range.

The below implementation was for LeetCode 307 Range Sum.
```c++
class Node {
    int val;
    int start, end;
    shared_ptr<Node> left, right;
    public:
    Node(int v, int s, int e, shared_ptr<Node> l = nullptr, shared_ptr<Node> r = nullptr):
        val(v), start(s), end(e), left(l), right(r){}
    friend class NumArray;
};


class NumArray {
    shared_ptr<Node> tree;
    vector<int> arr;
    
    shared_ptr<Node> buildTree(int start, int end){
        if (start == end){
            shared_ptr<Node> t = make_shared<Node>(arr[start], start, end);
            return t;
        }
        int mid = start + (end - start) / 2;
        shared_ptr<Node> l = buildTree(start, mid);
        shared_ptr<Node> r= buildTree(mid + 1, end);
        shared_ptr<Node> t = make_shared<Node>(l->val + r->val, start, end, l, r);
        return t;  
    }
    
    void updateTree(int i, shared_ptr<Node> n, int d){
        // i is the index of val changed in arr
        if (n->start == i && n->end == i){
            n->val += d;
        } else{
            int mid = n->start + (n->end - n->start) / 2;
            if (i <= mid){ // update left subtree
                updateTree(i, n->left, d);
            } else {
                updateTree(i, n->right, d);
            }
            n->val += d;
        }  
    }
    
    int query(int i, int j, shared_ptr<Node> n){
        if (n->start == i && n->end == j) return n->val;
        int mid = n->start + (n->end - n->start) / 2;
        if (j <= mid) return query(i, j, n->left);
        else if (i > mid) return query(i, j, n->right);
        else return query(i, mid, n->left) + query(mid + 1, j, n->right);
    }
    
public:
    NumArray(vector<int>& nums): tree(nullptr) {
        if (nums.empty()) return;
        arr = vector<int> (nums); 
        tree = buildTree(0, nums.size()-1);
    }
    
    void update(int i, int val) {
        int diff = val - arr[i];
        updateTree(i, tree, diff); // d indicates difference  
        arr[i] += diff;
    }
    
    int sumRange(int i, int j) {
        return query(i, j, tree);
    }
};

/*
 * A NumArray object will be instantiated and called as such:
 * NumArray* obj = new NumArray(nums);
 * obj->update(i,val);
 * int param_2 = obj->sumRange(i,j);
 */
```

## Relevant Problems
* Range Sum
  * See `RangeSum327.py`
* [Subarrays Distinct Element Sum of Squares II](https://leetcode.com/problems/subarrays-distinct-element-sum-of-squares-ii/description/)
  * `SubarraysDistinctSum2916.java` 
  * [Solution Explanation](#subarraysdistinctsum2916)


## Problem Solution

### SubarraysDistinctSum2916

LeetCode 2916. Subarrays Distinct Element Sum of Squares II (Hard)

**Problem Description:**
You are given a 0-indexed integer array nums. \
The distinct count of a subarray of nums is defined as: \
Let `nums[i..j]` be a subarray of nums consisting of all the indices from `i` to `j` such that `0 <= i <= j < nums.length`. 
Then the number of distinct values in `nums[i..j]` is called the distinct count of `nums[i..j]`. \
Return the sum of the squares of distinct counts of all subarrays of nums in modulo $10^9+7$. 

**Algorithm**
Let $f(i, j)$ be the distinct counts of `nums[i..j]`. 
Let $a_j$ be the last index before $j$ where `nums[j]` occur (set $a_j$ to -1 if no such index exists), then 
1. for all $i$ with $a_j < i < j$, we have $f(i, j) = f(i, j-1) + 1$
2. for all $i$ with $0  \le i \le a_j$, we have $f(i, j) = f(i, j-1)$

Let $g(i)$ be the sum of squares of distinct counts of all subarrays ends at index `i`. \
The answer we look for is $\sum_{i=1}^{n} g(i)$. \
Moreover, we have
$$
\begin{aligned}
    g(j) & = \sum_{i=0}^{j} f^2(i, j) \\
    & = \sum_{i=0}^{j-1} f^2(i, j) + 1 \\
    & = \sum_{i=0}^{a_j} f^2(i, j) + \sum_{i=a_j+1}^{j-1} f^2(i, j) + 1\\
    & = \sum_{i=0}^{a_j} f^2(i, j-1) + \sum_{i=a_j+1}^{j-1} [f(i, j-1) + 1]^2 + 1\\
    & = \sum_{i=0}^{a_j} f^2(i, j-1) + \sum_{i=a_j+1}^{j-1} [ f^2(i, j-1) + 2 \cdot f(i, j-1) + 1 ] + 1\\
    & = [\sum_{i=0}^{a_j} f^2(i, j-1) + \sum_{i=a_j+1}^{j-1} f^2(i, j-1)] + 2 \cdot \sum_{i=a_j+1}^{j-1} f(i, j-1) + (j - a_j) \\
    & = g(j-1) + 2 \cdot \sum_{i=a_j+1}^{j-1} f(i, j-1) + (j - a_j)
\end{aligned}
$$

Since the constraints on the input is 
`1 <= nums.length <= 10^5` and `1 <= nums[i] <= 10^5`, we may want to find an $O(n\log n)$ time solution. \
This requires us to compute $\sum_{i=a_j+1}^{j-1} f(i, j-1)$ efficiently in $O(\log n)$ time.

We can employ Segment tree with lazy propagation to update the values of each interval in $O(\log n)$ time.
