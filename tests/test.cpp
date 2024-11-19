#include "../expr_parser.h"

#include <catch2/catch_test_macros.hpp>

TEST_CASE("Rational constructors", "[rational]")
{
    // Default constructor
    REQUIRE(rational() == rational(0, 1));

    // Constructor with integer input
    REQUIRE(rational(5) == rational(5, 1));
    REQUIRE(rational(-3) == rational(-3, 1));

    // Constructor with numerator and denominator
    REQUIRE(rational(4, 2) == rational(2, 1));
    REQUIRE(rational(-6, 3) == rational(-2, 1));
    REQUIRE(rational(0, 5) == rational(0, 1));

    // Constructor with double input
    REQUIRE(rational(0.5, 1, true) == rational(1, 2));
    REQUIRE(rational(-1.25, 2, true) == rational(-5, 4));
    REQUIRE(rational(2.0, 0, true) == rational(2, 1));

}

TEST_CASE("Binary operators", "[rational]")
{
    // Addition of two rationals
    REQUIRE(rational(1, 2) + rational(1, 3) == rational(5, 6));
    REQUIRE(rational(-1, 2) + rational(1, 2) == rational(0, 1));
    REQUIRE(rational(3, 4) + rational(-3, 4) == rational(0, 1));

    // Subtraction of two rationals
    REQUIRE(rational(3, 4) - rational(1, 4) == rational(1, 2));
    REQUIRE(rational(-1, 2) - rational(1, 2) == rational(-1, 1));
    REQUIRE(rational(3, 4) - rational(3, 4) == rational(0, 1));

    // Multiplication of two rationals
    REQUIRE(rational(2, 3) * rational(3, 4) == rational(1, 2));
    REQUIRE(rational(-2, 3) * rational(3, 4) == rational(-1, 2));
    REQUIRE(rational(0, 1) * rational(5, 7) == rational(0, 1));

    // Division of two rationals
    REQUIRE(rational(2, 3) / rational(4, 5) == rational(5, 6));
    REQUIRE(rational(-2, 3) / rational(4, 5) == rational(-5, 6));
    REQUIRE(rational(3, 4) / rational(3, 4) == rational(1, 1));

    // Power of rational to int
    REQUIRE(rational(2).pow(2) == rational(4));
    REQUIRE(rational(-2).pow(2) == rational(4));
    REQUIRE(rational(-2).pow(3) == rational(-8));
    REQUIRE(rational(80085).pow(0) == rational(1));
    REQUIRE(rational(-80085).pow(0) == rational(1));
    REQUIRE(rational(2, 3).pow(0) == rational(1));
    REQUIRE(rational(2, 3).pow(3) == rational(8, 27));
    REQUIRE(rational(2, 3).pow(-3) == rational(27, 8));
}

TEST_CASE("Unary minus and integer operations", "[rational]")
{
    // Unary minus
    REQUIRE(-rational(1, 2) == rational(-1, 2));
    REQUIRE(-rational(-3, 4) == rational(3, 4));
    REQUIRE(-rational(0, 1) == rational(0, 1));

    // Addition with long long integer
    REQUIRE(rational(3, 4) + 1ll == rational(7, 4));
    REQUIRE(rational(1, 2) + (-1ll) == rational(-1, 2));
    REQUIRE(rational(0, 1) + 5ll == rational(5, 1));

    // Subtraction with integer
    REQUIRE(rational(3, 4) - 1ll == rational(-1, 4));
    REQUIRE(rational(1, 2) - (-1ll) == rational(3, 2));
    REQUIRE(rational(0, 1) - 5ll == rational(-5, 1));

    // Multiplication with integer
    REQUIRE(rational(3, 4) * 2ll == rational(3, 2));
    REQUIRE(rational(-1, 2) * (-2ll) == rational(1, 1));
    REQUIRE(rational(0, 1) * 5ll == rational(0, 1));

    // Division with integer
    REQUIRE(rational(3, 4) / 2ll == rational(3, 8));
    REQUIRE(rational(-1, 2) / (-2ll) == rational(1, 4));
}

// Tesim se na Vanoce, doufam ze tam dodelam vsechny ukoly

TEST_CASE("Comparison operators", "[rational]")
{
    // Equality operator
    REQUIRE(rational(1, 2) == rational(1, 2));
    REQUIRE(rational(3, 4) == rational(6, 8));  // Equality with normalization
    REQUIRE(!(rational(1, 2) == rational(2, 3)));

    // Spaceship operator
    REQUIRE((rational(1, 2) <=> rational(1, 2)) == 0);  // Equal
    REQUIRE((rational(1, 2) <=> rational(2, 3)) < 0);   // Less than
    REQUIRE((rational(3, 2) <=> rational(1, 2)) > 0);   // Greater than

    // Comparison with integer
    REQUIRE(rational(2, 1) == rational(2));
    REQUIRE(rational(-3, 1) == rational(-3));

}


TEST_CASE("Basic parsing", "[parser]") {
    // Additions
    REQUIRE(calculate(QString("10+15")) == rational(25));
    REQUIRE(calculate(QString("7+3*4")) == rational(19));  // Testing operator priority
    REQUIRE(calculate(QString("0+0")) == rational(0));
    REQUIRE(calculate(QString("5+(-5)")) == rational(0));
    REQUIRE(calculate(QString("100+(-50)+(-25)")) == rational(25));

    // Basic subtraction
    REQUIRE(calculate(QString("20-15")) == rational(5));
    REQUIRE(calculate(QString("-10-(-20)")) == rational(10));
    REQUIRE(calculate(QString("3-(-7)")) == rational(10));
    REQUIRE(calculate(QString("15-5-5")) == rational(5));
    REQUIRE(calculate(QString("10-20+5")) == rational(-5));

    // Basic multiplication
    REQUIRE(calculate(QString("6*7")) == rational(42));
    REQUIRE(calculate(QString("(-8)*4")) == rational(-32));
    REQUIRE(calculate(QString("(-10)*(-10)")) == rational(100));
    REQUIRE(calculate(QString("(-5)*(-5)")) == rational(25));
    REQUIRE(calculate(QString("5*(6+(-4))")) == rational(10));

    // Basic division
    REQUIRE(calculate(QString("100/4")) == rational(25));
    REQUIRE(calculate(QString("20/(-4)")) == rational(-5));
    REQUIRE(calculate(QString("(-30)/(5+5)")) == rational(-3));
    REQUIRE(calculate(QString("(-55)/(-5)")) == rational(11));
    REQUIRE(calculate(QString("100/(25/5)")) == rational(20));

    // Unary minus and negative numbers
    REQUIRE(calculate(QString("-50")) == rational(-50));
    REQUIRE(calculate(QString("-100")) == rational(-100));
    REQUIRE(calculate(QString("(-8)+5")) == rational(-3));
    REQUIRE(calculate(QString("(-10)/2")) == rational(-5));
    REQUIRE(calculate(QString("(-8)*4")) == rational(-32));

    // Expressions with parentheses
    REQUIRE(calculate(QString("(7+3)*4")) == rational(40));
    REQUIRE(calculate(QString("((2+3)*(5-2))")) == rational(15));
    REQUIRE(calculate(QString("(8-3)*(-2)")) == rational(-10));
    REQUIRE(calculate(QString("50/(2+3)")) == rational(10));
    REQUIRE(calculate(QString("100/(2+3*4-4)")) == rational(10));

    // Mixed operations
    REQUIRE(calculate(QString("((-4)*(-5))+10")) == rational(30));
    REQUIRE(calculate(QString("(-3)*(3+4)")) == rational(-21));
}

TEST_CASE("Invalid parsing", "[parser]")
{
    expr_parser parser("");

    // Obviously invalid inputs
    REQUIRE_THROWS_AS(parser.switch_input("Nan"), expr_eval_error);
    REQUIRE_THROWS_AS(parser.switch_input("0+a"), expr_eval_error);
    REQUIRE_THROWS_AS(parser.switch_input("a+0"), expr_eval_error);
    REQUIRE_THROWS_AS(parser.switch_input("{}"), expr_eval_error);
    REQUIRE_THROWS_AS(parser.switch_input("5:8"), expr_eval_error);

    // Mismatched parentheses
    REQUIRE_THROWS_AS(calculate(QString("(5+3")), expr_eval_error);
    REQUIRE_THROWS_AS(calculate(QString("5+3)")), expr_eval_error);
    REQUIRE_THROWS_AS(calculate(QString("((5+3*2")), expr_eval_error);
    REQUIRE_THROWS_AS(calculate(QString("5+(3*2))")), expr_eval_error);

    // Operators without operands
    REQUIRE_THROWS_AS(calculate(QString("+")), expr_eval_error);
    REQUIRE_THROWS_AS(calculate(QString("-")), expr_eval_error);
    REQUIRE_THROWS_AS(calculate(QString("*")), expr_eval_error);
    REQUIRE_THROWS_AS(calculate(QString("/")), expr_eval_error);
    REQUIRE_THROWS_AS(calculate(QString("5+*3")), expr_eval_error);
    REQUIRE_THROWS_AS(calculate(QString("5*/3")), expr_eval_error);
    REQUIRE_THROWS_AS(calculate(QString("5/")), expr_eval_error);
    REQUIRE_THROWS_AS(calculate(QString("*5")), expr_eval_error);
}




