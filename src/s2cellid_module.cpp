#include <RcppCommon.h>
#include "s2/s2cellid.h"
#include "s2/s2latlng.h"
#include "s2package_types.h"
#include <Rcpp.h>
#include <unordered_map>
#include <string>

S2CellId S2CellId_parent(S2CellId* c, int level){
  if(c->level() < level) Rcpp::stop("Supplied level larger than current level!");
  return c->parent(level);
}

// //' @export neighbours
// //[[Rcpp::export]]
// Rcpp::List neighbours(std::string token, int level){
//   std::vector<S2CellId> neighbours;
//   S2CellId id = S2CellId::FromToken(token);
//   id.AppendVertexNeighbors(level, &neighbours);
//   int n = neighbours.size();
//   Rcpp::List list( n ) ;
//   for( int j=0; j<n; j++) list[j] = neighbours[j] ;
//   return list;
// }

Rcpp::List neighbours(S2CellId* id, int level, bool token){
  std::vector<S2CellId> neighbours;
  id->AppendVertexNeighbors(level, &neighbours);
  int n = neighbours.size();
  Rcpp::List list( n ) ;
  for( int j=0; j<n; j++){
    if(token){
      list[j] = neighbours[j].ToToken();
    } else{
      list[j] = neighbours[j];
    }
  }
  return list;
}

// [[Rcpp::export]]
std::unordered_multimap<std::string, int> maptokens(Rcpp::StringVector x) {
  std::unordered_multimap<std::string, int> map;
  // std::vector<std::string> tokens(x);
  
  int n = x.size();
  for (int i = 0; i < n; i++) {
    map.insert(std::make_pair(std::string(x[i]), i));
  }
  
  return map;
}

Rcpp::List potentialneighbours(std::vector<S2Point> x, double R){
  int level = S2::kMinWidth.GetMaxLevel(4*R);
  int N = x.size();
  std::vector<S2CellId> ids(N);
  typedef std::unordered_multimap<S2CellId, int> IdMap;
  IdMap map;
  for (int i = 0; i < N; i++) {
    ids[i] = S2CellId::FromPoint(x[i]).parent(level);
    map.insert(std::make_pair(ids[i], i));
  }
  std::vector<int> J;
  std::vector<int> I;
  std::vector<double> dist;
  std::vector<S2CellId> neighbours;
  double d, jj;
  for (int i = 0; i < N; i++) {
    ids[i].AppendVertexNeighbors(level, &neighbours);
    int m = neighbours.size();
    for( int j=0; j<m; j++){
      auto r = map.equal_range(neighbours[j]);
      for (auto it = r.first; it != r.second; ++it) {
        jj = it->second;
        if(jj > i){
        // if(jj != i){
          d = acos(x[i].DotProd(x[jj]));
          if(d <= R){
            // CAUTION: Adding 1 to have R indices stating at 1 rather than 0.
            I.push_back(i+1);
            J.push_back(jj + 1);
            dist.push_back(d);
          }
        }
      }
    }
    neighbours.clear();
  }
  return Rcpp::List::create(Rcpp::Named("i")=I,
                            Rcpp::Named("j")=J,
                            Rcpp::Named("d")=dist);
}

RCPP_MODULE(S2CellId_module){
  using namespace Rcpp;
  
  class_<S2CellId>("S2CellId")
  .constructor()
  .method("face", &S2CellId::face)
  .method("level", &S2CellId::level)
  .method("parent", &S2CellId_parent)
  .method("ToPoint", &S2CellId::ToPoint)
  .method("ToString", &S2CellId::ToString)
  .method("ToToken", &S2CellId::ToToken)
  .method("VertexNeighbours", &neighbours)
  ;
  
  function("S2CellIdFromLatLng", &S2CellId::FromLatLng, "Docstring");
  function("S2CellIdFromPoint", &S2CellId::FromPoint);
  function("potentialneighbours", &potentialneighbours);
}
