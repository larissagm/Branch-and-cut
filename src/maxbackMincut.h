#ifndef MAXBACKMINCUT_H
#define MAXBACKMINCUT_H

#include <stdlib.h>
#include <vector>

std::vector<std::vector<int>> maxback(std::vector<std::vector<double> > x, int dim);
std::vector<std::vector<int>> mincut(std::vector<std::vector<double> > x, int dim);

#endif