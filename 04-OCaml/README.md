# Task 2019/04A

> ## Day 4: Secure Container
> 
> You arrive at the Venus fuel depot only to discover it's protected by a password. The Elves had written the password on a sticky note, but someone threw it out.
> 
> However, they do remember a few key facts about the password:
> 
> - It is a six-digit number.
> - The value is within the range given in your puzzle input.
> - Two adjacent digits are the same (like `22` in `122345`).
> - Going from left to right, the digits _never decrease_; they only ever increase or stay the same (like `111123` or `135679`).
> 
> Other than the range rule, the following are true:
> 
> - `111111` meets these criteria (double `11`, never decreases).
> - `223450` does not meet these criteria (decreasing pair of digits `50`).
> - `123789` does not meet these criteria (no double).
> 
> _How many different passwords_ within the range given in your puzzle input meet these criteria?
> 
> Your puzzle input is `171309-643603`.

## Considerations

Will need to work with a _candidate password_ not only as a number, but also as a string of digits. The data representation should encompass both cases.

The straight-forward approach (A) is to do an exhaustive search from _min_ to _max_ and filter the valid passwords. In the case of 6-digits long password, this is viable, and is independent of the validity rules.

Another approach (B) would be to derive the succeeding password from the preceding one algorithmically. This could be more complicated, but is still viable. This approach should perform faster.

It turns out that the simple approach (A) is just a degenerate case of (my implementation of) the second approach (B). I'm basing my solution on B.