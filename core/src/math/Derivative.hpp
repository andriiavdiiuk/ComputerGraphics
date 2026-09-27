#pragma once

#include <functional>

using Function = std::function<double(double)>;

double derivative(const Function& f, double t, double h = 1e-5);

double second_derivative(const Function& f, double t, double h = 1e-5);

