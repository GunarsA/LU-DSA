from __future__ import annotations

import random
from math import floor

# File
OUT_FILE = "server_big.in"
SOLUTION_FILE = "server_big.out"

NODE_COUNT = 20_000  # max 20_000
DIAMETER = 6       # less -> Denser graph

class nCounter : count = 1
RADIUS = floor( DIAMETER / 2 )
if DIAMETER % 2 == 0: RADIUS -= 1

Graph : dict[int, list[int]] = dict()

def gen_walk(start : int, prev : int):
    dist = 0
    current = start
    while True:
        if dist >= RADIUS: break
        if nCounter.count > NODE_COUNT: break
        v_next = random.choice(Graph[current])
        if(v_next != prev and random.randint(0, 1) == 0 ):
            # Walk
            prev = current
            current = v_next
            dist += 1
        else:
            # Gen
            v_next = nCounter.count
            nCounter.count += 1
            Graph[current].append(v_next)
            Graph[v_next] = [current]
            prev = current
            current = v_next
            dist += 1
    pass

def getDim(start):
    dist = 0
    current = start
    while True:
        if dist >= RADIUS: break
        if nCounter.count > NODE_COUNT: assert False
        new_vert = nCounter.count
        nCounter.count += 1
        Graph[current].append(new_vert)
        Graph[new_vert] = [current]
        current = new_vert
        dist += 1

def main():
    ofile_inp = open(OUT_FILE, "wt")
    ofile_sol = open(SOLUTION_FILE, "wt")

    # Center
    centers : list[int] = list()
    assert (DIAMETER >= 3)
    if DIAMETER % 2 == 1:
        centers.append(nCounter.count)
        Graph[nCounter.count] = list()
        nCounter.count += 1
    else :
        c1 = nCounter.count
        centers.append(c1)
        nCounter.count += 1
        centers.append(nCounter.count)
        Graph[c1] = [nCounter.count]
        Graph[nCounter.count] = [c1]
        nCounter.count += 1

    # And now we walk
    if DIAMETER % 2 == 1:
        getDim(centers[0])
        getDim(centers[0])
        while True:
            if nCounter.count > NODE_COUNT: break
            gen_walk(centers[0], 0)
    else:
        getDim(centers[0])
        getDim(centers[1])
        while True:
            if nCounter.count > NODE_COUNT: break
            sp = random.randint(0, 1)
            gen_walk(centers[sp], centers[(sp + 1) % 2])

    # Write to file
    max_edge_count = 0
    write_set : set[tuple[int, int]] = set()
    for vert, edge_list in Graph.items():
        if max_edge_count < len(edge_list):
            max_edge_count = len(edge_list)
        for edge in edge_list:
            e1 = (vert, edge)
            e2 = (edge, vert)
            if( (e1 in write_set) or (e2 in write_set)): continue
            if(random.randint(0, 1) == 0):
                write_set.add( e1 )
            else:
                write_set.add( e2 )

    # Stats
    print(f"R: {floor( DIAMETER / 2 )}")
    print(f"C: {centers}")
    print(f"Max Edges: {max_edge_count}")
    print(f"Line Count: {len(write_set)}, Vert Count: {NODE_COUNT}")
    # print(f"Set: {write_set}")
    # print(f"\nGraph: {Graph}")

    # To File Input
    ofile_inp.write(f"{NODE_COUNT}\n")
    for l in write_set:
        ofile_inp.write(f"{l[0]} {l[1]}\n")
    ofile_inp.write(f"0 0\n")
    # To File Solution
    if(DIAMETER % 2 == 1):
        ofile_sol.write(f"{floor( DIAMETER / 2 )}\n{centers[0]}")
    else:
        ofile_sol.write(f"{floor( DIAMETER / 2 )}\n{centers[0]} {centers[1]}\n")

    ofile_sol.close()
    ofile_inp.close()

if __name__ == "__main__":
    main()

    # A 	.- 	0 	-----
    # B 	-... 	1 	.----
    # C 	-.-. 	2 	..---
    # D 	-.. 	3 	...--
    # E 	. 	4 	....-
    # F 	..-. 	5 	.....
    # G 	--. 	6 	-....
    # H 	.... 	7 	--...
    # I 	.. 	8 	---..
    # J 	.--- 	9 	----.
    # K 	-.- 	. 	.-.-.-
    # L 	.-.. 	, 	--..--
    # M 	-- 	: 	---...
    # N 	-. 	? 	..--..
    # O 	--- 	- 	-....-
    # P 	.--. 	/ 	-..-.
    # Q 	--.- 	Tukšums ‘ ‘ 	.----.
    # R 	.-. 	Simbola beigas 	|
    # S 	...
    # T 	-
    # U 	..-
    # V 	...-
    # W 	.--
    # X 	-..-
    # Y 	-.--
    # Z 	--..

    # SOS - GLABIET MUSU DVESELES.
    # ...|---|...|.----.|-....-|.----.|--.|.-..|.-|-...|..|.|-|.----.|--|..-|...|..-|.----.|-..|...-|.|...|.|.-..|.|...|.-.-.-|
    #
