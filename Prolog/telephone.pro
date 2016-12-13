/*
Syst�me de production pour calculer le prix d'un appel
======================================================
Remarque : Pour simplifier le passage d'une tranche de tarifs � une autre (commencer un appel � 17h45 et le finir � 18h20 pendant un lundi par exemple), on pose deux cat�gories :
      - Cat�gorie 1 : La plus ch�re, d�finie pendant les heures actives des jours de la semaine
      - Cat�gorie 2 : La moins ch�re, d�finie pendant les heures inactives des jours de la semaine et les weekend
Le programme calcule alors la dur�e que l'appel prend pendant chaque tranche (pour notre exemple 15 minutes avant 18h et 20 minutes apr�s 18h) et fait le calcul en multipliant chaque dur�e par son tarif correspondant. 
On suppose que la dur�e d'un appel ne d�passe pas 12 heures = 720 minutes pour que cela fonctionne correctement.
*/

predicates 
nondeterm tarif_base(symbol, symbol, real, integer)		
nondeterm tarif_par_minute(symbol, symbol, integer, real)	
nondeterm nb_minutes_cat1(symbol, symbol, symbol, integer, integer, integer, integer)
nondeterm calculer_prix(symbol, symbol, integer, integer, integer)
nondeterm resultat(real, real)
nondeterm entre(integer, integer, integer, integer, integer, integer).
nondeterm calculer_resultat(real, real, real, real, real, real).
nondeterm ihm.

clauses
%%  BASE DES FAITS  %%
% Tarifs de base
tarif_base(C, A, T, M) :- C = "A", A = "local", T = 15, M = 2;
			  C = "A", A = "interprovincial", T = 25, M = 1;
			  C = "B", A = "local", T = 6, M = 0;
			  C = "B", A = "interprovincial", T = 15, M = 0.

% Tarifs par minutes
tarif_par_minute(C, A, Cat, T) :- C = "A", A = "local", Cat = 1, T = 2;
				  C = "A", A = "local", Cat = 2, T = 1;
				  C = "A", A = "interprovincial", Cat = 1, T = 40;
				  C = "A", A = "interprovincial", Cat = 2, T = 20;
				  C = "B", A = "local", Cat = 1, T = 5;
				  C = "B", A = "local", Cat = 2, T = 2;
				  C = "B", A = "interprovincial", Cat = 1, T = 44;
				  C = "B", A = "interprovincial", Cat = 2, T = 18.


%%  REGLES DE PRODUCTION  %%


% Calculer le nombre de minutes qui seront comptabilis�es dans le tarif cher (Categorie 1) en fonction du jour de la semaine de le compagnie
nb_minutes_cat1(C, A, J, H, M, D, N) :- D <= 0, N = 0, !;
					J = "samedi", N = 0, !; J = "dimanche", N = 0, !;	% On utilise le cut pour ne plus consid�rer le cas de J = samedi ou dimanche
				  	C = "A", A = "local",         	entre(8, 18, H, M, D, N), !;
				  	C = "A", A = "interprovincial", entre(8, 22, H, M, D, N), !;
				  	C = "B", A = "local",         	entre(8, 20, H, M, D, N), !;
				  	C = "B", A = "interprovincial", entre(8, 20, H, M, D, N), !.

% X : heure du commencement de la cat�gorie 1, Y : heure de la fin de cat�gorie 1
entre(X, Y, H, M, D, N) :- H >= X, H * 60 + M + D <= Y * 60, N = D, !.				% Appel commence apr�s X et se termine avant Y  = Tout l'appel pendant [X,Y]
entre(X, _, H, M, D, N) :- H <= X, H * 60 + M + D <= X * 60, N = 0, !.				% Appel commence et se termine avant X = Tout l'appel pendant [0, X]
entre(_, Y, H, M, D, N) :- H >= Y, H * 60 + M + D >= Y * 60, N = 0, !.				% Appel commence et se termine apr�s Y = Tout l'appel pendant [Y, minuit]
entre(X, Y, H, M, D, N) :- H <= X, H * 60 + M + D <= Y * 60, N = H * 60 + M + D - X * 60, !. 	% Appel commence avant X et se termine avant Y 
entre(X, Y, H, M, D, N) :- H >= X, H * 60 + M + D >= Y * 60, N = Y * 60 - (H * 60 + M), !.	% Appel commence avant Y et se termine apr�s Y


% Calculer le prix total pour Compagnie A et Compagnie B
calculer_prix(A, J, H, M, D) :- tarif_base("A", A, TMA, MIA),
				D1 = D - MIA,
				nb_minutes_cat1("A", A, J, H, M, D1, M1),
				M2 = D1 - M1,
				tarif_par_minute("A", A, 1, TA1),
				tarif_par_minute("A", A, 2, TA2),
				calculer_resultat(TMA, M1, TA1, M2, TA2, PA),
				
				tarif_base("B", A, TMB, MIB),
				D2 = D - MIB,
				nb_minutes_cat1("B", A, J, H, M, D2, N1),
				N2 = D2 - N1,
				tarif_par_minute("B", A, 1, TB1),
				tarif_par_minute("B", A, 2, TB2),
				calculer_resultat(TMB, N1, TB1, N2, TB2, PB),

				resultat(PA, PB).

% Cout total = Tarif minimum + Minutes dans Cat1 * Tarif Cat1 + Minutes dans Cat2 * Tarif Cat2
calculer_resultat(TM, P1, T1, P2, T2, R) :- R = TM + P1 * T1 + P2 * T2.

% Affichage de r�sultat
resultat(PA, PB) :- PA <= PB,  write("         Recommandation : S�lectionner A (", PA, " centimes) au lieu de B (", PB, " centimes)."), nl.
resultat(PA, PB) :- PA > PB,   write("         Recommandation : S�lectionner B (", PB, " centimes) au lieu de A (", PA, " centimes)."), nl.

% Interface utilisateur
ihm :-  write("Type d'appel       (local/interprovincial)  :  "), readln(A),
	write("Jour de l'appel    (lundi .. dimanche)      :  "), readln(J),
	write("Heure de l'appel   (entre 0 et 23)           :  "), readint(H),
	write("Minute de l'appel  (entre 0 et 59)           :  "), readint(M),
	write("Dur�e de l'appel   (en minutes)              :  "), readint(D),
	calculer_prix(A, J, H, M, D),
	nl, write("             R�essayer (oui/non)   : "), readln(R), R = "oui", nl, nl, ihm.

goal
ihm.