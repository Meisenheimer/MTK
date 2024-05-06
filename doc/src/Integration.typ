#import "../config.typ": env, mathbf

= Integration

#env("Definition")[
  A *weighted quadrature formula* $I_n (f)$ is a linear function

  $ I_n (f) = sum_(i=1)^n w_i f(x_i), $

  which approximates the integral of a function $f in C[a, b]$,

  $ I(f) = integral_a^b rho(x) f(x) upright("d") x, $

  where the weight function $rho in [a, b]$ satisfies $forall x in (a, b)$, $rho(x) > 0$. The points ${ x_i }$ at which the integrand $f$ is evaluated are called nodes or abscissas, and the multipliers ${ w_i }$ are called weights or coefficients.
]

#env("Definition")[
  A weighted quadrature formula has (polynomial) *degree of exactness* $d_E$ iff

  $ & forall f in PP_(d_E), & & E_n (f) = 0, \
    & exists g in PP_(d_E + 1), & & " s.t. " E_n (g) eq.not 0 $

  where $PP_d$ denotes the set of polynomials with degree no more than $d$.
]

#env("Theorem")[
  A weighted quadrature formula $I_n (f)$ satisfies $d_E <= 2 n - 1$.
]

#env("Definition")[
  The *error* or *remainder* of $I_n (f)$ is

  $ E_n (f) = I(f) - I_n (f), $

  where $I_n (f)$ is said to be convergent for $C[a, b]$ iff

  $ forall f in C[a, b], lim_(n -> +infinity) E_n (f) = 0. $
]

#env("Theorem")[
  Let $x_1, dots, x_n$ be given as distinct nodes of $I_n (f)$. If $d_E >= n - 1$, then its weights can be deduced as

  $ forall k in { 1 , dots, n }, w_k = integral_a^b rho(x) l_k (x) upright("d") x, $

  where $l_k (x)$ is the elementary Lagrange interpolation polynomial for pointwise interpolation applied to the given nodes.
]

== Newton-Cotes Formulas

#env("Definition")[
  A *Newton-Cotes formula* is a formula based on approximating $f(x)$ by interpolating it on uniformly spaced nodes $x_1, dots, x_n in [a, b]$.
]

=== Midpoint rule

#env("Definition")[
  The *midpoint rule* is a formula based on approximating $f(x)$ by the constant $f((a+b)/2)$.

  For $rho(x) equiv 1$, it is simply

  $ I_M (f) = (b - a) f((a+b)/2). $
]

#env("Theorem")[
  For $f in C^2[a, b]$, with weight functino $rho equiv 1$, the error (remainder) of midpoint rule satisfies

  $ exists xi in [a, b], " s.t. " E_M (f) = ((b - a)^3) / 24 f^(prime.double) (xi). $
]

#env("Corollary")[
  The midpoint rule has $d_E = 1$.
]

=== Trapezoidal rule

#env("Definition")[
  The *trapezoidal rule* is a formula based on approximating $f(x)$ by the straight line that connects $(a, f(a))$ and $(b, f(b))$.

  For $rho(x) equiv 1$, it is simply

  $ I_T (f) = (b-a)/2 (f(a) + f(b)). $
]

#env("Theorem")[
  For $f in C^2[a, b]$, with weight functino $rho equiv 1$, the error (remainder) of trapezoidal rule satisfies

  $ exists xi in [a, b], " s.t. " E_T (f) = -((b - a)^3)/12 f^(prime.double) (xi). $
]

#env("Corollary")[
  The trapezoidal rule has $d_E = 1$.
]

=== Simpson's rule

#env("Definition")[
  The *Simpson's rule* is a formula based on approximating $f(x)$ by the quadratic polynomial that goes through the points $(a, f(a))$, $((a + b)/2, f((a + b)/2))$ and $(b, f(b))$.

  For $rho(x) equiv 1$, it is simply

  $ I_S (f) = (b - a)/6 (f(a) + 4 f((a + b)/2) + f(b)). $
]

#env("Theorem")[
  For $f in C^4[a, b]$, with weight functino $rho equiv 1$, the error (remainder) of Simpson's rule satisfies

  $ exists xi in [a, b], " s.t. " E_T (f) = -((b - a)^5)/2880 f^((4)) (xi). $
]

#env("Corollary")[
  The Simpson's rule has $d_E = 3$.
]

== Gauss Formulas

#env("Theorem")[
  For an interval $[a, b]$ and a weight function $rho: [a, b] -> RR$, the nodes for gauss formula $I_n (f)$ is the root of the $n$th order orthogonal polynomial on $[a, b]$ with the weight function $rho(x)$.
]

#env("Theorem")[
  A Gauss formula $I_n (f)$ has $d_E = 2 n - 1$.
]
