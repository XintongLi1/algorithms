import java.util.List;
import java.util.HashMap;
import java.util.ArrayList;

class TrieNode {
    HashMap<Character, TrieNode> chars = new HashMap<>();
    String word = null;
}

public class WordSearchII212 {
    private TrieNode root;
    private List<String> res;
    private void insertWord (String word) {
        TrieNode cur = root;
        for (char c: word.toCharArray()){
            if (!cur.chars.containsKey(c)){
                cur.chars.put(c, new TrieNode());
            }
            cur = cur.chars.get(c);
        }
        cur.word = word;
    }
    public List<String> findWords(char[][] board, String[] words) {
        root = new TrieNode();
        res = new ArrayList<>();
        // Build trie
        for (String word: words) {
            insertWord(word);
        }
        int m = board.length;
        int n = board[0].length;
        for (int i = 0; i < m; i++){
            for (int j = 0; j < n; j++){
                dfs(i, j, root, board);
            }
        }
        return res;
    }

    public void dfs(int i, int j, TrieNode cur, char[][] board) {
        char ch = board[i][j];
        if (ch != '*' && cur.chars.containsKey(ch)) {
            cur = cur.chars.get(ch);
            if (cur.word != null) {
                res.add(cur.word);
                cur.word = null; // avoid duplicate word being added to result
            }
            board[i][j] = '*'; // already visited
            if (i > 0) dfs(i-1, j, cur, board);
            if (j > 0) dfs(i, j-1, cur, board);
            if (i < board.length - 1) dfs(i+1, j, cur, board);
            if (j < board[0].length - 1) dfs(i, j+1, cur, board);
            board[i][j] = ch;
        }
    }
}
