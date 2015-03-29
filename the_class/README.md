# Docs voor deze fancy klasse :)

## WTF? Templates?

Geen nood. Volgende klassen zijn al gedefinieerd:

    s_DFA
    s_NFA
    s_eNFA (epsilon symbool is 'e')
    
Alle drie slagen ze staten op als strings, en symbolen als chars.

## Hoe te includen?

Je moet enkel FSM.h includen.

    #include <FSM.h>

## Hoe werkt het?

De klasse houdt staten bij als ints. Om de overzet te maken van een string naar een int, gebruik je:

    std::string realState(int ID);
    int ID(std::string name);

Voor de rest moet je je geen zorgen maken over hoe de klasse werkt :)
    
## Hoe te initialiseren?

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


## Hoe inlezen van xml?

Dat is hetzelfde voor alle drie de klassen.

    TiXmlDocument doc;
    doc.LoadFile("DFA.xml");
    s_DFA D;  // eerst (leeg) initialiseren!
    D.from_xml(doc);



## Hoe outputten naar dot?

Opnieuw, vrij simpel, hetzelfde voor alle klassen.

    // D is een s_DFA
    std::ofstream output_file;
    output_file.open("DFA.dot");
    D.to_dot(output_file);
    output_file.close();
    
    
## Meer stuff?

Voila, een soort van contract (**DeltaResult** is het type wat delta teruggeeft: int voor een DFA, std::vector<int> voor een NFA)
    
    int ID(std::string naam);
    
    std::string realState(int _ID);
    
    DeltaResult delta(int from, char symbol);
    
    void set_delta(int from, char symbol, DeltaResult to);
    
    int add_state(std::string name, bool end);

    bool lazy_evaluation();  // schrapt alle onnodige staten (hernummerd!)
                             // returnt of er effectief staten geschrapt zijn.
    
Voor de eNFA is er natuurlijk ook nog:

    std::set<int> ECLOSE(int ID);

