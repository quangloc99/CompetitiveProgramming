The solution I was reading can be found [here](https://github.com/mostafa-saad/MyCompetitiveProgramming/blob/master/Olympiad/CEOI/official/2016/kangaroo-solution.pdf)

The idea in the solution is correct, but the whole solution was not! You can just see the fomula for array `A` and `D`, I just write `A`'s here:
```
A[n][i][j] = A[n][i - 1][j] - D[n - 1][i - 1][j - 1]
```
 
Take a look at the state `D[n - 1][i - 1][j - 1]`. What's wrong here is while `n` is *decresing*, `j` is also *decresing*  (in order to have the identity: `N - cf = n - j = (n - 2) - (j - 2)`). But, for example, if `N = 5, cf = 1`, then their difference is 4, but when we are at the dp state with `n = 3`, then our `j` will be `3 - 4 = -1`???

Ok maybe in the solution, they were not really clear, so we somehow must handle this edge case where `j` might be negative. But I think there *must not be such cases*. If **we did not swap `cs` and `cf`**, we can still have dp solution in `O(n^3)` with just a little modification: we decrease `j` by 1 when `i` is in front of `j`, but **keep `j` unchanged otherwise**. What I am saying here is if we dont have the condition `cs < cf` or `i < j` initially, then we have to check it by hand and decrease `j` if necessary. This is what I done for 51 points for this problem, and none of my dp states have negative index.

What leads to their error, in my opinion, is that they just think that swap `cs` and `cf` in the beginning is enough. 
Let's take a look at the first formula for `A`:
```
A[n][i][j] = D[n - 1][i][j - 1] + D[n - 1][i + 1][j - 1] + ... + D[n - 1][n - 2][j - 1]
```
So initially, we call `A[N][cs][cf]`, which we alraedy have `cs < cf`. But we can see here, nothing graduates that `D[n - 1][n - 2][j - 1] = D[N - 1][N - 2][cf - 1]` will have the second index (`N - 2`) smaller then the third index (`cf - 1`). If we tried to swap them again (because yes, we can still have the same result for the same reason we swap `cf` and `cs`) to reuse the fomula again, this is actually correct, but the identity `N - cf = n - j` won't be hold anymore. So yeah, the optimization is broken!

I hope I was wrong, because the offical solution seems to be really nice, and I actually learn a bit in writing dp fomula.
