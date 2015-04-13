#ifndef SINGLETON_HPP
#define SINGLETON_HPP

template <typename T> class Singleton
{
public:
    static QSharedPointer<T> Instance ()
    {
        if (NULL == _singleton)
        {
            _singleton = QSharedPointer<T> ( new T );
        }
        else
        {
        }

        return _singleton;
    }
protected:
    Singleton () {
    }
    ~Singleton () {}
private:
    static QSharedPointer<T> _singleton;
};

template <typename T> QSharedPointer<T> Singleton<T>::_singleton;

#endif // SINGLETON_HPP

