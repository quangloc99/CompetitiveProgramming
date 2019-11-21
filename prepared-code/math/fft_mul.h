#define cpl complex<double>
#define vcpl vector<cpl>
const double pi = atan(1) * 4;

inline vcpl fftmul(vcpl u, vcpl v) {
    unsigned cnt = 0;
    while ((1u << cnt) < max(u.size(), v.size())) ++cnt;
    ++cnt;

    auto fft = [&](vector<cpl>& a, bool inv) -> void {
        vcpl b = a;
        int n = 1 << cnt;
        for (int i = n; i--; ) {
            int f = 0, t = cnt;
            while (t--) f |= ((i >> t) & 1) << (cnt - t - 1);
            a[i] = b[f];
        }

        for (int step = 2; step <= n; step <<= 1) {
            for (int i = 0; i < n; i += step) {
                double ang = 2 * pi / step * (inv ? -1 : 1);
                cpl w(1), pw(cos(ang), sin(ang));
                for (int f = 0; f < step / 2; ++f) {
                    cpl x = a[i + f], y = a[i + f + step / 2] * w;
                    a[i + f] = x + y;
                    a[i + f + step / 2] = x - y;
                    w *= pw;
                }
            }
        }
        if (inv) for (int i = n; i--; ) a[i] /= n;
    };

    u.resize(1 << cnt);
    v.resize(1 << cnt);

    fft(u, 0);
    fft(v, 0);
    for (int i = (1 << cnt); i--; ) u[i] *= v[i];
    fft(u, 1);
    return u;
}
