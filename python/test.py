import sys
import math

# Adjust this path if needed
sys.path.insert(0, "build/Release")
import options_pricing_py as op


def expr_py(x, y):
    """Pure-Python value of the same expression (no AD, no tape)."""
    return math.sin(x + y) * math.exp(x / y) * math.cos(x * y)


def expr_ad(x0, y0):
    """Build expression once in AD and return value + adjoints."""
    x = op.Variable(float(x0))
    y = op.Variable(float(y0))

    z = op.sin(x + y) * op.exp(x / y) * op.cos(x * y)

    # seed gradient
    z.set_adjoint(1.0)
    # run backward
    op.tape_execute_backpass()

    # if you have z.value() exposed you can use it; otherwise use python eval for value
    val = expr_py(x0, y0)

    return val, float(x.adjoint()), float(y.adjoint())


def central_diff(x0, y0, eps=1e-6):
    """Central differences on the pure-Python expression."""
    fxp = expr_py(x0 + eps, y0)
    fxm = expr_py(x0 - eps, y0)
    dfdx = (fxp - fxm) / (2.0 * eps)

    fyp = expr_py(x0, y0 + eps)
    fym = expr_py(x0, y0 - eps)
    dfdy = (fyp - fym) / (2.0 * eps)

    return dfdx, dfdy


def rel_err(a, b):
    denom = max(1.0, abs(a), abs(b))
    return abs(a - b) / denom


def main():
    x0, y0 = 3.0, 4.0

    print("nodes on tape:", op.tape_nodes_count())

    val, ad_dx, ad_dy = expr_ad(x0, y0)
    fd_dx, fd_dy = central_diff(x0, y0, eps=1e-6)

    print(f"z(x,y)         : {val:.12g}")
    print(f"AD   dz/dx     : {ad_dx:.12g}")
    print(f"FD   dz/dx     : {fd_dx:.12g}")
    print(f"rel err dz/dx  : {rel_err(ad_dx, fd_dx):.3e}")
    print()
    print(f"AD   dz/dy     : {ad_dy:.12g}")
    print(f"FD   dz/dy     : {fd_dy:.12g}")
    print(f"rel err dz/dy  : {rel_err(ad_dy, fd_dy):.3e}")

    # Set tolerances (central diff at 1e-6 should be pretty good for this function)
    tol = 1e-6
    ok_x = rel_err(ad_dx, fd_dx) < tol
    ok_y = rel_err(ad_dy, fd_dy) < tol

    if not (ok_x and ok_y):
        raise AssertionError(
            f"Gradient check failed: ok_x={ok_x}, ok_y={ok_y}, tol={tol}"
        )

    print("\nPASS: AD gradients match finite differences within tolerance.")


if __name__ == "__main__":
    main()
