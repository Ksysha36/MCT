# Метод прогонки
function thomas(a_diag, b_diag, c_diag, d)
    n = length(d)
    cp = zeros(n)
    dp = zeros(n)
    y = zeros(n)

    cp[1] = c_diag[1] / b_diag[1]
    dp[1] = d[1] / b_diag[1]

    for i in 2:n
        denom = b_diag[i] - a_diag[i] * cp[i-1]
        cp[i] = c_diag[i] / denom
        dp[i] = (d[i] - a_diag[i] * dp[i-1]) / denom
    end

    y[n] = dp[n]
    for i in n-1:-1:1
        y[i] = dp[i] - cp[i] * y[i+1]
    end
    return y
end

# Решение уравнения -u'' = f на [0,1] с ГУ u(0)=a, u(1)=b
function solve_poisson(f, a, b, N)
    h = 1.0 / N
    x = range(0.0, 1.0, length=N+1)
    m = N - 1

    a_diag = zeros(m)
    b_diag = zeros(m)
    c_diag = zeros(m)
    d = zeros(m)

    for i in 1:m
        xi = x[i+1]
        d[i] = f(xi) * h^2
        b_diag[i] = 2.0

        if i > 1
            a_diag[i] = -1.0
        else
            d[i] += a
        end

        if i < m
            c_diag[i] = -1.0
        else
            d[i] += b
        end
    end

    y_inner = thomas(a_diag, b_diag, c_diag, d)
    y = zeros(N+1)
    y[1] = a
    y[N+1] = b
    y[2:end-1] = y_inner

    return x, y
end