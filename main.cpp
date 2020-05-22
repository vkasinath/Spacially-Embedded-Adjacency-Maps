#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <cmath> 
#include <list>
#include <iomanip>
#include <algorithm>
#define earthRadiusKm 6371.0
#ifndef M_PI
#define M_PI 3.14
#endif

//TODO: You must fill in all ?????? with the correct types.
typedef unsigned int ID_TYPE; //Type for user IDs
typedef std::pair<double, double> COORD_TYPE; //Type for a coordinate (latitude and longitude)
typedef std::map<ID_TYPE, std::list<ID_TYPE> > ADJ_TYPE; //Adjacency Lists type
typedef std::map<ID_TYPE, std::list<COORD_TYPE> > GEO_TYPE; //Positional "dictionary"

//Function forward declarations. DO NOT CHANGE these. 
double deg2rad(double deg);
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d);
void loadConnections(ADJ_TYPE& adj_lists, std::ifstream& loadfile);
void loadLocations(GEO_TYPE& locations, std::ifstream& loadfile);
void printAllUsersWithinDistance(const GEO_TYPE& locations, std::ofstream& outfile,
                                 const ID_TYPE& start_id, double max_distance);
void printDegreesHistogram(const ADJ_TYPE& adj_lists, std::ofstream& outfile);
void printDegreesOfAll(const ADJ_TYPE& adj_lists, std::ofstream& outfile);
void printFriendsWithDegree(const ADJ_TYPE& adj_lists, std::ofstream& outfile,
                            const ID_TYPE& start_id, unsigned int degree);
void printFriendsWithinDistance(const ADJ_TYPE& adj_lists, const GEO_TYPE& locations, 
                                std::ofstream& outfile, const ID_TYPE& start_id, 
                                double max_distance);
void printUsersWithinIDRange(std::ofstream& outfile, ADJ_TYPE::const_iterator begin, 
                             ADJ_TYPE::const_iterator end, 
                             ADJ_TYPE::const_iterator start_it, unsigned int offset);


// DO NOT CHANGE THIS FUNCTION
int main(int argc, char** argv){
    ADJ_TYPE adj_lists;
    GEO_TYPE locations;

    if(argc != 3){
        std::cout << "Correct usage is " << argv[0] 
                  << " [commands file] [output file]" << std::endl;
        return -1;
    }

    std::ifstream commands(argv[1]);
    if(!commands){
        std::cerr << "Problem opening " << argv[1] << " for reading!" 
                  << std::endl;
        return -1;
    }

    std::ofstream outfile(argv[2]);
    if(!outfile){
        std::cerr << "Problem opening " << argv[2] << " for writing!" 
                  << std::endl;
        return -1;
    }

    std::string token; //Read the next command
    while(commands >> token){
        if(token == "load-connections"){
            std::string filename;
            commands >> filename;

            std::ifstream loadfile(filename.c_str());
            if(!loadfile){
                std::cerr << "Problem opening " << filename 
                          << " for reading connections." << std::endl;
                return -1;
            }

            loadConnections(adj_lists, loadfile);
        }
        else if(token == "load-locations"){
            std::string filename;
            commands >> filename;

            std::ifstream loadfile(filename.c_str());
            if(!loadfile){
                std::cerr << "Problem opening " << filename 
                          << " for reading locations." << std::endl;
                return -1;
            }

            loadLocations(locations, loadfile);
        }
        else if(token == "print-degrees"){
            printDegreesOfAll(adj_lists, outfile);
        }
        else if(token == "print-degree-histogram"){
            printDegreesHistogram(adj_lists, outfile);
        }
        else if(token == "all-users-within-distance"){
            ID_TYPE start_id;
            double max_distance; //In kilometers
            commands >> start_id >> max_distance;

            printAllUsersWithinDistance(locations, outfile, start_id, 
                                        max_distance);
        }
        else if(token == "friends-within-distance"){
            ID_TYPE start_id;
            double max_distance; //In kilometers
            commands >> start_id >> max_distance;

            printFriendsWithinDistance(adj_lists, locations, outfile, 
                                       start_id, max_distance);
        }
        else if(token == "friends-with-degree"){
            ID_TYPE start_id,degree;
            commands >> start_id >> degree;

            printFriendsWithDegree(adj_lists, outfile, start_id, degree);
        }
        else if(token == "nodes-within-ID-range"){
            ID_TYPE start_id,offset;
            commands >> start_id >> offset;

            printUsersWithinIDRange(outfile, adj_lists.begin(), adj_lists.end(),
                                    adj_lists.find(start_id), offset);
        }
        else{
            std::cerr << "Unknown token \"" << token << "\"" << std::endl;
            return -1;
        }
    }

    return 0;
}




// This function converts decimal degrees to radians
// From https://stackoverflow.com/a/10205532
// DO NOT CHANGE THIS FUNCTION
double deg2rad(double deg) {
  return (deg * M_PI / 180);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * Taken from https://stackoverflow.com/a/10205532
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 * DO NOT CHANGE THIS FUNCTION
 */
double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d) {
  double lat1r, lon1r, lat2r, lon2r, u, v;
  lat1r = deg2rad(lat1d);
  lon1r = deg2rad(lon1d);
  lat2r = deg2rad(lat2d);
  lon2r = deg2rad(lon2d);
  u = sin((lat2r - lat1r)/2);
  v = sin((lon2r - lon1r)/2);
  return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

//////////////////TODO: IMPLEMENT ALL FUNCTIONS BELOW THIS POINT////////////////////

/**
 * Loads a list of connections in the format "a b" meaning b is a friend of a
 * into the adjacency lists data structure. a and b are IDs.
 * @param adj_lists Adjacency lists structure
 * @param loadfile File to read from
 */
void loadConnections(ADJ_TYPE& adj_lists, std::ifstream& loadfile){

  ID_TYPE k, v;

  while (loadfile >> k >> v){
    if(adj_lists.find(k) == adj_lists.end()){
      std::list<ID_TYPE> t;
      t.push_back(v);
      adj_lists.insert({k, t});
    }
    else
      adj_lists[k].push_back(v);
  }

  // std::cout << "\n*** Connections Data ***" << std::endl;
  // for(auto k1 : adj_lists)
  // {
  //   std::cout << k1.first << " is friended by (";
  //     for(auto v1 : k1.second)    //print all v's associated with k
  //       std::cout << " " << v1;
  //     std::cout << ")" << std::endl;
  // }

}

/**
 * Loads a list of locations in the format "id latitude longitude"
 * @param locations Location lookup table
 * @param loadfile File to read from
 */
void loadLocations(GEO_TYPE& locations, std::ifstream& loadfile){

  COORD_TYPE c;
  ID_TYPE k;

  while (loadfile >> k >> c.first >> c.second){
    if (locations.find(k) != locations.end())
      locations[k].push_back(c);
    else{
      std::list<COORD_TYPE> t;
      t.push_back(c);
      locations.insert({k, t});
    }
  }

  // std::cout << "\n*** Locations Data ***" << std::endl;
  // for(auto k1 : locations)
  // {
  //   std::cout << k1.first << " is located at (" << k1.second.first << ", " << k1.second.second << ")";

  //   std::cout << std::endl;
  // }

}

/**
 * Prints all users within a certain distance of a particular user.
 * Sorted from shortest distance to user to longest distance to user.
 * Rounds down to the next lowest km if the difference is a decimal
 * @param locations Location lookup table
 * @param outfile File to write output to
 * @param start_id User we are using as our "origin" (i.e. 0 distance)
 * @param max_distance Maximum distance from start_id that a printed user can be
 */
void printAllUsersWithinDistance(const GEO_TYPE& locations, std::ofstream& outfile,
                                 const ID_TYPE& start_id, double max_distance){


  if (locations.find(start_id) == locations.end()){
    outfile << "User ID " << start_id << " does not have a recorded location." << std::endl;
    return;
  }

  std::list<COORD_TYPE> l_c1 = locations.find(start_id)->second;
  std::map<unsigned int, std::list<ID_TYPE> > temp;

  for(const std::pair<const ID_TYPE, std::list<COORD_TYPE> > &k1 : locations){
    if (start_id != k1.first){
      std::list<COORD_TYPE> l_c2 = k1.second;

      for(const COORD_TYPE &c1 : l_c1){
        for(const COORD_TYPE &c2 : l_c2){
          unsigned int d = distanceEarth(c1.first, c1.second, c2.first, c2.second);
          if (d <= max_distance){
            if (temp.find(d) != temp.end()){
              std::list<ID_TYPE>::iterator x = std::find(temp[d].begin(), temp[d].end(), k1.first);
              if(x == temp[d].end())
                temp[d].push_back(k1.first);
            }
            else{
              std::list<ID_TYPE> t;
              t.push_back(k1.first);
              temp.insert({d, t});
            }
          }
        }
      }
    }
  }

  if (temp.size() > 0){
    outfile << "User IDs within " << max_distance << " km of user " << start_id << ":" << std::endl;
    for(const std::pair<const unsigned int, std::list<ID_TYPE> > &k1 : temp){
      outfile << " " << k1.first <<  " km:";
      for(const ID_TYPE &v1 : k1.second)
        outfile << " " << v1;
      outfile << std::endl;
    }
  }
  else
    outfile << "There are no users within " << max_distance << " km of user " << start_id << std::endl;


}

/**
 * Prints a sorted list of degrees (smallest to largest) along with how many users
 * have that degree. Only prints for degree >=1.
 * @param adj_lists Adjacency lists structure
 * @param outfile File to write output to
 */
void printDegreesHistogram(const ADJ_TYPE& adj_lists, std::ofstream& outfile){

  // map of degees to number of users
  std::map<unsigned int, unsigned int> temp;

  unsigned int ncount = 0;

  for(const std::pair<const ID_TYPE, std::list<ID_TYPE> > &k1 : adj_lists){
    unsigned int d = k1.second.size();      // temp variable for num degrees for each pair
    if (d != 0){
      ncount++;
      if(temp.find(d) == temp.end())
            temp.insert({d, 1});
      else
        temp[d]++;
    }
  }

  outfile << "Histogram for " << ncount << " users:" << std::endl;
  for(const std::pair<const unsigned int, unsigned int > &k1 : temp)
      outfile << " Degree " << k1.first << ": "<< k1.second << std::endl;

}

/**
 * Prints a sorted list of user IDs (smallest to largest) along with the degree for
 * each user. Only prints for degree >=1.
 * @param adj_lists Adjacency lists structure
 * @param outfile File to write output to
 */
void printDegreesOfAll(const ADJ_TYPE& adj_lists, std::ofstream& outfile){

  std::map<ID_TYPE, unsigned int> temp;
  unsigned int ncount = 0;

  for(const std::pair<const ID_TYPE, std::list<ID_TYPE> > &k1 : adj_lists){
    unsigned int d = k1.second.size();
    if (d!=0){
      ncount++;
      temp.insert({k1.first, d});
    }
  }

  outfile << "Degrees for " << ncount << " users:" << std::endl;

  for(const std::pair<const ID_TYPE, std::list<ID_TYPE> > &k1 : adj_lists){
    if (k1.second.size() >= 1)
      outfile << " " << k1.first << ": Degree " << k1.second.size() << std::endl;
  }
}

/**
 * Prints all friends of a particular user who have a particular degree.
 * Sorted by user ID (from smallest to largest).
 * @param adj_lists Adjacency lists structure
 * @param outfile File to write output to
 * @param start_id User whose friends we are looking at
 * @param degree The degree of friends we want to print. Will be >=1.
 */
void printFriendsWithDegree(const ADJ_TYPE& adj_lists, std::ofstream& outfile,
                            const ID_TYPE& start_id, unsigned int degree){

  std::list<ID_TYPE> temp;

  if (adj_lists.find(start_id) == adj_lists.end()){
    outfile << "There is no user with friends and ID " << start_id << std::endl;
    return;
  }


  std::list<ID_TYPE> f = adj_lists.find(start_id)->second;

  for(const ID_TYPE &k1 : f){
    unsigned int d = adj_lists.find(k1)->second.size();

    if(d == degree)
      temp.push_back(k1);
  }

  temp.sort();

  if (temp.size() > 0){
    outfile << "User " << start_id << " has " << temp.size() << " friend(s) with degree " << degree << ":";
    for(const ID_TYPE &k1 : temp){
      outfile << " " << k1;
    }
    outfile << std::endl;
  }
  else
    outfile << "User " << start_id << " has " << temp.size() << " friend(s) with degree " << degree << std::endl;

}


/**
 * Prints friends of a particular user within a certain distance of that user.
 * Sorted from shortest distance to user to longest distance to user.
 * @param adj_lists Adjacency lists structure
 * @param locations Location lookup table
 * @param outfile File to write output to
 * @param start_id User we are using as our "origin" (i.e. 0 distance)
 * @param max_distance Maximum distance from start_id that a printed user can be
 */
void printFriendsWithinDistance(const ADJ_TYPE& adj_lists, const GEO_TYPE& locations, 
                                std::ofstream& outfile, const ID_TYPE& start_id, 
                                double max_distance){

  std::map<double, std::list<ID_TYPE > > temp;


  if (adj_lists.find(start_id) == adj_lists.end()){
    outfile << "There is no user with friends and ID " << start_id << std::endl;
    return;
  }

  if(locations.find(start_id) == locations.end()){
    outfile << "User ID " << start_id << " does not have a recorded location." << std::endl;
    return;
  }

  std::list<COORD_TYPE> l_c1 = locations.find(start_id)->second;

  std::list<ID_TYPE> f = adj_lists.find(start_id)->second;

  for(const ID_TYPE &k1 : f){
    std::list<COORD_TYPE> l_c2 = locations.find(k1)->second;
    for(const COORD_TYPE &c1 : l_c1){
      for(const COORD_TYPE &c2 : l_c2){
        double d = distanceEarth(c1.first, c1.second, c2.first, c2.second);
        if (d <= max_distance){
           // distance found in map - add the user to the friend list in the map for the distance
          if (temp.find(d) != temp.end()){
            std::list<ID_TYPE>::iterator x = std::find(temp[d].begin(), temp[d].end(), k1);
            if(x == temp[d].end())
              temp[d].push_back(k1);
          }
          // distance not found in map - create new temp entry for distance and start list with user k1.first
          else{
            std::list<ID_TYPE> user;
            user.push_back(k1);
            temp.insert({d, user});
          }
        }
      }
    }
  }

  if (temp.size() > 0){
    outfile << "Friends within "<< max_distance << " km of user " << start_id << ":" << std::endl;
    for(const std::pair<const double, std::list<ID_TYPE> > &k1 : temp){
      std::list<ID_TYPE> x = k1.second;
      x.sort();
      outfile << " " << k1.first <<  " km:";
      for(const ID_TYPE &v1 : x)
        outfile << " " << v1 << std::endl;
    }
  }
  else
    outfile << "There are no friends within " << max_distance << " km of user " << start_id << std::endl;


}

/**
 * Prints users with an ID that is "close" to a particular user's ID.
 * Sorted from smallest to largest user ID.
 * Only prints for degree >=1.
 *
 * If there are M IDs that should be printed this function should only use
 * ADJ_TYPE::const_iterator::operator++ / ADJ_TYPE::const_iterator::operator-- O(M) times
 *
 * @param outfile File to write output to
 * @param begin The .begin() iterator for the adjacency lists structure
 * @param end The .end() iterator for the adjacency lists structure
 * @param start_it Iterator for user we are using as our baseline (i.e. 0 diifference)
 *        It is possible that the start_it was not found in the data.
 * @param offset Maximum absolute difference from start_id that a printed user's
 *               ID can have
 */
void printUsersWithinIDRange(std::ofstream& outfile, ADJ_TYPE::const_iterator begin, 
                             ADJ_TYPE::const_iterator end, 
                             ADJ_TYPE::const_iterator start_it, unsigned int offset){

  if (start_it == end){
    outfile << "There is no user with the requested ID" << std::endl;
    return;
  }

  ADJ_TYPE::const_iterator it = start_it;

  std::list<ID_TYPE> temp;

  double minv, maxv;
  ID_TYPE sval = start_it->first;

  minv = (double)sval - (double)offset;
  maxv = (double)sval + (double)offset;

  for (it--; it != begin; it--){
    if (it->first >= minv && it->first < sval)
      temp.push_back(it->first);
    else
      break;
  }

  if(begin->first >= minv && begin->first < sval)
    temp.push_back(begin->first);

  it = start_it;

  for (it++; it != end; it++){
    if (it->first <= maxv && it->first > sval)
      temp.push_back(it->first);
    else
      break;
  }

  if (temp.size() > 0){
    temp.sort();
    outfile <<"Users with an ID within +/-" << offset << " of " << sval << ":";
    for(const ID_TYPE &k1 : temp){
      outfile << " " << k1;
    }
    outfile << std::endl;
  }
  else
    outfile <<"There are no users with an ID within +/-" << offset << " of " << sval << std::endl;

}
