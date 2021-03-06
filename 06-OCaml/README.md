# Task 2019/06

> ## Day 6: Universal Orbit Map
> 
> You've landed at the Universal Orbit Map facility on Mercury. Because navigation in space often involves transferring between orbits, the orbit maps here are useful for finding efficient routes between, for example, you and Santa. You download a map of the local orbits (your puzzle input).
> 
> Except for the universal Center of Mass (`COM`), every object in space is in orbit around exactly one other object. An orbit looks roughly like this:
> ```
>                   \
>                    \
>                     |
>                     |
> AAA--> o            o <--BBB
>                     |
>                     |
>                    /
>                   /
> ```
> In this diagram, the object `BBB` is in orbit around `AAA`. The path that `BBB` takes around `AAA` (drawn with lines) is only partly shown. In the map data, this orbital relationship is written `AAA)BBB`, which means "`BBB` is in orbit around `AAA`".
> 
> Before you use your map data to plot a course, you need to make sure it wasn't corrupted during the download. To verify maps, the Universal Orbit Map facility uses _orbit count checksums_ - the total number of _direct orbits_ (like the one shown above) and _indirect orbits_.
> 
> Whenever `A` orbits `B` and `B` orbits `C`, then `A` _indirectly orbits_ `C`. This chain can be any number of objects long: if `A` orbits `B`, `B` orbits `C`, and `C` orbits `D`, then `A` indirectly orbits `D`.
> 
> For example, suppose you have the following map:
> ```
> COM)B
> B)C
> C)D
> D)E
> E)F
> B)G
> G)H
> D)I
> E)J
> J)K
> K)L
> ```
> Visually, the above map of orbits looks like this:
> ```
>         G - H       J - K - L
>        /           /
> COM - B - C - D - E - F
>                \
>                 I
> ```
> In this visual representation, when two objects are connected by a line, the one on the right directly orbits the one on the left.
> 
> Here, we can count the total number of orbits as follows:
> 
> - `D` directly orbits `C` and indirectly orbits `B` and `COM`, a total of `3` orbits.
> - `L` directly orbits `K` and indirectly orbits `J`, `E`, `D`, `C`, `B`, and `COM`, a total of `7` orbits.
> - `COM` orbits nothing.
> 
> The total number of direct and indirect orbits in this example is `42`.
> 
> _What is the total number of direct and indirect orbits_ in your map data?

## Considerations

The _center of mass_ object will always be identified as `COM`. Should there be any objects not orbiting `COM`, they won't be counted.

The overall problem can be modelled on a tree rooted on `COM`, with `COM`'s immediate orbiters being the root's direct children, and so on for the further objects. It is assumed that there will be no cycles in this graph.

The solution, _total number of direct and indirect orbits for all objects_ is given by the sum of all tree-nodes' depth (distance from root). Some edges will be counted multiple times.

A child node's depth is equal to its parent's depth plus `1`.

A _tree_ data structure would be difficult to construct from the provided input (list of randomly ordered items) - will instead construct a _hash table_.

The complete algo may look like this:

- for each input entry `AAA)BBB` insert/update a record for `AAA` with a list of satellites - initially `BBB`.
- for each child of `COM` compute its depth as `COM`'s depth (0) plus 1,
- and repeat that depth-computation for all children of each child of `COM`, and so on, recursively
- in the above process, keep track the sum of all depths in an accumulator - that will be the output value when all the nodes are traversed

I/O interface: can feed `input.txt` (list of those `AAA)BBB` entries) into _stdin_, and output the checksum onto _stdout_.


> ## Part Two
> 
> Now, you just need to figure out how many _orbital transfers_ you (`YOU`) need to take to get to Santa (`SAN`).
> 
> You start at the object `YOU` are orbiting; your destination is the object `SAN` is orbiting. An orbital transfer lets you move from any object to an object orbiting or orbited by that object.
> 
> For example, suppose you have the following map:
> ```
> COM)B
> B)C
> C)D
> D)E
> E)F
> B)G
> G)H
> D)I
> E)J
> J)K
> K)L
> K)YOU
> I)SAN
> ```
> Visually, the above map of orbits looks like this:
> ```
>                           YOU
>                          /
>         g - h       J - K - l
>        /           /
> com - b - c - D - E - f
>                \
>                 I - SAN
> ```
> In this example, `YOU` are in orbit around `K`, and `SAN` is in orbit around `I`. To move from `K` to `I`, a minimum of `4` orbital transfers are required:
> 
> - `K` to `J`
> - `J` to `E`
> - `E` to `D`
> - `D` to `I`
> 
> Afterward, the map of orbits looks like this:
> ```
>         G - H       J - K - L
>        /           /
> COM - B - C - D - E - F
>                \
>                 I - SAN
>                  \
>                   YOU
> ```
> _What is the minimum number of orbital transfers required_ to move from the object `YOU` are orbiting to the object `SAN` is orbiting? (Between the objects they are orbiting - _not_ between `YOU` and `SAN`.)

## Considerations

Need to navigate from a leaf towards the root - requires the data representation of leaves to have a link to parent node.

The algo may look like this:

- construct the table like before, but additionally with child->parent references
- trace path _A_ from `YOU`'s parent to the root `COM`, and trace path _B_ from `SAN`'s parent to `COM`
- eliminate common nodes between the two paths, (but must not lose the last common node)
- the lengths of the remaining parts of paths _A_ and _B_ added up is the result

The paths _A_ and _B_ mentioned above can be represented nicely by a singly linked list, with the order of nodes like so: `COM::B::...::YOU::[]`.