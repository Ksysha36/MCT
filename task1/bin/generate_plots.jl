import Pkg
Pkg.activate(dirname(@__DIR__))

using Laplace1D
using Plots

# Ваше точное решение и правая часть
u_exact(x) = sin(4x) * cos(3x)
f(x) = 25*sin(4x)*cos(3x) + 24*cos(4x)*sin(3x)
a = u_exact(0.0)
b = u_exact(1.0)

function main()
    # 1. Сравнение при N=50
    N_plot = 50
    x_plot, y_num = solve_poisson(f, a, b, N_plot)
    y_exact = u_exact.(x_plot)

    p1 = plot(x_plot, y_exact, label="Аналитическое", linewidth=2, color=:blue)
    scatter!(p1, x_plot, y_num, label="Численное", markersize=4, color=:red, markerstrokewidth=0)
    title!(p1, "Решение (N = $N_plot)")
    xlabel!(p1, "x")
    ylabel!(p1, "u(x)")

    # 2. Сходимость – используем N (как в вашем примере)
    N_values = [10, 20, 40, 80, 160, 320]
    errors_C = Float64[]
    errors_L2 = Float64[]

    for N in N_values
        h = 1.0 / N
        x, y = solve_poisson(f, a, b, N)
        u_ex = u_exact.(x)
        push!(errors_C, C_norm(u_ex, y))
        push!(errors_L2, L2_norm(u_ex, y, h))
    end

    # Эталонная линия O(N^{-2}) – масштабируем под первую точку
    ref_O2 = [errors_C[1] * (N_values[1] / N)^2 for N in N_values]

    p2 = plot(N_values, errors_C, label="C-норма", marker=:circle, linewidth=2,
              xscale=:log10, yscale=:log10)
    plot!(p2, N_values, errors_L2, label="L2-норма", marker=:square, linewidth=2)
    plot!(p2, N_values, ref_O2, label="O(N^{-2}) эталон", linestyle=:dash, color=:black)
    title!(p2, "Сходимость метода")
    xlabel!(p2, "Число узлов N")
    ylabel!(p2, "Ошибка")

    final_plot = plot(p1, p2, layout=(1, 2), size=(900, 450))
    savefig(final_plot, "plots.png")
    println("График сохранён в plots.png")
end

main()