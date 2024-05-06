#import "config.typ": *

#show: project.with(
  title: "Document for MTK",
  language: "en",
  show_outline: true,
  subtitle: none,
  authors: (
    (
      name: "Zeyu Wang",
      email: "zeyu.wang.0117@outlook.com",
      institude: none,
      corresponding: true,
    ),
  ),
)

#include "src/Polynomial.typ"

#include "src/Interpolation.typ"

#include "src/Integration.typ"

#include "src/Optimization.typ"

#include "src/IVP.typ"

#include "src/NumberTheory.typ"
