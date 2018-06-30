//
// Created by Alex on 25/05/2018.
//

#ifndef GRAPH2_GRAPH_HPP
#define GRAPH2_GRAPH_HPP

#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <string>

template<class node_T, class edge_T = bool>
class Graph {
public:
  /**Aliases */
  using Edges = std::map<const node_T *, edge_T>;
  using Nodes = std::map<const node_T, Edges>;
  /* Supponiamo di fissare un nodo. Tutti gli archi uscenti da esso possono essere rappresentati tramite una mappa che
   * colleghi il valore del nodo di arrivo al valore dell'arco che lo connette al nodo fissato. Questo compito è svolto
   * dal tipo Edges, che però sfrutta un pointer al nodo d'arrivo piuttosto che il valore del nodo stesso per
   * risparmiare memoria. Il tipo Nodes, invece, collega ogni nodo alla mappa dei propri archi.
   */
  
  /**Static members */
  static const edge_T no_edge;        //Costante che indica che due nodi non sono connessi. Utilizzata come valore speciale per alcune funzioni

private:
  /**Data */
  Nodes _nodes;
  
  /**Functions*/
  const node_T *
  _getPtr(const node_T &node) const;                          //Ritorna il pointer ad un nodo (utile per la struttura a mappe scelta)

public:
  /**Constructors & Destructor */
  Graph() = default;                                                        //Costruttore di default
  Graph(const Graph<node_T, edge_T> &to_copy);                              //Copy constructor
  ~Graph() = default;                                                       //Distruttore di default
  
  /**Getters */
  size_t nodesNumber() const;                                                //Numero totale di nodi
  size_t degree() const;                                                     //Numero totale di archi
  size_t degree(const node_T &node) const;                                   //Grado complessivo di un nodo
  size_t inDegree(const node_T &node) const;                                 //Grado input
  size_t outDegree(const node_T &node) const;                                //Grado output
  size_t outDegree_withEdge(const node_T &node,
                            const edge_T &edge) const;   //Grado output considerando solo un valore di archi
  edge_T edge(const node_T &starting_node, const node_T &target_node) const; //Ritorna l'arco tra due nodi
  
  /*References*/
  const node_T &node(unsigned int pos) const;                                 //Const reference al nodo in posizione pos
  std::vector<node_T> nodesVector() const;                                    //Ritorna un vettore contenente i nodi
  
  /**General*/
  void clear();
  size_t find(const node_T &node) const;                                      //Ritorna la posizione di un nodo
  void addNode(const node_T &new_node);                                       //Aggiungi un nodo
  void editNode(const node_T &old_node, const node_T &new_node);              //Modifica un nodo
  void popNode(const node_T &to_erase);                                       //Rimuovi un nodo
  //Imposta il valore di un arco UNIDIREZIONALE
  void setEdge(const node_T &starting_node, const node_T &target_node, const edge_T &new_value);
  //Imposta il valore di un arco BIDIREZIONALE
  void bsetEdge(const node_T &starting_node, const node_T &target_node, const edge_T &new_value);
  
  //Ritorna una lista di coppie nodo-arco uscenti/entranti dal nodo di partenza
  std::vector<std::pair<node_T, edge_T>> inwardsBranches(const node_T &starting_node) const;
  std::vector<std::pair<node_T, edge_T>> outwardsBranches(const node_T &starting_node) const;
  //Ritorna una lista di nodi connessi al nodo di partenza da un arco del valore richiesto
  std::vector<node_T> branches(const node_T &starting_node, const edge_T &edge) const;
  
  /**Operators */
  const node_T &operator[](unsigned int pos) const;                         //Accesso al nodo i-esimo (come la node)
  Graph<node_T, edge_T> &operator=(const Graph<node_T, edge_T> &to_copy);   //Assegnazione
};

/******************************************          IMPLEMENTATION          ******************************************/
template<class node_T, class edge_T>
const edge_T Graph<node_T, edge_T>::no_edge = edge_T();                     //Come valore speciale scegliamo il valore di default del tipo degli archi

/**#####################
 * ## Private methods ##
 * #####################
 */
template<class node_T, class edge_T>
const node_T *Graph<node_T, edge_T>::_getPtr(const node_T &node) const {
  auto it = _nodes.find(node);          //Cerca il nodo all'interno del grafo
  if (it == _nodes.end())               //Controlla che il nodo sia presente
    return nullptr;
  /* Ritorna l'indirizzo (&) del first dell'iteratore al nodo trovato. Questo perchè l'iteratore è un iteratore alla mappa
   * nodi - archi. In first è contenuta la chiave, cioè il nodo. Quello che ci interessa è il suo indirizzo in memoria.*/
  return &(it->first);
}

/**###############################
 * ## Constructors & Destructor ##
 * ###############################
 */

template<class node_T, class edge_T>
Graph<node_T, edge_T>::Graph(const Graph<node_T, edge_T> &to_copy) {
  *this = to_copy;                   //Utilizza l'operatore di assegnazione
}

/**
 * #############
 * ## Getters ##
 * #############
 */

template<class node_T, class edge_T>
size_t Graph<node_T, edge_T>::nodesNumber() const {
  return _nodes.size();             //Il numero dei nodi è la size della mappa nodi - archi
}

template<class node_T, class edge_T>
edge_T Graph<node_T, edge_T>::edge(const node_T &starting_node, const node_T &target_node) const {
  //Ritorna una copia dell'arco (DIRETTO) tra starting_node e target_node.
  const node_T *target_ptr = _getPtr(target_node);            //Cerca il pointer del nodo di arrivo
  if (_nodes.find(starting_node) == _nodes.end() ||
      target_ptr == nullptr)                                  //Controlla che entrambi i nodi siano presenti
    return no_edge;                                           //In tal caso ritorna un no_edge
  
  if (_nodes.at(starting_node)
            .count(target_ptr) == 0)                          //Controlla se esiste una connessione tra i due nodi
    return no_edge;
  
  return _nodes.at(starting_node).at(target_ptr);             //Ritorna l'arco contenuto nella mappa pointer - arco
}

template<class node_T, class edge_T>
size_t Graph<node_T, edge_T>::degree() const {
  //Ritorna il numero totale di archi nel grafo
  size_t count = 0;
  
  /* Deferenziare un iteratore di una mappa restituisce uno std::pair. Il primo elemento è la chiave, il secondo
   * è l'elemento mappato. */
  for (auto it = _nodes.begin(); it != _nodes.end(); it++) {
    //Sommiamo le dimensioni delle mappe pointer - arco associate ad ogni nodo
    count += (it->second).size();
  }
  
  return count;
}

template<class node_T, class edge_T>
size_t Graph<node_T, edge_T>::inDegree(const node_T &node) const {
  //Ritorna il grado di ingresso del nodo richiesto
  const node_T *node_ptr = _getPtr(node);                 //Cerca il nodo
  if (node_ptr == nullptr)                                //Controlla che il nodo esista
    return 0;
  
  //Conta gli archi entranti in esso
  size_t count = 0;
  for (auto it = _nodes.begin(); it != _nodes.end(); it++) {
    /* Conta le apparizioni del pointer al nodo richiesto all'interno di tutte le mappe pointer - archi, conta cioè
     * quante volte il nodo richiesto viene menzionato come nodo di arrivo di un arco. */
    count += (it->second).count(node_ptr);
  }
  
  return count;
}

template<class node_T, class edge_T>
size_t Graph<node_T, edge_T>::outDegree(const node_T &node) const {
  //Ritorna il grado di uscita del nodo richiesto
  if (_nodes.find(node) == _nodes.end())      //Controlla che il nodo esista
    return 0;
  return _nodes.at(node).size();              //Il grado di uscita è la dimensione della mappa ptr - archi di un nodo.
}

template<class node_T, class edge_T>
size_t Graph<node_T, edge_T>::outDegree_withEdge(const node_T &node, const edge_T &edge) const {
  if (_nodes.find(node) == _nodes.end())                        //Controlla che sia presente
    return 0;
  
  //Scorri tutti gli archi uscenti dal nodo e conta solo quelli con il valore richesto
  size_t count = 0;
  for (auto it = _nodes.at(node).begin(); it != _nodes.at(node).end(); it++) {
    if (it->second ==
        edge)                                     //L'elemento mappato si trova nel .second di uno std::pair
      count++;
  }
  
  return count;
}

template<class node_T, class edge_T>
size_t Graph<node_T, edge_T>::degree(const node_T &node) const {  //Il grado di un particolare nodo
  return inDegree(node) + outDegree(node);
}

/* #############
 * # Reference #
 * #############
 */

template<class node_T, class edge_T>
const node_T &Graph<node_T, edge_T>::node(unsigned int pos) const {
  auto it = _nodes.begin();
  
  //Non vengono eseguiti controlli (la richiesta di una posizione troppo alta restituirà un'errore delle mappe)
  std::advance(it, pos);
  return it->first;
}

template<class node_T, class edge_T>
std::vector<node_T> Graph<node_T, edge_T>::nodesVector() const {
  //Ritorna una lista dei nodi sotto forma di vettore
  std::vector<node_T> out(_nodes.size());             //Prepara un vettore della lunghezza corretta
  auto it = _nodes.begin();
  for (int i = 0; i < _nodes.size(); i++, it++) {     //Incrementa contatore e iteratore parallelamente
    out[i] = it->first;
  }
  return out;
}

/**#############
 * ## General ##
 * #############
 */

template<class node_T, class edge_T>
void Graph<node_T, edge_T>::clear() {
  _nodes.clear();                                   //Cancella tutto quanto, svuotando effettivamente il grafo
}

template<class node_T, class edge_T>
size_t Graph<node_T, edge_T>::find(const node_T &node) const {
  //Ritorna la posizione del nodo richiesto. Se il nodo non esiste, viene restituita la size di _nodes.
  return (size_t) std::distance(_nodes.begin(), _nodes.find(node));
}

template<class node_T, class edge_T>
void Graph<node_T, edge_T>::addNode(const node_T &new_node) {
  if (_nodes.find(new_node) != _nodes.end())        //Il nodo è già presente nel grafo
    return;
  
  /* Il nuovo nodo dovrà conteneree una mappa ai propri archi vuota. Possiamo crearne una appositamente */
  Edges construct_new;
  _nodes[new_node] = construct_new;
}

template<class node_T, class edge_T>
void Graph<node_T, edge_T>::editNode(const node_T &old_node, const node_T &new_node) {
  /* Uno std::unordered_set ha gli elementi costanti. Per modificare un nodo del grafo bisogna creare un nuovo nodo,
   * copiare gli archi di quello vecchio ed infine eliminare il nodo vecchio.
   */
  if (new_node == old_node) {                            //E' stato richiesta una modifica priva di significato
    return;
  }
  
  const node_T *old_ptr = _getPtr(old_node);
  if (old_ptr == nullptr)                                //Il nodo di cui è stata richiesta la modifica non esiste
    return;
  
  addNode(new_node);                                    //Aggiungi il nuovo nodo
  _nodes.at(new_node) = _nodes.at(old_node);            //Copia gli archi in uscita del vecchio nodo in quello nuovo
  
  //Copia gli archi in entrata:
  for (auto it = _nodes.begin(); it != _nodes.end(); it++) {
    if ((it->second).count(old_ptr) == 1) {             //Se c'è un arco tra un nodo e quello vecchio...
      edge_T e = edge(it->first, old_node);             //Copia il valore dell'arco
      setEdge(it->first, new_node, e);                  //Crea l'arco nodo -> new_node con lo stesso valore di quello sostituito
    }
  }
  //Elimina il vecchio nodo. Nella popNode vengono anche eliminati gli archi entranti nell'old_node che sono già stati sostituiti
  popNode(old_node);
}

template<class node_T, class edge_T>
void Graph<node_T, edge_T>::popNode(const node_T &to_erase) {
  //Elimina un nodo
  const node_T *erase_ptr = _getPtr(to_erase);
  if (erase_ptr == nullptr)                          //Controlla che il nodo sia effettivamente presente
    return;
  
  //Cerca ed elimina tutti gli archi entranti in questo nodo
  for (auto it = _nodes.begin(); it != _nodes.end(); it++) {
    (it->second).erase(erase_ptr);                  //Non ci sono problemi se viene richiesta la erase di una chiave non esistente
  }
  _nodes.erase(to_erase);                           //Eliminalo dalla mappa dei nodi (eliminando anche gli archi in uscita)
  return;
  
}

template<class node_T, class edge_T>
void Graph<node_T, edge_T>::setEdge(const node_T &starting_node, const node_T &target_node, const edge_T &new_value) {
  //Imposta il valore dell'arco (DIRETTO) tra starting_node e target_node con il valore richiesto.
  const node_T *target_ptr = _getPtr(target_node);
  if (_nodes.find(starting_node) == _nodes.end() ||
      target_ptr == nullptr)             //Controlla che i nodi siano presenti
    return;
  
  if (new_value == no_edge) {           //Implicita richiesta di eliminazione di un arco
    _nodes.at(starting_node).erase(target_ptr);
    return;
  }
  
  _nodes.at(starting_node)[target_ptr] = new_value;   //Assegna il nuovo valore dell'arco creando la nuova chiave
}

template<class node_T, class edge_T>
void Graph<node_T, edge_T>::bsetEdge(const node_T &starting_node, const node_T &target_node, const edge_T &new_value) {
  setEdge(starting_node, target_node, new_value);   //Chiama la setEdge due volte invertendo l'ordine dei nodi
  setEdge(target_node, starting_node, new_value);
}

template<class node_T, class edge_T>
std::vector<std::pair<node_T, edge_T>> Graph<node_T, edge_T>::inwardsBranches(const node_T &starting_node) const {
  std::vector<std::pair<node_T, edge_T>> branches;                //Prepara il vector da ritornare
  
  if (_nodes.find(starting_node) == _nodes.end()) {               //Controlla che il nodo sia presente
    return branches;
  }
  const node_T *node_ptr = _getPtr(starting_node);                //Prendi il pointer al nodo
  
  for (auto it = _nodes.begin(); it != _nodes.end(); it++) {
    if (it->second.count(node_ptr) == 1) {                        //Se esiste una connessione tra il nodo puntato da it e quello richiesto...
      branches.emplace_back(std::make_pair(it->first, it->second.at(node_ptr)));    //Metti in branches la coppia nodo - arco
    }
  }
  return branches;
}

template<class node_T, class edge_T>
std::vector<std::pair<node_T, edge_T>> Graph<node_T, edge_T>::outwardsBranches(const node_T &starting_node) const {
  std::vector<std::pair<node_T, edge_T>> branches;              //Prepara il vector da ritornare
  
  if (_nodes.find(starting_node) == _nodes.end()) {             //Il nodo non è presente
    return branches;
  }
  
  branches.resize(outDegree(starting_node));                    //Qui è possibile prevedere la dimensione del vettore
  int i = 0;
  for (auto it = _nodes.at(starting_node).begin(); it != _nodes.at(starting_node).end(); it++, i++) {
    branches[i] = std::make_pair(*(it->first), it->second);     //Scorri tutti i suoi archi in uscita e piazzali nel vettore
  }
  return branches;
}

template<class node_T, class edge_T>
std::vector<node_T> Graph<node_T, edge_T>::branches(const node_T &starting_node, const edge_T &edge) const {
  //Ritorna una lista di nodi connessi allo starting_node tramite un arco del valore richiesto
  std::vector<node_T> branches;                //Qui non è possibile conoscere la dimensione a priori
  
  if (edge == no_edge) { return branches; }    //E' stata richiesta la non connessione, torna il vettore vuoto
  
  if (_nodes.find(starting_node) == _nodes.end())
    return branches;                          //Nodo non trovato
  
  for (auto it = _nodes.at(starting_node).begin(); it != _nodes.at(starting_node).end(); it++) {
    if (it->second == edge) {
      branches.push_back(*(it->first));       //Scorri tutta la mappa dei nodi uscenti e mettili nel vettore
    }
  }
  return branches;
}

/**###############
 * ## Operators ##
 * ###############
 */

template<class node_T, class edge_T>
const node_T &Graph<node_T, edge_T>::operator[](unsigned int pos) const {
  return node(pos);
}

template<class node_T, class edge_T>
Graph<node_T, edge_T> &Graph<node_T, edge_T>::operator=(const Graph<node_T, edge_T> &to_copy) {
  this->_nodes = to_copy._nodes;
}

#endif //GRAPH2_GRAPH_HPP
