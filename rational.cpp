#include "rational.h"

#include <numeric>
#include <climits>

// Constructors
rational::rational() {}

rational::rational(int num) : nom(num) {}

rational::rational(long long int nom_, long long int denom_) : nom(nom_), denom(denom_) { normalize(); }

rational::rational(long long int num) : nom(num) {}

rational::rational(double num, int precision, bool apply_precision)
{
    if (apply_precision && precision != 0) {
        nom = static_cast<long long int>(num * (precision * 10));
        denom = precision * 10;
    } else {
        nom = static_cast<long long int>(num);
        denom = 1;
    }
    normalize();
}



void rational::normalize()
{
    long long int gcd = std::gcd(nom, denom);
    nom /= gcd;
    denom /= gcd;
    if (denom < 0) {
        nom = -nom;
        denom = -denom;
    }
}

void rational::convert_to_common_denom(rational& other)
{
    long long int common_denom = this->denom;
    if (common_denom != other.denom)
        common_denom *= other.denom;

    this->nom *= (common_denom / this->denom);
    other.nom *= (common_denom / other.denom);

    this->denom = common_denom;
    other.denom = common_denom;
}




// operators
rational rational::operator+(const rational& other) const
{
    rational rat1 = *this;
    rational rat2 = other;

    rat1.convert_to_common_denom(rat2);

    rat1.nom += rat2.nom;
    rat1.normalize();
    return rat1;
}

rational rational::operator-(const rational& other) const
{
    rational rat1 = *this;
    rational rat2 = other;

    rat1.convert_to_common_denom(rat2);

    rat1.nom -= rat2.nom;
    rat1.normalize();
    return rat1;
}

rational rational::operator*(const rational& other) const
{
    rational result;
    result.nom = this->nom * other.nom;
    result.denom = this->denom * other.denom;
    result.normalize();

    return result;
}

rational rational::operator/(const rational& other) const
{
    rational result;
    if (other == rational(0))
        throw std::runtime_error("Division by zero");
    result.nom = this->nom * other.denom;
    result.denom = this->denom * other.nom;
    result.normalize();

    return result;
}


bool rational::operator==(const rational& other) const
{
    return nom == other.nom && denom == other.denom;
}


rational rational::operator+(const long long int other) const
{
    rational result = rational(other * this->denom + this->nom, this->denom);
    result.normalize();
    return result;
}
rational rational::operator-(const long long int other) const
{
    rational result = rational(this->nom - (other * this->denom), this->denom);
    result.normalize();
    return result;
}
rational rational::operator/(const long long int other) const
{
    rational result = rational(this->nom, this->denom * other);
    result.normalize();
    return result;
}
rational rational::operator*(const long long int other) const
{
    rational result = rational(this->nom * other, this->denom);
    result.normalize();
    return result;
}

rational& rational::operator+=(long long int other) { *this = *this + other; return *this; }
rational& rational::operator-=(long long int other) { *this = *this - other; return *this; }
rational& rational::operator/=(long long int other) { *this = *this / other; return *this; }
rational& rational::operator*=(long long int other) { *this = *this * other; return *this; }

rational rational::operator-() const
{
    return {-nom, denom};
}

rational& rational::pow(const long long int power)
{
    if (power == 0) {
        nom = 1;
        denom = 1;
    } else if (power < 0) {
        long long int tmp = std::pow(denom, -power);
        denom = std::pow(nom, -power);
        nom = tmp;
    } else {
        nom = std::pow(nom, power);
        denom = std::pow(denom, power);
    }
    normalize();
    return *this;
}

std::ostream& operator<<(std::ostream& os, rational rat)
{
    os << rat.nom;
    if (rat.denom != 1) {
        os << "/";
        os << rat.denom;
    }
    return os;
}

rational::operator QString() const
{
    QString str;
    str +=  QString::number(nom);
    if (denom != 1) {
        str += "/" + QString::number(denom);
    }
    return str;
}

rational::operator double() const
{
    return static_cast<double>(nom) / static_cast<double>(denom);
}

rational::operator long long int() const
{
    return nom;
}



