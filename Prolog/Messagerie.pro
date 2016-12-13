Predicates
nondeterm cout_initial(symbol, string, string, integer)
nondeterm supplement_poids(integer, integer)
nondeterm supplement_delai(string, integer)
nondeterm cout_lettre(string, string, string, integer)
nondeterm cout_colis(string, string, string, integer, integer)
nondeterm questions(string, integer)

Clauses
cout_initial(T, "madrid", "barcelone", C)  :- T = lettre, C =  700; T = colis, C = 1200.
cout_initial(T, "madrid", "toledo", C)     :- T = lettre, C =  300; T = colis, C =  800.
cout_initial(T, "madrid", "badajos", C)    :- T = lettre, C =  500; T = colis, C = 1000.
cout_initial(T, "barcelone", "cadiz", C)   :- T = lettre, C = 1000; T = colis, C = 1600.
cout_initial(T, "barcelone", "gerona", C)  :- T = lettre, C =  300; T = colis, C =  800.
cout_initial(T, "barcelone", "badajos", C) :- T = lettre, C =  800; T = colis, C = 1500.

cout_initial(T, X, Y, C) :- cout_initial(T, Y, X, C), !.

supplement_delai("m", 1000).
supplement_delai("d", 0).

supplement_poids(P, 0) :- P <= 2000.
supplement_poids(P, S) :- P > 2000, S = (P-2000)*100.

cout_lettre(VO, VD, Delai, Cout) :- cout_initial(lettre, VO, VD, CI), 
				    supplement_delai(Delai, S), 
				    Cout = CI + S.

cout_colis(VO, VD, Delai, Poids, Cout) :- cout_initial(colis, VO, VD, CI), 
					  supplement_delai(Delai, S1), 
					  supplement_poids(Poids, S2), 
					  Cout = CI + S1 + S2.

questions("lettre", Cout) :- write("Ville d'origine ? "), readln(VO),
			     write("Ville de destination ? "), readln(VD),
		   	     write("Même jour ou jours différents (tapez m ou d) ? "), readln(Delai),
		   	     cout_lettre(VO, VD, Delai, Cout).

questions("colis", Cout) :- write("Ville d'origine ? "), readln(VO),
			    write("Ville de destination ? "), readln(VD),
		   	    write("Même jour ou jours différents (tapez m ou d) ? "), readln(Delai),
			    write("Poids du colis en gramme ? "), readint(Poids),
			    cout_colis(VO, VD, Delai, Poids, Cout).

Goal
questions("colis", Cout).