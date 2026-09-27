#include "Derivative.hpp"

double derivative(const Function& f, double t, double h)
{
    return (f(t + h) - f(t - h)) / (2.0f * h);
}

double second_derivative(const Function& f, double t, double h)
{
    return (f(t + h) - 2.0f * f(t) + f(t - h)) / (h * h);
}
