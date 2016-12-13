Domains
etat = s(symbol, symbol, symbol, symbol, symbol,  symbol, symbol, symbol, symbol).
etats = etat*

Predicates
nondeterm s(symbol, symbol, symbol, symbol, symbol,symbol, symbol, symbol, symbol).
nondeterm transition(etat, etat).
nondeterm final(etat).
nondeterm face(symbol).
nondeterm resoudre(etats, etats).
nondeterm visite(etat, etats).
nondeterm writelist(etats).
printstate(etat).

Clauses
final(s(x,v,x,v,v,v,a,v,a)).
face(x).
face(a).
face(v).

s(E1, E2, E3, E4, E5, E6, E7, E8, E9) :- face(E1), face(E2), face(E3), face(E4), face(E5), face(E6), face(E7), face(E8), face(E9).

transition(s(E1,E2,E3,E4,E5,v,E7,E8,E9), s(v,E2,E3,E4,E5,E1,E7,E8,E9)) :- not(E1 = v).
transition(s(E1,E2,E3,E4,E5,E6,E7,v,E9), s(v,E2,E3,E4,E5,E6,E7,E1,E9)) :- not(E1 = v).

transition(s(E1,E2,E3,E4,E5,E6,v,E8,E9), s(E1,v,E3,E4,E5,E6,E2,E8,E9)) :- not(E2 = v).
transition(s(E1,E2,E3,E4,E5,E6,E7,E8,v), s(E1,v,E3,E4,E5,E6,E7,E8,E2)) :- not(E2 = v).

transition(s(E1,E2,E3,v,E5,E6,E7,E8,E9), s(E1,E2,v,E3,E5,E6,E7,E8,E9)) :- not(E3 = v).
transition(s(E1,E2,E3,E4,E5,E6,E7,v,E9), s(E1,E2,v,E4,E5,E6,E7,E3,E9)) :- not(E3 = v).

transition(s(E1,E2,v,E4,E5,E6,E7,E8,E9), s(E1,E2,E4,v,E5,E6,E7,E8,E9)) :- not(E4 = v).
transition(s(E1,E2,E3,E4,E5,E6,E7,E8,v), s(E1,E2,E3,v,E5,E6,E7,E8,E4)) :- not(E4 = v).

transition(s(v,E2,E3,E4,E5,E6,E7,E8,E9), s(E6,E2,E3,E4,E5,v,E7,E8,E9)) :- not(E6 = v).
transition(s(E1,E2,E3,E4,E5,E6,v,E8,E9), s(E1,E2,E3,E4,E5,v,E6,E8,E9)) :- not(E6 = v).

transition(s(E1,v,E3,E4,E5,E6,E7,E8,E9), s(E1,E7,E3,E4,E5,E6,v,E8,E9)) :- not(E7 = v).
transition(s(E1,E2,E3,E4,E5,v,E7,E8,E9), s(E1,E2,E3,E4,E5,E7,v,E8,E9)) :- not(E7 = v).

transition(s(v,E2,E3,E4,E5,E6,E7,E8,E9), s(E8,E2,E3,E4,E5,E6,E7,v,E9)) :- not(E8 = v).
transition(s(E1,E2,v,E4,E5,E6,E7,E8,E9), s(E1,E2,E8,E4,E5,E6,E7,v,E9)) :- not(E8 = v).

transition(s(E1,v,E3,E4,E5,E6,E7,E8,E9), s(E1,E9,E3,E4,E5,E6,E7,E8,v)) :- not(E9 = v).
transition(s(E1,E2,E3,v,E5,E6,E7,E8,E9), s(E1,E2,E3,E9,E5,E6,E7,E8,v)) :- not(E9 = v).

%resoudre([F|Ss], [F|Ss]) :- final(F),!.
resoudre([S|Ss], [F, S|Ss]) :- final(F), transition(S, F), !, writelist([F, S|Ss]), !.
resoudre([S|Ss], R) :- transition(S, Z), not(visite(Z, Ss)), resoudre([Z,S|Ss], R), !.

writelist([]) :- nl.
writelist([S|Ss]) :- printstate(S), nl, writelist(Ss).

printstate(s(E1, E2, E3, E4, E5, E6, E7, E8, E9)) :- write("[ ", E1, " | ", E2, " | ", E3, " ]"), nl,
						     write("[ ", E4, " | ", E5, " | ", E6, " ]"), nl,
						     write("[ ", E7, " | ", E8, " | ", E9, " ]"), nl.

visite(S, [Z|Ss]) :- Z = S, !; visite(S, Ss).

Goal
resoudre([s(a,v,a,v,v,v,x,v,x)], H).