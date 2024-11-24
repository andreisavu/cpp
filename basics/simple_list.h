#pragma once

/**
 * @brief A simple node implementation
 * @tparam T The type of the value in the node
 */
template <typename T>
class SimpleNode
{
public:
    SimpleNode(T value, std::shared_ptr<SimpleNode<T>> next)
        : value(value), next(next) {}
    ~SimpleNode() = default;

    SimpleNode(SimpleNode const &other) = delete;
    SimpleNode &operator=(SimpleNode const &other) = delete;

    SimpleNode(SimpleNode &&other) noexcept = delete;
    SimpleNode &operator=(SimpleNode &&other) noexcept = delete;

    T value;
    std::shared_ptr<SimpleNode<T>> next;
};

/**
 * @brief A simple list implementation
 * @tparam T The type of the values in the list
 * 
 * This class is not thread-safe. In debug builds, it will actively detect and
 * report any access from multiple threads by throwing an exception.
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
        : _head(other._head), _tail(other._tail), _size(other._size), _sorted_ascending(other._sorted_ascending)
    {
        other.clear();
    }

    SimpleList &operator=(SimpleList &&other) noexcept
    {
        _head = other._head;
        _tail = other._tail;
        _size = other._size;
        _sorted_ascending = other._sorted_ascending;
        other.clear();
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

    void push_back(T value) noexcept;

    void insert_sorted(T value);
    void merge(SimpleList<T> &other);

    T const &front() const;
    T const &back() const;

    void reverse() noexcept;
    void sort() noexcept;
    void unique() noexcept;

    void keep_if(std::function<bool(T)> const &func) noexcept;

    void remove_if(std::function<bool(T)> const &func) noexcept;
    void remove(T const &value) noexcept;

    void transform(std::function<T(T)> const &func) noexcept;

    void clear() noexcept;

    int count() const noexcept { return _size; }
    int count_if(std::function<bool(T)> const &func) const noexcept;
    int count(T const &value) const noexcept;

    bool empty() const noexcept { return _size == 0; }
    bool sorted_ascending() const noexcept { return _sorted_ascending; }

    bool contains(T const &value) const noexcept;

private:
    std::shared_ptr<SimpleNode<T>> _head;
    std::shared_ptr<SimpleNode<T>> _tail;

    int _size = 0;
    bool _sorted_ascending = true;

    void _merge_sorted(SimpleList<T> &other);

#ifdef _DEBUG
    mutable std::atomic<int> _thread_id;
    mutable bool _has_owner = false;
#endif

    void _check_thread_safety() const
    {
#ifdef _DEBUG
        if (_has_owner && _thread_id != std::this_thread::get_id())
        {
            throw std::logic_error("List is not thread-safe");
        }
        else if (!_has_owner)
        {
            _thread_id = std::this_thread::get_id();
            _has_owner = true;
        }
#endif
    }
};

template <typename T>
void SimpleList<T>::push_front(T value) noexcept
{
    _check_thread_safety();
    if (_head != nullptr)
    {
        _sorted_ascending &= (_head->value >= value);
    }
    _head = std::make_shared<SimpleNode<T>>(value, _head);
    if (_tail == nullptr)
    {
        _tail = _head;
    }
    ++_size;
}

template <typename T>
void SimpleList<T>::push_back(T value) noexcept
{
    _check_thread_safety();
    if (_head == nullptr)
    {
        push_front(value);
        return;
    }
    _sorted_ascending &= (_tail->value <= value);
    _tail->next = std::make_shared<SimpleNode<T>>(value, nullptr);
    _tail = _tail->next;
    ++_size;
}

template <typename T>
T const &SimpleList<T>::front() const
{
    _check_thread_safety();
    if (_head == nullptr)
    {
        throw std::out_of_range("List is empty");
    }
    return _head->value;
}

template <typename T>
T const &SimpleList<T>::back() const
{
    _check_thread_safety();
    if (_tail == nullptr)
    {
        throw std::out_of_range("List is empty");
    }
    return _tail->value;
}

template <typename T>
void SimpleList<T>::clear() noexcept
{
    _check_thread_safety();
    _head = nullptr;
    _tail = nullptr;
    _size = 0;
    _sorted_ascending = true;
}

template <typename T>
void SimpleList<T>::reverse() noexcept
{
    _check_thread_safety();
    if (_size <= 1)
    {
        return; // Already reversed and sorted
    }

    std::shared_ptr<SimpleNode<T>> previous = nullptr;
    auto current = _head;

    bool found_unsorted_pair = false;
    T last_value; // Keep track of the previous value for sorting check

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
        std::shared_ptr<SimpleNode<T>> next = current->next;
        current->next = previous;
        previous = current;
        current = next;
    }

    _tail = _head;
    _head = previous;
    _sorted_ascending = !found_unsorted_pair;
}

template <typename T>
void SimpleList<T>::insert_sorted(T value)
{
    _check_thread_safety();
    if (!_sorted_ascending)
    {
        throw std::logic_error("List is not sorted");
    }
    if (_head == nullptr || _head->value >= value)
    {
        push_front(value);
        return;
    }
    auto current = _head.get();
    while (current->next != nullptr && current->next->value < value)
    {
        current = current->next.get();
    }
    current->next = std::make_shared<SimpleNode<T>>(value, current->next);
    if (current->next->next == nullptr)
    {
        _tail = current->next;
    }
    ++_size;
}

template <typename T>
void SimpleList<T>::sort() noexcept
{
    _check_thread_safety();
    if (_sorted_ascending)
    {
        return; // the list is already sorted
    }
    auto current = _head;
    clear();
    while (current != nullptr)
    {
        insert_sorted(current->value);
        current = current->next;
    }
}

template <typename T>
void SimpleList<T>::keep_if(std::function<bool(T)> const &func) noexcept
{
    _check_thread_safety();
    // Remove elements from the head that don't satisfy the predicate
    while (_head != nullptr && !func(_head->value))
    {
        _head = _head->next;
        --_size;
    }
    if (_head == nullptr)
    {
        _tail = nullptr;
        _sorted_ascending = true;
        return;
    }
    if (_size == 1)
    {
        _sorted_ascending = true;
        return;
    }

    auto previous = _head;
    auto current = previous->next;
    bool sorted_after_filter = true;

    while (current != nullptr)
    {
        if (!func(current->value))
        {
            previous->next = current->next;
            current = previous->next;
            --_size;
        }
        else
        {
            // Check sorting only for elements we're keeping
            sorted_after_filter &= (previous->value <= current->value);
            previous = current;
            current = current->next;
        }
    }
    if (previous != nullptr)
    {
        _tail = previous;
    }
    _sorted_ascending = sorted_after_filter;
}

template <typename T>
void SimpleList<T>::remove_if(std::function<bool(T)> const &func) noexcept
{
    keep_if([&func](T x)
            { return !func(x); });
}

template <typename T>
void SimpleList<T>::remove(T const &value) noexcept
{
    remove_if([&value](T x)
              { return x == value; });
}

template <typename T>
void SimpleList<T>::transform(std::function<T(T)> const &func) noexcept
{
    _check_thread_safety();
    auto current = _head;
    std::shared_ptr<SimpleNode<T>> previous = nullptr;
    bool sorted_after_transform = true;
    while (current != nullptr)
    {
        current->value = func(current->value);
        if (previous != nullptr && previous->value > current->value && sorted_after_transform)
        {
            sorted_after_transform = false;
        }
        previous = current;
        current = current->next;
    }
    _sorted_ascending = sorted_after_transform;
}

template <typename T>
T SimpleList<T>::pop_front()
{
    _check_thread_safety();
    if (_head == nullptr)
    {
        throw std::out_of_range("List is empty");
    }
    auto value = front();
    _head = _head->next;
    if (_head == nullptr)
    {
        _tail = nullptr;
    }
    --_size;
    // Popping the front element doesn't change the sorted state because we don't
    // know if the list would have been sorted after popping. If the list was
    // sorted before popping, it is still sorted after popping.
    return value;
}

template <typename T>
bool SimpleList<T>::contains(T const &value) const noexcept
{
    _check_thread_safety();
    return std::find(begin(), end(), value) != end();
}

template <typename T>
int SimpleList<T>::count_if(std::function<bool(T)> const &func) const noexcept
{
    _check_thread_safety();
    int count = 0;
    for (auto it : *this)
    {
        if (func(it))
        {
            ++count;
        }
    }
    return count;
}

template <typename T>
int SimpleList<T>::count(T const &value) const noexcept
{
    _check_thread_safety();
    return count_if([&value](T x)
                    { return x == value; });
}

template <typename T>
void SimpleList<T>::unique() noexcept
{
    _check_thread_safety();
    if (_size <= 1)
    {
        return; // Already unique
    }
    sort(); // Sorting is required to make sure duplicates are adjacent
    auto current = _head;
    while (current->next != nullptr)
    {
        if (current->value == current->next->value)
        {
            current->next = current->next->next;
            --_size;
        }
        else
        {
            current = current->next;
        }
    }
    _tail = current;
}

template <typename T>
void SimpleList<T>::merge(SimpleList<T> &other)
{
    _check_thread_safety();
    if (_sorted_ascending && other._sorted_ascending)
    {
        _merge_sorted(other);
        return;
    }

    // Otherwise, perform a naive merge of the two lists that ignores the sorted state
    // If sorted state is important, the lists should be sorted first or after merging

    if (other.empty())
    {
        return;
    }
    if (_head == nullptr)
    {
        _head = other._head;
    }
    else
    {
        _tail->next = other._head;
    }
    _tail = other._tail;
    _size += other._size;
    _sorted_ascending = false; // Merging two lists invalidates the sorted state

    other.clear();
}

template <typename T>
void SimpleList<T>::_merge_sorted(SimpleList<T> &other)
{
    if (other.empty())
    {
        return;
    }
    // If this list is empty, copy the other list
    if (_head == nullptr)
    {
        _head = other._head;
        _tail = other._tail;
        _size = other._size;
        _sorted_ascending = other._sorted_ascending;
        other.clear();
        return;
    }
    // Both lists are non-empty, merge them while maintaining the sorted state
    auto current = _head;
    auto other_current = other._head;
    clear(); // Clear this list before merging
    while (current != nullptr && other_current != nullptr)
    {
        if (current->value < other_current->value)
        {
            push_back(current->value);
            current = current->next;
        }
        else
        {
            push_back(other_current->value);
            other_current = other_current->next;
        }
    }
    // Append the remaining elements from this list
    while (current != nullptr)
    {
        push_back(current->value);
        current = current->next;
    }
    // Append the remaining elements from the other list
    while (other_current != nullptr)
    {
        push_back(other_current->value);
        other_current = other_current->next;
    }
    other.clear();
}