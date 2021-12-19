#pragma once

#include <inttypes.h>
#include <scope.h>

#include <memory>
#include <vector>

class Object : public std::enable_shared_from_this<Object> {
public:
    virtual ~Object() = default;
};

class Number : public Object {
    int64_t value_;

public:
    Number(int64_t value) : value_{value} {
    }
    int64_t GetValue() const {
        return value_;
    }
};

class Symbol : public Object {
    std::string name_;

public:
    Symbol(std::string name) : name_{name} {
    }
    const std::string& GetName() const {
        return name_;
    }
};

class Cell : public Object {
    std::shared_ptr<Object> first_;
    std::shared_ptr<Object> second_;

public:
    Cell(std::shared_ptr<Object> first, std::shared_ptr<Object> second)
        : first_{first}, second_{second} {
    }
    std::shared_ptr<Object> GetFirst() const {
        return first_;
    }
    std::shared_ptr<Object> GetSecond() const {
        return second_;
    }

    void SetFirst(std::shared_ptr<Object> object) {
        first_ = object;
    }

    void SetSecond(std::shared_ptr<Object> object) {
        second_ = object;
    }
};

class Procedure : public Object {
    std::shared_ptr<Scope> scope_;
    std::vector<std::shared_ptr<Object>> formal_arguments_;
    std::vector<std::shared_ptr<Object>> body_;

public:
    Procedure(std::shared_ptr<Scope> scope,
              const std::vector<std::shared_ptr<Object>>& formal_arguments,
              const std::vector<std::shared_ptr<Object>>& body)
        : scope_(scope), formal_arguments_(formal_arguments), body_(body) {
    }

    std::shared_ptr<Scope> GetScope() const {
        return scope_;
    }

    auto GetFormalArguments() const {
        return formal_arguments_;
    }

    auto GetBody() const {
        return body_;
    }
};

class Bool : public Object {
    bool value_;

public:
    Bool(bool value) : value_(value) {
    }

    bool IsTrue() const {
        return value_;
    }
};

template <class T>
std::shared_ptr<T> As(const std::shared_ptr<Object>& obj) {
    return std::static_pointer_cast<T>(obj);
}

template <class T>
bool Is(const std::shared_ptr<Object>& obj) {
    return std::dynamic_pointer_cast<T>(obj) != nullptr;
}
