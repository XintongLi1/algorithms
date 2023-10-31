/*
 * LeetCode 2111. Minimum Operations to Make the Array K-Increasing
 * Problem Description:
 *   The array arr is called K-increasing if arr[i-k] <= arr[i] holds for every index i, where k <= i <= n-1
 *   In one operation, you can choose an index i and change arr[i] into any positive integer.
 *   Return the minimum number of operations required to make the array K-increasing for the given k.
 * Related problem:
 *   Longest Increasing Subsequence
 * Solution:
 *   The core is to make the numbers at index [i, i+k, i+2k, ...] non-decreasing for all 0 <= i < k.
 *   The minimum number of operations to make arr = [i, i+k, i+2k, ...] non-decreasing is 
 *          the length of arr - the longest non-decreasing subsequence of arr
 */


import java.util.ArrayList;
import java.util.List;


public class KIncreasing {
    public int kIncreasing(int[] arr, int k) {
        int count = 0;
        int arrLen = arr.length;
        for (int i = 0; i < k; i++){
            List<Integer> arrK = new ArrayList<>();
            for (int j = i; j < arrLen; j += k){
                arrK.add(arr[j]);
            }
            count += arrK.size() - longestNonIncreasingSubsequence(arrK);
        }
        return count;
    }

    private int longestNonIncreasingSubsequence(List<Integer> arr){
        List<Integer> longestSubsequence = new ArrayList<>();
        for (int i = 0; i < arr.size(); i++){
            int num = arr.get(i);
            if (longestSubsequence.isEmpty() || num >= longestSubsequence.get(longestSubsequence.size()-1)){
                longestSubsequence.add(num);
            } else {
                // replace arr.get(i) with a number in the longestSubsequence
                int upperIdx = upperBound(longestSubsequence, num);
                longestSubsequence.set(upperIdx, num);
            }
        }
        return longestSubsequence.size();
    }
    private int upperBound(List<Integer> arr, int n){
        // Binary search
        int low = 0;
        int high = arr.size() - 1;
        while (low < high){
            int mid = low + (high - low) / 2;
            if (n < arr.get(mid)){
                high = mid;
            } else {
                // If arr = [1, 4, 6, 10, 12], n = 6
                //   we should return 3, which is the index of 10
                low = mid + 1;
            }
        }
        return low;
    }

    public static void assertEquals(int expected, int actual, int testNum) {
        if (expected != actual) {
            throw new AssertionError("Test " + testNum + " failed! Expected: " + expected + ", but got: " + actual);
        }
    }

    public static void main(String[] args) {
        KIncreasing solution = new KIncreasing();

        int[] arr1 = {1, 5, 3, 2, 8, 7, 6};
        int k1 = 2;
        assertEquals(2, solution.kIncreasing(arr1, k1), 1);

        int[] arr2 = {1, 2, 3, 4, 5};
        int k2 = 3;
        assertEquals(0, solution.kIncreasing(arr2, k2), 2);

        int[] arr3 = {5, 4, 3, 2, 1};
        int k3 = 1;
        assertEquals(4, solution.kIncreasing(arr3, k3), 3);

        int[] arr4 = {};
        int k4 = 3;
        assertEquals(0, solution.kIncreasing(arr4, k4), 4);
        
        int[] arr5 = {4, 1, 5, 2, 6, 2};
        int k5 = 2;
        assertEquals(0, solution.kIncreasing(arr5, k5), 5);
    }
}
