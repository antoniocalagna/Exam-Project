//
// Created by Alex on 25/05/2018.
//

#ifndef GRAPH2_GRAPH_HPP
#define GRAPH2_GRAPH_HPP

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

template<typename node_T, typename edge_T>
class Graph {
public:
  /**Aliases */
  using Nodes = std::unordered_set<node_T>;
  using NodeEdges = std::unordered_map<node_T, edge_T>;
  using Edges = std::unordered_map<node_T, NodeEdges>;
  
  /**Static members */
  static const edge_T no_edge;

private:
  /**Data */
  Nodes _nodes;
  Edges _edges;

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
size_t Graph<node_T, edge_T>::degree() const {
  //Ritorna il numero totale di archi nel grafo
  size_t count = 0;
  
  /* Deferenziare un iteratore di una mappa restituisce uno std::pair. Il primo elemento è la chiave, il secondo
   * è l'elemento mappato. */
  for (auto it = _edges.begin(); it != _edges.end(); it++) {
    count += (it->second).size();     //In questo caso vogliamo la size della mappa interna
  }
  
  return count;
}

template<typename node_T, typename edge_T>
edge_T Graph<node_T, edge_T>::edge(const node_T &starting_node, const node_T &target_node) const {
  //Ritorna una copia dell'arco (DIRETTO) tra starting_node e target_node.
  
  if (_nodes.find(starting_node) == _nodes.end() ||
      _nodes.find(starting_node) == _nodes.end())   //Controlla che siano entrambi presenti
    return no_edge;
  if (_edges.at(starting_node)
            .count(target_node) == 0)               //Controlla se esiste una connessione tra i due nodi
    return no_edge;
  
  return _edges.at(starting_node).at(target_node);
}

template<typename node_T, typename edge_T>
size_t Graph<node_T, edge_T>::inDegree(const node_T &node) const {
  //Ritorna il grado di ingresso del nodo richiesto
  //Ricerca il nodo
  if (_nodes.find(node) == _nodes.end())
    return 0;
  
  //Conta fli archi entranti in esso
  size_t count = 0;
  for (auto it = _edges.begin(); it != _edges.end(); it++) {
    count += _edges.at(it->first).count(node);
  }
  
  return count;
}

template<typename node_T, typename edge_T>
size_t Graph<node_T, edge_T>::outDegree(const node_T &node) const {
  //Ritorna il grado di uscita del nodo richiesto
  if (_nodes.find(node) == _nodes.end())      //Controlla che il nodo esista
    return 0;
  return _edges.at(node).size();
}

template<typename node_T, typename edge_T>
size_t Graph<node_T, edge_T>::outDegree_withEdge(const node_T &node, const edge_T &edge) const {
  if (_nodes.find(node) == _nodes.end())                        //Controlla che sia presente
    return 0;
  
  //Scorri tutti gli archi uscenti dal nodo e conta solo quelli con il valore richesto
  size_t count = 0;
  for (auto it = _edges.at(node).begin(); it != _edges.at(node).end(); it++) {
    if (it->second == edge)                       //L'elemento mappato si trova nel .second di uno std::pair
      count++;
  }
  
  return count;
}

template<typename node_T, typename edge_T>
size_t Graph<node_T, edge_T>::degree(const node_T &node) const {
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
    out[i] = *it;
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
  _edges.clear();
}

template<typename node_T, typename edge_T>
size_t Graph<node_T, edge_T>::find(const node_T &node) const {
  //Ritorna la posizione del nodo richiesto. Se il nodo non esiste, viene restituita la size di _nodes.
  return (size_t) std::distance(_nodes.begin(), _nodes.find(node));
}

template<typename node_T, typename edge_T>
void Graph<node_T, edge_T>::addNode(const node_T &new_node) {
  _nodes.insert(new_node);
  _edges[new_node] = NodeEdges();                        //Alloca anche la mappa
}

template<typename node_T, typename edge_T>
void Graph<node_T, edge_T>::editNode(const node_T &old_node, const node_T &new_node) {
  /* Uno std::unordered_set ha gli elementi costanti. Per modificare un nodo del grafo bisogna creare un nuovo nodo,
   * copiare gli archi di quello vecchio ed infine eliminare il nodo vecchio.
   */
  addNode(new_node);                                    //Aggiungi il nuovo nodo
  _edges.at(new_node) = _edges.at(old_node);            //Copia gli archi in uscita
  
  //Copia gli archi in entrata
  for (auto it = _edges.begin(); it != _edges.end(); it++) {
    if ((it->second).count(old_node) == 1)             //Controlla se l'elemento puntato ha una connessione com iò nodo
      (it->second)[new_node] = (it->second).at(old_node); //In questo caso copiane l'arco (usa [] per sicurezza)
  }
  popNode(old_node);                                  //Elimina il vecchio nodo
}

template<typename node_T, typename edge_T>
void Graph<node_T, edge_T>::popNode(const node_T &to_erase) {
  //Elimina un nodo
  if (_nodes.find(to_erase) == _nodes.end())
    return;                                          //Risparmiati tutte le operazioni
  //Cerca tutti gli archi entranti in questo nodo
  for (auto it = _edges.begin(); it != _edges.end(); it++) {
    if ((it->second).count(to_erase) != 0) {
      (it->second).erase(to_erase);
    }
  }
  _edges.erase(to_erase);
  _nodes.erase(to_erase);                    //Eliminalo dal set dei nodi
  return;
  
}

template<typename node_T, typename edge_T>
void Graph<node_T, edge_T>::setEdge(const node_T &starting_node, const node_T &target_node, const edge_T &new_value) {
  //Imposta il valore dell'arco (DIRETTO) tra starting_node e target_node con il valore richiesto.
  if (_nodes.find(starting_node) == _nodes.end() ||
      _nodes.find(target_node) == _nodes.end())             //Controlla che i nodi siano presenti
    return;
  
  if (new_value == no_edge) {                               //Implicita richiesta di eliminazione di un nodo
    _edges.at(starting_node).erase(target_node);
    return;
  }
  
  _edges.at(starting_node)[target_node] = new_value;
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
  for (auto it = _edges.at(starting_node).begin(); it != _edges.at(starting_node).end(); it++) {
    if (it->second == edge) {
      //In second è salvato il valore del contenuto mappato, in questo caso dell'arco (la chiave è l'elemento!)
      branches.push_back(it->first);
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
  this->_edges = to_copy._edges;
}

#endif //GRAPH2_GRAPH_HPP
