#import "../config.typ": env, mathbf

= Interpolation

== Polynomial Interpolation

=== Lagrange formula

#env("Definition")[
  To interpolate given points $(x_0, f(x_0)), dots, (x_n, f(x_n))$, the Lagrange formula is

  $ p_n (x) = sum_(i=0)^n f(x_i) l_i (x), $

  where the *elementary Lagrange interpolation polynomial* (or *fundamental polynomial*) for pointwise interpolation $l_k (x)$ is

  $ l_k (x) = product_(i=0)^n (x - x_i)/(x_k - x_i). $

  In particular, for $n = 0, l_0 (x) = 1$.
]

=== Newton formula

#env("Definition")[
  The $k$th divided difference ($k in NN^+$) on the *table of divided differences*

  #align(center)[
    #table(
      columns: (auto, auto, auto, auto, auto),
      stroke: none,
      align: center + horizon,
      $x_0$,table.vline(), $f[x_0]$, "", "", "",
      $x_1$,table.vline(), $f[x_1]$, $f[x_0, x_1]$, "", "",
      $x_2$,table.vline(), $f[x_2]$, $f[x_1, x_2]$, $f[x_0, x_1, x_2]$, "",
      $x_3$,table.vline(), $f[x_3]$, $f[x_2, x_3]$, $f[x_1, x_2, x_3]$, $f[x_0, x_1, x_2, x_3]$,
      $dots.c$, $dots.c$, $dots.c$, $dots.c$, $dots.c$,
    )
  ]

  where the *divided differences* satisfy

  $ & f[x_0] & & =  f(x_0), \
    & f[x_0, dots, x_k] & & = (f[x_1, dots, x_k] - f[x_0, dots, x_{k-1}]) / (x_k - x_0). $
]

#env("Corollary")[
  Suppose $(i_0, dots, i_k)$ is a permutation of $(0, dots, k)$. Then

  $ f[x_0, dots, x_k] = f[x_(i_0), dots, x_(i_k)]. $
]

#env("Theorem")[
  For distinct points $x_0, dots, x_n$ and $x$, we have

  $ f(x) = f[x_0] + f[x_0, x_1] (x - x_0) + dots.c + f[x_0, dots, x_n] product_(i=0)^(n-1) (x - x_i) + f[x_0, dots, x_n, x] product_(i=0)^n (x - x_i). $
]

#env("Definition")[
  The *Newton formula* for interpolating the points $(x_0, f(x_0)), dots, (x_n, f(x_n))$ is

  $ p_n (x) = f[x_0] + f[x_0, x_1] (x - x_0) + dots.c + f[x_0, dots, x_n] product_(i=0)^(n-1) (x - x_i). $
]

=== Neville-Aitken algorithm

#env("Definition")[
  Denote $p_0^([i])(x) = f(x_i)$ for $i = 0, dots, n$. For all $k = 0, dots, n-1$ and $i = 0, dots, n - k - 1$, define

  $ p_(k+1)^([i]) (x) = ((x - x_i) p_(k)^([i+1]) (x) - (x - x_(x+k+1)) p_(k)^([i]) (x)) / (x_(i+k+1) - x_i). $

  Then each $p_(k)^([i]) (x)$ is the interpolating polynomial for the function $f$ at the points $x_i, dots, x_{i+k}$. In particular, $p_(n)^([0]) (x)$ is the interpolating polynomial of degree $n$ for the function $f$ at the points $x_0, dots, x_n$.
]

=== Hermite interpolation

#env("Definition")[
  Given distinct points $x_0, dots, x_k$ in $[a, b]$, non-negative integers $m_0, dots, m_k$, and a function $f in C^M [a, b]$ where $M = limits(max)_(i = 0, dots, k) (m_i)$, the *Hermite interpolation problem* seeks a polynomial $p(x)$ of the lowest degree satisfies

  $ forall i in {0, dots, k}, forall mu in {0, dots, m_i}, p^((mu)) (x_i) = f^((mu)) (x_i). $
]

#env("Definition", name: "Generalized divided difference")[
  Let $x_0, dots, x_k$ be $k + 1$ pairwise distinct points with each $x_i$ repeated $m_i + 1$ times; write $N = k + sum_(i=0)^k m_i$. The $N$th divided difference associated with these points is the cofficient of $x^N$ in the polynomial p that uniquely solves the Hermite interpolation problem.
]

#env("Corollary")[
  The $n$th divided difference at $n + 1$ "confluent" (i.e. identical) points is

  $ f[x_0, dots, x_0] = 1 / (n!) f^((n)) (x_0), $

  where $x_0$ is repeated $n + 1$ times on the left-hand side.
]

=== Approximation

#env("Definition")[
  Given condition functions $c_0, dots, c_k: PP_n -> RR^+$, the *Approximation problem* seeks a polynomial $p_n (x)$ of the given degree $n$ satisfies a unconstrained optimization

  $ min_(p_n in PP_n) sum_(i=0)^k c_i (p_n^((m_i)) ). $

  where condition function $c(p)$ includes but is not limited to

  $ |p^((m)) (x)|, ( p_n^((m))(x))^2, integral_a^b |p^((m))| upright("d") x, integral_a^b (p^((m)))^2 upright("d") x. $
]

#env("Example")[
  For non-negative integers $m_0, dots, m_k$ and condition functions $c_i (p_n) = (p_n^((m_i)) (x))^2$, denote by

  $ p_n (x) = sum_(i=0)^n c_i x^i $

  the polynomial of the given degree $n$, then the $m$th derivative of $p_n$ is

  $ p_n^((m)) (x) = sum_(i=m)^n (i!) / ((i-m)!) c_i x^(i-m). $

  All above implies the least squares system

  $ cases(p_n^((m_0)) (x) = & sum_(i=m_0)^n (i!) / ((i-m_0)!) c_i x^(i - m_0) = 0\,,
    & dots.c,
    p_n^((m_k)) (x) = & sum_(i=m_k)^n (i!) / ((i-m_k)!) c_i x^(i - m_k) = 0\,,) $

  which can be solved by algorithms such as Householder transformation.
]

=== Error analysis

#env("Theorem")[
  Let $f in C^n [a, b]$ and suppose that $f^((n+1)) (x)$ exists at each point of $(a, b)$. Let $p_n (x) in PP_n$ denote the unique polynomial that coincides with f at $x_0, dots, x_n$. Define

  $ R_n (f; x) = f(x) - p_n (x), $

  as the *Cauchy remainder* of the polynomial interpolation.

  If $a <= x_0 < dots.c < x_n <= b$, then there exists some $xi in (a, b)$ satisfies

  $ R_n (f; x) = (f^{(n+1)}(\xi))  / ((n+1)!) product_(i=0)^n (x - x_i) $

  where the value of $xi$ depends on $x, x_0, dots, x_n$ and $f$.
]

#env("Theorem")[
  For the Hermite interpolation problem, denote $N = k + sum_(i=0)^k m_i$. Denote by $p_N (x) in PP_N$ the unique solution of the problem. Suppose $f^((N+1)) (x)$ exists in $(a, b)$. Then there exists some $xi in (a, b)$ satisfies

  $ R_N (f; x) = (f^((N+1))(xi)) / ((N+1)!) product_(i=0)^k (x - x_i)^(m_i+1). $
]

== Spline

#env("Definition")[
  Given nonnegative integers $n$, $k$, and a strictly increasing sequence $a = x_1 < dots.c < x_N = b$, the set of *spline* functions of degree $n$ and smoothness class $k$ relative to the partition ${ x_i }$ is

  $ SS_n^k = { s: s in C^k [a, b]; forall i in {1, dots, N-1}, s |_([x_i, x_(i+1)]) in PP_n }, $

  where $x_i$ is the *knot* of the spline.
]

=== Cubic spline

#env("Definition", name: "Boundary conditions of splines")[
  The followings are common boundary conditions of cubic splines.

  - The *complete cubic spline* $s$ satisfies $s^prime (a) = f^prime (a), s^prime (b) = f^prime (b)$;
  - The *cubic spline with specified second derivatives* $s$ satisfies $s^prime.double (a) = f^prime.double (a), s^prime.double (b) = f^prime.double (b)$;
  - The *natural cubic spline* $s$  satisfies $s^prime.double (a) = s^prime.double (b) = 0$;
  - The *not-a-knot cubic spline* $s$ satisfies $s^prime.triple (x)$ exists at $x = x_2$ and $x = x_(N-1)$.
  - The *periodic cubic spline* $s$ satisfies $s(a) = s(b), s^prime (a) = s^prime (b), s^prime.double (a) = s^prime.double (b)$.
]

#env("Theorem")[
  Denote $m_i = s^prime (x_i), M_i = s^prime.double (x_i)$ for $s in SS^2_3$, then


  $ & forall i = 2, 3, dots, N-1, #h(1em) &  & lambda_i m_(i-1) + 2 m_i + mu_i m_i+1 = 3 mu_i f[x_i, x_(i+1)] + 3 lambda_i f[x_(i-1), x_i], \
    & forall i = 2, 3, dots, N-1, #h(1em) &  & mu_i M_(i-1) + 2 M_i + lambda_i m_(i+1) = 6 f[x_(i-1), x_i, x_(i+1)], $

  where

  $ mu_i = (x_i - x_(i-1)) / (x_(i+1) - x_(i-1)), #h(1em) lambda_i = (x_(i+1) - x_i) / (x_(i+1) - x_(i-1)). $

  In particular, $m_i$ and $M_i$ should be replaced to the derivatives given at the boundary.
]

#env("Theorem")[
  Cubic spline $s in SS^2_3$ from the linear system of $lambda_i, mu_i, m_i, M_i$ and the boundary conditions.
]

=== B-spline

#env("Definition")[
  *B-splines* are defined recursively by

  $ B_i^(n+1) (x) = (x - x_(i-1))(x_(i+n) - x_(i-1)) B_i^n (x) + (x_(i+n+1) - x) / (x_(i+n+1) - x_(i-1)) B_(i+1)^n (x), $

  where recursion base is the B-spline of degree zero

  $ B^0_i (x) = cases(
            1\, & #h(1em) x in (x_(i-1), x_i]\,,
            0\, & #h(1em) "otherwise".) $
]

#env("Theorem")[
    The ${ B_i^n (x) }$ forms a basis of $SS^(n-1)_n$.
]

#env("Definition")[
    For $N in NN^*$, the *support* of a $B_i^n (x)$ is

    $ upright("supp") {B_i^n (x)} = overline({ x in RR: B_i^n (x) eq.not 0 }) = [x_(i-1), x_(i+n)]. $
]

#env("Theorem", name: "Integrals of B-splines")[
    The average of a B-spline over its support only depends on its degree,

    $ 1 / (t_(i+n) - t_(i-1)) integral_(t_(i-1))^(t_(i+n)) B_i^n (x) upright("d") x = 1/(n+1). $
]

#env("Theorem", name: "Derivatives of B-splines")[
    For $n >= 2$, we have

    $ forall x in RR, #h(1em) upright("d") / (upright("d") x) B_i^n (x) = (n B_i^(n-1) (x)) / (x_(i+n-1) - x_(i-1)) - (n B_(i+1)^(n-1)(x)) / (x_(i+n) - x_i). $

    For $n = 1$, it holds for all $x$ except $x_(i-1), t_i, t_(i+1)$, where the derivative of $B_i^1 (x)$ is not defined.
]

=== Error analysis

#env("Theorem")[
    Suppose a function $f in C^4 [a, b]$, is interpolated by a complete cubic spline or a cubic spline with specified second derivatives at its end points. Then

    $ forall m = 0, 1, 2, |f^((m)) (x) - s^((m)) (x)| <= c_m h^(4 - m) max_(x in [a, b]) |f^((4)) (x)|, $

    where $c_0 = 1 / 16, c_1 = c_2 = 1 / 2$ and $h = limits(max)_(i = 1, dots, N-1) |x_(i+1) - x_i|$.
]