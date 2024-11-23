#pragma once

template <typename T>
class SimpleNode
{
public:
    SimpleNode(T value, std::unique_ptr<SimpleNode<T>> next) : value(value), next(std::move(next)) {}
    ~SimpleNode() = default;

    T value;
    std::unique_ptr<SimpleNode<T>> next;
};

template <typename T>
class SimpleList
{
public:
    SimpleList() = default;
    ~SimpleList() = default;

    // Delete copy constructor and copy assignment operator
    SimpleList(const SimpleList &) = delete;
    SimpleList &operator=(const SimpleList &) = delete;

    // Default move constructor and move assignment operator
    SimpleList(SimpleList &&) = default;
    SimpleList &operator=(SimpleList &&) = default;

    void add(T value) noexcept;
    T const &get(int index) const;
    int get_size() const noexcept;

private:
    std::unique_ptr<SimpleNode<T>> head;
    int size = 0;
};

template <typename T>
void SimpleList<T>::add(T value) noexcept
{
    head = std::make_unique<SimpleNode<T>>(value, std::move(head));
    ++size;
}

template <typename T>
T const &SimpleList<T>::get(int index) const
{
    auto current = head.get();
    for (int i = 0; i < index; ++i)
    {
        if (current == nullptr)
        {
            throw std::out_of_range("Index out of range");
        }
        current = current->next.get();
    }
    if (current == nullptr)
    {
        throw std::out_of_range("Index out of range");
    }
    return current->value;
}

template <typename T>
int SimpleList<T>::get_size() const noexcept
{
    return size;
}
