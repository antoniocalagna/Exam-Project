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

template<typename node_T, typename edge_T>
class Graph {
public:
  /**Aliases */
  using Edges = std::map<const node_T * const, edge_T>;
  using Nodes = std::map<const node_T, Edges>;
  /* Supponiamo di fissare un nodo. Tutti gli archi uscenti da esso possono essere rappresentati tramite una mappa che
   * colleghi il valore del nodo di arrivo al valore dell'arco che lo connette al nodo fissato. Questo compito è svolto
   * dal tipo Edges, che però sfrutta un pointer al nodo d'arrivo piuttosto che il valore del nodo stesso per
   * risparmiare memoria. Il tipo Nodes, invece, collega ogni nodo alla mappa dei propri archi.
   */
  
  /**Static members */
  static const edge_T no_edge;

private:
  /**Data */
  Nodes _nodes;
  
  /**Functions*/
  const node_T *const _getPtr(const node_T &node) const;

public:
  /**Constructors & Destructor */
  Graph() = default;
  Graph(const Graph<node_T, edge_T> &to_copy);
  ~Graph() = default;
  
  /**Getters */
  size_t nodesNumber() const;                                                //Numero di nodi
  size_t degree() const;                                                     //Numero di archi
  size_t degree(const node_T &node) const;                                   //Grado complessivo di un nodo
  size_t inDegree(const node_T &node) const;                                 //Grado input
  size_t outDegree(const node_T &node) const;                                //Grado output
  size_t outDegree_withEdge(const node_T &node, const edge_T &edge) const;   //Grado output considerando solo un peso
  edge_T edge(const node_T &starting_node, const node_T &target_node) const; //Arco tra due nodi
  
  /*References*/
  const node_T &node(unsigned int pos) const;                                 //Const reference al nodo in posizione pos
  std::vector<node_T> nodesVector() const;                                    //Ritorna un vettore contenente i nodi
  
  /**General*/
  void clear();
  size_t find(const node_T &node) const;                                      //Ritorna la posizione di un nodo
  void addNode(const node_T &new_node);                                       //Aggiungi un nodo
  void editNode(const node_T &old_node, const node_T &new_node);               //Modifica un nodo
  void popNode(const node_T &to_erase);                                       //Rimuovi un nodo
  //Imposta il valore di un arco UNIDIREZIONALE
  void setEdge(const node_T &starting_node, const node_T &target_node, const edge_T &new_value);
  //Imposta il valore di un arco BIDIREZIONALE
  void bsetEdge(const node_T &starting_node, const node_T &target_node, const edge_T &new_value);
  
  //Ritorna una lista di nodi connessi al nodo di partenza da un arco del valore richiesto
  std::vector<node_T> branches(const node_T &starting_node, const edge_T &edge) const;
  
  /**Operators */
  const node_T &operator[](unsigned int pos) const;
  Graph<node_T, edge_T> &operator=(const Graph<node_T, edge_T> &to_copy);
};

/******************************************          IMPLEMENTATION          ******************************************/
template<typename node_T, typename edge_T>
const edge_T Graph<node_T, edge_T>::no_edge = edge_T();

/**#####################
 * ## Private methods ##
 * #####################
 */
template<typename node_T, typename edge_T>
const node_T *const Graph<node_T, edge_T>::_getPtr(const node_T &node) const {
  auto it = _nodes.find(node);
  if (it == _nodes.end())
    return nullptr;
  return &(it->first);
}

/**###############################
 * ## Constructors & Destructor ##
 * ###############################
 */

template<typename node_T, typename edge_T>
Graph<node_T, edge_T>::Graph(const Graph<node_T, edge_T> &to_copy) {
  *this = to_copy;
}

/**
 * #############
 * ## Getters ##
 * #############
 */

template<typename node_T, typename edge_T>
size_t Graph<node_T, edge_T>::nodesNumber() const {
  return _nodes.size();
}

template<typename node_T, typename edge_T>
edge_T Graph<node_T, edge_T>::edge(const node_T &starting_node, const node_T &target_node) const {
  //Ritorna una copia dell'arco (DIRETTO) tra starting_node e target_node.
  const node_T *const target_ptr = _getPtr(target_node);
  if (_nodes.find(starting_node) == _nodes.end() ||
      target_ptr == nullptr)                                 //Controlla che entrambi i nodi siano presenti
    return no_edge;
  
  if (_nodes.at(starting_node)
            .count(target_ptr) == 0)               //Controlla se esiste una connessione tra i due nodi
    return no_edge;
  
  return _nodes.at(starting_node).at(target_ptr);
}

template<typename node_T, typename edge_T>
size_t Graph<node_T, edge_T>::degree() const {
  //Ritorna il numero totale di archi nel grafo
  size_t count = 0;
  
  /* Deferenziare un iteratore di una mappa restituisce uno std::pair. Il primo elemento è la chiave, il secondo
   * è l'elemento mappato. */
  for (auto it = _nodes.begin(); it != _nodes.end(); it++) {
    count += (it->second).size();     //In questo caso vogliamo la size della mappa interna
  }
  
  return count;
}

template<typename node_T, typename edge_T>
size_t Graph<node_T, edge_T>::inDegree(const node_T &node) const {
  //Ritorna il grado di ingresso del nodo richiesto
  const node_T *const node_ptr = _getPtr(node);
  //Ricerca il nodo
  if (node_ptr == nullptr)
    return 0;
  
  //Conta fli archi entranti in esso
  size_t count = 0;
  for (auto it = _nodes.begin(); it != _nodes.end(); it++) {
    count += (it->second).count(node_ptr);
  }
  
  return count;
}

template<typename node_T, typename edge_T>
size_t Graph<node_T, edge_T>::outDegree(const node_T &node) const {
  //Ritorna il grado di uscita del nodo richiesto
  if (_nodes.find(node) == _nodes.end())      //Controlla che il nodo esista
    return 0;
  return _nodes.at(node).size();
}

template<typename node_T, typename edge_T>
size_t Graph<node_T, edge_T>::outDegree_withEdge(const node_T &node, const edge_T &edge) const {
  if (_nodes.find(node) == _nodes.end())                        //Controlla che sia presente
    return 0;
  
  //Scorri tutti gli archi uscenti dal nodo e conta solo quelli con il valore richesto
  size_t count = 0;
  for (auto it = _nodes.at(node).begin(); it != _nodes.at(node).end(); it++) {
    if (it->second == edge)                                     //L'elemento mappato si trova nel .second di uno std::pair
      count++;
  }
  
  return count;
}

template<typename node_T, typename edge_T>
size_t Graph<node_T, edge_T>::degree(const node_T &node) const {  //Il grado di un particolare nodo
  return inDegree(node) + outDegree(node);
}

/* #############
 * # Reference #
 * #############
 */

template<typename node_T, typename edge_T>
const node_T &Graph<node_T, edge_T>::node(unsigned int pos) const {
  auto it = _nodes.begin();
  
  //Non vengono eseguiti controlli (la richiesta di una posizione troppo alta restituirà un'errore delle mappe)
  std::advance(it, pos);
  return *it;
}

template<typename node_T, typename edge_T>
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

template<typename node_T, typename edge_T>
void Graph<node_T, edge_T>::clear() {
  _nodes.clear();
}

template<typename node_T, typename edge_T>
size_t Graph<node_T, edge_T>::find(const node_T &node) const {
  //Ritorna la posizione del nodo richiesto. Se il nodo non esiste, viene restituita la size di _nodes.
  return (size_t) std::distance(_nodes.begin(), _nodes.find(node));
}

template<typename node_T, typename edge_T>
void Graph<node_T, edge_T>::addNode(const node_T &new_node) {
  if (_nodes.find(new_node) != _nodes.end())                  //Il nodo è già presente nel grafo
    return;
  _nodes[new_node] = Edges();                                //Inizializza con il costruttore di default le connessioni del nuovo nodo
}

template<typename node_T, typename edge_T>
void Graph<node_T, edge_T>::editNode(const node_T &old_node, const node_T &new_node) {
  /* Uno std::unordered_set ha gli elementi costanti. Per modificare un nodo del grafo bisogna creare un nuovo nodo,
   * copiare gli archi di quello vecchio ed infine eliminare il nodo vecchio.
   */
  const node_T *const new_ptr = _getPtr(new_node);
  const node_T *const old_ptr = _getPtr(old_node);
  if (old_ptr == nullptr)                                //Il nodo di cui è stata richiesta la modifica non esiste
    return;
  
  addNode(new_node);                                    //Aggiungi il nuovo nodo
  _nodes.at(new_node) = _nodes.at(old_node);            //Copia gli archi in uscita
  
  //Copia gli archi in entrata
  for (auto it = _nodes.begin(); it != _nodes.end(); it++) {
    if((it->second).count(old_ptr) == 1) {
      edge_T e = edge(it->first, old_node);             //Copia il valore dell'arco tra il nodo puntato da it e il nodo da modificare
      setEdge(it->first, old_node, no_edge);            //Elimina l'arco verso il nodo da modificare
      setEdge(it->first, new_node, e);                  //Crea un arco con lo stesso valore verso il nuovo nodo
    }
  }
  popNode(old_node);                                  //Elimina il vecchio nodo
}

template<typename node_T, typename edge_T>
void Graph<node_T, edge_T>::popNode(const node_T &to_erase) {
  //Elimina un nodo
  const node_T *const erase_ptr = _getPtr(to_erase);
  if (erase_ptr == nullptr)                          //Cerca se il nodo è effettivamente presente
    return;                                          //Risparmiati tutte le operazioni
  
  //Cerca ed elimina tutti gli archi entranti in questo nodo
  for (auto it = _nodes.begin(); it != _nodes.end(); it++) {
      (it->second).erase(erase_ptr);
  }
  _nodes.erase(to_erase);                    //Eliminalo dal set dei nodi
  return;
  
}

template<typename node_T, typename edge_T>
void Graph<node_T, edge_T>::setEdge(const node_T &starting_node, const node_T &target_node, const edge_T &new_value) {
  //Imposta il valore dell'arco (DIRETTO) tra starting_node e target_node con il valore richiesto.
  const node_T *const target_ptr = _getPtr(target_node);
  if (_nodes.find(starting_node) == _nodes.end() ||
      target_ptr == nullptr)             //Controlla che i nodi siano presenti
    return;
  
  if (new_value == no_edge) {                               //Implicita richiesta di eliminazione di un nodo
    _nodes.at(starting_node).erase(target_ptr);
    return;
  }
  
  _nodes.at(starting_node)[target_ptr] = new_value;
}

template<typename node_T, typename edge_T>
void Graph<node_T, edge_T>::bsetEdge(const node_T &starting_node, const node_T &target_node, const edge_T &new_value) {
  setEdge(starting_node, target_node, new_value);
  setEdge(target_node, starting_node, new_value);
}

template<typename node_T, typename edge_T>
std::vector<node_T> Graph<node_T, edge_T>::branches(const node_T &starting_node, const edge_T &edge) const {
  //Ritorna una lista di nodi connessi allo starting_node tramite un arco del valore richiesto
  std::vector<node_T> branches;                //Qui non è possibile conoscere la dimensione a priori
  
  if (edge == no_edge) { return branches; }    //E' stata richiesta la non connessione
  
  if (_nodes.find(starting_node) == _nodes.end())
    return branches;                          //Nodo non trovato
  
  for (auto it = _nodes.at(starting_node).begin(); it != _nodes.at(starting_node).end(); it++) {
    if (it->second == edge) {
      //In second è salvato il valore del contenuto mappato, in questo caso dell'arco (la chiave è l'elemento!)
      branches.push_back(*(it->first));
    }
  }
  return branches;
}

/**###############
 * ## Operators ##
 * ###############
 */

template<typename node_T, typename edge_T>
const node_T &Graph<node_T, edge_T>::operator[](unsigned int pos) const {
  return node(pos);
}

template<typename node_T, typename edge_T>
Graph<node_T, edge_T> &Graph<node_T, edge_T>::operator=(const Graph<node_T, edge_T> &to_copy) {
  this->_nodes = to_copy._nodes;
}

#endif //GRAPH2_GRAPH_HPP
