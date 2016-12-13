Predicates

nondeterm prof(symbol)
nondeterm matiere(symbol)
nondeterm jour(symbol)
nondeterm cours(symbol, symbol, symbol)

nondeterm profs_differents(symbol, symbol, symbol)
nondeterm jours_differents(symbol, symbol, symbol)
nondeterm posterieur(symbol, symbol)

nondeterm solution(symbol, symbol, symbol, symbol, symbol, symbol)

Clauses
matiere(logique). 	matiere(maths). 	matiere(programmation).
prof(saida). 		prof(farid). 		prof(khaled).
jour(lundi). 		jour(mercredi). 	jour(jeudi).

cours(saida, programmation, _).
cours(farid, _, J) :- not(J = lundi).
cours(_, M, lundi) :- not(M = logique).

profs_differents(P1, P2, P3) :- P1 <> P2, P2 <> P3, P1 <> P3.
jours_differents(J1, J2, J3) :- J1 <> J2, J2 <> J3, J1 <> J3.
posterieur(J1, J2) :- J1 = mercredi, J2 = lundi; J1 = jeudi, J2 = mercredi; J1 = jeudi, J2 = lundi.

solution(PP, JP, PM, JM, PL, JL) :-     prof(PL),jour(JL), cours(PL, logique, JL),
					prof(PM),jour(JM), cours(PM, maths, JM),
					prof(PP),jour(JP), cours(PP, programmation, JP),
					profs_differents(PL, PM, PP),
					jours_differents(JL, JM, JP),
					posterieur(JP, JL).

Goal 
solution(PorfProg, JourProg, ProfMaths, JourMaths, ProfLogique, JourLogique).