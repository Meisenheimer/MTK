#import "../config.typ": env, mathbf

= Initial Value Problem

#env("Definition")[
  For $T >= 0$, $mathbf(f): RR^n times [0, T] -> RR^n$ and $mathbf(u)_0 in RR^n$, the *initial value problem* (IVP) is to find $u(t) in C^1$ satisfies

  $ mathbf(u)^prime = mathbf(f) (mathbf(u) (t), t), #h(1em) mathbf(u) (0) = mathbf(u)_0. $
]

#env("Notation")[
  To numerically solve the IVP, we are given initial condition $mathbf(u)_0 = mathbf(u) (t_0)$, and want to compute approximations ${ mathbf(u)_k, k = 1, 2, dots }$ such that

  $ mathbf(u)_k approx mathbf(u)(t_k), $

  where $k$ is the uniform time step size and $t_n = n k$.
]

== Linear Multistep Method

#env("Definition")[
  For solving the IVP, an s-step *linear multistep method* (LMM) has the form

  $ sum_(j=0)^s alpha_j mathbf(u)_(n+j) = k sum_(j=0)^s beta mathbf(f) (mathbf(u)_(n+j), t_(n+j)), $

  where $alpha_s = 1$ is assumed WLOG.
]

#env("Definition")[
  An LMM is *explicit* if $beta_s = 0$, otherwise it is *implicit*.
]

== Runge-Kutta Method

#env("Definition")[
  An s-stage *Runge-Kutta method* (RK) is a one-step method of the form

  $ & mathbf(y)_i & & = mathbf(f) (mathbf(u)_n + k sum_(j=1)^s a_(i j) mathbf(y)_j, t_n + c_i k), \
    & mathbf(u)_(i+1) & & = mathbf(u)_i + k sum_(j=1)^s b_j mathbf(y)_j, $

  where $i = 1, dots, s$ and $a_(i j), b_j, c_i in RR$.
]

#env("Definition")[
  The \textsf{Butcher tableau} is one way to organize the coefficients of an RK method as follows

  #align(center)[
    #table(
      columns: (auto, auto, auto, auto),
      stroke: none,
      align: center + horizon,

      $c_1$, table.vline(), $a_(11)$, $dots.c$, $a_(1s)$,
      $dots.v$, table.vline(), $dots.v$, "", $dots.v$,
      $c_s$, table.vline(), $a_(s 1)$, $dots.c$, $a_(s s)$,
      table.hline(), table.hline(), table.hline(), table.hline(),
      "", table.vline(), $b_1$, $dots.c$, $b_s$,
    )
  ]

  The matrix $A = (a_(i j))_(s times s)$ is called the RK matrix and $mathbf(b) = (b_1, dots, b_s)^T, mathbf(c) = (c_1, dots, c_s)^T$ are called the RK weights and the RK nodes.
]

#env("Definition")[
  An s-stage *collocation method* is a numerical method for solving the IVP, where we

  + choose $s$ distinct collocation parameters $c_1, dots, c_s$,
  + seek $s$-degree polynomial $p$ satisfying
    $ forall i = 1, 2, dots, s, #h(1em) mathbf(p) (t_n) = mathbf(u)_n " and " mathbf(p)^prime (t_n + c_i k) = mathbf(f) (mathbf(p) (t_n + c_i k), t_n + c_i k), $
  + set $mathbf(u)_(n+1) = mathbf(p) (t_(n+1))$.
]

#env("Theorem")[
  The s-stage collocation method is an s-stage IRK method with

  $ a_(i j) = integral_0^(c_i) l_j (tau) upright(d) tau, #h(1em) b_j = integral_0^1 l_j (tau) upright("d") tau, $

  where $i, j = 1, dots, s$ and $l_k (tau)$ is the elementary Lagrange interpolation polynomial.
]

== Theoretical analysis

#env("Definition")[
  A function $mathbf(f): RR^n times [0, +infinity) -> RR^n$ is *Lipschitz continuous* in its first variable over some domain

  $ Omega = { (mathbf(u), t): ||mathbf(u) - mathbf(u)_0|| <= a, t in [0, T] } $

  iff

  $ exists L >= 0, " s.t. " forall (mathbf(u), t) in Omega, #h(1em) ||mathbf(f) (mathbf(u), t) - mathbf(f) (mathbf(v), t) <= ||mathbf(u) - mathbf(v)||. $
]

=== Error analysis

#env("Definition")[
  The *local truncation error* $tau$ is the error caused by replacing continuous derivatives with numerical formulas.
]

#env("Definition")[
  A numerical formulas is *consistent* if $limits(lim)_(k -> 0) tau = 0$.
]

=== Stability

#env("Definition")[
  The *region of absolute stability* (RAS) of a numerical method, applied to

  $ mathbf(u)^prime = lambda mathbf(u), #h(1em) mathbf(u)_0 = mathbf(u) (t_0), $

  is the region $Omega$ that

  $ forall mathbf(u)_0, #h(1em) forall lambda k in Omega, #h(1em) lim_(n -> +infinity) mathbf(u)_n = 0. $
]

#env("Definition")[
  The *stability function* of a one-step method is a function $R: CC -> CC$ that satisfies

  $ mathbf(u)_(n+1) = R(z) mathbf(u)_n $

  for the $mathbf(u)^prime = lambda mathbf(u)$ where $upright("Re") (E(lambda)) <= 0$ and $z = k lambda$.
]

#env("Definition")[
  A numerical method is *stable* or *zero stable* iff its application to any IVP with $mathbf(f) (mathbf(u), t)$ Lipschitz continuous in $mathbf(u)$ and continuous in $t$ yields

  $ forall T > 0, #h(1em) lim_(k -> 0, N k = t) ||mathbf(u)_n|| < infinity. $
]

#env("Definition")[
  A numerical method is *A($bold(alpha)$)-statble* if the region of absolute stability $Omega$ satisfies

  $ {z in CC: pi - alpha <= arg(z) <= pi + alpha} subset.eq Omega. $
]

#env("Definition")[
  A numerical method is *A-statble* if the region of absolute stability $Omega$ satisfies

  $ { z in CC: upright("Re") (z) <= 0 } subset.eq Omega. $
]

#env("Definition")[
  A one-step method is *L-stable* if it is A-stable, and its stability function satisfies

  $ lim_(z -> infinity) |R(z)| = 0. $
]

#env("Definition")[
  An one-step method is *I-stable* iff its stability function satisfies

  $ forall y in RR, |R(y mathbf(i))| <= 1. $
]

#env("Definition")[
  An one-step method is *B-stable* (or *contractive*) if for any contractive ODE system, every pair of its numerical solutions $mathbf(u)_n$ and $mathbf(v)_n$ satisfy

  $ forall n in NN, ||u_(n+1) - v_(n+1)|| <= ||u_n - v_n||. $
]

#env("Definition")[
  An RK method is *algebraically stable* iff the RK weights $b_1, dots, b_s$ are nonnegative, the *algebraic stability matrix* $M = (b_i a_(i j) + b_i a_(j i) - b_i b_j)_(s times s)$ is positive semidefinite.
]

#env("Theorem")[
  The order of accuracy of an implicit A-stable LMM satisfies $p <= 2$. An explicit LMM cannot be A-stable.
]

#env("Theorem")[
  No ERK method is A-stable.
]

#env("Theorem")[
  An RK method is A-stable if and only if it is I-stable and all poles of its stability function $R(z)$ have positive real parts.
]

#env("Theorem")[
  If an A-stable RK method with a nonsingular RK matrix $A$ is stiffly accurate, then it is L-stable.
]

#env("Theorem")[
  If an A-stable RK method with a nonsingular RK matrix $A$ satisfies

  $ forall i in {1, dots, s}, #h(1em) a_(i 1) = b_i, $

  then it is L-stable.
]

#env("Theorem")[
  B-stable one-step methods are A-stable.
]

#env("Theorem")[
  An algebraically stable RK method is B-stable and A-stable.
]

=== Convergence

#env("Definition")[
  A numerical method is convergent iff its application to any IVP with $mathbf(f) (mathbf(u), t)$ Lipschitz continuous in $mathbf(u)$ and continuous in $t$ yields

  $ forall T > 0, #h(1em) lim_(k -> 0, n k = T) mathbf(u)_n = mathbf(u) (T). $
]

#env("Theorem")[
  A numerical method is convergent iff it is consistent and stable.
]

== Important Methods

=== Forward Euler's method

#env("Definition")[
  The *forward Euler's method* solves the IVP by

  $ mathbf(u)_(n+1) = mathbf(u)_n + k mathbf(f) (mathbf(u)_n, t_n). $
]

#env("Theorem")[
  The region of absolute stability for forward Euler's method is

  $ { z in CC: |1 + z| <= 1 }. $
]

=== Backward Euler's method

#env("Definition")[
  The *backward Euler's method* solves the IVP by

  $ mathbf(u)_(n+1) = mathbf(u)_n + k mathbf(f) (mathbf(u)_(n+1), t_(n+1)). $
]

#env("Theorem")[
  The region of absolute stability for backward Euler's method is

  $ { z in CC: |1 - z| >= 1 \}. $
]

=== Trapezoidal method

#env("Definition")[
  The *trapezoidal method* solves the IVP by

  $ mathbf(u)_(n+1) = mathbf(u)_n + k/2 (mathbf(f) (mathbf(u)_n, t_n) + mathbf(f) (mathbf(u)_(n+1), t_(n+1))). $
]

#env("Theorem")[
  The region of absolute stability for trapezoidal method is

  $ { z in CC: abs((2 + z)/(2 - z)) >= 1 }. $
]

=== Midpoint method (Leapfrog method)

#env("Definition")[
    The *midpoint method (Leapfrog method)* solves the IVP by

    $ mathbf(u)_(n+1) = mathbf(u)_(n-1) + 2 k mathbf(f) (mathbf(u)_n, t_n). $
]

#env("Theorem")[
    The region of absolute stability for midpoint method is

    $ { z in CC: abs(z plus.minus sqrt(1 + z^2)) <= 1 } eq.quest { 0 }. $
]

=== Heun's third-order RK method

#env("Definition")[
    The *Heun's third-order formula* is an ERK method of the form

    $ cases(
      & mathbf(y)_1 & = mathbf(f)(mathbf(u)_n, t_n)\,,
      & mathbf(y)_2 & = mathbf(f)(mathbf(u)_n + k/3 mathbf(y)_1, t_n + k/3)\,,
      & mathbf(y)_3 & = mathbf(f)(mathbf(u)_n + (2 k)/3 mathbf(y)_2, t_n + (2 k)/3)\,,
      & mathbf(u)_(n+1) & = mathbf(u)_n + k/4 (mathbf(y)_1 + 3 mathbf(y)_3).
    ) #h(1em)
    #table(
      columns: (auto, auto, auto, auto),
      stroke: none,
      align: center + horizon,
      $0$, table.vline(), $0$, $0$, $0$,
      $1/3$, table.vline(), $1/3$, $0$, $0$,
      $2/3$, table.vline(), $0$, $2/3$, $0$,
      table.hline(), table.hline(), table.hline(), table.hline(),
      "", table.vline(), $1/4$, $0$, $3/4$,
    ) $
]

=== Classical fourth-order RK method

#env("Definition")[
    The *classical fourth-order RK method* is an ERK method of the form

    $ cases(
      & mathbf(y)_1 & = mathbf(f)(mathbf(u)_n, t_n)\,,
      & mathbf(y)_2 & = mathbf(f)(mathbf(u)_n + k/2 mathbf(y)_1, t_n + k/2)\,,
      & mathbf(y)_3 & = mathbf(f)(mathbf(u)_n + k/2 mathbf(y)_2, t_n + k/2)\,,
      & mathbf(y)_4 & = mathbf(f)(mathbf(u)_n + k mathbf(y)_3, t_n + k)\,,
      & mathbf(u)_(n+1) & = mathbf(u)_n + k/6 (mathbf(y)_1 + 2 mathbf(y)_2 + 2 mathbf(y)_3 + mathbf(y)_4).
    ) #h(1em)
    #table(
      columns: (auto, auto, auto, auto, auto),
      stroke: none,
      align: center + horizon,
      $0$, table.vline(), $0$, $0$, $0$, $0$,
      $1/2$, table.vline(), $1/2$, $0$, $0$, $0$,
      $1/2$, table.vline(), $0$, $1/2$, $0$, $0$,
      $1$, table.vline(), $0$, $0$, $1$, $0$,
      table.hline(), table.hline(), table.hline(), table.hline(),
      "", table.vline(), $1/6$, $1/3$, $1/3$, $1/6$,
    ) $
]

=== TR-BDF2 method

#env("Definition")[
    The *TR-BDF2 method* is an one-step method of the form

    $ cases(
      & mathbf(u)_* & = mathbf(u)_n + k/4 (mathbf(f) (mathbf(u)_n, t_n) + mathbf(f) (mathbf(u)_*, t_n + k/2))\,,
      & mathbf(u)_(n+1) & = 1/3 (4 mathbf(u)_* - mathbf(u)_n + k mathbf(f) (mathbf(u)_(n+1), t_(n+1))).
    ) $
]
