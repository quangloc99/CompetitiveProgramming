// If this code is used, make sure the file name match the class name!
/**
 * BEFORE READING CODE:
 * Java is really lengthy (at least for Java 8). So I used the advantage of initialization block.
 * Basically, every initialization block run before the constructor and they run in the declaration order.
 * And as I understand, every properties (or global variables) is also counted as initialization block.
 */
/**
 * @Author Tran Quang Loc (darkkcyan)
 * Link to problem statement + online submitting: https://contest.yandex.ru/roiarchive/contest/2169/problems/4/
 * The statement is in Russian, so I did a short translation at the end of this file.
 * Idea: it is easy to see that from one point, we just need to greedily book the seat that we can seat for the longest time,
 * and if we do not reach the destination, then we continue this greedy stragtegy.
 * After find out that seat, we can use binary lifting for faster finding the destination.
 */
import java.io.*;
import java.lang.*;
import java.util.*;
import java.util.function.*;
import static java.lang.Integer.parseInt;
import static java.lang.Long.parseLong;
import static java.lang.Math.*;
import static java.lang.System.*;
import static java.util.Arrays.*;
import static java.util.Collections.*;

public class Main {
    PrintWriter out = new PrintWriter(System.out, false);
    BufferedReader in = new BufferedReader(new InputStreamReader(System.in));
    StringTokenizer stok = null;
    String next() {
        while (stok == null || !stok.hasMoreTokens())
            try {
                stok = new StringTokenizer(in.readLine());
            } catch (IOException e) { throw new RuntimeException(e); }
        return stok.nextToken();
    }

    public static void main(String args[]) throws Exception {
        if (args.length > 0) {
            setIn(new FileInputStream(args[0] + ".inp"));
            setOut(new PrintStream(args[0] + ".out"));
        }
        Main solver = new Main();
        solver.out.flush();  //  could be replace with a method, but nah, this is just competitive programming :p
    }
    ///// Actual solution below /////
    ////////////////////////////////////////////////////////////////
    class Pair implements Comparable<Pair> {
        int x, y;
        public Pair(int x_, int y_) { x = x_; y = y_; }

        @Override
        public int compareTo(Pair other) {
            return x == other.x ? y - other.y : x - other.x;
        }
    }

    final int maxlogn = 20;
    int n = parseInt(next()), m = parseInt(next()), k = parseInt(next());

    ArrayList<Pair>[] booked = new ArrayList[n];
    int[] furthest = new int[n];

    {
        setAll(booked, i -> new ArrayList<>());
        for (int i = m; i-- > 0; ) {
            int s = parseInt(next()) - 1, t = parseInt(next()) - 1, a = parseInt(next()) - 1;
            booked[a].add(new Pair(s, t));
        }

        for (int i = 0; i < n; ++i) furthest[i] = i;
        for (int i = 0; i < k; ++i) {
            sort(booked[i]);
            int prev = 0;
            for (Pair cur: booked[i]) {
                furthest[prev] = max(furthest[prev], cur.x);
                prev = cur.y;
            }
            furthest[prev] = n - 1;
        }

        for (int i = 1; i < n; ++i) {
            furthest[i] = max(furthest[i - 1], furthest[i]);
        }

        int[][] up = new int[maxlogn][n];
        for (int i = n; i-->0; ) {
            up[0][i] = furthest[i];
            for (int f = 0; f < maxlogn - 1; ++f) 
                up[f + 1][i] = up[f][up[f][i]];
        }

        for (int q = parseInt(next()); q --> 0; ) {
            int u = parseInt(next()) - 1, v = parseInt(next()) - 1;
            int ans = 0;
            for (int i = maxlogn; i --> 0; ) {
                if (up[i][u] >= v) continue;
                u = up[i][u];
                ans += 1 << i;
            }
            // err.println(u + " " + ans);
            if (u != v) {
                if (furthest[u] < v) ans = -1;
                else ++ans;
            }
            out.println(ans);
        }
    }

    //////////////////////////////
}


/**
 * A train, with k seats, run in 1 direction from station 1 to n. There are m already booked tickets.
 * Each ticket has 3 numbers on it: s, t, and a - the passenger that bought this ticket will go from station a to b
 * and he will seat at the a-th seat. You want to travel on the train too, but because of the booked tickets,
 * some seat may not be avaliable all the time. So you are planning to buy some tickets in order to switch
 * your seat if your current seat is booked at some point during the trip. You are given m already booked tickets,
 * and q queries. Each queries has 2 numbers: x and y. Find out for each query the number of minimum tickets you need to buy
 * if you want to go from station x to y.
 * Sample test explaination:
 * - The first query has no answer, since all the seat are booked from station 2 to 3.
 * - For the second query, you can book 2 ticket:
 *   + 3rd to 4th station on the 2st seat.
 *   + 4th to 5th station on the 1st seat.
 * - For the the last query, just book the first seat for the whole trip.
 */
