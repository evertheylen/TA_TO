# Docs voor deze fancy klasse :)

## DFA, NFA, eNFA

### WTF? Templates?

Geen nood. Volgende klassen zijn al gedefinieerd:

    s_DFA
    s_NFA
    s_eNFA (epsilon symbool is 'e')
    
Alle drie slagen ze staten op als strings, en symbolen als chars.

### Hoe te includen?

Je moet enkel FSM.h includen.

    #include "FSM.h"

Opgepast: boost moet geinstalleerd zijn op je machine!

### Hoe werkt het?

De klasse houdt staten bij als ints. Om de overzet te maken van een string naar een int, gebruik je:

    std::string realState(int ID);
    int ID(std::string name);

Voor de rest moet je je geen zorgen maken over hoe de klasse werkt :)
    
### Hoe te initialiseren?

Drie voorbeelden:

    s_DFA D({"q0", "q1", "q2"},  // states (numbered)
        {'a', 'b', 'c'},         // alphabet
        0, {2});                 // from -> {to}
    
    // the delta function must be manually set.
    D.set_delta(D.ID("q0"), 'b', D.ID("q1"));
    D.set_delta(D.ID("q1"), 'a', D.ID("q2"));
    
    
    
    s_NFA N({"q0", "q1", "q2"},  // states (numbered)
        {'a', 'b', 'c'},         // alphabet
        0, {2});                 // from -> {to} (numbered!)
    
    N.set_delta(0, 'b', {1,2});
    N.set_delta(1, 'a', {1,0});
    
    
    
    s_NFA E({"q0", "q1", "q2"},  // states (numbered)
        {'a', 'b', 'c'},         // alphabet
        0, {2});                 // from -> {to} (numbered!)
    
    E.set_delta(0, 'e', {1,2});  // epsilon!
    E.set_delta(1, 'a', {1,0});

Alle drie de klassen kunnen ook zonder parameters geconstruct worden.


### Hoe inlezen van xml?

Dat is hetzelfde voor alle drie de klassen.

    TiXmlDocument doc;
    doc.LoadFile("DFA.xml");
    s_DFA D;  // eerst (leeg) initialiseren!
    D.from_xml(doc);



### Hoe outputten naar dot?

Opnieuw, vrij simpel, hetzelfde voor alle klassen.

    // D is een s_DFA
    std::ofstream output_file;
    output_file.open("DFA.dot");
    D.to_dot(output_file);
    output_file.close();
    
    
### Meer stuff?

Voila, een soort van contract.

**DeltaResult** is het type wat delta teruggeeft: **int** voor een DFA, **std::set<int>** voor een NFA.
    
    int ID(std::string naam);
    
    std::string realState(int _ID);
    
    DeltaResult delta(int from, char symbol);
    
    void set_delta(int from, char symbol, DeltaResult to);
    
    int add_state(std::string name, bool end);

    bool lazy_evaluation();  // schrapt alle onnodige staten (hernummerd!)
                             // returnt of er effectief staten geschrapt zijn.
    
    bool isFinal(int ID);
    
    bool isInSigma(char symb);
    
Voor de eNFA is er natuurlijk ook nog:

    std::set<int> ECLOSE(int ID);
    
    void _ECLOSE(int ID, std::set<int> states);  // zal de ECLOSE van ID in de verzameling states zetten.
                                                 // betere performance dan de gewone ECLOSE.



## Runner (en Walker)

### WTF is dat nu weer?

Een klasse die de automaat kan overlopen.

### Cool! Hoe kan ik het gebruiken?

Je hebt opnieuw maar één bestand te includen.

    #include "Run.h"

Daarna kan je gewoon een Runner aanmaken als volgt:

    s_DFA D( .... );
    
    D.set_delta( .... );
    D.set_delta( .... );
    
    s_DFA_Runner r(D);
    
    bool accepted = r.process("abbcbbdbbabbaaa");  // geeft een bool weer of het geaccepteerd is.
    r.process("bbcb");  // je kan dit meerdere keren doen.

### Niet cool. Ik haat templates.

![wonka](http://weknowmemes.com/generator/uploads/generated/g1334269157528340013.jpg)
    
### WTF is Walker?

Een klasse gebruikt door Runner. Ik ga niet verder ingaan op de details, maar in principe kan een walker maar
karakter per karakter input verwerken, en kan je een walker ook niet zomaar hergebruiken.

Elke keer je `process(...)` uitvoert, zal er een Walker geinstantieerd worden die de gegeven input afgaat.

## Testen

Ik heb een testframework genaam CATCH gebruikt voor de tests. Het is zoals gtest, maar dan zonder de vreselijk irritante 
manier om te includen. Het zou zichzelf grotendeels moeten uitwijzen, mocht je geinteresseerd zijn in de tests.

Als er iemand nog wat tests wil schrijven, dat zou zeer geapprecieerd worden :).


