/**
* A class for creating objects, with the type of object created based on a key
* adapted from http://stackoverflow.com/questions/1809670/how-to-implement-serialization-in-c
* 
* @param T the super class that all created classes derive from
*/

#include <string>
#include <memory>
#include <functional>
#include <utility>

template<typename T>
class Factory { 
private: 
    typedef std::function<std::shared_ptr<T>(const std::string&)> CreateObjectFunc;

    /**
    * A map keys (string) to functions (CreateObjectFunc)
    * When creating a new type, we simply call the function with the required key
    */
    std::map<std::string, CreateObjectFunc> mObjectCreator;

    /**
    * Pointers to this function are inserted into the map and called when creating objects
    *
    * @param S the type of class to create
    * @return a object with the type of S
    */
    template<typename S> 
    static std::shared_ptr<T> createObject(const std::string& id){ 
        return std::make_shared<S>(id); 
    }
public:

    /**
    * Registers a class to that it can be created via createObject()
    *
    * @param S the class to register, this must ve a subclass of T
    * @param clazz the name to associate with the class. must be unique
    */ 
    template<typename S> 
    void registerClass(const std::string& clazz){ 
        if (mObjectCreator.find(clazz) != mObjectCreator.end()){ 
            // TODO: handle error
        }
        mObjectCreator.insert(std::make_pair(clazz, &createObject<S>)); 
    }

    /**
    * Returns true if a given key exists
    *
    * @param clazz the name to check if exists
    * @return true if the clazz exists
    */
    bool hasClass(const std::string& clazz){
        return mObjectCreator.find(clazz) != mObjectCreator.end();
    } 

    /**
    * Creates an object based on a class name. It will return null if the key doesn't exist
    *
    * @param clazz the class name of the object to create
    * @param id the id of the state / event
    * @return the new object or null if the clazz doesn't exist
    */
    std::shared_ptr<T> createObject(const std::string& clazz, const std::string& id){
        //Don't use hasClass here as doing so would involve two lookups
        typename std::map<std::string, CreateObjectFunc>::iterator iter = mObjectCreator.find(clazz); 
        if (iter == mObjectCreator.end()){ 
            return std::shared_ptr<T>(); // NULL shared ptr
        }
        //calls the required createObject() function
        return ((*iter).second)(id);
    }
};
