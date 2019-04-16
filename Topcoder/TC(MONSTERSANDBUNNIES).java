/**
 * @Author: Tran Quang Loc (darkkcyan)
 * Note that the file name is only for noting that this is the solution for the problem named "Monster and bunnies".
 * See the code for the formula :).
 */
import java.io.*;
import java.util.*;

public class MonstersAndBunnies {
  double[][] dp = null;
	public double survivalProbability(int M, int B) {
    if (M == 0) return 1;
    if (M == 1 && B == 0) return 0;
    if (dp == null) {
      dp = new double[M + 1][B + 1];
      for (double[] i : dp) Arrays.fill(i, -1);
    }
    if (dp[M][B] > -1) return dp[M][B];
    dp[M][B] = 0;
    double ans = 0;
    double all = B * (B - 1) / 2 + M * (M - 1) / 2 + B * M + B + M;
    double coef = all - B * (B - 1) / 2 - B / 2.0;
    if (M > 1) {
      ans += M * (M - 1) / 2 * survivalProbability(M - 2, B);
    }
    if (B > 0) {
      ans += B * survivalProbability(M, B - 1) / 2;
    }
    if (M > 0 && B > 0) {
      ans += B * M * survivalProbability(M, B - 1);
    }
    ans /= coef;
    // System.err.printf("%d %d %f %f\n", M, B, ans, all);
    return dp[M][B] = ans;

  }
}
