import sys
import math
import numpy as np

sys.path.insert(0, "build/Release")
import options_pricing_py as op

# -----------------------
# Parameters (as floats)
# -----------------------
S0_f = 100.0
K_f = 100.0
r_f = 0.05
sigma_f = 0.2
T_f = 1.0
N = 200_000

rng = np.random.default_rng()

# Precompute sqrt(T) as float, then wrap into Variable (until op.sqrt exists)
sqrtT_f = math.sqrt(T_f)

payoff_sum = 0.0
delta_sum = 0.0

for _ in range(N):
    # IMPORTANT: clear the graph each path
    op.tape_reset()

    # Random sample as Variable too (so you can differentiate wrt it if you want)
    Z = op.Variable(float(rng.normal()))

    # Wrap every parameter as Variable
    S0 = op.Variable(S0_f)
    K = op.Variable(K_f)
    r = op.Variable(r_f)
    sigma = op.Variable(sigma_f)
    T = op.Variable(T_f)
    sqrtT = op.Variable(sqrtT_f)

    half = op.Variable(0.5)
    one = op.Variable(1.0)

    # drift = (r - 0.5*sigma^2) * T
    sigma2 = sigma * sigma
    drift = (r - half * sigma2) * T

    # diffusion = sigma * sqrt(T) * Z
    diffusion = sigma * sqrtT * Z

    # ST = S0 * exp(drift + diffusion)
    ST = S0 * op.exp(drift + diffusion)

    # payoff = max(ST - K, 0)  (max not overloaded, so do branch on value)
    intrinsic_val = ST.value() - K.value()
    payoff_val = intrinsic_val if intrinsic_val > 0.0 else 0.0

    payoff_sum += payoff_val

    # Pathwise Delta:
    # If in-the-money, d payoff / d ST = 1; else 0
    if intrinsic_val > 0.0:
        ST.set_adjoint(1.0)          # seed dpayoff/dST
        op.tape_execute_backpass()   # propagate to S0
        delta_sum += S0.adjoint()

# Discounting (still scalar)
disc = math.exp(-r_f * T_f)

price = disc * (payoff_sum / N)
delta = disc * (delta_sum / N)

print("European call price:", price)
print("Delta:", delta)
print("Delta (finite diff check):", (price + 0.01 * delta - disc * (payoff_sum / N)) / 0.01)
