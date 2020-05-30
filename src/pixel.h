#ifndef PIXEL_H
#define PIXEL_H

#include <iostream>
#include <cmath>


// As a template class, it has to be written entirely in the header
template<typename T>
class Pxl
{
public:
    T r, g, b;


    // ============================================================
    // 	Constructors
    // ============================================================
    Pxl()
    {
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }
    template<typename T2>
    explicit Pxl(Pxl<T2> p)
        :r(p.r), g(p.g), b(p.b)
    {
    }
    Pxl(const T& r, const T& g, const T& b)
    {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    // ============================================================
    //	Utils
    // ============================================================

    Pxl<T> clamped()
    {
        return Pxl<T>(this->clamp_color(this->r), this->clamp_color(this->g), this->clamp_color(this->b));
    }
    
    double dst2(Pxl<int> const& op)
    {
        Pxl<int> df = (*this) - op;
        return df.r*df.r+df.g*df.g+df.b*df.b;
    }

    double dst(Pxl<int> const& op)
    {
        return sqrt(dst2(op));
    }

    template<typename T2>
    Pxl<T2> seen_as()
    {
        return Pxl<T2>(this->r, this->g, this->b);
    }

    // ============================================================
    // 	Operators
    // ============================================================
    Pxl<T> operator+(const Pxl<T>& other) const
    {
        return Pxl<T>(this->r + other.r, this->g + other.g, this->b + other.b);
    }
    template<typename T2>
    Pxl<T>& operator+=(const Pxl<T2>& other)
    {
        this->r += other.r;
        this->g += other.g;
        this->b += other.b;

        return *this;
    }

    // scal
    template<typename T2>
    Pxl<T> operator*(const T2& scal) const
    {
        return Pxl<T>(this->r * scal, this->g * scal, this->b * scal);
    }

    // selfscal
    template<typename T2>
    Pxl<T>& operator*=(const T2& scal)
    {
        this->r *= scal;
        this->g *= scal;
        this->b *= scal;

        return *this;
    }


    // Prod
    template<typename T2>
    Pxl<T> operator*(const Pxl<T2>& other) const
    {
        return Pxl<T>(this->r * other.r, this->g * other.g, this->b * other.b);
    }
    // SelfProd
    template<typename T2>
    Pxl<T>& operator*=(const Pxl<T2>& other) 
    {
        this->r *= other.r;
        this->g *= other.g;
        this->b *= other.b;

        return *this;
    }

    // Div
    template<typename T2>
    Pxl<T> operator/(const Pxl<T2>& other) const
    {
        T rr, rg, rb;
        rr = other.r > 0 ? this->r / other.r : 0;
        rg = other.g > 0 ? this->g / other.g : 0;
        rb = other.b > 0 ? this->b / other.b : 0;
        return Pxl<T>(rr, rg, rb);
    }
    // SelfProd
    template<typename T2>
    Pxl<T>& operator/=(const Pxl<T2>& other) 
    {
        this->r /= other.r;
        this->g /= other.g;
        this->b /= other.b;

        return *this;
    }


    // sub
    Pxl<T> operator-(const Pxl<T>& other) const
    {
        return Pxl<T>(this->r - other.r, this->g - other.g, this->b - other.b);
    }
    template<typename T2>
    Pxl<T>& operator-=(const Pxl<T2>& other)
    {
        this->r -= other.r;
        this->g -= other.g;
        this->b -= other.b;

        return *this;
    }

    // display
    friend std::ostream& operator<<(std::ostream& os, const Pxl<T>& pxl)
    {
        return os << "(" << pxl.r << ", " << pxl.g << ", " << pxl.b << ")";
    }

    // ============================================================
    // 	Destructor
    // ============================================================
    ~Pxl() = default;
private:
    T red, green, blue;

    T clamp_color(const T& color)
    {
        if(color < 0)
            return 0;
        if(color > 255)
            return 255;
        return color;
    }
};

#endif // PIXEL_H
