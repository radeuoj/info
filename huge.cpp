#include <iostream>
#include <iomanip>

struct Huge {
    static const int BASE = 10000;
    static const int DIGITS = 200;

    int digits = 0;
    int v[DIGITS] = { 0 };

    friend std::istream& operator>>(std::istream& in, Huge& h) {
        std::string s;
        in >> s;

        for (int j = s.size() - 1; j >= 0; h.digits++) {
            for (int k = 1; k < BASE && j >= 0; k *= 10, j--) {
                h.v[h.digits] += (s[j] - '0') * k;
            }
        }

        return in;
    }

    friend std::ostream& operator<<(std::ostream& out, const Huge& h) {
        out << h.v[h.digits - 1];

        for (int i = h.digits - 2; i >= 0; i--) {
            out << std::setfill('0') << std::setw(4) << h.v[i];
        }

        return out;
    }

    Huge() = default;

    Huge(int other) {
        digits = 1;
        v[0] = other;
    }

    Huge(const Huge& other) {
        digits = other.digits;
        std::copy(other.v, other.v + DIGITS, v);
    }

    void operator*=(int other) {
        int c = 0;
        for (int i = 0; i < digits || c != 0; i++) {
            if (i == digits) digits++;

            v[i] = v[i] * other + c;
            c = v[i] / BASE;
            v[i] %= BASE;
        }
    }
    
    Huge operator*(int other) {
        Huge res = *this;
        res *= other;
        return res;
    }

    void operator/=(int other) {
        int r = 0;
        for (int i = digits - 1; i >= 0; i--) {
            r = r * BASE + v[i];
            v[i] = r / other;
            r = r % other;
        }

        while (digits > 0 && v[digits - 1] == 0) {
            digits--;
        }
    }

    Huge operator/(int other) {
        Huge res = *this;
        res /= other;
        return res;
    }

    void operator-=(const Huge& other) {
        int c = 0;
        for (int i = 0; i < digits; i++) {
            v[i] = v[i] - other.v[i] - c;

            if (v[i] < 0) {
                c = 1;
                v[i] += BASE;
            } else {
                c = 0;
            }
        }

        while (digits > 0 && v[digits - 1] == 0) {
            digits--;
        }
    }

    Huge operator-(const Huge& other) {
        Huge res = *this;
        res -= other;
        return res;
    }

    bool operator<(const Huge& other) {
        if (digits != other.digits) {
            return digits < other.digits;
        }

        for (int i = digits - 1; i >= 0; i--) {
            if (v[i] != other.v[i]) {
                return v[i] < other.v[i];
            }
        }

        return false;
    }
};

int main() {
    Huge a;
    int b;
    std::cin >> a >> b;

    std::cout << "* " << a * b << '\n';
    std::cout << "/ " << a / b << '\n';
    std::cout << "- " << a - b << '\n';
    std::cout << "< " << (a < b) << '\n';
}