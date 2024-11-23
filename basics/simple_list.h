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

    Iterator begin() const noexcept
    {
        return Iterator(_head.get());
    }

    Iterator end() const noexcept
    {
        return Iterator(nullptr);
    }

    void push_front(T value) noexcept;
    T pop_front();
    T const &head() const;

    void reverse() noexcept;
    void sort() noexcept;
    void clear() noexcept;

    int size() const noexcept;
    bool empty() const noexcept;

    bool contains(T const &value) const noexcept;

private:
    std::unique_ptr<SimpleNode<T>> _head;
    int _size = 0;

    std::unique_ptr<SimpleNode<T>> _insert_sorted(std::unique_ptr<SimpleNode<T>> head, T value) noexcept;
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
void SimpleList<T>::clear() noexcept
{
    _head = nullptr;
    _size = 0;
}

template <typename T>
void SimpleList<T>::reverse() noexcept
{
    std::unique_ptr<SimpleNode<T>> prev = nullptr;
    std::unique_ptr<SimpleNode<T>> current = std::move(_head);
    while (current != nullptr)
    {
        std::unique_ptr<SimpleNode<T>> next = std::move(current->next);
        current->next = std::move(prev);
        prev = std::move(current);
        current = std::move(next);
    }
    _head = std::move(prev);
}

template <typename T>
std::unique_ptr<SimpleNode<T>> SimpleList<T>::_insert_sorted(std::unique_ptr<SimpleNode<T>> head, T value) noexcept
{
    if (head == nullptr || head->value >= value)
    {
        return std::make_unique<SimpleNode<T>>(value, std::move(head));
    }
    auto current = head.get();
    while (current->next != nullptr && current->next->value < value)
    {
        current = current->next.get();
    }
    current->next = std::make_unique<SimpleNode<T>>(value, std::move(current->next));
    return head;
}

template <typename T>
void SimpleList<T>::sort() noexcept
{
    std::unique_ptr<SimpleNode<T>> sorted_head = nullptr;
    while (_head != nullptr)
    {
        T value = pop_front();
        sorted_head = _insert_sorted(std::move(sorted_head), value);
    }
    _head = std::move(sorted_head);
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

template <typename T>
bool SimpleList<T>::contains(T const &value) const noexcept
{
    return std::find(begin(), end(), value) != end();
}