#pragma once

/**
 * @brief A simple node implementation
 * @tparam T The type of the value in the node
 */
template <typename T>
class SimpleNode
{
public:
    SimpleNode(T value, std::unique_ptr<SimpleNode<T>> next)
        : value(value), next(std::move(next)) {}
    ~SimpleNode() = default;

    T value;
    std::unique_ptr<SimpleNode<T>> next;
};

/**
 * @brief A simple list implementation
 * @tparam T The type of the values in the list
 */
template <typename T>
class SimpleList
{
public:
    SimpleList() = default;
    ~SimpleList() = default;

    // Delete copy constructor and copy assignment operator
    SimpleList(const SimpleList &) = delete;
    SimpleList &operator=(const SimpleList &) = delete;

    SimpleList(SimpleList &&other) noexcept
        : _head(std::move(other._head)), _size(other._size)
    {
        other._size = 0;
    }

    SimpleList &operator=(SimpleList &&other) noexcept
    {
        _head = std::move(other._head);
        _size = other._size;
        other._size = 0;
        return *this;
    }

    /**
     * @brief A simple iterator implementation
     */
    class Iterator
    {
    public:
        Iterator(SimpleNode<T> *node) : current(node) {}

        T const &operator*() const { return current->value; }

        Iterator &operator++() noexcept
        {
            current = current->next.get();
            return *this;
        }

        bool operator!=(Iterator const &other) const noexcept
        {
            return current != other.current;
        }

    private:
        SimpleNode<T> *current;
    };

    Iterator begin() noexcept
    {
        return Iterator(_head.get());
    }

    Iterator end() noexcept
    {
        return Iterator(nullptr);
    }

    void push_front(T value) noexcept;
    T pop_front();
    T const &head() const;

    int size() const noexcept;
    bool empty() const noexcept;

private:
    std::unique_ptr<SimpleNode<T>> _head;
    int _size = 0;
};

template <typename T>
void SimpleList<T>::push_front(T value) noexcept
{
    _head = std::make_unique<SimpleNode<T>>(value, std::move(_head));
    ++_size;
}

template <typename T>
int SimpleList<T>::size() const noexcept
{
    return _size;
}

template <typename T>
bool SimpleList<T>::empty() const noexcept
{
    return _size == 0;
}

template <typename T>
T const &SimpleList<T>::head() const
{
    if (_head == nullptr)
    {
        throw std::out_of_range("List is empty");
    }
    return _head->value;
}

template <typename T>
T SimpleList<T>::pop_front()
{
    if (_head == nullptr)
    {
        throw std::out_of_range("List is empty");
    }
    auto value = head();
    _head = std::move(_head->next);
    --_size;
    return value;
}