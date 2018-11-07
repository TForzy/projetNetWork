#include "network.h"
#include "random.h"
#include "iostream"

/*! Resizes the list of nodes (\ref values) */
    void Network::resize(const size_t& s){ //comment initialiser ?
      values.clear();
      for (int i(0); i< s;++i) { values.push_back(RNG.uniform_double(0,1));}
    }
/*! Adds a bidirectional link between two nodes
  @param[in] a,b the indexes if the two nodes
  @param[out] success the link was succesfully inserted
 */
    bool Network::add_link(const size_t& a, const size_t& b)
    {
      //on test si le lien existe déjà ou bien si a et b ont des valeurs aberrantes
      if (a>=size() or b>=size() or a==b)
      {
        return false;
      }
      for (auto voisin : neighbors(a))
      {
        if (voisin == b){return false;}//On a pas besoin de tester dans l'autre sens
      }
      links.insert(std::make_pair(a,b));
      links.insert(std::make_pair(b,a));
      return true;
    }

/*! Creates random connections between nodes: each node *n* will be linked with
*degree(n)* other nodes (randomly chosen) where *degree(n)* is Poisson-distributed.
  All previous links are cleared first.
  @param mean_deg the average of the Poisson distribution.
 */
    size_t Network::random_connect(const double& mean_deg)
    {
       links.clear();
       int somme_connections(0);
       size_t deg(0);

       for (int i(0); i < size(); ++i)
       {
         //Si on demande en moyenne 2 liens par noeud à la fin de la création :
         /*int D = (RNG.poisson(mean_deg) - degree(i));
         if (D>=0) {deg = D;} else {deg=0;}*/
         //Sinon, si on veut créer en moyenne 2 liens par noeud :
         deg = (RNG.poisson(mean_deg));
         if (deg >= size()){deg=size()-1;}
         if (deg >= (size()-neighbors(i).size())) {deg = (size() - neighbors(i).size() - 1);}
         //on teste que le nombre de liens à créer n'est possible

         while (deg>0)
         {
           RNG.initialize();
           int k (RNG.uniform_double(0,size()-1));
           if (add_link(i,k))
           {
             ++somme_connections;
             --deg;
           }
         }
       }
       return somme_connections;
    }
/*! Resets all node values.
  @param[in] vector of new node values
  @param[out] number of nodes succesfully reset
 */
    size_t Network::set_values(const std::vector<double>& vect){
    int s;
    int compteur_setting(0);
    if (vect.size()<=size()){ s = vect.size();}
    else { s = size();}
    for (int i(0); i<s; ++i)
    {
      values[i]=(vect[i]);
      ++compteur_setting;
    }
     return compteur_setting;
    }

/*! Number of nodes */
    size_t Network::size() const{
      return (values.size());
    }
/*! Degree (number of links) of node no *n* */
    size_t Network::degree(const size_t& n) const{
        return (links.count(n));
    }
/*! Value of node no *n* */
    double Network::value(const size_t& n) const{
      return values[n];
    }
/*! All node values in descending order */

    std::vector<double> Network::sorted_values() const
    {
	    std::vector<double> descending_order(values);
	    std::sort(descending_order.begin(), descending_order.end());
	    std::reverse(descending_order.begin(), descending_order.end());
	    return descending_order;
    }

/*! All neighbors (linked) nodes of node no *n* */
    std::vector<size_t> Network::neighbors(const size_t& n) const{
      std::vector<size_t>voisins;
      for (auto element : links)
      {
        if (element.first == n) voisins.push_back(element.second);//renvoie direct le tableau des noeuds associés à la clé n
      }
      return voisins;
    }
