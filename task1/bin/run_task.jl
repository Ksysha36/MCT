import Pkg
Pkg.activate(dirname(@__DIR__))

using Laplace1D
using Printf

# Точное решение и правая часть (ваш вариант)
u_exact(x) = sin(4x) * cos(3x)
f(x) = 25*sin(4x)*cos(3x) + 24*cos(4x)*sin(3x)
a = u_exact(0.0)
b = u_exact(1.0)

function main()
    print("Введите натуральное число N (количество интервалов): ")
    N = parse(Int, readline())
    h = 1.0 / N

    x, y = solve_poisson(f, a, b, N)
    u_ex = u_exact.(x)

    errC = C_norm(u_ex, y)
    errL2 = L2_norm(u_ex, y, h)

    println("\nРезультаты для N = $N:")
    println("C-норма ошибки:  ", errC)
    println("L2-норма ошибки: ", errL2)
end

main()