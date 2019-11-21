/**
 * Author: Tran Quang Loc
 * Tested with UVA 10104.
 * Code sample https://github.com/quangloc99/CompetitiveProgramming/blob/master/UVA/10104.cpp
 */
tuple<llong, llong, llong> extended_gcd(llong a, llong b) {
    if (b == 0) return {a, 1, 0};
    llong gcd, x1, y1;                              // 2 lines can be written as one in C++17
    tie(gcd, x1, y1) = extended_gcd(b, a % b);      // auto [gcd, x1, y1] = extended_gcd(b, a % b);
    return {gcd, y1, x1 - (a / b) * y1};
}
