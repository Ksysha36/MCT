function C_norm(true_sol, num_sol)
    maximum(abs.(true_sol .- num_sol))
end

function L2_norm(true_sol, num_sol, h)
    sqrt(h * sum((true_sol .- num_sol).^2))
end