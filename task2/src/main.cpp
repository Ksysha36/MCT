#include "inmost.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <chrono>
#include <iomanip>

using namespace INMOST;

double u_exact(double x, double y) {
    return sin(4*x) * cos(3*x);   // не зависит от y
}

double f_rhs(double x, double y) {
    return 25*sin(4*x)*cos(3*x) + 24*cos(4*x)*sin(3*x);
}

double g_boundary(double x, double y) {
    return u_exact(x, y);  
}

// Индекс для внутреннего узла (i,j), i,j = 1..N-1
inline int idx(int i, int j, int N) {
    return (i-1) + (j-1) * (N-1);
}

int main() {
    std::vector<int> Ns = {10, 20, 40, 80, 160}; // число интервалов по стороне
    std::cout << std::scientific << std::setprecision(6);

    for (int N : Ns) {
        double h = 1.0 / N;
        int n_inner = N - 1;
        int n_unknowns = n_inner * n_inner;

        std::cout << "\n===== N = " << N << " (" << n_unknowns << " неизвестных) =====" << std::endl;

        Sparse::Matrix A;
        Sparse::Vector b;
        A.SetInterval(0, n_unknowns);
        b.SetInterval(0, n_unknowns);

        auto t_start = std::chrono::high_resolution_clock::now();

        // Заполнение матрицы и правой части
        for (int j = 1; j <= n_inner; ++j) {
            double y = j * h;
            for (int i = 1; i <= n_inner; ++i) {
                double x = i * h;
                int row = idx(i, j, N);

                A[row][row] += 4.0;
                b[row] = f_rhs(x, y) * h * h;

                // левый сосед (i-1, j)
                if (i > 1) {
                    A[row][idx(i-1, j, N)] += -1.0;
                } else {
                    b[row] += g_boundary(0.0, y);
                }
                
                if (i < n_inner) {
                    A[row][idx(i+1, j, N)] += -1.0;
                } else {
                    b[row] += g_boundary(1.0, y);
                }
                
                if (j > 1) {
                    A[row][idx(i, j-1, N)] += -1.0;
                } else {
                    b[row] += g_boundary(x, 0.0);
                }
                
                if (j < n_inner) {
                    A[row][idx(i, j+1, N)] += -1.0;
                } else {
                    b[row] += g_boundary(x, 1.0);
                }
            }
        }

        auto t_assem = std::chrono::high_resolution_clock::now();
        double assemble_time = std::chrono::duration<double>(t_assem - t_start).count();

        // Решение системы (CG + ILU2)
        Solver solver(Solver::INNER_ILU2);
        solver.SetMatrix(A);
        solver.SetParameter("absolute_tolerance", "1e-12");
        solver.SetParameter("relative_tolerance", "1e-10");

        Sparse::Vector solution;
        bool solved = solver.Solve(b, solution);
        double solve_time = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - t_assem).count();

        if (!solved) {
            std::cout << "Solver failed: " << solver.ReturnReason() << std::endl;
            return 1;
        }

        // Вычисление ошибок
        double errC = 0.0;
        double sum_sq = 0.0;
        for (int j = 1; j <= n_inner; ++j) {
            double y = j * h;
            for (int i = 1; i <= n_inner; ++i) {
                double x = i * h;
                int row = idx(i, j, N);
                double num = solution[row];
                double exact = u_exact(x, y);
                double diff = std::abs(num - exact);
                if (diff > errC) errC = diff;
                sum_sq += diff * diff;
            }
        }
        double errL2 = h * std::sqrt(sum_sq);  // для 2D: ∫ e^2 ≈ h^2 Σ e^2, тогда L2 = sqrt(h^2 Σ e^2) = h sqrt(Σ e^2)

        std::cout << "  h = " << h << "\n";
        std::cout << "  C-норма  = " << errC << "\n";
        std::cout << "  L2-норма = " << errL2 << "\n";
        std::cout << "  Время сборки: " << assemble_time << " с\n";
        std::cout << "  Время решения: " << solve_time << " с\n";
        std::cout << "  Итераций: " << solver.Iterations() << "\n";

        std::cout << "DATA: " << h << " " << errC << " " << errL2 << " " << assemble_time << " " << solve_time << std::endl;
    }

    return 0;
}