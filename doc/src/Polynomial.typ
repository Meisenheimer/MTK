#import "../config.typ": env, mathbf

= Polynomial

== Single Variable Polynomial

#env("Definition")[
  Denoted by $VV$ a linear space and $x$ the variable, a *(single variable) polynomial* over $VV$ is defined as

  $ p_n(x) = sum_(i=0)^n c_i x^i, $

  where $c_0, dots, c_n in VV$ are constants that called the *coefficients of the polynomial*.
]

#env("Definition")[
  Given a polynomial $p(x) = sum_(i=0)^n c_i x^i$ where $c_n eq.not 0$, the degree of $p(x)$ is marked as $upright(deg) (p(x)) = n$. In particular, the degree of zero polynomial $p(x) = 0$ is $upright(deg) (0) = -infinity$.
]

#env("Theorem")[
  Denoted by $PP_n = { p: upright(deg) (p) <= n }$ the set of polynomials with degree no more than $n$ ($n >= 0$), and $PP = union.big_(n=0)^infinity PP_n$ the set contains all polynomials, then $PP_n$ is a linear space and satisfies

  $ { 0 } = PP_0 subset PP_1 subset dots.c subset PP_n subset dots.c PP $
]

== Orthogonal Polynomial

#env("Definition")[
  Given a weight function $rho(x): [a, b] -> RR^+$, satisfies

  $ integral_a^b rho(x) upright(d) x > 0, integral_a^b x^k rho(x) upright(d) x > 0 " exists". $

  The set of *orthogonal polynomials* on $[a, b]$ with the weight function $rho(x)$ is defined as

  $ {p_i, i in NN} subset L_rho ([a, b]) = { f(x): integral_a^b f^2(x) rho(x) upright(d) x < infinity }. $

  where ${p_i, i in NN}$ are calculate from ${x^n, n in NN}$ using the Gram-Schmidt process with the inner product

  $ forall f, g in L_rho ([a, b]), angle.l f, g angle.r = integral_a^b rho(x) f(x) g(x) upright(d) x. $
]

#env("Theorem")[
  Orthogonal polynomials $p_(n-1) (x), p_n (x), p_(n+1) (x)$ satisfies

  $ p_(n+1) (x) = (a_n + b_n x) p_n (x) + c_n p_(n-1) (x). $

  where $a_n, b_n, c_n$ are depends on $[a, b]$ and $rho$.
]

#env("Theorem")[
  The orthogonal polynomial $p_n (x)$ on $[a, b]$ with the weight function $rho(x)$ has $n$ roots on $(a, b)$.
]

=== Legendre polynomial

#env("Definition")[
  The *Legendre polynomial* is defined on $[-1, 1]$ with the weight function $rho(x) = 1$.
]

#env("Theorem")[
  The Legendre polynomials ${p_i (x), i in NN }$ satisfies

  $ integral_(-1)^1 p_i (x) p_j (x) upright(d) x = cases(2 / (2 i + 1)\, & #h(1em) i = j, 0\, & #h(1em) i eq.not j. ) $
]

#env("Theorem")[
  The Legendre polynomial $p_(n-1), p_n, p_(n+1)$ satisfies

  $ p_(n+1) (x) = (2 n + 1) / (n + 1) x p_n (x) - n / (n + 1) p_(n-1) (x). $
]

#env("Example")[
  The first three terms of Legendre polynomials is

  $ p_0(x) = 1, #h(1em) p_1(x) = x, #h(1em) p_2(x) = 3/2 x^2 - 1/2. $
]

=== Chebyshev polynomial of the first kind

#env("Definition")[
  The *Chebyshev polynomial of the first kind* is defined on $[-1, 1]$ with the weight function $rho(x) = 1 / sqrt(1 - x^2)$.
]

#env("Theorem")[
  The Chebyshev polynomials of the first kind ${p_i (x), i in NN}$ satisfies

  $ integral_(-1)^1 1/sqrt(1 - x^2) p_i (x) p_j (x) upright(d) x = cases(
    pi & #h(1em) i = j = 0,
    pi / 2 & #h(1em) i = j eq.not 0,
    0 & #h(1em) i eq.not j.) $
]

#env("Theorem")[
  The Chebyshev polynomial of the first kind $p_(n-1), p_n, p_(n+1)$ satisfies

  $ p_(n+1) (x) = 2 x p_n (x) - p_(n-1) (x). $
]

#env("Example")[
  The first three terms of Chebyshev polynomials of the first kind is

  $ p_0 (x) = 1, #h(1em) p_1 (x) = x, #h(1em) p_2 (x) = 2 x^2 - 1. $
]

=== Chebyshev polynomial of the second kind

#env("Definition")[
  The *Chebyshev polynomial of the second kind* is defined on $[-1, 1]$ with the weight function $rho(x) = sqrt(1 - x^2)$.
]

#env("Theorem")[
  The Chebyshev polynomials of the second kind ${p_i (x), i in NN}$ satisfies

  $ integral_(-1)^1 sqrt(1 - x^2) p_i (x) p_j (x) upright(d) x = cases(
    pi / 2\, & #h(1em) i = j,
    0\, & #h(1em) i eq.not j.) $
]

#env("Theorem")[
  The Chebyshev polynomial of the second kind $p_(n-1), p_n, p_(n+1)$ satisfies

  $ p_(n+1) (x) = 2 x p_n (x) - p_(n-1) (x). $
]

#env("Example")[
  The first three terms of Chebyshev polynomials of the second kind is

  $ p_0 (x) = 1, #h(1em) p_1 (x) = 2x, #h(1em) p_2 (x) = 4 x^2 - 1. $
]

=== Laguerre polynomial

#env("Definition")[
  The *Laguerre polynomial* is defined on $[0, +infinity)$ with the weight function $rho(x) = x^alpha e^(-x)$.
]

#env("Theorem")[
  The Laguerre polynomial ${p_i (x), i in NN}$ satisfies

  $ integral_0^(+infinity) x^alpha e^(-x) p_i (x) p_j (x) upright(d) x = cases(
    Gamma(n+alpha+1) / (n!)\, & #h(1em) i = j,
    0\, & #h(1em) i eq.not j.) $
]

#env("Theorem")[
  For $alpha = 0$, the Laguerre polynomial $p_(n-1), p_n, p_(n+1)$ satisfies

  $ p_(n+1) (x) = (2 n + 1 - x) p_n (x) - n^2 p_(n-1) (x). $
]

#env("Example")[
  For $alpha = 0$, the first three terms of Laguerre polynomial is

  $ p_0 (x) = 1, #h(1em) p_1 (x) = -x + 1, #h(1em) p_2 (x) = x^2 - 4x + 2. $
]

=== Hermite polynomial (probability theory form)

#env("Definition")[
  The *Hermite polynomial* is defined on $(-infinity, +infinity)$ with the weight function $rho(x) = (1 / sqrt(2 pi)) e^(-(x^2) / 2)$.
]

#env("Theorem")[
  The Hermite polynomial ${p_i (x), i in NN}$ satisfies

  $ integral_0^(+infinity) 1/ sqrt(2 pi) e^(-(x^2) / 2) p_i (x) p_j (x) upright(d) x = cases(
    n!\, & #h(1em) i = j,
    0\, & #h(1em) i eq.not j.) $
]

#env("Theorem")[
  For $alpha = 0$, the Hermite polynomial $p_(n-1), p_n, p_(n+1)$ satisfies

  $ p_(n+1) (x) = x p_n (x) - n p_(n-1) (x). $
]

#env("Example")[
  For $alpha = 0$, the first three terms of Hermite polynomial is

  $ p_0 (x) = 1, #h(1em) p_1 (x) = x, #h(1em) p_2 (x) = x^2 - 1. $
]
