/**
 * @Author Tran Quang Loc (darkkcyan)
 * The solution is the same as in the analysis.
 */
import java.io.*;
import java.lang.*;
import java.util.*;
import static java.lang.Integer.*;
import static java.lang.Long.*;
import static java.lang.Math.*;
import static java.lang.System.*;
import static java.util.Arrays.*;
import static java.util.Collections.*;

class Main {
    final int max_c = 50;
    long combination[][] = new long[max_c][max_c];
    void pre_compute_combination() {
      for (int i = 0; i < max_c; ++i) {
        combination[i][0] = combination[i][i] = 1;
        for (int f = 1; f < i; ++f) 
          combination[i][f] = combination[i - 1][f] + combination[i - 1][f - 1];
      }
    }

    Main() throws IOException {
      in = new BufferedReader(new InputStreamReader(System.in));
      pre_compute_combination();
      int ntest = parseInt(next());
      for (int testcase = 1; testcase <= ntest; ++testcase) {
        int c = parseInt(next());
        int n = parseInt(next());

        long total_cases = combination[c][n];

        double dp[] = new double[c + 1];
        dp[c] = 0;
        for (int cur_cards = c; cur_cards-- > 0; ) {
          double coef = 1;
          if (cur_cards >= n) 
            coef -= 1.0 * combination[c - cur_cards][0] * combination[cur_cards][n] / total_cases;
          double s = 0;
          for (int new_cards = 1; new_cards <= n && cur_cards + new_cards <= c; ++new_cards) {
            double cur_val = combination[c - cur_cards][new_cards];
            cur_val *= combination[cur_cards][n - new_cards];
            cur_val /= total_cases;
            cur_val *= dp[cur_cards + new_cards];
            s += cur_val;
          }
        
          dp[cur_cards] = (1 + s) / coef;
        }
        out.printf("Case #%d: %.07f\n", testcase, dp[0]);
      }
    }

    //////////////////////////////
    BufferedReader in;
    StringTokenizer stok = null;
    String next() throws IOException {
        while (stok == null || !stok.hasMoreTokens()) {
            stok = new StringTokenizer(in.readLine());
        }
        return stok.nextToken();
    }


    public static void main(String args[]) throws IOException {
        if (args.length > 0) {
            setIn(new FileInputStream(args[0] + ".inp"));
            setOut(new PrintStream(args[0] + ".out"));
        }
        setOut(new PrintStream(out, false));
        new Main();
        out.flush();
    }
}
