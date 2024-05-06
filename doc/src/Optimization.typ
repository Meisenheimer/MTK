#import "../config.typ": env, mathbf

= Optimization

== One-dimensional Line Search

#env("Definition")[
  Given a function $f: RR^n -> RR$, a initial point $mathbf(x)$ and a direction $mathbf(d)$, denoted by $phi(alpha) = f(mathbf(x) + alpha mathbf(d))$, a *one-dimensional line search* method solves the problem

  $ phi(alpha) =  min_(t in RR^+) phi(t). $
]

#env("Method", name: "Success-failure method")[
  For a one-dimensional line search problem, the *success-failure method* is an inexact one-dimensional line search method to solve the interval $[a, b] in [0, +infinity)$ that exact solution $alpha^* in [a, b]$, where we

  + Choose initial value $alpha_0 in [0, +infinity)$, $h_0 > 0$, $t > 0$(commonly choose $t = 2$), calculate $phi(alpha_0)$ and let $k = 0$;
  + Let $alpha_(k+1) = alpha_k + h_k$ and calculate $phi(alpha_(k+1))$, if $phi(alpha_(k+1)) < phi(alpha_k)$, then go to (3), otherwise go to (4);
  + Let $h_(k+1) = t h_k$, $alpha = alpha_k$, $k = k + 1$, and go to (2);
  + If $k = 0$, then let $h_k = -h_k$ and go to (2), otherwise stop and the solution $[a, b]$ satisfies $ a = min{alpha, alpha_k}, #h(1em) b = max{alpha, alpha_k}. $
]

#env("Definition")[
  A general form of one-dimensional line search method is the following three steps:

  + *Initialization*: given initial point $mathbf(x)$ and acceptable error $epsilon > 0$, $delta > 0$;
  + *Iteration*: calculate the direction $mathbf(d)$ and step size $alpha$ that $f(mathbf(x) + alpha mathbf(d)) = limits(min)_(t in RR^+) f(mathbf(x) + t mathbf(d))$ and let $mathbf(x) = mathbf(x) + alpha mathbf(d)$;
  + *Stop condition*: if $||nabla f(mathbf(x))|| <= epsilon$ or $U_(RR^n) (x, delta)$ includes the exact solution, then the current $mathbf(x)$ is the solution.

  where the iteration step are repeated until $mathbf(x)$ satisfies the stop condition.
]

#env("Definition")[
  Given a method, denoted by ${mathbf(x)_k}$ the sequence of the iteration and $mathbf(x)^*$ the exact solution, the method is *(Q-)linear convergence* if

  $ lim_(k -> infinity) (||mathbf(x)_(k+1) - mathbf(x)^*||)/(||mathbf(x)_k - mathbf(x)^*||) in (0, 1), $

  the method is *(Q-)sublinear convergence* if

  $ lim_(k -> infinity) (||mathbf(x)_(k+1) - mathbf(x)^*||)/(||mathbf(x)_k - mathbf(x)^*||) = 1, $

  the method is *(Q-)superlinear convergence* if

  $ lim_(k -> infinity) (||mathbf(x)_(k+1) - mathbf(x)^*||)/(||mathbf(x)_k - mathbf(x)^*||) = 0. $

  For a superlinear convergence method,  the method is $r$-order linear convergence if

  $ lim_(k -> infinity) (||mathbf(x)_(k+1) - mathbf(x)^*||)/(||mathbf(x)_k - mathbf(x)^*||^r) in [0, +infinity), $

  where when $r = 2$ is called *(Q-)quadratic convergence*.
]

#env("Remark")[
  There is another *R-convergence* for judging a sequence which use another Q-convergence sequence as the boundary of ${||mathbf(x)_k - x^*||}$, but is not needed here.
]

#env("Method", name: "Golden section method")[
  Given the initial point $mathbf(x)$, an interval $[a, b]$ and $delta > 0$,

  - The iteration step is:
    + Calculate the two testing points $lambda = a + (1 - k) (b - a)$ and $mu = a + k (b - a)$ where $k = (sqrt(5) - 1)/2$ is the golden ratio;
    + If $phi(lambda) > phi(mu)$, let $a = lambda$, otherwise let $b = mu$.
  - The stop condition is $b - a <= delta$;
  - The solution is $mathbf(x) + (a+b)/2 mathbf(d)$.
]

#env("Theorem")[
  The golden section method is a *linear convergent* method.
]

#env("Method", name: "Fibonacci method")[
  Given the initial point $mathbf(x)$, an interval $[a, b]$ and $delta > 0$,

  - The $k$-th iteration step is:
    + Calculate the two testing points $lambda = a + F_(k) / F_(k+2) (b - a)$ and $mu = a + F_(k+1) / F_(k+2) (b - a)$ where $F_k$ is the $k$-th fibonacci number and $k$;
    + If $phi(lambda) > phi(mu)$, let $a = lambda$, otherwise let $b = mu$.
  - The stop condition is $b - a <= delta$;
  - The solution is $mathbf(x) + (a+b)/2 mathbf(d)$.
]

#env("Theorem")[
  The Fibonacci method is a *linear convergent* method.
]

#env("Method", name: "Bisection method")[
  Given the initial point $mathbf(x)$, an interval $[a, b]$ and $delta > 0$,

  - The iteration step is:
    + Calculate the midpoint $m = (a + b)/2$ and $phi(m)$;
    + If $nabla f(m) dot.c d < 0$, let $a = m$, otherwise let $b = m$.
  - The stop condition is $b - a <= delta$;
  - The solution is $mathbf(x) + (a+b)/2 mathbf(d)$.
]

#env("Theorem")[
  The bisection method is a *linear convergent* method.
]

#env("Method", name: "Newton's method")[
  Given the initial point $mathbf(x)$ and $epsilon > 0$,

  - The iteration step is:
    + Calculate $(nabla^2 f(mathbf(x)))^T dot.c mathbf(d)$ and $(nabla f(mathbf(x)))^T dot.c mathbf(d)$;
    + Let $mathbf(x) = mathbf(x) - ((nabla f(mathbf(x)))^T dot.c mathbf(d))/((nabla^2 f(mathbf(x)))^T dot.c mathbf(d))$;
  - The stop condition is $(nabla f(mathbf(x)))^T dot.c mathbf(d) <= epsilon$;
  - The solution is $mathbf(x)$.
]

#env("Theorem")[
  The Newton's method is a *quadratic convergent* method.
]

== Unconstrained Optimization

#env("Definition")[
  Given a convex function $f: RR^n -> RR$, a *unconstrained optimization* method solves the problem

  $ min_(mathbf(x) in RR^n) f(mathbf(x)) $

  by

  + *Initialization*: given initial point $mathbf(x)$ and acceptable error $epsilon > 0$, $delta > 0$;
  + *Iteration*: calculate the direction $mathbf(d)$ and step size $alpha$, then let $mathbf(x) = mathbf(x) + alpha mathbf(d)$;
  + *Stop condition*: if $||nabla f(mathbf(x))|| <= epsilon$ or $U_(RR^n) (mathbf(x), delta)$ includes the exact solution, then the current $mathbf(x)$ is the solution.
]

#env("Method", name: "Gradient descent method")[
  Given the initial point $mathbf(x)$ and $epsilon > 0$,

  - The iteration step is:
    + Calculate $mathbf(d) = -nabla f(mathbf(x))$ and step size $alpha$ by a line search method;
    + Let $mathbf(x) = mathbf(x) + alpha mathbf(d)$;
  - The stop condition is $||nabla f(mathbf(x))|| <= epsilon$;
  - The solution is $mathbf(x)$.
]

#env("Theorem")[
  The gradient descent method is a *linear convergent* method.
]

#env("Method", name: "Quasi-Newton method")[
  Given the initial point $mathbf(x)$, $epsilon > 0$ and a matrix $H in RR^(n times n)$ (usually the identity matrix),

  - The $k$-th iteration step is:
    + Calculate $mathbf(d)_k = -H_k nabla f(mathbf(x)_k)$ and step size $alpha_k$ by a line search method;
    + Let $mathbf(x)_k = mathbf(x)_k + alpha_k mathbf(d)_k$ and $H_(k+1) = r_k (H_k)$ where the function $r_k$ is a *update* depends on $mathbf(x)_k$, $mathbf(x)_(k+1)$, $nabla f(mathbf(x)_k)$ and $nabla f(mathbf(x)_(k+1))$;
  - The stop condition is $||nabla f(mathbf(x)_k)|| <= epsilon$;
  - The solution is $mathbf(x)_k$ that satisfies the stop condition.
]

#env("Definition")[
  Let $mathbf(s)_k = mathbf(x)_(k+1) - mathbf(x)_k$ and $mathbf(y)_k = nabla f(mathbf(x)_(k+1)) - nabla f(mathbf(x)_k)$, the *Symmetric Rank-1 update (SR1)* is

  $ H_(k+1) = H_k + ((mathbf(s)_k - H_k mathbf(y)_k) (mathbf(s)_k - H_k mathbf(y)_k)^T) / ((mathbf(s)_k - H_k mathbf(y)_k)^T mathbf(y)_k). $

  The *DFP update* is a rank-2 update defined as

  $ H_(k+1) = H_k + (mathbf(s)_k mathbf(s)_k^T) / (mathbf(s)_k^T mathbf(y)_k) - (H_k mathbf(y)_k mathbf(y)_k^T H_k) / (mathbf(y)_k^T H_k mathbf(y)_K). $

  The *BFGS update* is a rank-2 update defined as

  $ H_(k+1) = H_k + (1 + (mathbf(y)_k^T H_k mathbf(y)_k) / (mathbf(s)_k^T mathbf(y)_k)) (mathbf(s)_k mathbf(s)_k^T) / (mathbf(s)_k^T mathbf(y)_k) - (mathbf(s)_k mathbf(y)_k^T H_k + H_k mathbf(y)_k mathbf(s)_k^T) / (mathbf(s)_k^T mathbf(y)_K). $
]

#env("Theorem")[
  The Quasi-Newton method is a *superlinear convergent* method.
]

#env("Method", name: "Newton's method")[
  Given the initial point $mathbf(x)$ and $epsilon > 0$,

  - The iteration step is:
    + Calculate $mathbf(d) = -(nabla^2 f(mathbf(x)))^(-1) nabla f(mathbf(x))$ and step size $alpha$ by a line search method;
    + Let $mathbf(x) = mathbf(x) + alpha mathbf(d)$;
  - The stop condition is $||nabla f(mathbf(x))|| <= epsilon$;
  - The solution is $mathbf(x)$.
]

#env("Theorem")[
  The Newton's method is a *quadratic convergent* method.
]
