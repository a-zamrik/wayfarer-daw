
// C++ implementation of the above approach 
#include <condition_variable> 
#include <iostream> 
#include <mutex> 
#include <queue> 
  
// Thread-safe queue 
template <typename T> 
class TSQueue { 
private: 
    std::queue<T> m_queue; 
    std::mutex m_mutex; 
  
public: 

    void push(T item) 
    { 
        this->m_mutex.lock();
            this->m_queue.push(item); 
        this->m_mutex.unlock();
    } 

    bool try_pop(T & item)
    {
        bool did_pop = false;
        this->m_mutex.lock();
            if (this->m_queue.size())
            {
                item =  m_queue.front(); 
                m_queue.pop(); 
                did_pop = true;
            }
        this->m_mutex.unlock();

        return did_pop; 
    }
}; 
  
