# Task 2019/03B


> ## Day 3: Crossed Wires
> 
> The gravity assist was successful, and you're well on your way to the Venus refuelling station. During the rush back on Earth, the fuel management system wasn't completely installed, so that's next on the priority list.
> 
> Opening the front panel reveals a jumble of wires. Specifically, _two wires_ are connected to a central port and extend outward on a grid. You trace the path each wire takes as it leaves the central port, one wire per line of text (your puzzle input).
> 
> The wires twist and turn, but the two wires occasionally cross paths. To fix the circuit, you need to _find the intersection point closest to the central port_. Because the wires are on a grid, use the Manhattan distance for this measurement. While the wires do technically cross right at the central port where they both start, this point does not count, nor does a wire count as crossing with itself.
> 
> For example, if the first wire's path is `R8,U5,L5,D3`, then starting from the central port (`o`), it goes right `8`, up `5`, left `5`, and finally down `3`:
> ```
> ...........
> ...........
> ...........
> ....+----+.
> ....|....|.
> ....|....|.
> ....|....|.
> .........|.
> .o-------+.
> ...........
> ```
> 
> Then, if the second wire's path is `U7,R6,D4,L4`, it goes up `7`, right `6`, down `4`, and left `4`:
> ```
> ...........
> .+-----+...
> .|.....|...
> .|..+--x-+.
> .|..|..|.|.
> .|.-X--+.|.
> .|..|....|.
> .|.......|.
> .o-------+.
> ...........
> ```
> 
> These wires cross at two locations (marked `X`), but the lower-left one is closer to the central port: its distance is `3 + 3 = 6`.
> 
> Here are a few more examples:
> 
> - `R75,D30,R83,U83,L12,D49,R71,U7,L72
>    U62,R66,U55,R34,D71,R55,D58,R83` = distance `159`
> - `R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51
>     U98,R91,D20,R16,D67,R40,U7,R15,U6,R7` = distance `135`
> 
> _What is the Manhattan distance_ from the central port to the closest intersection?


## Considerations

The input format is: multi-line (two lines), comma-separated codes, each code consisting of direction flag and section length.

Could load all wires into a 2d-map, and then search for intersections around `[0,0]`. That would occupy too much memory.
The segments in input data have lengths of up to 999, and there are many of them (could accumulate larger offsets still).

Will translate and sort the wire-segment-data (given in relative/incremental notation) into absolute notation (absolute coordinates for start and end of segment), and then search through that for intersections.

For each found intersection: could calc its distance from `[0,0]` and keep note of smallest distance (no need to keep track the intersections themselves).

It is not necessary to keep track of the segments' order and continuity. If there were more than two wires, it would be necessary to keep track of which wire is which (wire id).

The wire segments are orthogonal to the axes, so they could be represented as three values (on-axis coordinate, and two values as from+to on the perpendicualr axis). Which axis - horizontal or vertical - could be inferred from context. This approach will not be used - it would complicate things.
Will instead represent wire segments as `[X1, Y1, X2, Y2]`.

How to store the set of segments?: singly-linked lists.

The _from,to_ values might be used as _from-exclusive_, _to-inclusive_ - this way the origin `[0,0]` would be rejected.

> ## Part Two
> 
> It turns out that this circuit is very timing-sensitive; you actually need to _minimize the signal delay_.
> 
> To do this, calculate the _number of steps_ each wire takes to reach each intersection; choose the intersection where the _sum of both wires' steps_ is lowest. If a wire visits a position on the grid multiple times, use the steps value from the _first_ time it visits that position when calculating the total value of a specific intersection.
> 
> The number of steps a wire takes is the total number of grid squares the wire has entered to get to that location, including the intersection being considered. Again consider the example from above:
> ```
> ...........
> .+-----+...
> .|.....|...
> .|..+--X-+.
> .|..|..|.|.
> .|.-X--+.|.
> .|..|....|.
> .|.......|.
> .o-------+.
> ...........
> ```
> 
> In the above example, the intersection closest to the central port is reached after `8+5+5+2 = 20` steps by the first wire and `7+6+4+3 = 20` steps by the second wire for a total of `20+20 = 40` steps.
> 
> However, the top-right intersection is better: the first wire takes only `8+5+2 = 15` and the second wire takes only `7+6+2 = 15`, a total of `15+15 = 30` steps.
> 
> Here are the best steps for the extra examples from above:
> 
> - `R75,D30,R83,U83,L12,D49,R71,U7,L72
>     U62,R66,U55,R34,D71,R55,D58,R83` = `610` steps
> - `R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51
>     U98,R91,D20,R16,D67,R40,U7,R15,U6,R7` = `410` steps
> 
> _What is the fewest combined steps the wires must take to reach an intersection?_

## Considerations

It would be useful to extend the records of wire segments (now containing start-point and end-point) with the length of wire leading up to that point.
Alternatively, the same information can be updated in accumulators while searching for intersections, if doing a linear search in order - then there would be no need for extending the wire segment records.

