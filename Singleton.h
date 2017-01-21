#ifndef SINGLETON_H
#define SINGLETON_H
#include <memory>
#include <type_traits>
#include <iostream>

//#define SINGLETON_OPTIONS

template<typename CLASS>
class Singleton
{
public:
    //static_assert(!std::is_destructible<CLASS>(), "Деструктор производного класса должен быть объявлен protected!");
    //static_assert(!std::is_constructible<CLASS>(),
    //              "Конструктор производного класса должен быть объявлен protected!");



    template<typename ... Args>
    inline static void create(Args... arsg)
    {
        if (m_pInstance!=nullptr)
            return;

        m_pInstance = new CLASS(arsg...);
        static DeleterThis p ;
    }

    inline static CLASS& get()
    {
        return *m_pInstance;
    }

    inline static CLASS* const getPtr()
    {
        return m_pInstance;
    }


    Singleton(const Singleton&) = delete;
    void * operator =(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    void * operator =(Singleton&&) = delete;

protected:
    Singleton()
    {
        //std::cout << "______________________________________" << std::endl;
        //std::cout << std::is_constructible<CLASS>() << "djgksdjglkfjgsdf" << std::endl;
    }
    ~Singleton()
    {
        m_pInstance = nullptr;
    }

    //void * operator =(const Singleton<CLASS>&);

    //void* operator new(size_t);


private:
    class DeleterThis
    {
    public:
        ~DeleterThis()
        {
            delete getThis();
        }
    };

    friend class DeleteThis;

    static CLASS* m_pInstance;

    inline static Singleton* getThis()
    {
        return static_cast<Singleton*>(getPtr());
    }
};

template<typename CLASS>
CLASS* Singleton<CLASS>::m_pInstance{nullptr};



#endif // SINGLETON_H








