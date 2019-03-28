/**
 * @Author Tran Quang Loc (darkkcyan)
 * Idea: let's call pos[i] be the position of i-th element after the array was sorted. If we build a graph with n verties and the edges
 * are from i to pos[i], then we got a graph with cycles.
 * If we consider the most simpole graph with only 1 cycle, then the optimal way to sort it is: we just swap element u with pos[u],
 * where pos[u] must contains the smallest element of the array. So the result is:
 *    (sum of all elements) - (min element) + (min element) * (n - 1)
 * when there there are more cycles, we notice that beside doing like above (that is, take the smallest element of the cycle and swap
 * around), there is another way: we swap the smallest element of the cycle, with the smallest element of the array, then we use that
 * new elemnet to swap arround, and finally, we swap back the old element. See the 3 smaple test for more detail.
 * So the answer will be sum of min of the that 2 ways's costs for each cycle. 
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
    int n;
    long a[];
    Integer pos[];

    int dsu[];
    int findp(int u) {
        return dsu[u] = dsu[u] == u ? u : findp(dsu[u]);
    }

    void join(int u, int v) {
        dsu[findp(u)] = findp(v);
    }

    long min_in_group[];
    long group_sum[];
    int group_count[];

    Main() throws IOException {
        in = new BufferedReader(new InputStreamReader(System.in));

        for (int testcase = 1; ; ++testcase) {
            n = parseInt(read());
            if (n == 0) break;
            a = new long[n];
            for (int i = 0; i < n; ++i) a[i] = parseLong(read());

            pos = new Integer[n];
            for (int i = 0; i < n; ++i) {
                pos[i] = i;
            }
            sort(pos, (u, v) -> (int)(a[u] - a[v]));

            dsu = new int[n];
            min_in_group = new long [n];
            group_sum = new long[n];
            group_count = new int[n];
            for (int i = 0; i < n; ++i) {
                dsu[i] = i;
                min_in_group[i] = Integer.MAX_VALUE;
            }

            for (int i = 0; i < n; ++i) {
                join(i, pos[i]);
            }
            for (int i = 0; i < n; ++i) {
                int root = findp(i);
                min_in_group[root] = Math.min(min_in_group[root], a[i]);
                group_sum[root] += a[i];
                ++group_count[root];
            }

            long min_num = a[0];
            for (int i = 0; i < n; ++i) min_num = Math.min(min_num, a[i]);

            long ans = 0;
            for (int i = 0; i < n; ++i) {
                int root = findp(i);
                if (root != i) continue;
                if (group_count[i] <= 1) continue;
                long s = group_sum[root] - min_in_group[root];
                int c = group_count[root] - 1;
                long cur_ans = s + Math.min(min_in_group[root] * c, min_in_group[root] * 2 + min_num * (2 + c));
                ans += cur_ans;
            }

//            if (testcase != 1) out.printf("\n");
            out.printf("Case %d: ", testcase);
            out.println(ans + "\n");
        }
    }

    //////////////////////////////
    BufferedReader in;
    StringTokenizer stok = null;
    String read() throws IOException {
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
