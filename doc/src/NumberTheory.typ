#import "../config.typ": env, mathbf

= Number Theory

== Prime Number

#env("Definition")[
  A *prime number* (or a *prime*) is a natural number greater than $1$ that is not a product of two smaller natural numbers.
]

#env("Definition")[
  A *composite number* (or a *composite*) is a natural number greater than $1$ that is a product of two smaller natural numbers.
]

=== Primality testing

#env("Theorem")[
  For a integer $n in NN$, if it is a product of two natural number $a$ and $b$ thar $a <= b$, then

  $ 1 <= a <= sqrt(n) <= b <= n. $
]

#env("Method", name: "Trial division")[
  Given a integer $n$, the *trial division method* divides $n$ by each integer from $2$ up to $sqrt(n)$. Any such integer dividing $n$ evenly establishes $n$ as composite, otherwise it is prime.
]

#env("Theorem", name: "Fermat's little theorem")[
  For a prime number $p$ and a number $a$ that $upright("gcd") (a, p) = 1$, then $a^(p-1) equiv 1 (upright("mod") p)$
]

#env("Method")[
  The *Miller-Rabin* algorithm is a method of primality testing, where given a number $n$, where we

  + determine directly for small numbers such as $p = 2$.
  + factorize the number $p = u times 2^t$;
  + choose a number $a$ that $upright("gcd") (a, p) = 1$, and calculate $a^u, a^(u times 2), a^(u times 2^2), dots, a^(u times 2^(t-1))$;
  + if $a^u equiv 1 (upright("mod") p)$, or $exists a^(u times k), k < t$ that $a^(u times k) equiv p-1 (upright("mod") p)$ then $p$ passes the test, otherwise, $p$ is a composite number;
  + repeat above steps to eliminate error.

  For numbers less than $2^32$, choose $a in { 2, 7, 61 }$ is enough, for numbers less than $2^{64}$, choose $a in { 2, 325, 9375, 28178, 450775, 9780504, 1795265022 }$ is enough.
]

=== Sieves

#env("Method", name: "Sieve of Eratosthenes")[
  Given a upper limit $n$, the *sieve of Eratosthenes* solves all the prime numbers up to $n$ by marking composite numbers, where we

  + create a list of consecutive integers from $2$ to $n$: ${2, 3, 4, dots, n}$;
  + initially, let $p = 2$, the smallest prime number;
  + enumerate the multiples of $p$ by counting in increments of $p$ from $2p$ to $n$, and mark them in the list;
  + find the smallest number in the list greater than $p$ that is not marked;
  + if there was no such number, the method is terminated and the numbers remaining not marked in the list are all the primes below $n$, otherwise let $p$ now equal the new number which is the next prime, and repeat from step (3).
]
