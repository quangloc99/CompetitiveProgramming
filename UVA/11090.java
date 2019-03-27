/**
 * @Author: Tran Quang Loc (darkkcyan)
 * Idea: it is easy to prove that the cycle must be a simple cycle. This observation give me a dp solution.
 * If we call dp[length][root][u] is minimum distance from `root` to `u` with totally `length` edges. Then the answer to
 * this problem is min([dp[depth][u][u] / depth for depth in range(1, n + 1) for u in range(1, n + 1)]).
 */
import java.io.*;
import java.util.Arrays;
import java.util.StringTokenizer;

import static java.lang.System.*;
import static java.lang.Integer.*;
import static java.lang.Long.*;
import static java.lang.Math.*;

class Main {
    public static void main(String[] args) {
        try {
            if (args.length > 0) {
                String name = args[0];
                setIn(new FileInputStream(name + ".inp"));
                setOut(new PrintStream(new FileOutputStream(name + ".out")));
            }
            new Main();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    BufferedReader in;
    StringTokenizer stok = null;

    String read() throws IOException {
        while (stok == null || !stok.hasMoreTokens()) {
            stok = new StringTokenizer(in.readLine());
        }
        return stok.nextToken();
    }

    int n, m;
    long[][] gr;
    final long inf = (long)1e15;

    Main() throws IOException {
        in = new BufferedReader(new InputStreamReader(System.in));

        int t = parseInt(in.readLine());
        for (int testcase = 1; testcase <= t; ++testcase) {
            n = parseInt(read());
            m = parseInt(read());
            gr = new long[n][n];
            for (long[] i: gr) Arrays.fill(i, inf);
            for (int i = 0; i < m; ++i) {
                int u = parseInt(read()) - 1;
                int v = parseInt(read()) - 1;
                long c = parseLong(read());
                gr[u][v] = Math.min(gr[u][v], c);
            }
            long[][][] dp = new long[n + 1][n][n];
            for (int i = 0; i < n; ++i)
                for (int f = 0; f < n; ++f)
                    if (i != f) dp[0][i][f] = inf;

            double ans = inf;
            for (int cycle_len = 1; cycle_len <= n; ++cycle_len) {
                for (long[] i: dp[cycle_len]) Arrays.fill(i, inf);
                for (int root = 0; root < n; ++root) {
                    for (int u = 0; u < n; ++u)
                        for (int v = 0; v < n; ++v) {
                            dp[cycle_len][root][v] = Math.min(dp[cycle_len][root][v], dp[cycle_len - 1][root][u] + gr[u][v]);
                        }
                    if (dp[cycle_len][root][root] < inf) {
                        ans = Math.min(ans, 1d * dp[cycle_len][root][root] / cycle_len);
                    }
                }
            }

            out.printf("Case #%d: ", testcase);
            if (ans >= inf) out.println("No cycle found.");
            else out.printf("%.2f\n", ans);
        }
    }
}
