#ifndef RATIONAL_H
#define RATIONAL_H

#include <cstdlib>
#include <iostream>
#include <QString>

class rational
{
private:
    long long int nom = 0;
    long long int denom = 1;
    void normalize();
    void convert_to_common_denom(rational& other);

public:
    rational();
    rational(long long int num);
    rational(int num);
    rational(long long int nom_, long long int denom_);
    rational(double num, int precision, bool apply_precision);

    rational(const rational& other) = default;
    rational(rational&& other) = default;
    rational& operator=(const rational& other) = default;
    rational& operator=(rational&& other) = default;


    rational operator+(const rational& other) const;
    rational operator-(const rational& other) const;
    rational operator/(const rational& other) const;
    rational operator*(const rational& other) const;

    // Likely just for debugging and testing
    auto operator<=>(const rational& other) const
    {
        if (auto cmp = denom <=> other.denom; cmp == 0)
            return nom <=> other.nom;
        rational r1 = *this;
        rational r2 = other;

        r1.convert_to_common_denom(r2);
        return r1.nom <=> r2.nom;
    }

    bool operator==(const rational& other) const;

    rational operator-() const;

    rational operator+(long long int other) const;
    rational operator-(long long int other) const;
    rational operator/(long long int other) const;
    rational operator*(long long int other) const;
    rational& operator+=(long long int other);
    rational& operator-=(long long int other);
    rational& operator/=(long long int other);
    rational& operator*=(long long int other);
    rational &pow(const long long int power);


    operator QString() const;
    operator double() const;
    operator long long int() const;

    friend std::ostream& operator<<(std::ostream& os, rational rat);

};

#endif // RATIONAL_H
