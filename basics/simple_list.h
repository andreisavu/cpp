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
        : head_(other.head_), tail_(other.tail_), size_(other.size_), sorted_ascending_(other.sorted_ascending_)
    {
        other.clear();
    }

    SimpleList &operator=(SimpleList &&other) noexcept
    {
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        sorted_ascending_ = other.sorted_ascending_;
        other.clear();
        return *this;
    }

    /**
     * @brief A simple iterator implementation
     */
    class Iterator
    {
    public:
        Iterator(SimpleNode<T> *node) : current_(node) {}

        T const &operator*() const { return current_->value; }

        Iterator &operator++() noexcept
        {
            current_ = current_->next.get();
            return *this;
        }

        bool operator!=(Iterator const &other) const noexcept
        {
            return current_ != other.current_;
        }

    private:
        SimpleNode<T> *current_;
    };

    Iterator begin() const noexcept
    {
        return Iterator(head_.get());
    }

    Iterator end() const noexcept
    {
        return Iterator(nullptr);
    }

    void pushFront(T value) noexcept;
    T popFront();

    void pushBack(T value) noexcept;

    void insertSorted(T value);
    void merge(SimpleList<T> &other);

    T const &front() const;
    T const &back() const;

    void reverse() noexcept;
    void sort() noexcept;
    void unique() noexcept;

    void keepIf(std::function<bool(T)> const &func) noexcept;

    void removeIf(std::function<bool(T)> const &func) noexcept;
    void remove(T const &value) noexcept;

    void transform(std::function<T(T)> const &func) noexcept;

    void clear() noexcept;

    int count() const noexcept { return size_; }
    int countIf(std::function<bool(T)> const &func) const noexcept;
    int count(T const &value) const noexcept;

    bool empty() const noexcept { return size_ == 0; }
    bool isSortedAscending() const noexcept { return sorted_ascending_; }

    bool contains(T const &value) const noexcept;

private:
    std::shared_ptr<SimpleNode<T>> head_;
    std::shared_ptr<SimpleNode<T>> tail_;

    int size_ = 0;
    bool sorted_ascending_ = true;

    void _merge_sorted(SimpleList<T> &other);

#ifdef _DEBUG
    mutable std::atomic<int> thread_id_;
    mutable bool has_owner_ = false;
#endif

    void _check_thread_safety() const
    {
#ifdef _DEBUG
        if (has_owner_ && thread_id_ != std::this_thread::get_id())
        {
            throw std::logic_error("List is not thread-safe");
        }
        else if (!_has_owner)
        {
            thread_id_ = std::this_thread::get_id();
            has_owner_ = true;
        }
#endif
    }
};

template <typename T>
void SimpleList<T>::pushFront(T value) noexcept
{
    _check_thread_safety();
    if (head_ != nullptr)
    {
        sorted_ascending_ &= (head_->value >= value);
    }
    head_ = std::make_shared<SimpleNode<T>>(value, head_);
    if (tail_ == nullptr)
    {
        tail_ = head_;
    }
    ++size_;
}

template <typename T>
void SimpleList<T>::pushBack(T value) noexcept
{
    _check_thread_safety();
    if (head_ == nullptr)
    {
        pushFront(value);
        return;
    }
    sorted_ascending_ &= (tail_->value <= value);
    tail_->next = std::make_shared<SimpleNode<T>>(value, nullptr);
    tail_ = tail_->next;
    ++size_;
}

template <typename T>
T const &SimpleList<T>::front() const
{
    _check_thread_safety();
    if (head_ == nullptr)
    {
        throw std::out_of_range("List is empty");
    }
    return head_->value;
}

template <typename T>
T const &SimpleList<T>::back() const
{
    _check_thread_safety();
    if (tail_ == nullptr)
    {
        throw std::out_of_range("List is empty");
    }
    return tail_->value;
}

template <typename T>
void SimpleList<T>::clear() noexcept
{
    _check_thread_safety();
    head_ = nullptr;
    tail_ = nullptr;
    size_ = 0;
    sorted_ascending_ = true;
}

template <typename T>
void SimpleList<T>::reverse() noexcept
{
    _check_thread_safety();
    if (size_ <= 1)
    {
        return; // Already reversed and sorted
    }

    std::shared_ptr<SimpleNode<T>> previous = nullptr;
    auto current = head_;

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

    tail_ = head_;
    head_ = previous;
    sorted_ascending_ = !found_unsorted_pair;
}

template <typename T>
void SimpleList<T>::insertSorted(T value)
{
    _check_thread_safety();
    if (!sorted_ascending_)
    {
        throw std::logic_error("List is not sorted");
    }
    if (head_ == nullptr || head_->value >= value)
    {
        pushFront(value);
        return;
    }
    auto current = head_.get();
    while (current->next != nullptr && current->next->value < value)
    {
        current = current->next.get();
    }
    current->next = std::make_shared<SimpleNode<T>>(value, current->next);
    if (current->next->next == nullptr)
    {
        tail_ = current->next;
    }
    ++size_;
}

template <typename T>
void SimpleList<T>::sort() noexcept
{
    _check_thread_safety();
    if (sorted_ascending_)
    {
        return; // the list is already sorted
    }
    auto current = head_;
    clear();
    while (current != nullptr)
    {
        insertSorted(current->value);
        current = current->next;
    }
}

template <typename T>
void SimpleList<T>::keepIf(std::function<bool(T)> const &func) noexcept
{
    _check_thread_safety();
    // Remove elements from the head that don't satisfy the predicate
    while (head_ != nullptr && !func(head_->value))
    {
        head_ = head_->next;
        --size_;
    }
    if (head_ == nullptr)
    {
        tail_ = nullptr;
        sorted_ascending_ = true;
        return;
    }
    if (size_ == 1)
    {
        sorted_ascending_ = true;
        return;
    }

    auto previous = head_;
    auto current = previous->next;
    bool sorted_after_filter = true;

    while (current != nullptr)
    {
        if (!func(current->value))
        {
            previous->next = current->next;
            current = previous->next;
            --size_;
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
        tail_ = previous;
    }
    sorted_ascending_ = sorted_after_filter;
}

template <typename T>
void SimpleList<T>::removeIf(std::function<bool(T)> const &func) noexcept
{
    keepIf([&func](T x)
            { return !func(x); });
}

template <typename T>
void SimpleList<T>::remove(T const &value) noexcept
{
    removeIf([&value](T x)
              { return x == value; });
}

template <typename T>
void SimpleList<T>::transform(std::function<T(T)> const &func) noexcept
{
    _check_thread_safety();
    auto current = head_;
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
    sorted_ascending_ = sorted_after_transform;
}

template <typename T>
T SimpleList<T>::popFront()
{
    _check_thread_safety();
    if (head_ == nullptr)
    {
        throw std::out_of_range("List is empty");
    }
    auto value = front();
    head_ = head_->next;
    if (head_ == nullptr)
    {
        tail_ = nullptr;
    }
    --size_;
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
int SimpleList<T>::countIf(std::function<bool(T)> const &func) const noexcept
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
    return countIf([&value](T x)
                    { return x == value; });
}

template <typename T>
void SimpleList<T>::unique() noexcept
{
    _check_thread_safety();
    if (size_ <= 1)
    {
        return; // Already unique
    }
    sort(); // Sorting is required to make sure duplicates are adjacent
    auto current = head_;
    while (current->next != nullptr)
    {
        if (current->value == current->next->value)
        {
            current->next = current->next->next;
            --size_;
        }
        else
        {
            current = current->next;
        }
    }
    tail_ = current;
}

template <typename T>
void SimpleList<T>::merge(SimpleList<T> &other)
{
    _check_thread_safety();
    if (sorted_ascending_ && other.sorted_ascending_)
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
    if (head_ == nullptr)
    {
        head_ = other.head_;
    }
    else
    {
        tail_->next = other.head_;
    }
    tail_ = other.tail_;
    size_ += other.size_;
    sorted_ascending_ = false; // Merging two lists invalidates the sorted state

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
    if (head_ == nullptr)
    {
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        sorted_ascending_ = other.sorted_ascending_;
        other.clear();
        return;
    }
    // Both lists are non-empty, merge them while maintaining the sorted state
    auto current = head_;
    auto other_current = other.head_;
    clear(); // Clear this list before merging
    while (current != nullptr && other_current != nullptr)
    {
        if (current->value < other_current->value)
        {
            pushBack(current->value);
            current = current->next;
        }
        else
        {
            pushBack(other_current->value);
            other_current = other_current->next;
        }
    }
    // Append the remaining elements from this list
    while (current != nullptr)
    {
        pushBack(current->value);
        current = current->next;
    }
    // Append the remaining elements from the other list
    while (other_current != nullptr)
    {
        pushBack(other_current->value);
        other_current = other_current->next;
    }
    other.clear();
}