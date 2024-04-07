#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <iostream>
#include <memory>

namespace miheev
{
  template< typename T >
  class List
  {
  public:
    T data_;
    List< T >* next_;
    bool isEmpty_;
    struct ConstIterator;
    struct Iterator;

    List();
    List(T data);
    List(const List& toCopy);
    List(List&& rhs) noexcept;
    explicit List(size_t count, const T& value);
    ~List();

    List& operator=(const List& list);
    T& operator[](size_t i);

    bool empty() const;
    void swap(List< T >& aList);
    void clear();
    size_t size() const;
    T& front();
    void pushFront(T data);
    void popFront();
    void pushBack(T data);
    void eraseAfter(Iterator iter);
    void remove(T data);
    void print(std::ostream& stream = std::cout, const char& splitter = ' ') const;

    template < class P >
    void removeIf(P functor);

    Iterator begin();
    Iterator end();

  };
}

template< typename T >
typename miheev::List< T >::Iterator miheev::List< T >::begin()
{
  return this;
}

template< typename T >
typename miheev::List< T >::Iterator miheev::List< T >::end()
{
  return nullptr;
}

template< typename T >
struct miheev::List< T >::ConstIterator
{
private:
  List< T >* cur;
public:
  friend class List< T >;
  ConstIterator(const List< T >*);
  ~ConstIterator() = default;
  ConstIterator& operator=(const ConstIterator&);

  ConstIterator& operator++();
  ConstIterator operator++(int);

  const T& operator*();
  const T* operator->();

  bool operator!=(const ConstIterator& rhs) const;
  bool operator==(const ConstIterator& rhs) const;
};

template< typename T >
struct miheev::List< T >::Iterator
{
  List<T>* cur;

  using this_t = List< T >::Iterator;

  Iterator();
  Iterator(List< T >* head);
  Iterator(const this_t&) = default;
  this_t& operator=(const this_t&) = default;
  List<T>& operator+(size_t n);
  T& operator*();
  T* operator->() const;
  explicit operator bool() const;
  this_t& operator++();
  this_t operator++(int);
  bool operator!=(const this_t & rhs) const;
  bool operator==(const this_t & rhs) const;

  void eraseAfter();
  bool empty() const;
  this_t next();
};

template< typename T >
miheev::List< T >::Iterator::Iterator():
  cur(nullptr)
{}

template< typename T >
miheev::List< T >::Iterator::Iterator(List < T >* head):
  cur(head)
{}

template< typename T >
miheev::List< T >& miheev::List< T >::Iterator::operator+(size_t n)
{
  List< T >* temp = cur;
  for (; n > 0; n--)
  {
    temp++;
  }
  return *temp;
}

template< typename T >
T& miheev::List< T >::Iterator::operator*()
{
  return cur->data_;
}

template< typename T >
T* miheev::List< T >::Iterator::operator->() const
{
  return std::addressof(cur->data_);
}

template< typename T >
miheev::List< T >::Iterator::operator bool() const
{
  return cur->next_ != nullptr;
}

template< typename T >
bool miheev::List< T >::Iterator::empty() const
{
  return cur->isEmpty_;
}

template< typename T >
typename miheev::List< T >::Iterator::this_t& miheev::List< T >::Iterator::operator++()
{
  cur = cur->next_;
  return *this;
}

template< typename T >
typename miheev::List< T >::Iterator::this_t miheev::List< T >::Iterator::operator++(int)
{
  this_t copy(*this);
  cur = cur->next_;
  return copy;
}

template< typename T >
typename miheev::List< T >::Iterator::this_t miheev::List< T >::Iterator::next()
{
  return this_t(this->cur->next_);
}

template< typename T >
bool miheev::List< T >::Iterator::operator!=(const this_t & rhs) const
{
  return cur != rhs.cur;
}

template< typename T >
bool miheev::List< T >::Iterator::operator==(const this_t & rhs) const
{
return cur == rhs.cur;
}

template< typename T >
void miheev::List< T >::Iterator::eraseAfter()
{
  List< T >* temp = cur->next_;
  cur->next_ = temp->next_;
  temp->next_ = nullptr;
  delete temp;
}

template< typename T >
miheev::List< T >::List():
  data_(0),
  next_(nullptr),
  isEmpty_(true)
{}

template< typename T >
miheev::List< T >::List(T data):
  data_(data),
  next_(nullptr),
  isEmpty_(false)
{}

template< typename T >
miheev::List< T >::List(const List& toCopy)
{
  data_ = toCopy.data_;
  isEmpty_ = toCopy.isEmpty_;
  if (toCopy.next_)
  {
    next_ = new List< T >(*toCopy.next_);
  }
  else
  {
    next_ = nullptr;
  }
}

template< typename T >
miheev::List< T >::List(List&& rhs) noexcept:
  next_{nullptr},
  isEmpty_{true}
{

  for (size_t i = 0; i < rhs.size(); i++)
  {
    pushBack(rhs[i]);
  }
}

template< typename T >
miheev::List< T >::List(size_t count, const T& value):
  next_(nullptr),
  isEmpty_(true)
{
  for (size_t i = 0; i < count; i++)
  {
    pushBack(value);
  }
}

template< typename T >
miheev::List< T >& miheev::List< T >::operator=(const miheev::List< T >& list)
{
  data_ = list.data_;
  isEmpty_ = list.isEmpty_;
  if (next_)
  {
    List< T >* old = next_;
    next_ = nullptr;
    delete old;
  }
  if (list.next_)
  {
    next_ = new List< T >(*(list.next_));
  }
  return *this;
}

template< typename T >
miheev::List< T >::~List()
{
  if (next_)
  {
    delete next_;
  }
}

template< typename T >
bool miheev::List< T >::empty() const
{
  return isEmpty_;
}

template < typename T >
void miheev::List< T >::swap(List< T >& aList)
{
  List< T > temp = *this;
  clear();
  while (!aList.empty())
  {
    pushBack(aList.front());
    aList.popFront();
  }
  aList.clear();
  while (!temp.empty())
  {
    aList.pushBack(temp.front());
    temp.popFront();
  }
}

template < typename T >
T& miheev::List< T >::operator[](size_t i)
{
  List < T >* node = this;
  while (i > 0)
  {
    if (node->next_)
    {
      node = node->next_;
    }
    i--;
  }
  return node->data_;
}

template < typename T >
void miheev::List< T >::clear()
{
  if (next_ != nullptr)
  {
    List< T >* temp = next_;
    next_ = nullptr;
    delete temp;
  }
  isEmpty_ = true;
  data_ = 0;
}

template < typename T >
size_t miheev::List< T >::size() const
{
  if (isEmpty_)
  {
    return 0;
  }
  if (!next_)
  {
    return 1;
  }
  return 1 + next_->size();
}

template < typename T >
T& miheev::List< T >::front()
{
  return data_;
}

template < typename T >
void miheev::List< T >::pushFront(T data)
{
  if (!isEmpty_)
  {
    List< T >* temp = next_;
    next_ = new List< T >(data_);
    next_->next_ = temp;
  }
  data_ = data;
  isEmpty_ = false;
}

template < typename T >
void miheev::List< T >::popFront()
{
  if (!next_)
  {
    data_ = 0;
    isEmpty_ = true;
  }
  if (isEmpty_)
  {
    return;
  }
  List< T >* temp = next_;
  data_ = next_->data_;
  next_ = next_->next_;
  temp->next_ = nullptr;
  delete temp;
}

template < typename T >
void miheev::List< T >::pushBack(T data)
{
  if (isEmpty_)
  {
    data_ = data;
  }
  else
  {
    if (next_ == nullptr)
    {
      next_ = new List< T >(data);
    }
    else
    {
      next_->pushBack(data);
    }
  }
  isEmpty_ = false;
}

template < typename T >
void miheev::List< T >::eraseAfter(miheev::List< T >::Iterator iter)
{
  iter.eraseAfter();
}

template < typename T >
void miheev::List< T >::remove(T data)
{
  Iterator iter(begin());
  while(iter)
  {
    if (*(iter.next()) == data)
    {
      iter.eraseAfter();
    }
    iter++;
  }
  if (data_ == data)
  {
    popFront();
  }
}

template < typename T >
void miheev::List< T >::print(std::ostream& stream, const char& splitter) const
{
  stream << data_ << splitter;
  if (next_)
  {
    next_->print(stream, splitter);
  }
  else
  {
    std::cout << '\n';
  }
}

template < typename T >
template < typename P >
void miheev::List< T >::removeIf(P functor)
{
  Iterator iter(begin());
  while (iter)
  {
    if (functor(*(iter.next())))
    {
      iter.eraseAfter();
    }
    if (iter)
    {
      iter++;
    }
  }
}

#endif
