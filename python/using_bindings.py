"""Example usage of the options_pricing_py bindings."""
import sys
sys.path.insert(0, 'build/Release')
import options_pricing_py as op

x = op.Variable(3.0)
y = op.Variable(4.0)

z = op.sin(x + y) * op.exp(x / y) * op.cos(x * y)
print("nodes on tape:", op.tape_nodes_count())

# seed gradient
z.set_adjoint(1.0)
# run backward
op.tape_execute_backpass()

print("dz/dx:", x.adjoint())
print("dz/dy:", y.adjoint())
