import numpy as np
import math

S_0 = 100 # Current Stock Price
K = 100 # Strike price
N = 1000000 # Number of samples
r = 0.05 # Risk free interest rate
sigma = 0.2 # Volatility
T = 1 # days until strike

rng = np.random.default_rng()

sum = 0

for i in range(N):
    sample = rng.normal()

    #Compute ST (price of the stock at time T given the sample)
    ST = S_0 * math.exp((r - (0.5*pow(sigma, 2))*T + sigma*math.sqrt(T)*sample))
    profit = max(ST - K, 0)
    sum += profit

estimated_profit = sum/N

discounted_price = math.exp(-1 * r * T) * estimated_profit

print("Price of the European call: " + str(discounted_price))