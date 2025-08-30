#include "../../include/graph_db/edge.h"
#include<shared_mutex>
namespace graph_db{
    void Edge::set_property(std::string key,PropertyValue p){
        std::unique_lock lock(mutex_);
        properties_[key]=p;
    }
    bool Edge::has_property(std::string s){
        return properties_.find(s)!=properties_.end();
    }
    void Edge::remove_property(std::string s){
        std::unique_lock lock(mutex_);
        properties_.erase(s);
    }
    PropertyValue Edge::get_property(std::string s){
        std::shared_lock lock(mutex_);
        auto it=properties_.find(s);
        if(it==properties_.end()){
            throw std::runtime_error("Property not found "+s);
        }
        return it->second;
    }
}