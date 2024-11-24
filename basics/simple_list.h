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

    SimpleNode(SimpleNode const &other) = delete;
    SimpleNode &operator=(SimpleNode const &other) = delete;

    SimpleNode(SimpleNode &&other) noexcept = delete;
    SimpleNode &operator=(SimpleNode &&other) noexcept = delete;

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

    void filter(std::function<bool(T)> const &func) noexcept;
    void transform(std::function<T(T)> const &func) noexcept;

    void clear() noexcept;

    int size() const noexcept { return _size; }
    bool empty() const noexcept { return _size == 0; }
    bool sorted() const noexcept { return _sorted; }

    bool contains(T const &value) const noexcept;

private:
    std::unique_ptr<SimpleNode<T>> _head;
    int _size = 0;
    bool _sorted = true;

    std::unique_ptr<SimpleNode<T>> _insert_sorted(std::unique_ptr<SimpleNode<T>> head, T value) noexcept;
};

template <typename T>
void SimpleList<T>::push_front(T value) noexcept
{
    if (_head != nullptr && _head->value < value)
    {
        _sorted = false; // The list is no longer sorted
    }
    _head = std::make_unique<SimpleNode<T>>(value, std::move(_head));
    ++_size;
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
    _sorted = true;
}

template <typename T>
void SimpleList<T>::reverse() noexcept
{
    if (_size <= 1) 
    {
        return; // Already reversed and sorted
    }

    std::unique_ptr<SimpleNode<T>> previous = nullptr;
    std::unique_ptr<SimpleNode<T>> current = std::move(_head);

    bool found_unsorted_pair = false;
    T last_value;  // Keep track of the previous value for sorting check

    while (current != nullptr)
    {
        // Store the current value before moving pointers
        T current_value = current->value;

        // If we have a previous value, check sorting
        if (previous != nullptr && !found_unsorted_pair)
        {
            found_unsorted_pair = (last_value < current_value);
        }
        last_value = current_value;

        // Perform the reversal
        std::unique_ptr<SimpleNode<T>> next = std::move(current->next);
        current->next = std::move(previous);
        previous = std::move(current);
        current = std::move(next);
    }
    
    _head = std::move(previous);
    _sorted = !found_unsorted_pair;
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
    if (_sorted)
    {
        return; // the list is already sorted
    }
    std::unique_ptr<SimpleNode<T>> sorted_head = nullptr;
    while (_head != nullptr)
    {
        T value = pop_front();
        sorted_head = _insert_sorted(std::move(sorted_head), value);
    }
    _head = std::move(sorted_head);
    _sorted = true;
}

template <typename T>
void SimpleList<T>::filter(std::function<bool(T)> const &func) noexcept
{
    // Remove elements from the head that don't satisfy the predicate
    while (_head != nullptr && !func(_head->value))
    {
        _head = std::move(_head->next);
        --_size;
    }

    if (_size <= 1)
    {
        _sorted = true;
        return;
    }

    auto previous = _head.get();
    auto current = previous->next.get();
    bool sorted_after_filter = true;

    while (current != nullptr)
    {
        if (!func(current->value))
        {
            previous->next = std::move(current->next);
            current = previous->next.get();
            --_size;
        }
        else
        {
            // Check sorting only for elements we're keeping
            sorted_after_filter &= (previous->value <= current->value);
            previous = current;
            current = current->next.get();
        }
    }

    _sorted = sorted_after_filter;
}

template <typename T>
void SimpleList<T>::transform(std::function<T(T)> const &func) noexcept
{
    auto current = _head.get();
    SimpleNode<T> *prev = nullptr;
    bool sorted_after_transform = true;
    while (current != nullptr)
    {
        current->value = func(current->value);
        if (prev != nullptr && prev->value > current->value && sorted_after_transform)
        {
            sorted_after_transform = false;
        }
        prev = current;
        current = current->next.get();
    }
    _sorted = sorted_after_transform;
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
    // Popping the front element doesn't change the sorted state because we don't
    // know if the list would have been sorted after popping. If the list was
    // sorted before popping, it is still sorted after popping.
    return value;
}

template <typename T>
bool SimpleList<T>::contains(T const &value) const noexcept
{
    return std::find(begin(), end(), value) != end();
}