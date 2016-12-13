Domains
etat = vases(integer, integer)
liste_etats = etat*

Predicates
nondeterm ajouter(etat, liste_etats,liste_etats)
nondeterm appartient(etat, liste_etats)
nondeterm vases(integer, integer)
nondeterm transition(etat, etat)
nondeterm chercher(etat, liste_etats, liste_etats)
nondeterm reste(integer, integer, integer, integer)
nondeterm somme(integer, integer, integer)
nondeterm superieur(integer, integer, integer)
Clauses

vases(0, 0).

ajouter(X, L, [X|L]).

appartient(T, [T]).
appartient(T, [X|L]) :- T = X; appartient(T, L).

reste(A, B, X, Y) :- Y = A - X + B.
somme(A, B, Y) :- Y = A + B.
superieur(A, B, R) :- somme(A, B, S), S > R.

transition(vases(A, B), vases(0, B)) :- A > 0.
transition(vases(A, B), vases(A, 0)) :- B > 0.

transition(vases(A, B), vases(5, B)) :- A < 5.
transition(vases(A, B), vases(A, 3)) :- B < 3.

transition(vases(A, B), vases(Y, 3)) :- superieur(A, B, 3), reste(A, B, 3, Y).
transition(vases(A, B), vases(0, Y)) :- not(superieur(A, B, 3)), somme(A, B, Y).

transition(vases(A, B), vases(5, Y)) :- superieur(A, B, 5), reste(5, 3, 5, Y).
transition(vases(A, B), vases(Y, 0)) :- not(superieur(A, B, 5)), somme(A, B, Y).

chercher(E, [E|L], [E|L]).
% chercher(E, [T|L], R) :- transition(T, E2), not(appartient(E2, [T|L])), chercher(E, [E2|[T|L]], L2), R = L2.
chercher(E, L, R) :- L = [T|_], transition(T, E2), not(appartient(E2, L)), chercher(E, [E2|L], R).

Goal
chercher(vases(0, 2), [vases(0, 0)], R).