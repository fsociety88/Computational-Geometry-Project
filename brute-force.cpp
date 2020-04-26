#include<bits/stdc++.h>
using namespace std;

#define ll long long int
#define LD long double

const int N = 100010;

int inf = 1e9;
int mod = 1e9 + 7;

/** To find Run-time of code**/
class Timer {
private:
    using clock_t = std::chrono::high_resolution_clock;
    using second_t = std::chrono::duration<double, std::ratio<1> >;
    std::chrono::time_point<clock_t> m_beg;
public:
    Timer() : m_beg(clock_t::now()) {}
    void reset() { m_beg = clock_t::now(); }
    double elapsed() const {
        return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
    }
}T;
/** END **/

const LD kEps = 1e-9;
const LD kPi = 2 * acos(0);

LD Sq(LD x) {
    return x * x;
}

struct Point {
    LD x, y;

    Point() {}
    Point(LD a, LD b) : x(a), y(b) {}
    Point(const Point& a) : x(a.x), y(a.y) {}

    void operator = (const Point& a) {
        x = a.x;
        y = a.y;
    }
    Point operator + (const Point& a) const {
        Point p(x + a.x, y + a.y);
        return p;
    }
    Point operator - (const Point& a) const {
        Point p(x - a.x, y - a.y);
        return p;
    }
    Point operator * (LD a) const {
        Point p(x * a, y * a);
        return p;
    }
    Point operator / (LD a) const {
        assert(abs(a) > kEps);
        Point p(x / a, y / a);
        return p;
    }
    Point& operator += (const Point& a) {
        x += a.x;
        y += a.y;
        return *this;
    }
    Point& operator -= (const Point& a) {
        x -= a.x;
        y -= a.y;
        return *this;
    }
    Point& operator *= (LD a) {
        x *= a;
        y *= a;
        return *this;
    }
    Point& operator /= (LD a) {
        assert(abs(a) > kEps);
        x /= a;
        y /= a;
        return *this;
    }

    bool IsZero() const {
        return abs(x) < kEps && abs(y) < kEps;
    }
    bool operator == (const Point& a) const {
        return (*this - a).IsZero();
    }
    LD CrossProd(const Point& a) const {
        return x * a.y - y * a.x;
    }
    LD CrossProd(Point a, Point b) const {
        a -= *this;
        b -= *this;
        return a.CrossProd(b);
    }
    LD DotProd(const Point& a) const {
        return x * a.x + y * a.y;
    }
    friend ostream& operator<<(ostream& out, Point m);
};

ostream& operator<<(ostream& out, Point p) {
    out << "(" << p.x << ", " << p.y << ")";
    return out;
}

struct PointUtils {
    /*
    A, B, C Orientation
    Counter clockwise -> 1;
    Clockwise -> -1;
    Collinear -> 0;
    */
    static int orient(Point A, Point B, Point C) {
        LD X = (B - A).CrossProd(C - A);
        if(X < 0) {
            return -1;
        }
        if(X < kEps) {
            return 0;
        }
        return 1;
    }
    //Returns magnitude of orientation A, B, C
    static LD OrientMag(Point A, Point B, Point C) {
        return (B - A).CrossProd(C - A);
    }
    //Check whether given points form convex polygon or not
    static bool isConvex(vector<Point> P) {
        bool hasPos = false, hasNeg = false;
        int n = (int)P.size();
        for(int i = 0; i < n; i++) {
            int x = orient(P[i], P[(i + 1) % n], P[(i + 2) % n]);
            if(x > 0) {
                hasPos = true;
            }
            if(x < 0) {
                hasNeg =true;
            }
        }
        return !(hasPos && hasNeg);
    }
    static bool half(Point A) {
        assert(A.x != 0 && A.y != 0);
        return (A.y > 0 || (A.y == 0 && A.x < 0));
    }
    //Check whether P is inside a disk considering A & B as diameter
    static bool InDisk(Point A, Point B, Point P) {
        return ((A - P).DotProd(B - P) <= 0);
    }
    //Check whether P is on the segment A & B
    static bool OnSegment(Point A, Point B, Point P) {
        return orient(A, B, P) == 0 && InDisk(A, B, P);
    }
    //Return Proper Intersection of Segments A,B & C,D
    static bool ProperIntersect(Point A, Point B, Point C, Point D, Point &out) {
        int oa = orient(C, D, A);
        int ob = orient(C, D, B);
        int oc = orient(A, B, C);
        int od = orient(A, B, D);
        if(oa * ob < 0 && oc * od < 0) {
            out = (A * OrientMag(C, D, B) - B * OrientMag(C, D, A)) / (OrientMag(C, D, B) - OrientMag(C, D, A));
            return true;
        }
        return false;
    }
    //Returns Intersection Points of Segments A,B & C,D
    static vector<Point> IntersectSeg(Point A, Point B, Point C, Point D) {
        Point out;
        if(ProperIntersect(A, B, C, D, out)) {
            return {out};
        }
        vector<Point> s;
        if(OnSegment(A, B, C)) {
            s.push_back(C);
        }
        if(OnSegment(A, B, D)) {
            s.push_back(D);
        }
        if(OnSegment(C, D, A)) {
            s.push_back(A);
        }
        if(OnSegment(C, D, B)) {
            s.push_back(B);
        }
        return s;
    }

};
PointUtils PU;

signed main()
{
    //freopen("IN", "r", stdin);
    //freopen("OUT", "w", stdout);

    ios_base::sync_with_stdio(0);
    cin.tie(0); cout.tie(0);
    cout << fixed << setprecision(6);

    int n;
    cin >> n;

    Point P1[n];
    for (int i = 0; i < n; i++)
        cin >> P1[i].x >> P1[i].y;

    int m;
    cin >> m;

    Point P2[m];
    for (int i = 0; i < m; i++)
        cin >> P2[i].x >> P2[i].y;

    vector<Point> I;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            Point temp;
            if (PU.ProperIntersect(P1[i], P1[(i + 1) % n], P2[j], P2[(j + 1) % m], temp))
                I.push_back(temp);
        }
    }

    cout << "Intersection Points\n";
    for (Point it : I)
        cout << it << "\n";

    cout << "\nDone in " << 1000.0 * T.elapsed() << " ms.\n";
    return 0;
}
