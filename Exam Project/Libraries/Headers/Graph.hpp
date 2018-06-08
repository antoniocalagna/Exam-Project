//
// Created by Alex on 25/05/2018.
//

#ifndef GRAPH2_GRAPH_HPP
#define GRAPH2_GRAPH_HPP

#include <vector>
#include <map>
#include <algorithm>

template<typename node_T, typename edge_T>
class Graph {
public:
  /**Aliases */
  using NodeEdges = std::map<std::size_t, edge_T>;
  using Nodes = std::vector<node_T>;
  using Edges = std::vector<NodeEdges>;
  
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
  std::size_t nodesNumber() const;                                                  //Numero di nodi
  std::size_t degree() const;                                                 //Numero di archi
  std::size_t degree(const node_T &node) const;                               //Grado complessivo di un nodo
  std::size_t inDegree(const node_T &node) const;                             //Grado input
  std::size_t outDegree(const node_T &node) const;                            //Grado output
  edge_T edge(const node_T &starting_node, const node_T &target_node) const;  //Arco tra due nodi
  
  /*References*/
  node_T &node(unsigned int pos);                                             //Reference al nodo in posizione pos
  const std::vector<node_T> &nodesVector() const;                             //Ritorna una const reference ai nodi
  
  /**General*/
  void clear();
  std::size_t find(const node_T &node) const;   //Ritorna la posizione di un nodo
  void addNode(const node_T &new_node);         //Aggiungi un nodo
  void popNode(const node_T &to_erase);         //Rimuovi un nodo
  //Imposta il valore di un arco UNIDIREZIONALE
  void setEdge(const node_T &starting_node, const node_T &target_node, const edge_T &new_value);
  //Imposta il valore di un arco BIDIREZIONALE
  void bsetEdge(const node_T &starting_node, const node_T &target_node, const edge_T &new_value);
  
  //Ritorna una lista di nodi connessi al nodo di partenza da un arco del valore richiesto
  std::vector<node_T> branches(const node_T &starting_node, const edge_T &edge);
  
  /**Operators */
  node_T &operator[](unsigned int pos);
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
std::size_t Graph<node_T, edge_T>::nodesNumber() const {
  return _nodes.size();
}

template<typename node_T, typename edge_T>
std::size_t Graph<node_T, edge_T>::degree() const {
  //Ritorna il numero totale di archi nel grafo
  std::size_t count = 0;
  
  for (int i = 0; i < _edges.size(); i++) {
    count += (std::size_t) std::distance(_edges[i].begin(), _edges[i].end());
  }
  
  return count;
}

template<typename node_T, typename edge_T>
edge_T Graph<node_T, edge_T>::edge(const node_T &starting_node, const node_T &target_node) const {
  //Ritorna una copia dell'arco (DIRETTO) tra starting_node e target_node.
  //Trova le posizioni dei due archi
  std::size_t pos_starting = find(starting_node);
  std::size_t pos_target = find(target_node);
  
  if (pos_starting == _nodes.size() ||
      pos_target == _nodes.size()) { return no_edge; } //Controlla che entrambi esistano
  
  if (_edges[pos_starting].count(pos_target) == 0) { return no_edge; }   //Controlla che esista un arco tra i nodi
  /* La .count(const key_type& k) delle map conta il numero di elementi che hanno quella chiave. */
  
  return _edges[pos_starting].at(pos_target);
  //Bisogna usare la funzione .at() perchè l'operatore [] non è const
}

template<typename node_T, typename edge_T>
std::size_t Graph<node_T, edge_T>::inDegree(const node_T &node) const {
  //Ritorna il grado di ingresso del nodo richiesto
  //Ricerca il nodo
  std::size_t pos = find(node);
  
  //Conta i nodi entranti in esso
  std::size_t count = 0;
  for (int i = 0; i < _edges.size(); i++) {
    count += _edges[i].count(pos);
  }
  
  return count;
}

template<typename node_T, typename edge_T>
std::size_t Graph<node_T, edge_T>::outDegree(const node_T &node) const {
  //Ritorna il grado di uscita del nodo richiesto
  //Ricerca il nodo
  std::size_t pos = find(node);
  return _edges[pos].size();
}

template<typename node_T, typename edge_T>
std::size_t Graph<node_T, edge_T>::degree(const node_T &node) const {
  return inDegree(node) + outDegree(node);
}

/* #############
 * # Reference #
 * #############
 */

template<typename node_T, typename edge_T>
node_T &Graph<node_T, edge_T>::node(unsigned int pos) {
  return _nodes[pos];
}

template<typename node_T, typename edge_T>
const std::vector<node_T> &Graph<node_T, edge_T>::nodesVector() const {
  /*
   * Questa funzione ritorna una const reference in maniera tale da:
   * -Risparmiare memoria quando le funzioni lavorano sul vettore dei nodi se si passa questa funzione come parametro
   * -Proteggere i dati del grafo nel caso in cui il ritorno della funzione venga assegnato ad una reference non const
   * -Permettere la normale copia del vettore
   */
  return _nodes;
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
std::size_t Graph<node_T, edge_T>::find(const node_T &node) const {
  /* Ritorna la posizione del nodo richiesto all'interno del vettore. Se il nodo non esiste, ritorna
   * la size() del vettore (cioè la posizione successiva all'ultimo elemento)
   */
  //Ricerca il nodo e ritorna la distanza del suo iteratore dal _nodes.begin()
  return (std::size_t) std::distance(_nodes.begin(), std::find(_nodes.begin(), _nodes.end(), node));
}

template<typename node_T, typename edge_T>
void Graph<node_T, edge_T>::addNode(const node_T &new_node) {
  _nodes.push_back(new_node);
  _edges.resize(_nodes.size());
}

template<typename node_T, typename edge_T>
void Graph<node_T, edge_T>::popNode(const node_T &to_erase) {
  //Elimina il nodo dal grafo
  std::size_t pos = find(to_erase);
  if (pos == _nodes.size()) { return; }    //Controlla che il nodo esista
  
  _nodes.erase(_nodes.begin() + pos);
  _edges.erase(_edges.begin() + pos);
}

template<typename node_T, typename edge_T>
void Graph<node_T, edge_T>::setEdge(const node_T &starting_node, const node_T &target_node, const edge_T &new_value) {
  //Imposta il valore dell'arco (DIRETTO) tra starting_node e target_node con il valore richiesto.
  std::size_t pos_starting = find(starting_node);
  std::size_t pos_target = find(target_node);
  
  if (pos_starting == _nodes.size() ||
      pos_target == _nodes.size()) { return; } //Controlla che entrambi esistano
  
  if (new_value == Graph<node_T, edge_T>::no_edge) {
    //Implicita richiesta di eliminazione del nodo
    _edges[pos_starting].erase(pos_target);
    return;
  }
  
  _edges[pos_starting][pos_target] = new_value;
}

template<typename node_T, typename edge_T>
void Graph<node_T, edge_T>::bsetEdge(const node_T &starting_node, const node_T &target_node, const edge_T &new_value) {
  setEdge(starting_node, target_node, new_value);
  setEdge(target_node, starting_node, new_value);
}

template<typename node_T, typename edge_T>
std::vector<node_T> Graph<node_T, edge_T>::branches(const node_T &starting_node, const edge_T &edge) {
  //Ritorna una lista di nodi connessi allo starting_node tramite un arco del valore richiesto
  std::vector<node_T> branches;
  
  if (edge == Graph<node_T, edge_T>::no_edge) { return branches; }    //E' stata richiesta la non connessione
  
  //Controlla che il nodo richiesto esista
  std::size_t pos = find(starting_node);
  
  if (pos == _nodes.size()) { return branches; }  //Nodo non trovato
  
  for (auto it = _edges[pos].begin(); it != _edges[pos].end(); it++) {
    if (it->second == edge) {
      //In second è salvato il valore del contenuto mappato, in questo caso dell'arco (la chiave è l'indice!)
      //In first, invece, è salvata la chiave, che in questo caso è corrispondente alle posizioni nel vettore!
      branches.push_back(_nodes[it->first]);
    }
  }
  
  return branches;
}

/**###############
 * ## Operators ##
 * ###############
 */

template<typename node_T, typename edge_T>
node_T &Graph<node_T, edge_T>::operator[](unsigned int pos) {
  return node(pos);
}

template<typename node_T, typename edge_T>
Graph<node_T, edge_T> &Graph<node_T, edge_T>::operator=(const Graph<node_T, edge_T> &to_copy) {
  this->_nodes = to_copy._nodes;
  this->_edges = to_copy._edges;
}

#endif //GRAPH2_GRAPH_HPP
