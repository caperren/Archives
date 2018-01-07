% Here are our initial facts describing the Griffin's family tree.
female(thelma).
female(babs).
female(lois).
female(carol).
female(meg).

male(frances).
male(mickey).
male(carter).
male(peter).
male(patrick).
male(adam).
male(stewie).
male(chris).
male(carolBaby).

married(frances, thelma).
married(carter, babs).
married(peter, lois).
married(adam, carol).

% a predicate that defines an inverse relationship between people who are
% married.
marriedI(X,Y) :- married(X,Y).
marriedI(X,Y) :- married(Y,X).

parent(frances, peter).
parent(thelma, peter).
parent(mickey, peter).

parent(babs, lois).
parent(carter, lois).
parent(babs, patrick).
parent(carter, patrick).
parent(babs, carol).
parent(carter, carol).

parent(lois, stewie).

parent(peter, stewie).
parent(lois, meg).
parent(peter, meg).
parent(lois, chris).
parent(peter, chris).

parent(carol, carolBaby).
parent(adam, carolBaby).

% Part 1. Family relations.

% 1. Define a predicate `child/2` that inverts the parent relationship.
child(X, Y) :- parent(Y, X).

% 2. Define two predicates `isMother/1` and `isFather/1`.
isMother(X) :- parent(X, _), female(X).
isFather(X) :- parent(X, _), male(X).

% 3. Define a predicate `grandparent/2`.
grandparent(X, Y) :- parent(X, Z), parent(Z, Y).


% 4. Define a predicate `sibling/2`, where siblings share ar least one parent.
sibling(X, Y) :- parent(Z, X), parent(Z, Y), X \= Y.

% 5. Define two predicates `sister/2` and `brother/2`.
sister(X, Y) :- sibling(X, Y), female(X), X \= Y.
brother(X, Y) :- sibling(X, Y), male(X), X \= Y.


% 6. Define a predicate `siblingsInLaw/2`, where a sibling-in-law is either
%    married to a sibling or the sibling of a spouse.
siblingsInLaw(X, Y) :- marriedI(Y, Z), sibling(X, Z) ; marriedI(X, Z), sibling(Y, Z).


% 7. Define two predicates `aunt/2` and `uncle/2` (these should include aunts and
%    uncles who are related by marriage).
aunt(X, Y) :- female(X), parent(Z, Y), sibling(X, Z) ; female(X), parent(Z, Y), siblingsInLaw(X, Z).
uncle(X, Y) :- male(X), parent(Z, Y), sibling(X, Z) ; male(X), parent(Z, Y), siblingsInLaw(X, Z).



% 8. Define the predicate `cousin/2`.
cousin(X, Y) :- parent(Z, X), sibling(Z, A), parent(A, Y).


% 9. Define the predicate `ancestor/2`.
ancestor(X, Y) :- parent(X, Y) ; parent(Z, Y), ancestor(X, Z).


% Extra credit: define the predicate `related/2`.
related(X, Y) :- marriedI(X, Y) ;
                 sibling(X, Y) ;
                 siblingsInLaw(X, Y);
                 aunt(X, Y) ;
                 aunt(Y, X) ;
                 uncle(X, Y) ;
                 uncle( Y, X) ;
                 cousin(X, Y) ;
                 ancestor(X, Y) ;
                 ancestor( Y, X).
