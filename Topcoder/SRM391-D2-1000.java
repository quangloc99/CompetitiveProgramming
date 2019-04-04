// This is not a valid java file because the file name does not match the class's name.
/**
 * @Author: Tran Quang Loc
 * Editorial: https://www.topcoder.com/tc?module=Static&d1=match_editorials&d2=srm391
 * This solution implements the same idea as in the editorial.
 * Notes:
 * - This is the top down dp solution. So before doing dp, we must assign every nodes's cost that cannot be reached from top left, cell
 * by 0. The funny things is the bottom up solution with bfs/dfs don't have this problem.
 */
import java.io.*;
import java.util.*;

public class MarbleCollectionGame {
	int n, m;
	ArrayList<Integer> gr[];

	ArrayList<Integer>[] reverseGraph(ArrayList<Integer>[] gr) {
		ArrayList<Integer>[] newGr = new ArrayList[gr.length];
		for (int i = 0; i < gr.length; i++) {
			newGr[i] = new ArrayList<>();
		}
		for (int u = 0; u < gr.length; ++u) {
			for (int v: gr[u]) {
			    newGr[v].add(u);
            }
		}
		return newGr;
	}

	int dsu[];
	int componentCost[];
	int findp(int u) { return dsu[u] = dsu[u] == u ? u : findp(dsu[u]);}
	void join(int u, int v) {
//		System.err.printf("JOIN %d %d\n", u, v);
	    u = findp(u); v = findp(v);
	    if (u == v) return;
	    dsu[u] = v;
	    componentCost[v] += componentCost[u];
	}

	ArrayList<Integer> stack = new ArrayList<>();
	boolean vis[];

	void predfs(int u) {
		if (vis[u]) return;
		vis[u] = true;
		for (int v: gr[u]) {
			predfs(v);
		}
		stack.add(u);
	}

	void postdfs(int u) {
		vis[u] = true;
		for (int v: gr[u]) {
		    if (vis[v]) continue;
		    join(u, v);
			postdfs(v);
		}
	}

	int calAns(int u) {
	    u = findp(u);
		if (vis[u]) return componentCost[u];
		vis[u] = true;
		int curMax = 0;
		for (int v: gr[u]) {
			if (findp(v) == u) continue;
			curMax = Math.max(calAns(v), curMax);
		}
		componentCost[u] += curMax;
		return componentCost[u];
	}

	public int collectMarble(String[] board) {
		n = board.length;
		m = board[0].length();
		gr = new ArrayList[n * m];
		dsu = new int[gr.length];
		componentCost = new int[gr.length];
		for (int i = 0; i < n; ++i) {
			for (int f = 0; f < m; f++) {
				int u = i * m + f;
				gr[u] = new ArrayList<>();
				dsu[u] = u;
				char t = board[i].charAt(f);
				componentCost[u] = t == 'U' || t == 'L' || t == '#' ? 0 : (int)t - (int)'0';

				if (t == '#') continue;
				if (i < n - 1 && board[i + 1].charAt(f) != '#') gr[u].add((i + 1) * m + f);
				if (f < m - 1 && board[i].charAt(f + 1) != '#') gr[u].add(i * m + f + 1);
				if (t == 'U' && i > 0 && board[i - 1].charAt(f) != '#') gr[u].add((i - 1) * m + f);
				if (t == 'L' && f > 0 && board[i].charAt(f - 1) != '#') gr[u].add(i * m + f - 1);
			}
		}

		vis = new boolean[gr.length];
		predfs(0);
		for (int i = 0; i < gr.length; ++i) if (!vis[i]) componentCost[i] = 0;
		for (int i = 1; i < gr.length; ++i) {
			predfs(i);
		}
		gr = reverseGraph(gr);
		Arrays.fill(vis, false);
		for (int i = stack.size(); i-- > 0; ) {
			int u = stack.get(i);
			if (vis[u]) continue;
			postdfs(u);
		}
		for (int i = 0; i < gr.length; ++i) {
			if (i == findp(i)) continue;
			gr[findp(i)].addAll(gr[i]);
		}
		int ans = 0;
		Arrays.fill(vis, false);
		for (int i = 0; i < gr.length; ++i) ans = Math.max(ans, calAns(i));
		return ans;
	}
}
