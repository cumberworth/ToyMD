#ifndef SPACE_HPP
#define SPACE_HPP

#include <array>
#include <cmath>

namespace LJMD {

typedef std::array<double, 3> Vector3;

inline Vector3 operator-(const Vector3& v) {
    Vector3 vo {v};
    for (int i {0}; i != 3; ++i) {
        vo[i] *= -1;
    }

    return vo;
}

inline Vector3 operator+(const Vector3& v1, const Vector3& v2) {
    Vector3 vo {};
    for (int i {0}; i != 3; ++i) {
        vo[i] = v1[i] + v2[i];
    }

    return vo;
}

inline Vector3 operator+(const double s, const Vector3& v) {
    Vector3 vo {};
    for (int i {0}; i != 3; ++i) {
        vo[i] = s + v[i];
    }

    return vo;
}

inline Vector3 operator+(const Vector3& v, const double s) {
    Vector3 vo {};
    for (int i {0}; i != 3; ++i) {
        vo[i] = s + v[i];
    }

    return vo;
}

inline void operator+=(Vector3& v1, const Vector3& v2) {
    for (int i {0}; i != 3; ++i) {
        v1[i] += v2[i];
    }
}

inline Vector3 operator-(const Vector3& v1, const Vector3& v2) {
    Vector3 vo {};
    for (int i {0}; i != 3; ++i) {
        vo[i] = v1[i] - v2[i];
    }

    return vo;
}

inline void operator-=(Vector3& v1, const Vector3& v2) {
    for (int i {0}; i != 3; ++i) {
        v1[i] -= v2[i];
    }
}

inline Vector3 operator*(const double s, const Vector3& v) {
    Vector3 vo {};
    for (int i {0}; i != 3; ++i) {
        vo[i] = s * v[i];
    }

    return vo;
}

inline Vector3 operator*(const Vector3& v, const double s) {
    Vector3 vo {};
    for (int i {0}; i != 3; ++i) {
        vo[i] = s * v[i];
    }

    return vo;
}

inline void operator*=(Vector3& v, const double s) {
    for (int i {0}; i != 3; ++i) {
        v[i] *= s;
    }
}

inline Vector3 operator/(const Vector3& v, const double s) {
    Vector3 vo {};
    for (int i {0}; i != 3; ++i) {
        vo[i] = v[i] / s;
    }

    return vo;
}

inline Vector3 operator/(const double s, const Vector3& v) {
    Vector3 vo {};
    for (int i {0}; i != 3; ++i) {
        vo[i] = s / v[i];
    }

    return vo;
}

inline void operator/=(Vector3& v, const double s) {
    for (int i {0}; i != 3; ++i) {
        v[i] /= s;
    }
}

inline double sqnorm(const Vector3 v) {
    double sn {0};
    for (int i {0}; i != 3; ++i) {
        sn += std::pow(v[i], 2);
    }

    return sn;
}

inline Vector3 round(const Vector3 v) {
    Vector3 vo {};
    for (int i {0}; i != 3; ++i) {
        vo[i] = std::round(v[i]);
    }

    return vo;
}

inline Vector3 floor(const Vector3 v) {
    Vector3 vo {};
    for (int i {0}; i != 3; ++i) {
        vo[i] = std::floor(v[i]);
    }

    return vo;
}

class SimpleCubicPBC {
  public:
    SimpleCubicPBC(double boxl): boxl {boxl}, m_iboxl {1 / boxl} {};

    inline Vector3 calc_diff(Vector3 v1, Vector3 v2) {
        //Vector3 vo {v1 - v2};
        //Vector3 voo {vo};
        Vector3 vo {};
        vo[0] = v1[0] - v2[0];
        vo[1] = v1[1] - v2[1];
        vo[2] = v1[2] - v2[2];

        vo[0] -= boxl*std::round(vo[0]*m_iboxl);
        vo[1] -= boxl*std::round(vo[1]*m_iboxl);
        vo[2] -= boxl*std::round(vo[2]*m_iboxl);
        //vo *= m_iboxl;
        //vo = round(vo);
        //vo *= boxl;
        // check this works
        //vo -= boxl * round((vo)*m_iboxl);
        return vo;
    }

    // check this works
    inline void wrap(Vector3& v) { v -= boxl * floor(v * m_iboxl); }

    const double boxl;

    //mod
  //private:
    const double m_iboxl;
};

} // namespace LJMD

#endif // INTEGRATOR_HPP
