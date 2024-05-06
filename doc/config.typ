#let tiny-size =	6pt
#let script-size =	8pt
#let footnote-size =	9pt
#let small-size =	10pt
#let normal-size = 10.95pt
#let large-size =	12pt
#let Large-size =	14.4pt
#let LARGE-size =	17.28pt
#let huge-size =	20.74pt
#let Huge-size =	24.88pt

#let project(
  papersize: "a4", // 21(cm) * 29.7(cm)
  language: "en",
  show_outline: true,
  title: "",
  subtitle: none,
  authors: (),
  date: datetime.today(),
  abstract: [],
  keywords: none,
  // fontfamily: "Linux Libertine",
  // fontfamily: "Latin Modern Math",
  fontfamily: "New Computer Modern",
  bibliography_path: none,
  body,
) = {
  set document(author: authors.map(a => a.name), title: title)
  set page(
    paper: papersize,
    // margin: (left: 3.18cm, right: 3.18cm, top: 2.54cm, bottom: 2.54cm),
    // margin: (x: 3cm, y: 2.5cm),
    columns: 1,
    numbering: "1",
    number-align: center,
    header: locate(loc => {
      if counter(page).at(loc).first() > 1 [
        #set text(size: large-size)
        #title
        #h(1fr)
        #if language == "en" [
          #date.display("[month repr:long] [day], [year]")
        ]
        #if language == "cn" [
          #date.display("[year]年[month]月[day]日")
        ]
        #line(length: 100%)
      ]}),
  )
  set text(font: fontfamily, lang: language, size: normal-size)
  show par: set block(above: 0.58em, below: 0.58em)
  show heading: it => {
    let number = if it.numbering != none {
      counter(heading).display(it.numbering)
      h(7pt, weak: true)
    }
    set text(size: normal-size, weight: "bold")
    set par(first-line-indent: 0pt)
    if it.level == 1 {
      set align(center)
      set text(size: normal-size)
      v(15pt, weak: true)
      number
      it.body
      v(normal-size, weak: true)
    } else if it.level == 2 {
      set text(size: normal-size)
      v(normal-size, weak: true)
      number
      it.body
      v(normal-size, weak: true)
    } else {
      v(11pt, weak: true)
      number
      let styled = if it.level == 2 { strong } else { emph }
      styled(it.body + [. ])
      h(7pt, weak: true)
    }
  }

  // Title Page
  set page(numbering: "A")
  align(center)[
    #v(20em, weak: false)
    #text(2em, title) \
    #if subtitle != none [
      #text(1.2em, subtitle) \
    ]
    #v(2.5em, weak: true)
  ]

  let parse_authors(authors) = {
    let affiliations = ()
    let parsed_authors = ()
    let corresponding = ()
    let pos = 0
    for author in authors {
      if "affiliation" in author {
        if author.affiliation not in affiliations {
          affiliations.push(author.affiliation)
        }
        pos = affiliations.position(a => a == author.affiliation)
        author.insert("affiliation_parsed", pos)
      } else {
        // if author has no affiliation, just use the same as the previous author
        author.insert("affiliation_parsed", pos)
      }
      parsed_authors.push(author)
      if "corresponding" in author {
        if author.corresponding {
          corresponding = author
        }
      }
    }
    (
      authors: parsed_authors,
      affiliations: affiliations,
      corresponding: corresponding,
    )
  }

  let authors_parsed = parse_authors(authors)

  // List Authors
  pad(top: 0.3em, bottom: 0.3em, x: 2em, grid(
    columns: (1fr,) * calc.min(3, authors_parsed.authors.len()),
    gutter: 1em,
    ..authors_parsed.authors.map(author => align(center)[
      #set text(size: Large-size)
      #author.name
      #footnote[Email: #link("mailto:" + authors_parsed.corresponding.email, authors_parsed.corresponding.email)] \
      #if author.institude != none [
        #set text(size: large-size)
        #author.institude
      ]
    ]),
  ))

  align(
    center,
  )[
    #v(2.5em, weak: true)
    #if language == "en" [
      #text(Large-size, date.display("[month repr:long] [day], [year]"))
    ]
    #if language == "cn" [
      #text(Large-size, date.display("[year]年[month]月[day]日"))
    ]
    #v(5em, weak: true)
  ]
  set par(justify: true)

  // Abstract & Keywords
  if abstract != [] [
    #pagebreak(weak: true)
    #if language == "en" [
      #heading(
        outlined: false,
        numbering: none,
        text(large-size, weight: "bold", [Preface]),)
    ]
    #if language == "cn" [
      #heading(
        outlined: false,
        numbering: none,
        text(large-size, weight: "bold", [前言]),)
    ]
    #align(center)[
      #block(width: 90%, [
        #align(left)[
          #abstract
        ]
      ])
    ]
    #pagebreak(weak: true)
  ]

  // Main body.
  set page(numbering: "i")
  set enum(numbering: "(1)", indent: 2em)
  set list(indent: 2em)

  counter("env").update((0, 0))
  counter("part").update(0)
  set heading(numbering: "1.1 ")
  show heading: it => {
    let number = if it.numbering != none {
      counter(heading).display(it.numbering)
      h(7pt, weak: true)
    }
    set text(size: normal-size, weight: "bold")
    set par(first-line-indent: 0pt)
    if it.level == 1 {
      pagebreak(weak: true)
      counter("env").update(counter(heading).get())
      set text(size: huge-size)
      v(Huge-size, weak: true)
      "Chapter "
      number
      "\n"
      v(Huge-size, weak: true)
      it.body
      v(Huge-size, weak: true)
    } else if it.level == 2 {
      set text(size: LARGE-size)
      v(LARGE-size, weak: true)
      number
      it.body
      v(normal-size, weak: true)
    } else if it.level == 3 {
      set text(size: Large-size)
      v(Large-size, weak: true)
      number
      it.body
      v(normal-size, weak: true)
    } else if it.level == 4 {
      set text(size: large-size)
      v(large-size, weak: true)
      number
      it.body
      v(normal-size, weak: true)
    } else {
      set text(size: normal-size)
      v(normal-size, weak: true)
      it.body
      v(normal-size, weak: true)
    }
  }

  // Outline
  if show_outline == true [
    #show heading: it => {
      let number = if it.numbering != none {
        counter(heading).display(it.numbering)
        h(7pt, weak: true)
      }
      set text(size: normal-size, weight: "bold")
      set par(first-line-indent: 0pt)
      if it.level == 1 {
        counter("env").update(counter(heading).get())
        set text(size: huge-size)
        v(Huge-size, weak: true)
        it.body
        v(Huge-size, weak: true)
      }
    }
    #counter(page).update(1)
    #pagebreak(weak: true)
    #set text(size: large-size)
    #if language == "cn" [
      #outline(title: [目录], depth: 3, indent: true)
    ]
    #if language == "en" [
      #outline(title: [Contents], depth: 3, indent: true)
    ]
    #pagebreak(weak: true)
  ]

  // Body
  set page(numbering: "1")
  counter(page).update(1)
  set par(justify: true, first-line-indent: 2em)
  body

  // Bibliography
  if bibliography_path != none{
    show heading: it => {
      let number = if it.numbering != none {
        counter(heading).display(it.numbering)
        h(7pt, weak: true)
      }
      set text(size: normal-size, weight: "bold")
      set par(first-line-indent: 0pt)
      if it.level == 1 {
        counter("env").update(counter(heading).get())
        set text(size: huge-size)
        v(Huge-size, weak: true)
        it.body
        v(Huge-size, weak: true)
      }
    }

    pagebreak(weak: true)
    if language == "cn" {
      bibliography(bibliography_path, title: "参考文献", style: "gb-7714-2015-numeric")
    }
    if language == "en" {
      bibliography(bibliography_path, title: "Bibliography", style: "association-for-computing-machinery")
    }
    show bibliography: set block(spacing: 0.58em)
    show bibliography: set par(first-line-indent: 0em)
  }
}

#let part(title) = {
  pagebreak(weak: true)
  counter("part").step()
  align(center + horizon)[
    #set text(size: Huge-size, weight: "bold")
    Part #counter("part").display("1") \
    #title
  ]
  pagebreak(weak: true)
}

#let env(title, body, name: none) = {
  v(1em, weak: true)
  set text(size: normal-size, weight: "bold")
  set par(first-line-indent: 0em, justify: true)
  title
  " "
  counter("env").step(level: 2)
  counter("env").display("1.")
  if name != none {
    " ("
    name
    ")"
  }
  " "
  set text(size: normal-size, weight: "regular")
  body
  v(2em, weak: true)
  // "\n"
}

#let mathbf(body) = {
  $upright(bold(body))$
}