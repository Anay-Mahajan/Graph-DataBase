#include<graph_db/edge.h>

using namespace std;
namespace graph_db{
    void Edge::set_property(string key,PropertyValue p){
        properties_[key]=p;
    }
    bool Edge::has_property(string s){
        return properties_.find(s)!=properties_.end();
    }
    void Edge::remove_property(string s){
        properties_.erase(s);
    }
    PropertyValue Edge::get_property(string s){
        auto it=find(properties_.begin(),properties_.end(),s);
        if(it==properties_.end()){
            throw runtime_error("Property not found "+s);
        }
        return it->second;
    }
}